# Beta9 Engine / Blaze adapter integration

## Scope and provenance

Pinned upstream: `be47258850ffdc3295257475afdf8b8f6cb53176` (beta9).
The Engine interface and factory now have a concrete implementation, not an
uncompiled header. The Blaze adapter includes upstream chapter windows, slices,
page estimation, interruptible full-document preparation/adoption, search,
source locations, turn previews and text samples. Its old public entry points
remain usable by the current ReaderSession while that session is migrated.

The vendor library/data/extensions are unchanged in this adapter checkpoint.
Fork-only FB2 semantic preview/alignment/notes/styles and comics geometry modules
remain separate. Overlapping shared adapter code is merged against the beta8
base; the general EPUB algorithms remain upstream implementations.

Related dependencies are integrated with their consumers: PageMap/estimation,
ResourceCache fresh reloads, host PositionCodec via Engine, native source cache,
font-input identities and interrupted-operation diagnostics. The existing
reason-bearing cache-save overload remains for the old ReaderSession; the Engine
API also implements the upstream no-argument and stepped cache saves.

## Fork boundaries

- FB2 format is selected before configure/CSS. It loads its raw XML stream and
  bypasses EPUB mimetype/container checks. It never becomes an artificial spine.
- Every FB2 open clears runtime windows/slices, including inherited saved/runtime
  keys and mode changes. `setChapterMode` applies the appearance and viewport by
  an actual whole-book replacement with source anchor restoration, not a no-op.
- Whole-book guards suppress redundant full-document builds/refinement/rough page
  estimates. The Engine page query is exact; `bookPages` remains one-based.
- `openPreview(Engine&)` dispatches to the existing semantic FB2 writer before
  EPUB DocFragment lookup. The source DOM is not rewritten.
- Comics screens retain pairing, standalone/wide-image geometry, hit identity,
  and bounded pair loading. Nonlinear column grouping falls back to upstream.
  Target previews load their own request set without navigating the reading
  document. Layout signatures include source document, viewport and fork grouping.
- The shared PageDrawBuffer remains a single extracted primitive, with upstream
  unsupported-format tracking added. No flash/waveform workaround is introduced.

## Derived state and saved data

The adapter can emit upstream `page_estimate` even for a whole-book EPUB, for a
future chapter-mode reopening. The complete upstream persistedCheckpoint method
is therefore included now: transient display fields are removed, then oversized
estimates lose `read`, then the optional estimate, before image views are trimmed.
The source anchor and unrelated user data retain priority. The background writer
and storage schemas are unchanged. See the field contract in `doc/storage.md`.
Style sample cache keys exclude window/estimate/frame-only state as upstream does.

## Validation status

Initial CI `44610120056` stopped at compile dependencies: ResourceCache::fresh
and a Qt5 refresh test-double signature. They were corrected without bypassing
checks. CI `44611131876` passed Qt5/offline/package/contents; native linking found
the missing explicit desktop source entry for page_estimate.cpp. That source is
now linked; no test or linker failure was suppressed.

CI `44612383960`: Qt5/package PASS; native runs reached CTest and failed
the new FB2 Engine-boundary page mapping assertion. All other selected checks
passed. The assertion now reports mapped page, current page and source anchor
to distinguish a mapping regression from a page-boundary test assumption.
The reported anchor was /?xml[1].0, an XML processing instruction rather than
a rendered body passage (mapped=2/current=0). The test now selects a real body
passage through Engine search/goTo before asserting the page/one-based mapping
and repeating after reflow. The equality assertion remains strict. This is a
fixture correction, not an initial-bookmark renderer fix.

The focused workflow includes upstream engine-contract, spine-window and
chapter-slices tests in addition to the previous FB2/comics/links/layout/session
matrix. Added checks exercise actual engine-only chapter -> full build ->
prepared preview -> adoption, raw FB2 mode/reflow/exact pages/preview lifetime,
comics preview pair loading, reload fingerprint/revocation, and the upstream
checkpoint byte-budget regression. The earlier source-location corpus remains.

Final factory-consumer runtime: `4dad4fdf33f868081a89cbac72e16c3280d54c43`.
[CI 44619223088](https://github.com/Mutaracha/kobalt_ru/actions/runs/44619223088)
passed focused native regressions and Qt5/offline/package/contents checks.
This supersedes the intermediate missing direct UI includes in 44615981873 and
44618298018 (reading_page.cpp needs both engine/links.h and engine/types.h).
The preceding adapter/body-passage regression checkpoint 44f57097 also passed
native and Qt5 in CI 44615814323. No device acceptance is inferred from either.

## Still pending — not a device handoff

The ReaderSession, opening job and style sample consumers now own Engine values
created by createEngine. Their previous whole-book scheduling remains; they do
not yet implement the beta9 Hybrid reader lifecycle. Worker-affine shared deletion
and retained-open ownership are unchanged. The fork frameImage hook has an
image(page) default and an adapter override for complete bounded comics pairs.
Comics readiness tests call that hook through Engine&.
The rendering-mode preference, new ReaderSession values and UI/strip consumers
must still be integrated together. Engine-only Hybrid tests do not establish
reader/UI/device adoption. G1–G9 final gates remain open. Full upstream suite,
sanitizers, Android and physical-device validation have not been run.
The old post-frame-load freeze is not diagnosed or claimed fixed here.
