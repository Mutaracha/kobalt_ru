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


## Implemented foundation checkpoint (CI pending)

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
