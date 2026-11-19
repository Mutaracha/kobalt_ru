#include "reader_fork_styles.h"

namespace kobalt::forkstyles {
const char *fb2LineHeightSelectors()
{
    return ",v,title,subtitle,epigraph,cite,annotation,text-author";
}

const char *fb2NoteSelectors()
{
    return R"css(,
body[name="notes"] section, body[name="comments"] section)css";
}

const char *noteLabelsCss()
{
    return R"css(/* Raise only the note's own label, never every link or inline number. */
a.label, a._idFootnoteAnchor {
    -cr-hint: late;
    -cr-only-if: inside-inpage-footnote;
    vertical-align: super !important;
    font-size: 0.75em !important;
}
body[name="notes"] section > title[data-kobalt-note-number="1"],
body[name="comments"] section > title[data-kobalt-note-number="1"] {
    -cr-hint: late;
    -cr-only-if: inside-inpage-footnote;
    display: run-in !important;
    font-size: inherit !important;
    font-weight: normal !important;
}
body[name="notes"] section > title[data-kobalt-note-number="1"] p,
body[name="comments"] section > title[data-kobalt-note-number="1"] p {
    -cr-hint: late;
    -cr-only-if: inside-inpage-footnote;
    display: inline !important;
    vertical-align: super !important;
    font-size: 0.75em !important;
    font-weight: normal !important;
}
body[name="notes"] section > title[data-kobalt-note-number="1"] p:after,
body[name="comments"] section > title[data-kobalt-note-number="1"] p:after {
    -cr-hint: late;
    -cr-only-if: inside-inpage-footnote;
    content: "\00a0";
}
)css";
}
} // namespace kobalt::forkstyles
