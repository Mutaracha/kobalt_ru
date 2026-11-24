# FB2 cached-layout rejection: bounded observation

## Why this step

The 4d9985e2 device upload demonstrates parsed cache hits with ~3.7/16.5 s in the pagination phase. It does not record the reason the engine declines saved layout. `openedFromCache_` is set after LoadDocument and before Render: it cannot certify that Render will reuse the saved page geometry. Also the pagination phase includes applySlice/groupColumns/rebuildComicSpreads; its duration alone is not proof that checkRenderContext failed. Existing CRLog messages are not included in the device diagnostics ZIP (only event/state JSON is present). Consequently no exact invalidation cause has yet been established, and this change makes no speed claim.

## Change

New ordered patch **036-fb2-cache-layout-observation.patch**, after the startup/cache work and before controls/presentation. 035 remains withdrawn. No cache identity/compatibility decisions, CSS, font registration, serialization, rendering policy, source guards or preload eligibility are changed.

A per-document, non-serialized small trace observes the existing `checkRenderContext()` comparisons. The adapter resets the trace immediately before its main `engine_->Render()`, then includes `model_layout_check` in opening resource stats and emits `fb2-cache-layout-check` for FB2. No extra DOM walk/hash evaluation/render pass is introduced: the instrumentation uses already computed operands. Tracing is separate for each document; no new global state/callback/lifetime coupling. This is intentionally narrower than a renderer/cancellation redesign.

`model_layout_check` fields:

- `checks`, `rejected`: number of context checks and observed failed checks during the main Render call; zero checks is distinct from successful reuse.
- `reasons`: OR of independent comparison bits: 1 missing root font, 2 style hash, 4 stylesheet/default style/default font/family-font aggregate hash, 8 document flags, 16 width, 32 height. Multiple conditions may differ even though the old engine logging prints only the first else-if.
- `style/saved_style`, `stylesheet/saved_stylesheet`, `flags/saved_flags`, `width/saved_width`, `height/saved_height`: actual/current operands versus the saved cache-header operands at the first rejection (or first check if none failed). Reasons are a union over checks; operands are not a history of all checks.
- `policy_match`: the adapter rendering-policy comparison before its existing forceReinitStyles branch. A policy mismatch still invokes the same invalidation as before; the boolean does not bypass anything.

Interpretation: only a successful parsed cache hit with checks>0 and rejected=0 supports context compatibility during that call. A nonzero reason identifies a rejection category, not its underlying cause. Bit4 needs further component isolation, not deleting the hash. checks=0 with long pagination directs the investigation outside this context check. The root-font bit is not a general statement about every loaded node font. Saved/current hashes are serialized as exact JSON doubles (all are uint32, safely below 2^53).

## Validation

Resource-backed `fb2_cache_session` now requires the observation to be present and the reason mask to agree with rejection count on cold, warm and changed-source cycles. It retains existing actual close/save/reopen and changed-source isolation assertions. It prints dimensions/reasons/policy for host investigation but deliberately does not assert an unproven 'every cache hit reuses layout' hypothesis. Native regression pass does not reproduce or resolve the device-specific delay.

- Local complete ordered apply to pinned 1796a325: PASS.
- Initial CI45489596816: series and native PASS, Qt5 failed because its old QJsonObject lacks initializer-list construction. Fixed with the repository's existing qt::object compatibility helper. No artifact was recommended from that failed run.
- Runtime **1da4445c4e8d92880b3f3b4a805df3984f0a7ec3**, **CI45491097786: all three jobs PASS** (series/ordered apply/inventory, native selected regressions, Qt5 compile/package/check-package/qemu).
- [Qt5 artifact14965972454](https://github.com/Mutaracha/kobalt_ru/actions/runs/45491097786/artifacts/14965972454), Kobalt-ru-beta10-series-qt5, 44,590,237 bytes.
- Reading this session's native job text via gh was blocked at redirected log-storage hosts; no specific native reason values are claimed from that unavailable text. Job/test success is confirmed by GitHub job result.

## Narrow device gate

This package retains FB2 Home preload and the Instrument Serif About correction. No cold A/B or reboot matrix is requested. Keep files/layout/Accurate/cache unchanged, turn detailed logging ON, manually open **A1 → A2 → A1**, closing each book back to Home after it is readable. Do not clear caches or reboot between them. This alternation avoids treating a same-book retained reopen as disk cache validation; the logs remain authoritative about actual retained/cache path. Stopwatch is optional; this run is for the new rejection operands, not another timing comparison. Then collect diagnostics. If checking About anyway, confirm its literal Kobalt wordmark visually; this is separate from FB2 cache acceptance.

Next: use actual cache-hit render-check evidence to select a bounded correction; preserve all compatibility guards and test changed layout/source isolation. No claim that the slowdown is already fixed. Hybrid correctness checks, complete new Monotype family and persistent menu-status toggle remain queued independently.
