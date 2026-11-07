#pragma once
#include "support/qt_compat.h"
#include <QByteArray>
#include <QRegularExpression>
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
    if (attribute.empty())
        return false;
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
    return fb2 ? QByteArray() : inPage ? QByteArray(":epub-notes-v2:inpage-on:")
                                      : QByteArray(":epub-notes-v2:inpage-off:");
}

// Standard EPUB semantics, resolved by CREngine's own CSS/footnote pipeline.
// KOReader css_tweaks.lua (9040116701c8): footnote-inpage_epub and
// inpage_footnote_font-size_smaller. Popup hints/classification stay separate.
inline constexpr auto epubInPageCss = R"rrcss(
*[type~="note"], *[type~="endnote"], *[type~="footnote"], *[type~="rearnote"],
*[role~="doc-note"], *[role~="doc-endnote"], *[role~="doc-footnote"], *[role~="doc-rearnote"] {
    -cr-only-if: -fb2-document;
    -cr-hint: footnote-inpage;
    margin: 0 !important;
}
*, autoBoxing {
    -cr-hint: late;
    -cr-only-if: -fb2-document inpage-footnote;
    font-size: 0.8urem !important;
}
)rrcss";
} // namespace kobalt::notes
