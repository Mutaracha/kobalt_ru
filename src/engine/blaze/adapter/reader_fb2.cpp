#include "engine_document.h"
#include "reader_fb2.h"
#include "column_view.h"
#include "support/strings.h"
#include <lvdocview.h>
#include <QXmlStreamWriter>
#include <algorithm>
#include <functional>
#include <QSet>

namespace kobalt {
namespace {
QString qstring(const lString32 &s) {
    return QString::fromUtf8(UnicodeToUtf8(s).c_str());
}
lString32 cre(const QString &s) {
    return Utf8ToUnicode(s.toUtf8().constData());
}
} // namespace

bool fb2::preserveAlignment(ldomNode *node)
{
    const auto name = qstring(node->getNodeName());
    if (name == "body") {
        const auto kind = qstring(node->getAttributeValue("name"));
        if (kind == "notes" || kind == "comments")
            return true;
    }
    static const QSet<QString> tags{ "title", "subtitle", "epigraph", "poem", "stanza", "v",
                                     "text-author", "date" };
    return tags.contains(name);
}

bool fb2::preserveBeautifier(ldomNode *node)
{
    static const QSet<QString> tags{ "title", "subtitle", "epigraph", "poem", "stanza", "v",
                                     "text-author", "cite", "annotation", "date" };
    return tags.contains(qstring(node->getNodeName()));
}

bool fb2::inlineElement(const QString &name)
{
    return name == "emphasis" || name == "strikethrough";
}

void fb2::bindLanguage(ldomNode *root, const QString &language)
{
    if (!root) return;
    // The synthetic document root is not searched by TextLangMan. Attach to
    // FictionBook so layout records its lang-node index and later formatting
    // (paint, selection and hit-testing) resolves the same language even while
    // an appearance preview has changed the global fallback. Explicit source
    // language attributes, including those on descendants, remain authoritative.
    for (int i = 0; i < root->getChildCount(); ++i) {
        auto *node = root->getChildNode(i);
        if (!node->isElement() || !node->isNodeName("FictionBook")) continue;
        const auto attribute = root->getDocument()->getAttrNameIndex("lang");
        if (node->getAttributeValue(0, attribute).empty())
            node->setAttributeValue(0, attribute, cre(language).c_str());
    }
}

void fb2::markNumericNoteTitles(ldomNode *root)
{
    if (!root) return;
    const auto marker = root->getDocument()->getAttrNameIndex("data-kobalt-note-number");
    QVector<QPair<ldomNode *, bool>> pending{ { root, false } };
    while (!pending.isEmpty()) {
        const auto entry = pending.takeLast();
        auto *node = entry.first;
        if (!node || !node->isElement()) continue;
        bool notes = entry.second;
        if (node->isNodeName("body")) {
            const auto name = node->getAttributeValue("name");
            notes = name == U"notes" || name == U"comments";
        }
        auto *parent = node->getUnboxedParent();
        if (notes && node->isNodeName("title") && parent && parent->isNodeName("section")) {
            auto number = qstring(node->getText()).trimmed();
            if (number.size() >= 3 && ((number.startsWith('[') && number.endsWith(']'))
                    || (number.startsWith('(') && number.endsWith(')'))))
                number = number.mid(1, number.size() - 2).trimmed();
            const bool numeric = !number.isEmpty() && number.size() <= 16
                    && std::all_of(number.cbegin(), number.cend(), [](QChar c) { return c.isDigit(); });
            node->setAttributeValue(0, marker, numeric ? U"1" : U"");
        }
        for (int i = node->getChildCount() - 1; i >= 0; --i)
            pending.append({ node->getChildNode(i), notes });
    }
}

bool EngineDocument::openFb2Preview(EngineDocument &source, QSize size, QString *error) {
    auto fail = [error](const QString &message) {
        *error = message;
        return false;
    };
    auto *document = source.engine_->getDocument();
    auto pointer = document->createXPointer(cre(source.position()));
    if (pointer.isNull())
        return fail(strings::get("error.no_section_to_preview"));
    auto *start = pointer.getNode();
    while (start && !start->isNodeName("p") && !start->isNodeName("v")
           && !start->isNodeName("subtitle") && !start->isNodeName("body"))
        start = start->getParentNode();
    if (!start) return fail(strings::get("error.no_section_to_preview"));
    // Retain semantic ancestors (title/poem/cite/nested section), but omit
    // preceding siblings. Never serialize CREngine's generated boxing nodes.
    QSet<ldomNode *> ancestors;
    auto *body = start;
    while (body && !body->isNodeName("body")) {
        ancestors.insert(body);
        body = body->getParentNode();
    }
    if (!body) return fail(strings::get("error.no_section_to_preview"));
    ancestors.insert(body);
    int offset = 0;
    bool mapped = false;
    QVector<ldomNode *> pending{start};
    while (!pending.isEmpty()) {
        auto *part = pending.takeLast();
        if (part->isElement() && part->isBoxingNode(true) && !part->isBoxingNode()) continue;
        if (part == pointer.getNode()) {
            mapped = true;
            if (part->isText())
                offset += pointer.getOffset();
            else
                for (int i = 0; i < qMin(pointer.getOffset(), part->getChildCount()); ++i)
                    offset += part->getChildNode(i)->getText().length();
            break;
        }
        if (part->isText()) offset += part->getText().length();
        else for (int i = part->getChildCount() - 1; i >= 0; --i)
            pending.append(part->getChildNode(i));
    }
    if (!mapped || offset < 0) return fail(strings::get("error.no_section_to_preview"));
    const auto language = qstring(source.engine_->getLanguage()).trimmed();
    QByteArray previewBytes;
    QXmlStreamWriter writer(&previewBytes);
    writer.writeStartDocument("1.0", true);
    writer.writeDefaultNamespace("http://www.gribuser.ru/xml/fictionbook/2.0");
    writer.writeStartElement("FictionBook");
    writer.writeNamespace("http://www.w3.org/1999/xlink", "l");
    writer.writeNamespace("http://www.w3.org/1999/xlink", "xlink");
    const auto attributes = [&writer](ldomNode *node) {
        for (int i = 0; i < node->getAttrCount(); ++i) {
            const auto *attr = node->getAttribute(i);
            const auto name = qstring(node->getDocument()->getAttrName(attr->id));
            const auto ns = qstring(node->getDocument()->getNsName(attr->nsid));
            if (name == "xmlns" || (ns == "xmlns" && (name == "l" || name == "xlink"))
                    || name.startsWith("data-kobalt-") || name == "HasFirstLetter") continue;
            writer.writeAttribute(ns.isEmpty() ? name : ns + ':' + name,
                                  qstring(node->getDocument()->getAttrValue(attr->index)));
        }
    };
    auto *root = document->getRootNode();
    for (int i = 0; i < root->getChildCount(); ++i) {
        auto *child = root->getChildNode(i);
        if (child->isElement() && child->isNodeName("FictionBook")) {
            attributes(child);
            break;
        }
    }
    writer.writeStartElement("description");
    writer.writeStartElement("title-info");
    if (!language.isEmpty())
        writer.writeTextElement("lang", language);
    writer.writeEndElement(); // title-info
    writer.writeEndElement(); // description
    int characters = 0, visited = 0;
    bool reached = false, valid = true;
    std::function<void(ldomNode *, int, bool)> write = [&](ldomNode *part, int depth, bool firstBlock) {
        if (!valid) return;
        if (part == start) reached = firstBlock = true;
        if ((!reached || (characters >= 8192 && !firstBlock)) && !ancestors.contains(part)) return;
        if (++visited > 20000 || depth > 128 || previewBytes.size() > 2 * 1024 * 1024) {
            valid = false;
            return;
        }
        if (part->isText()) {
            const auto text = part->getText();
            characters += text.length();
            writer.writeCharacters(qstring(text));
            return;
        }
        if (!part->isElement()) return;
        const bool boxing = part->isBoxingNode();
        if (!boxing && part->isBoxingNode(true)) return; // generated pseudo content
        if (!boxing) {
            writer.writeStartElement(qstring(part->getNodeName()));
            attributes(part);
            if (part == start) writer.writeAttribute("data-kobalt-preview-anchor", "1");
        }
        for (int i = 0; i < part->getChildCount(); ++i) {
            write(part->getChildNode(i), depth + 1, firstBlock);
            if (!valid || (reached && characters >= 8192 && !firstBlock)) break;
        }
        if (!boxing) writer.writeEndElement();
    };
    write(body, 0, false);
    writer.writeEndElement(); // FictionBook
    writer.writeEndDocument();
    if (!valid || !reached || writer.hasError() || previewBytes.size() > 2 * 1024 * 1024)
        return fail(strings::get("error.no_section_to_preview"));
    // Restore a codepoint offset within the marked block, not a flattened
    // paragraph. Inline text may be split differently after parsing/boxing.
    auto settings = source.state();
    settings.remove("position");
    userStyleSheet_ = source.userStyleSheet_;
    if (!open(previewBytes, size, settings, error))
        return false;
    ldomNode *block = nullptr;
    pending = {engine_->getDocument()->getRootNode()};
    while (!pending.isEmpty()) {
        auto *part = pending.takeLast();
        if (!part->isElement()) continue;
        if (part->getAttributeValue("data-kobalt-preview-anchor") == U"1") {
            block = part;
            break;
        }
        for (int i = part->getChildCount() - 1; i >= 0; --i) pending.append(part->getChildNode(i));
    }
    if (!block) return fail(strings::get("error.no_section_to_preview"));
    pending = {block};
    ldomNode *boundary = nullptr;
    while (!pending.isEmpty()) {
        auto *part = pending.takeLast();
        if (part->isElement() && part->isBoxingNode(true) && !part->isBoxingNode()) continue;
        if (part->isText()) {
            const int length = part->getText().length();
            if (offset < length) {
                if (goTo(qstring(ldomXPointer(part, offset).toString()))) return true;
                break;
            }
            boundary = offset == length ? part : nullptr;
            offset -= length;
        } else for (int i = part->getChildCount() - 1; i >= 0; --i)
            pending.append(part->getChildNode(i));
    }
    if (!offset && boundary && goTo(qstring(ldomXPointer(boundary, boundary->getText().length()).toString())))
        return true;
    return fail(strings::get("error.no_section_to_preview"));
}
} // namespace kobalt
