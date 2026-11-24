> АРХИВ: исторический контекст, не текущий план и не инструкция установки.
> Старые pending/next не возобновлять. Текущий источник — FB2_FORK_PLAN.md в корне.

# Beta.9 migration — stage 1: library compatibility boundary

## Result

**Implemented / focused native + Qt5 CI PASS / not a device handoff.**

Runtime: `866f5b05feb647a3a0c6e492ada774a665efb9c5`.
[CI 44594490713](https://github.com/Mutaracha/kobalt_ru/actions/runs/44594490713):
focused native EPUB/FB2/comics and UI/model regressions, reported-book FB2 check,
offline checks, Qt5 compile/package/contents PASS. Qt6 skipped; Android and physical
device not run. Full beta.9 upstream suite, sanitizers, Hybrid and all-variant
coverage were **not** run by this stage.

The CI package is an intermediate compatibility artifact, not the finished beta.9
fork. It should not be substituted for the last device-testing handoff. No release,
tag or pull request was created.

## What actually changed

- Imported 175 library/build/data/license files from upstream beta.9
  `be47258850ffdc3295257475afdf8b8f6cb53176`. Before publishing, all 175 file contents
  were compared byte-for-byte against that tree. The application adapter was not
  copied at this stage. Vendor whitespace is preserved rather than reformatted.
- Enabled upstream `dependencies/local-engine.cmake` for desktop, Kobo and Android.
  Library objects stay under platform output; application source globs exclude
  the imported directory. Build dependencies order generated headers/archive
  before application objects and cross-platform FreeType symbol-renaming steps.
- Adopted beta.9 external dependency recipes/cache without external CREngine
  download/build. Existing engine patches remain temporarily tracked as retired,
  unused inputs; there is no second application of that patch stack.
- Generated default CSS/pattern names from Blaze `data`, retaining the fork's FB2
  stylesheet template and actual shipped EN/RU hyphenation assets.
- Updated the cache checksum format from 11 to 13 and rendering policy to upstream
  version 12; cache regression verifies the changed checksum identity. Saved book
  settings, bookmarks, annotations and source positions are not deleted or migrated.
- Updated acknowledgements/provenance and the source-archive missing/damaged
  external-dependency probe (harfbuzz rather than the no-longer-downloaded engine).
  The Git-free archive test itself was not executed in this focused CI.

## First integration failure and correction

Initial runtime `8b86ca0d` compiled for native and Qt5; offline/package checks
passed, but native `engine.page-layout` failed at the kept-block chapter boundary:
“A kept block at the end of a chapter made an empty page”.

Blaze makes the removal of zero-height pages an explicit adapter policy. Beta.9
calls `setKoblazeDropEmptyPages(true)` before Render. The intermediate fork adapter
now makes that same call behind `KOBLAZE_DROP_EMPTY_PAGES`. Neither vendor source
nor the page-layout assertion was changed to suppress the failure. The rerun at
866f5b05 passed, including the later reported-FB2 integration step.

## What this does not migrate

The existing whole-book adapter remains under `src/engine/crengine`. New upstream
`Engine`/ReaderSource/ReaderSession APIs, Hybrid/spine slicing/background adoption,
Activity/highlight notes, lazy Settings, custom sleep editor, Kobo lifecycle and
the rest of the beta.9 UI are not integrated yet. The application version is not
bumped to pretend those changes are present. This stage intentionally isolates
library compatibility from the next architectural change; it does not replace the
chosen final beta.9 EPUB pipeline with the old one.

Next: bring in the new upstream adapter/session, relocate fork adapter extensions,
and implement the FB2 whole-document contract from
[beta9-engine-contracts.md](beta9-engine-contracts.md). Recheck G1–G9 there: stage 1
is not a PASS for those final integration gates.
