#include "engine_document.h"
#include "reader_fb2.h"
#include "column_view.h"
#include "support/strings.h"
#include <lvdocview.h>
#include <QXmlStreamWriter>
#include <algorithm>

namespace kobalt {
namespace {
QString qstring(const lString32 &s) {
    return QString::fromUtf8(UnicodeToUtf8(s).c_str());
}
lString32 cre(const QString &s) {
    return Utf8ToUnicode(s.toUtf8().constData());
}
} // namespace

void fb2::normalizeNoteTitles(ldomNode *root) {
    // Iterative traversal: neither source nesting nor title markup grows the
    // C++ call stack. Keep all DOM nodes/IDs so XPointer structure is unchanged.
    if (!root)
        return;
    const auto marker = root->getDocument()->getAttrNameIndex("data-kobalt-note-number");
    QVector<QPair<ldomNode *, bool>> pending{{root, false}};
    while (!pending.isEmpty()) {
        const auto entry = pending.takeLast();
        auto *node = entry.first;
        if (!node || !node->isElement())
            continue;
        bool notes = entry.second;
        if (node->isNodeName("body")) {
            const auto name = node->getAttributeValue("name");
            notes = name == U"notes" || name == U"comments";
            if (!notes)
                continue;
        }
        // FB2 run-in titles can already be inside an engine autoBoxing
        // after parsing. Ignore only engine boxes, never publisher containers.
        auto *parent = node->getUnboxedParent();
        if (notes && node->isNodeName("title") && parent && parent->isNodeName("section")) {
            const auto before = qstring(node->getText());
            if (before.size() <= 32) {
                const auto number = fb2::noteNumber(before);
                const bool numeric = !number.isEmpty() &&
                    std::all_of(number.cbegin(), number.cend(), [](QChar c) { return c.isDigit(); });
                node->setAttributeValue(0, marker, numeric ? U"1" : U"");
                if (numeric && number != before) {
                    QVector<ldomNode *> leaves{node};
                    bool first = true;
                    while (!leaves.isEmpty()) {
                        auto *leaf = leaves.takeLast();
                        if (leaf->isText()) {
                            const bool blank = qstring(leaf->getText()).trimmed().isEmpty();
                            leaf->setText(first && !blank ? cre(number) : lString32());
                            if (!blank)
                                first = false;
                        } else {
                            for (int i = leaf->getChildCount() - 1; i >= 0; --i)
                                leaves.append(leaf->getChildNode(i));
                        }
                    }
                }
            }
            continue;
        }
        for (int i = node->getChildCount() - 1; i >= 0; --i)
            pending.append({node->getChildNode(i), notes});
    }
}

bool EngineDocument::openFb2Preview(EngineDocument &source, QSize size, QString *error) {
    auto fail = [error](const QString &message) {
        *error = message;
        return false;
    };
    auto *document = source.engine_->getDocument();
    auto pointer = document->createXPointer(cre(source.position()));
    auto *node = pointer.isNull() ? nullptr : pointer.getNode();
    while (node && !node->isNodeName("section") && !node->isNodeName("body"))
        node = node->getParentNode();
    if (!node)
        node = document->getRootNode();
    QStringList paragraphs;
    int total = 0;
    constexpr int previewLimit = 8192;
    for (int i = 0; i < node->getChildCount() && total < previewLimit; ++i) {
        auto *child = node->getChildNode(i);
        if (!child->isElement())
            continue;
        const auto name = qstring(child->getNodeName());
        if (name.compare("p", Qt::CaseInsensitive) == 0 || name.compare("v", Qt::CaseInsensitive) == 0) {
            const auto text = qstring(child->getText()).trimmed();
            if (!text.isEmpty()) {
                paragraphs.append(text);
                total += text.size();
            }
        }
    }
    if (paragraphs.isEmpty()) {
        const auto text = qstring(node->getText()).left(previewLimit).trimmed();
        if (text.isEmpty())
            return fail(strings::get("error.no_section_to_preview"));
        paragraphs.append(text);
    }
    const auto language = qstring(source.engine_->getLanguage()).trimmed();
    QByteArray previewBytes;
    QXmlStreamWriter writer(&previewBytes);
    writer.writeStartDocument("1.0", true);
    writer.writeDefaultNamespace("http://www.gribuser.ru/xml/fictionbook/2.0");
    writer.writeStartElement("FictionBook");
    writer.writeStartElement("description");
    writer.writeStartElement("title-info");
    if (!language.isEmpty())
        writer.writeTextElement("lang", language.left(2));
    writer.writeEndElement(); // title-info
    writer.writeEndElement(); // description
    writer.writeStartElement("body");
    writer.writeStartElement("section");
    for (const auto &paragraph : paragraphs)
        writer.writeTextElement("p", paragraph);
    writer.writeEndElement(); // section
    writer.writeEndElement(); // body
    writer.writeEndElement(); // FictionBook
    writer.writeEndDocument();
    // The synthetic document has its own DOM, so the original XPointer cannot
    // be restored. open() recognizes the bytes as FB2 and skips the EPUB path.
    auto settings = source.state();
    settings.remove("position");
    userStyleSheet_ = source.userStyleSheet_;
    return open(previewBytes, size, settings, error);
}
} // namespace kobalt
