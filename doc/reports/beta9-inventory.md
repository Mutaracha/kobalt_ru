# beta9 file inventory — intermediate, not semantic completion

Fork source: `55e80b5483ceb500e1d1d4672b4adc2de61e15cf`.
Pinned upstream: `be47258850ffdc3295257475afdf8b8f6cb53176`.
Compared all paths in the fork Git tree with the pinned upstream source archive;
identical means exact file bytes. No live dirty-root files used as baseline.

Whole-tree counts (includes docs and user diagnostic uploads):
- different: 289
- fork-only: 149
- identical: 1037
- upstream-only: 57

Vendored `src/engine/blaze/crengine/` counts:
- identical: 139

[TSV](beta9-inventory.tsv) lists all 353 nonidentical runtime/build/assets/
test paths, including fork-only and upstream-only paths. Docs/user evidence are
excluded only from this detailed list, not from whole-tree counts above.

## Reviewed group this checkpoint

Nine whole upstream files are exact: widget keyboard cpp/h, keyboard factory API,
native keyboard header and Qt5/Qt6 cpp, ModalOverlay cpp/h, ChoiceSheet cpp.
Platform service factory boundaries and two model-test factories have only
QLineEdit-to-QWidget keyboard parameter/owner changes this checkpoint; unrelated
platform services remain as before. Fork divider/checkbox presentation retained.

Vertical-spacing showStylingChoices and dismissSheet match the pinned upstream
function bodies exactly. This does not prove the surrounding close/apply/input
lifecycle correct on the affected device. Stronger tests and investigation remain
necessary; no freeze fix claimed.

## Semantic audit still open

The TSV is a complete path/byte comparison for this checkpoint, NOT a declaration
that every remaining delta is intentional. Each different/upstream-only path still
needs retained-fork / dependency-port / obsolete / blocked ownership review.
Annotations/note editor, remaining KOSync policy and sleep/storage are not closed.
Existing historical beta8 inventory is not substituted for this gate. No vendor
or EPUB Hybrid algorithm edits made to reduce the diff mechanically.
