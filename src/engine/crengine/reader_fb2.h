#pragma once
#include <QByteArray>

namespace kobalt::fb2 {
// Parsed FB2 cache identity. Bump for incompatible parser/preprocessing or
// default FB2 CSS changes, never for a release or code-only move. EPUB adds no marker.
inline QByteArray cachePolicy(bool enabled) {
    return enabled ? QByteArray(":fb2-v2:") : QByteArray();
}
// CREngine already paginates and paints these blocks below the referencing
// text. KOReader enables the same mechanism in css_tweaks.lua (in-page FB2
// notes/endnotes). Keep the separate `footnote` hint for multi-paragraph popups.
// This stylesheet is applied only on the explicit FB2 document path.
inline constexpr auto overridesCss = R"rrcss(
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
a[type="note"] {
    color: black;
}
)rrcss";
} // namespace kobalt::fb2
