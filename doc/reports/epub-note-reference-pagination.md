# EPUB page-bottom note can start one page after its reference

## Observed behavior

With in-page footnotes enabled, a reference can be left in the last line of a
page while the entire corresponding note starts at the bottom of the following
page. This is not a continuation of a note that already began beside its reference.

In the attached example from *Кокаиновые короли*, displayed page 22 of 997 ends
with the reference “2” after “Скайлэб”. No part of note 2 is visible there.
The note “Орбитальная космическая станция США.” appears on page 23 instead.
The reference's inline number also makes the final line's spacing worth checking.

Device: Kobo Libra 2, portrait. Exact font, font size and line-spacing values
have not been captured; displayed page numbers are layout-dependent.

## Reproduction

1. Open the supplied EPUB and enable page-bottom notes.
2. Navigate to the passage containing “Скайлэб”.
3. Adjust the reading layout so its reference occurs in the final body line.
4. Observe that the first note line does not fit below it; turn the page.
5. The entire note begins on the next page, separated from its reference.

## Expected behavior

Prefer keeping the reference-bearing body line and at least the first line of
its note on the same page. Move the reference-bearing line to the next page if
necessary. Long-note continuations may still span pages. Oversized unsplittable
objects and pages containing several references require an explicit fallback.

## Relevant application functions

- `EngineDocument::applyStyleSheet()` marks explicit note bodies with
  `footnote-inpage` and uses `font-size: 0.8rem`. It also applies the selected
  line-height to block elements and adds note-specific spacing overrides.
  Inspect the **computed** first note line, including publisher superscript
  spans and backlink labels: the note body's font size alone does not establish
  its actual first-line height.
- `EngineDocument::markPageNotes()` identifies some grouped/labeled notes and
  marks start/continuation paragraphs. Confirm which target and which first
  rendered line are recognized for the supplied publisher markup.
- `ColumnView::noteTop()` translates note fragments to their bottom-of-page
  screen position. This affects painting/hit testing, not the decision about
  which page owns a reference and its note.

## Confirmed pagination path in CREngine

In `crengine/src/lvpagesplitter.cpp`:

1. `PageSplitState2::addLinesToPage()` accepts the main-content lines before
   processing their footnotes via `addLinesFootnotesToPage()`.
2. `PageSplitState2::getAvailableHeightForFootnotes()` subtracts the already
   accepted body height and either existing note height or `footnote_margin`.
   The initial margin is one root-font-size unit (`FOOTNOTE_MARGIN_REM`).
3. In `PageSplitState2::addLinesFootnotesToPage()`, when there are no note lines
   on the page yet and the first note line exceeds that available height, the
   code appends the note to `delayed_footnotes` and continues. It explicitly
   does **not** flush the page in that branch. It does not roll back the body
   line containing the reference either.
4. `pushDelayedFootnotes()` and `addFootnoteToPage()` then place the deferred
   note on a later page. A pending deferred note also causes subsequent notes
   to be deferred, which preserves note order but can propagate the separation.

This branch directly explains how a reference remains on the previous page.
It is a deliberate existing fallback, not evidence of a newly broken link
resolver. No change to the splitter algorithm has been established as the
origin of a version-to-version regression.

## Why styling changes can expose this behavior

Differences between a uniform font-size override covering note descendants and
a font size set only on the note container can change the computed height of
publisher-supplied descendants. Resetting note margins/padding, applying a block
line-height override, and changing superscript/backlink styling can also change
the first-line height. A small change is enough to cross the comparison in
`addLinesFootnotesToPage()` at the bottom of a page.

These are inspection targets, not a proven attribution for this particular
EPUB. The exact threshold should be measured with the same font, viewport and
computed styles. A smaller note can conceal the fallback without fixing it.

## Suggested investigation and regression coverage

- Instrument the reference line's start/end, page bottom, first note-line height,
  reserved separator margin and available note height before the delay branch.
- Consider reserving room for the first note line **before** committing the
  reference-bearing body line. Rewinding after commitment needs to restore
  seen-note bookkeeping, note order and any already-added fragments as well.
- Add a fixture where a short note has its reference on the last candidate line.
  Assert reference and first note fragment belong to the same page when both
  fit on a fresh page; a mere nonzero note count somewhere in the book is not
  sufficient.
- Cover multiple notes on one line, previously delayed notes, long-note
  continuations, two columns, cache reopen, superscript references and a first
  note line too tall to fit even on a fresh page.

The screenshots demonstrate the symptom; a controlled rendering comparison is
still required to attribute the layout difference to an individual style rule.
