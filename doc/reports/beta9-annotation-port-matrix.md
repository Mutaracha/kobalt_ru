# Annotation alignment: dependency/consumer review

Baseline: fork cba1e229, upstream be472588. This is a source-level review, not a
completed port or a device test. Freeze investigation is deferred by the user.

## Already available prerequisites

- Generic QWidget keyboard factory and widget/native QLineEdit/QTextEdit support
  restored in 55e80b54 (explicit dictionary forward declaration in 0b2e5156).
- ReaderSession already has upstream BookmarkLocations.pages/order/bookPages/
  chapters and bookmarkLocations options/cancellation. Its cpp differs from
  upstream only by an explicit atomic include and frameImage() comics hook.
  Do not rewrite reader_slices or Hybrid just to consume this existing API.
- highlight_geometry.h and render/highlight_paint.h are byte-identical upstream.
  They need no annotation-specific fork replacement.

## Coordinated restoration order

| Group / files | Actual remaining difference | Consumer / validation gate |
|---|---|---|
| ui/controls/rows.{h,cpp} | Missing ListRow::setNote, noteHeight/Lead/Gap, label/style/layout/icon; also upstream SegmentedRow::prepend and empty-title control layout. Light secondary text differs. | Restore whole shared upstream implementation after checking all row consumers, not only annotation rows; UI theme/list geometry tests at 212/300 ppi, light/dark. |
| storage/highlight_store.{h,cpp} | Full records already retain note; compactNote flag, hasHighlightNote and note in HighlightExcerpt missing. | Compact vs full record reads must agree; do not invent another note store or migrate to a duplicate pipeline. Storage roundtrip/full+compact tests. |
| reading/highlight_session.{h,cpp} | Missing setNote/editor-owned retry and saved callback; bookPages discarded; chapter assignment still walks chapters instead of consuming returned chapter. | Consume existing bookmarkLocations (not new engine algorithms); retain note on range adjustment, save rollback/retry and simultaneous pending writes; upstream session.highlight-session cases including note length/whitespace. |
| app/reader/panels/bookmark_list.{h,cpp} | Missing note metadata, pending page navigation, note-aware measured rows and public excerptFont. | Depends on rows and note-bearing excerpt/session data. Test lazy rows, page boundaries, navigation before exact page available, clear reused note labels. |
| app/reader/panels/note_editor.{h,cpp} | Entire files upstream-only. Owns QTextEdit/controller, passage quote, save state, error retention, deferred owner destruction. | Whole upstream implementation; tests/reader_ui.cpp noteEditorPassageLines plus multiline input and close while saving/error. Add build source discovery/explicit registration checks. |
| app/reader/window/reader_annotations.cpp + reader_p.h | Missing note actions/popover text, editor ownership, save/close callbacks; stale page/chapter consumers. | Contextual integration across build/refresh/input/sleep/exit paths; no detached pasted cpp with missing fields. Preserve accepted labeled fork highlight palette. |
| ui/controls/highlight_palette.h | Fork retains visible translated color/style captions and larger cells; upstream has unlabeled swatches/compact height and static HighlightStyleSample::height(). | Intentional presentation delta, NOT safe to overwrite with upstream. Reconcile upstream geometry consumers with caption-bearing cell API; test text/selection clipping. |

## Important non-solutions

1. Copying only bookmark_list.cpp does not compile without ListRow note APIs and
   cannot display notes without storage/session excerpt data.
2. Copying only reader_annotations.cpp drops required private owner fields and
   palette APIs. A local mock note implementation would duplicate upstream.
3. Existing Highlight.note storage is not evidence that editable notes and compact
   note previews are implemented. The model/UI pipeline is incomplete.
4. The chapter-location change can consume the ReaderSession API already present;
   it is not permission to modify EPUB Hybrid rendering or enable FB2 partial layout.
5. Model/UI passes are distinct from Kobo native keyboard ABI/device verification.

## Status

Reviewed prerequisites and boundaries; restoration remains in progress, not done.
Next implementation unit: shared row support + storage/session note metadata and
its existing upstream tests, then BookmarkList/NoteEditor and Reader consumers.
Sleep/exit changes that are required for editor ownership belong to that consumer
integration; unrelated standby behavior and the deferred freeze are not reopened.


## Implemented foundation checkpoint (CI verified, device pending)

Whole upstream files restored: rows.h/cpp, highlight_store.h/cpp,
highlight_session.h/cpp, bookmark_list.h/cpp (8 files). No parallel note storage.
Reader annotationRows/bookmarkRows functions and annotationDetails callback now
use whole upstream bodies against existing ReaderSession APIs. All other Reader
annotation lifecycle/gesture behavior remains unchanged at this checkpoint.

Restored upstream compact-note assertions/setNote tests in highlight_session,
retaining fork test harness scheduling/diagnostic repairs. List tests are whole
upstream methods. Updated the two model ListRow secondary-color assertions to
upstream's plain light ink / secondary dark ink policy. Added annotationNoteRows:
hydrated note bounds, clearing on paging/reuse, no stale note text, smaller row
without note, selectable pending page number (density/theme variants).
Focused CI builds/runs highlight-store and choice/list UI tests as well as the
previous checks; they were not all selected in the preceding package.

Full NoteEditor and quick-annotation Reader integration remain OPEN. The model
setNote API and saved/editor-owned retry support are prerequisites, not a claim
that the reader already exposes an editable-note action. This package adds
correct previews of notes already stored/imported and shared list/model support.

The database consumer is part of this group too: whole upstream
annotation_database.cpp restored (header/tests already identical). Compact SELECT
projects note presence and excerpt SELECT reads/validates note type/length/bytes;
without this consumer the restored model would still lose SQLite note metadata.
No database schema migration. Focused annotation-database checks added.


Final runtime `4e014dc57c04ec35a5d3dd3e0cba7f73fa91b698` / build 254:
[CI 45033749297](https://github.com/Mutaracha/kobalt_ru/actions/runs/45033749297) SUCCESS: native 9m55s;
Qt5/offline/package/contents 5m01s, artifact 14758934067 (37359704 bytes).
Qt6 skipped; Android/full suite/device not run. Focused selection includes
annotation-database/highlight-store/highlight-session, component.ui, list note
variants and choiceSheet/modal divider/keyboard tests, plus existing selected
EPUB/FB2/settings/lifecycle/desktop checks. Does not claim all Reader annotation
interaction suites (desktop.annotation* is not in this regex).

Intermediate 29487359 / CI 45033710524: Qt5 passed, native failed the restored
compact-note assertion. Whole upstream SQLite consumer in 4e014dc5 resolves the
missing projection; the assertion is retained and the follow-up run passes.


## Note editor integration checkpoint (CI verified, device pending)

NoteEditor.h/cpp restored whole from pinned beta9. Reader openNoteEditor,
showSavedAnnotationAt, showSavedAppearance and action-card implementation restored
with contextual boundaries: quick-mode settle/reopen calls excluded until the
separate gesture port; existing fork section headings/labeled palette retained;
existing common.cancel used where upstream introduces Discard (no unapproved new
copy). Draft Add/Edit note actions use upstream save-passage-first sequencing.
Upstream saved/saveFailed callbacks deliver editor completion/error, preserving
its text and shared model retry contract. No second note pipeline.

Upstream handle hit/dot region helper and syncPopoverExclusions restored so card
buttons own their area while only drawn dots pass through over the card. This is
a note-menu input dependency, not a claimed fix of the deferred Settings freeze.

Five new strings explicitly approved by the user in-session: Добавить заметку;
Изменить заметку; Напишите заметку; Не удалось сохранить заметку. Ваш текст
сохранён в редакторе.; Заметка слишком длинная для сохранения. English unchanged
from upstream. No other new annotation wording imported implicitly.

Tests: upstream noteEditorPassageLines, standalone noteEditorSaveState; upstream
annotationNotes integration test excluding the not-yet-ported initial drag/reopen
block and using existing Cancel caption in its assertion. Focused CI adds note
editor, palette label, annotationNotes, annotations/selection/grip checks.
Quick annotations and delayed menu reopen remain open. Native/Qt5/device pending.

Initial NoteEditor CI 45036473875 / runtime 86cd111f: Qt5/offline/package passed,
native CMake configuration rejected new ui.noteEditorPassageLines without an area
label. Added noteEditor.* to the existing annotations test area and explicit
one-line descriptions for the new tests; no test removed or assertion relaxed.
Follow-up native verification required before handoff.

Second CI 45037014565 / 3357f0ba: Qt5 passed; native found two stale
integration expectations. annotationSelection assumed Save directly adjoined
Cancel, but Add note now correctly sits between them; assertions now require
both exact boundaries and apply label-fit checks to all three buttons.
annotationNotes assumed the later upstream automatic menu-on-release gesture;
this port retains toggle-mode's explicit tap, so the test now taps the fresh
selection before opening Add note. No production gesture/timing workaround.


Final NoteEditor runtime 09307e421d649527cdcaa6854255e335f9861409 / build 257:
[CI 45037956822](https://github.com/Mutaracha/kobalt_ru/actions/runs/45037956822) SUCCESS; native 11m00s,
Qt5/offline/package/contents 4m51s. Artifact 14760328232, 37377574 bytes.
The selected note editor and desktop note/annotation/selection/grip integration
checks pass together with previous selected regressions. Qt6 skipped; full suite,
Android and physical device not run. Quick-mode/menu-reopen and remaining shared
implementations are still open; this is not a completed all-file semantic audit.


## Quick/toggle and drag consumer group — CI pending

Reader annotation implementation now follows pinned upstream as a whole except
retained section headers/labeled palette, common.cancel and existing dictionary
caption. Upstream quick preference, Settings selector, feature normalization,
navigation/button gating, temporary-mode settle callbacks, delayed reopen timer
and pressed-handle geometry restored together. Six new RU selector strings were
explicitly approved in session; English comes verbatim from pinned beta9. Section
caption reuses existing features.annotations (no extra unapproved wording).

Qt5 refresh imports upstream drag-only direct/partial submission and one text
waveform cleanup over accumulated drag damage; it does not overwrite the fork's
comics refresh or page-turn policy. The upstream reopen cleanup request is guarded
by a valid released selection and active reader, not unconditional idle flashing.
Qt5 ABI tests now assert direct step and exactly one REAGL/full cleanup. Device
waveform/ghosting behavior remains unverified and must be checked separately.

Whole upstream annotationNotes/Selection/Grip/quick tours updated; selection tour
retains the fork caption/section geometry assertions instead of expecting their
absence. preference tests cover valid/invalid and feature-hidden quick state.
The prior package's explicit-tap limitation is superseded, not silently kept in
the new tests. Remaining KOSync, sleep/storage and full semantic inventory OPEN.
