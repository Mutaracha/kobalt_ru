# Upstream alignment — stage 1: retired engine patches

Baseline: fork `bfd13a1f`, pinned upstream beta.9 `be472588`.

## Change

Removed `dependencies/crengine-patches.txt` and its 26 `crengine-*.patch` files.
These belonged to the retired external CREngine checkout. The current engine
build uses `dependencies/local-engine.cmake` and `src/engine/blaze`; no production,
build, platform, workflow or test consumer of the retired list/patch names remains.
The series remains available in Git history. This does not assert each historical
hunk has an identical modern implementation: current Blaze is authoritative,
not a source to which the obsolete series should be applied again.

Kept the generic apply-patches helper and unrelated dependency patches: lunasvg
and OpenSSL still use that machinery. Kept dependency recipes, manifest and cache
code byte-identical. The cache already filters crengine patch files out of its
inputs; the retired list is not an input. Removal therefore does not change the
shared dependency cache identity or force an unrelated library rebuild. The
now-empty exclusion is deliberately retained for this isolated step.

Corrected the stale stage-1 header in the engine provenance document. No changes
to Engine/ReaderSession, EPUB Hybrid algorithms, FB2 rendering, schema, cache
versions, resources, strings or waveform.

## Validation

Focused static checks PASS:
- exactly 27 retired files removed;
- no remaining literal consumer in src/dependencies/platforms/tests/.github;
- dependency cache/local-engine/CMakeLists/apply-patches/manifest equal baseline;
- deleted patch files match the pre-existing cache exclusion;
- in-tree Blaze header and build path present.

`git diff --check` is required on the explicit publication patch. Local CMake,
Qt SDK and Docker are unavailable; native/Qt5/package NOT RUN locally. Push CI
pending at publication; no new device acceptance or performance claim.

Next: align shared preset/sample consumers with pinned upstream, carrying the
whole API/helper/test dependency group. FB2 Hybrid preparation remains separate;
this step does not implement or enable it.
