#pragma once
#include "support/qt_compat.h"
#include <QByteArray>
#include <QRegularExpression>
#include <QVector>
#include <lvtinydom.h>

namespace kobalt::notes {
// Shared with popup recognition: semantic notes only, never arbitrary numeric
// links/chapter targets. Namespace prefixes are not significant in EPUB.
inline bool token(ldomNode *node, const char *name, const char *value, bool anyNamespace = false)
{
    static const QRegularExpression whitespace("\\s+");
    const auto attribute = anyNamespace
            ? node->getAttributeValue(LXML_NS_ANY, node->getDocument()->getAttrNameIndex(
                      Utf8ToUnicode(name).c_str()))
            : node->getAttributeValue(name);
    return QString::fromUtf8(UnicodeToUtf8(attribute).c_str())
            .split(whitespace, qt::skipEmptyParts)
            .contains(value, Qt::CaseInsensitive);
}

inline bool noteElement(ldomNode *node)
{
    return token(node, "type", "footnote", true) || token(node, "type", "endnote", true)
            || token(node, "type", "note", true) || token(node, "type", "rearnote", true)
            || token(node, "role", "doc-footnote") || token(node, "role", "doc-endnote")
            || token(node, "role", "doc-note") || token(node, "role", "doc-rearnote")
            || token(node, "class", "footnote") || token(node, "class", "endnote");
}

inline QByteArray epubCachePolicy(bool fb2, bool inPage)
{
    return fb2 ? QByteArray() : inPage ? QByteArray(":epub-notes-v1:inpage-on:")
                                      : QByteArray(":epub-notes-v1:inpage-off:");
}

// Called on fresh EPUB DOMs, after initial styles, before final style init.
// Keep text, IDs, links and XPointers intact. Mark independently of the switch
// so toggling needs only stylesheet reflow, never a reload or another DOM walk.
inline void markEpubNotes(ldomDocument *document)
{
    const auto attribute = document->getAttrNameIndex("data-kobalt-inpage-note");
    struct Frame { ldomNode *node; bool inNote; };
    QVector<Frame> pending{{document->getRootNode(), false}};
    while (!pending.isEmpty()) {
        const auto frame = pending.takeLast();
        auto *node = frame.node;
        if (!node || !node->isElement())
            continue;
        const auto style = node->getStyle();
        // Do not trust source-supplied private markers, even inside a note.
        if (node->hasAttribute(attribute))
            node->setAttributeValue(0, attribute, U"");
        const bool excluded = !style.isNull() && STYLE_HAS_CR_HINT(style, FOOTNOTE_IGNORE);
        const bool hinted = !style.isNull() && STYLE_HAS_CR_HINT(style, FOOTNOTE);
        const bool marked = !frame.inNote && !excluded && !node->isNodeName("a")
                && (noteElement(node) || hinted);
        if (marked) {
            node->setAttributeValue(0, attribute, U"1");
            // One complete note, not overlapping fragments for nested markup.
        }
        for (int i = node->getChildCount() - 1; i >= 0; --i)
            pending.append({node->getChildNode(i), frame.inNote || marked});
    }
}

// Existing KOReader/CREngine page splitter owns reservation, separator,
// continuation and rendering. Unreferenced notes are not page footnotes.
inline constexpr auto epubInPageCss = R"rrcss(
[data-kobalt-inpage-note="1"] {
    -cr-hint: footnote footnote-inpage;
    margin: 0 !important;
    font-size: 0.8urem;
}
)rrcss";
} // namespace kobalt::notes
