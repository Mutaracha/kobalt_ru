> АРХИВ: исторический контекст, не текущий план и не инструкция установки.
> Старые pending/next не возобновлять. Текущий источник — FB2_FORK_PLAN.md в корне.

# Beta.9 stage 2 — adapter paths and nullable UI boundaries

**Implemented / focused native + Qt5 CI PASS / not a device handoff.**

Runtime: `5bab5b4d0dc64150a9845ba06ae5c785cf0c1d95`.
[CI 44597171704](https://github.com/Mutaracha/kobalt_ru/actions/runs/44597171704)
PASS: offline checks, focused native EPUB/FB2/comics/UI/model matrix, reported FB2
fixture and Qt5 compile/package/contents. Qt6 skipped; Android, full beta.9 suite,
sanitizers, Hybrid and device not run. The generated intermediate package is not
the final test handoff requested by the user.

## Changes and evidence

- Moved 39 files from the old engine/crengine directory. Adapter-specific files
  and fork extensions now live in engine/blaze/adapter. Worker/activity and common
  links/word_counts declarations live in engine root, matching beta.9 ownership.
- Compared every relocated file with its previous content after removing include
  directives: all 39 bodies unchanged. New paths alone do not imply the new
  upstream Engine/ReaderSession implementation is integrated.
- Updated application/platform/test includes, generated-header templates and
  performance source identities. Application globs exclude only the vendored
  library subtrees, so the adapter is compiled and library objects are not doubled.
- Added a nullable-panel visibility predicate for the menu strip, with a safe
  stacking anchor if Appearance has not been constructed. It does not create UI
  panels for a visibility check. Tests cover null Settings/Appearance and hidden
  menu, missing count, Zen, Chapters and Search conditions. Existing menu/reopen
  and strip typography regressions remain selected.
- Prepared [device checklist](../validation/beta9-device.md) at the user's request.
  It is explicitly a draft, not an instruction to test this intermediate runtime.

## Remaining work

The old full-book adapter implementation is still active, now under its target
path. The next stage must integrate beta.9 Engine/adapter/ReaderSession contracts,
including FB2 whole-document open/preview/reflow handling and EPUB Hybrid local
versus book-wide pages. Then come new lazy UI, sleep/Activity/Kobo lifecycle,
translations, regression checks and the actual device package handoff. No
beta.9 completion, runtime Hybrid success or device acceptance is claimed here.
