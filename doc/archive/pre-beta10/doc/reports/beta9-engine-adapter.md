> АРХИВ: исторический контекст, не текущий план и не инструкция установки.
> Старые pending/next не возобновлять. Текущий источник — FB2_FORK_PLAN.md в корне.

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

The ReaderSession now implements the pinned beta9 Hybrid worker lifecycle,
including estimate publication, interruptible full builds, next-turn adoption,
layout-generation gates and stepped cache saves. Its source is upstream except
for the frameImage publication hook and an explicit atomic include. The opening
job preserves only FB2 raw-resource probe and legacy-notes/per-book-comics merges.
Chapter equality, cancellable StarDict preparation, preview eviction and wallpaper
worker helpers supply the complete session API without new UI controls/strings.

Latest session-integration runtime: `7079c75f5903966e94b9e034fa24e7b8f036bac5`.
[CI 44633797097](https://github.com/Mutaracha/kobalt_ru/actions/runs/44633797097)
passed focused native regressions and Kobo Qt5/offline/package/contents.

The earlier styling/checkpoint sanitation failures are covered by transient-key
cache tests and upstream persisted-state test sanitation. ReadingBook now uses
upstream book-level progress/page coordinates for activity, native percentage,
checkpoints and remaining time; its upstream fragment-aware finish criterion
passes the new non-final-fragment regression and existing appearance-exit checks.

The remaining highlight/markup/native-activity failures exposed an omitted
upstream test-launcher change, not evidence requiring a new EPUB implementation:
beta9 run-test.cmake pins legacy checks to Accurate unless the environment selects
another mode. Restoring that file byte-for-byte preserves the assertions and
leaves explicit Hybrid/Fast test overrides effective. The new FB2 session check
also runs within app.exec, allowing aboutToQuit to stop the engine worker before
Qt teardown. The prior FB2 process-exit cause was not independently isolated.
Both changes were verified together by the final CI.

The passing matrix includes upstream Hybrid/adoption session checks, FB2
Accurate/Hybrid whole-book frames, four turns, reflow, exact display/bookmark
values, semantic preview source identity and the non-final-fragment finish test.
No product rendering change or assertion relaxation was needed in that final
infrastructure correction.

Latest validated extension: `8e241eedf3086dc85beff7743b586a15d3df496d`,
[CI 44637222380](https://github.com/Mutaracha/kobalt_ru/actions/runs/44637222380):
focused native and Kobo Qt5/package PASS. ReadingBook now carries upstream
checkpoint request generations, deferred saves, close snapshots and position-save
epoch callbacks. The upstream-derived close/choice/forced-save regression passes:
a stale completion neither overwrites the newer choice nor acknowledges its epoch.
The optional native failure diagnostic follows upstream storage semantics.

The first run, 44635988130, passed the model regression and Qt5 but found a missing
upstream test barrier: advancedText blocked the worker before pending background
work settled. Porting upstream's pre-gate idle wait preserves the strict no-worker
assertion and the fork's interface-Sans title assertion. The final CI above passed.
Local source checks passed; Kobo syntax could not run without a prepared SDK image.

Latest native-transaction extension: `cb96eb65f80dc556f29bccbb40fd2ffd281121b9`,
[CI 44705458156](https://github.com/Mutaracha/kobalt_ru/actions/runs/44705458156):
focused native and Kobo Qt5/offline/package/contents PASS. The Qt5/Qt6 native
reading-session implementations, progress transaction hook and bridge retry API
are pinned upstream beta9. Exit/USB use upstream progress-before-fallback-time
ordering. Fork unmapped-FB2 and sync policy remain intact. Pewter tests exercise
hook invocation, committed acknowledgment, refused begin and commit rollback.
Source checks passed; local SDK syntax was unavailable. A separate Qt6 package
or device run was not performed.

Host mode preference/application, wake/recovery consumers and wiring of the
model's save-epoch callbacks, activity/calendar, sleep-position and preset consumers,
plus UI/strip/navigation surfaces still need integration. Session/native CI does
not establish complete UI/device adoption. G1–G9 final gates remain open. Full
upstream suite, sanitizers, Android and physical-device validation have not run.
The old post-frame-load freeze is not diagnosed or claimed fixed here.
