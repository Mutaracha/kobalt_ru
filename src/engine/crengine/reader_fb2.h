#pragma once
#include <QByteArray>
#include <QString>
class ldomNode;

namespace kobalt::fb2 {
// Parsed FB2 cache identity. Bump for incompatible parser/preprocessing or
// default CSS changes. Include the page-note mode; EPUB adds no marker.
inline QByteArray cachePolicy(bool enabled, bool inPage = true) {
    return !enabled ? QByteArray() : inPage ? QByteArray(":fb2-v4:inpage-on:")
                                           : QByteArray(":fb2-v4:inpage-off:");
}
// Only unambiguous numeric labels are normalized; prose/roman labels are kept.
inline QString noteNumber(QString text) {
    const auto original = text;
    text = text.trimmed();
    if (text.size() >= 3 && ((text.startsWith('[') && text.endsWith(']')) ||
                             (text.startsWith('(') && text.endsWith(')'))))
        text = text.mid(1, text.size() - 2).trimmed();
    if (text.isEmpty())
        return original;
    for (const auto character : text)
        if (!character.isDigit())
            return original;
    return text;
}
void normalizeNoteTitles(ldomNode *root);

// Popup recognition is unconditional. Only FB2 uses these overrides.
inline constexpr auto overridesCss = R"rrcss(
body[name="notes"] > section,
body[name="comments"] > section { -cr-hint: footnote; }
a[type="note"] { color: black; }
)rrcss";
// Ready-made KOReader/CREngine pagination, not a separate footer widget.
inline constexpr auto inPageCss = R"rrcss(
body[name="notes"] > section,
body[name="comments"] > section {
    -cr-hint: footnote footnote-inpage;
    margin: 0 !important;
    font-size: 0.8urem;
}
body[name="notes"] > title,
body[name="comments"] > title {
    margin-bottom: 0;
    padding-bottom: 0.5em;
}
body[name="notes"] section > title[data-kobalt-note-number="1"] p,
body[name="comments"] section > title[data-kobalt-note-number="1"] p {
    display: inline;
    vertical-align: super;
    font-size: 65%;
    font-weight: normal;
}
body[name="notes"] section > title[data-kobalt-note-number="1"] p:after,
body[name="comments"] section > title[data-kobalt-note-number="1"] p:after { content: "\00a0"; }
)rrcss";
} // namespace kobalt::fb2
