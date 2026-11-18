#include "engine_document.h"
#include "reader_fb2.h"
#include "column_view.h"
#include "support/strings.h"
#include <lvdocview.h>
#include <QXmlStreamWriter>
#include <algorithm>
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
    auto *node = pointer.getNode();
    while (node && !node->isNodeName("section") && !node->isNodeName("body"))
        node = node->getParentNode();
    if (!node)
        node = document->getRootNode();
    // Keep the containing paragraph intact (including the text before the
    // bookmark), then map the bookmark into its flattened preview text. Merely
    // slicing at the bookmark would change first-line indentation and shaping.
    auto *start = pointer.isNull() ? nullptr : pointer.getNode();
    while (start && start != node && !start->isNodeName("p") && !start->isNodeName("v"))
        start = start->getParentNode();
    if (!start || start == node)
        start = node;
    int offset = 0;
    bool mapped = false;
    QVector<ldomNode *> pending{start};
    while (!pending.isEmpty()) {
        auto *part = pending.takeLast();
        if (part == pointer.getNode()) {
            mapped = true;
            if (part->isText())
                offset += qBound(0, pointer.getOffset(), part->getText().length());
            else
                for (int i = 0; i < qMin(pointer.getOffset(), part->getChildCount()); ++i)
                    offset += part->getChildNode(i)->getText().length();
            break;
        }
        if (part->isText())
            offset += part->getText().length();
        else
            for (int i = part->getChildCount() - 1; i >= 0; --i)
                pending.append(part->getChildNode(i));
    }
    if (!mapped)
        return fail(strings::get("error.no_section_to_preview"));
    QStringList paragraphs;
    int total = 0;
    constexpr int previewLimit = 8192;
    bool reached = false;
    pending = {node};
    while (!pending.isEmpty() && total < previewLimit) {
        auto *part = pending.takeLast();
        if (part == start) reached = true;
        if (!part->isElement()) continue;
        if ((part->isNodeName("p") || part->isNodeName("v")) && reached) {
            const auto text = qstring(part->getText());
            if (!text.trimmed().isEmpty()) {
                paragraphs.append(text);
                total += text.size();
            }
        } else {
            for (int i = part->getChildCount() - 1; i >= 0; --i)
                pending.append(part->getChildNode(i));
        }
    }
    if (start == node) {
        paragraphs = QStringList{qstring(node->getText()).left(offset + previewLimit)};
    }
    if (paragraphs.isEmpty() || paragraphs.first().trimmed().isEmpty())
        return fail(strings::get("error.no_section_to_preview"));
    const auto language = qstring(source.engine_->getLanguage()).trimmed();
    QByteArray previewBytes;
    QXmlStreamWriter writer(&previewBytes);
    writer.writeStartDocument("1.0", true);
    writer.writeDefaultNamespace("http://www.gribuser.ru/xml/fictionbook/2.0");
    writer.writeStartElement("FictionBook");
    // Match the book's effective root language, including an explicit override
    // of metadata. Never truncate regional/script subtags to two characters.
    auto *root = document->getRootNode();
    for (int i = 0; i < root->getChildCount(); ++i) {
        auto *child = root->getChildNode(i);
        if (child->isElement() && child->isNodeName("FictionBook")) {
            const auto lang = qstring(child->getAttributeValue("lang"));
            if (!lang.isEmpty()) writer.writeAttribute("lang", lang);
            break;
        }
    }
    writer.writeStartElement("description");
    writer.writeStartElement("title-info");
    if (!language.isEmpty())
        writer.writeTextElement("lang", language);
    writer.writeEndElement(); // title-info
    writer.writeEndElement(); // description
    writer.writeStartElement("body");
    writer.writeStartElement("section");
    for (int i = 0; i < paragraphs.size(); ++i) {
        writer.writeStartElement("p");
        if (!i) writer.writeAttribute("id", "kobalt-preview-anchor");
        writer.writeCharacters(paragraphs[i]);
        writer.writeEndElement();
    }
    writer.writeEndElement(); // section
    writer.writeEndElement(); // body
    writer.writeEndElement(); // FictionBook
    writer.writeEndDocument();
    // The synthetic DOM has different paths. Resolve its marked paragraph and
    // restore the mapped text offset rather than reusing the source XPointer.
    auto settings = source.state();
    settings.remove("position");
    userStyleSheet_ = source.userStyleSheet_;
    if (!open(previewBytes, size, settings, error))
        return false;
    auto *anchor = engine_->getDocument()->getElementById(U"kobalt-preview-anchor");
    while (anchor && anchor->isElement() && anchor->getChildCount())
        anchor = anchor->getChildNode(0);
    if (!anchor || !anchor->isText() || offset < 0 || offset > anchor->getText().length())
        return fail(strings::get("error.no_section_to_preview"));
    if (!goTo(qstring(ldomXPointer(anchor, offset).toString())))
        return fail(strings::get("error.no_section_to_preview"));
    return true;
}
} // namespace kobalt
