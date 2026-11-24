# 1da4445c: device render-context evidence

Input: original `test & diagnostic/1da4445c/diagnostics.zip`, uploaded in 74e25306. 3772 events across current/previous JSONL, sorted by UTC. Both startup events identify unmodified runtime1da4445c, verbose=true. User reports completing the requested check, without new stopwatch values or explicit About-font acceptance.

## Observed results

| UTC | Path | Parsed cache | Context reasons | Style current / saved | Pagination ms | Session total ms |
|---|---|---|---:|---|---:|---:|
| 21:51:00.525 | first startup background preload | hit | 2 | 3058107977 / 1217214335 | not assigned to UI | not a reading session |
| 21:51:24.097 | manual A1 | hit, retained=false | 2 | 2216272813 / 988614183 | 3659 | 8140 |
| 21:52:01.530 | manual A2 | hit, retained=false | 2 | 2966508233 / 3234035049 | 16810 | 21823 |
| 21:52:17.347 | manual A1 again | hit, retained=false | 0 | 2216272813 / 2216272813 | 10 | 4023 |
| 21:53:12.837 | after second startup, background preload A1 | hit | 2 | 3539955513 / 2216272813 | not assigned to UI | not a reading session |

All five observations have checks=1. Reasons2 means exactly the style-hash comparison fails; it is no longer merely inferred from the pagination duration. In every case dimensions are 1240x1613 and flags=27, identical to saved values; root font is present; policy_match=true; stylesheet current/saved agree. The first preload stylesheet is 4284261554, whereas the subsequent four observations use 1662611122: do not claim a single unchanged global state across the whole archive. For each individual check, however, stylesheet equals its own cached value.

All five cache-save events succeed (15, 1, 1, 1, 10 ms). The repeated foreground A1's saved_style is exactly the style value written after the preceding A1 render. On the next application startup the same saved_style is rejected with a different current style. Thus the device can reuse a saved A1 layout on disk within this process; the fast repeat is explicitly not retained reuse. This is a narrower result than 'cache is always broken', and not proof that any particular user setting changed.

First preload completes with ready=true in18508ms; second in6420ms. No cancelled-preload wait is observed in this small series. These are internal background engine operations, not extra manual stopwatch/UI-reading times. The three reported session totals are log boundaries, not manually measured times; the user supplied no new stopwatch figures.

## What is established / not established

Established: the current repeated-layout problem in these samples is the **style-hash guard**, not page dimensions, document flags, rendering-policy mismatch or the separately checked stylesheet aggregate. The second manual A1 has a compatible context and a ~10ms pagination phase. After a process restart its saved hash is incompatible again. This observation justifies examining style-hash constituents and cross-process reproducibility rather than modifying preload or forcing reuse.

Not established: which style-hash constituent changes, why, or whether every change is spurious. `calcStyleHash()` includes node style/font hash, image/text spacing/render flags/interline, and a global settings hash. That global hash includes registered-font/fallback inventory, language/hyphenation and shaping/DPI/weight/optical-size settings. A matching stylesheet aggregate does not rule all of these out. Lazy initialization, real settings differences and nondeterminism are hypotheses, not conclusions from this ZIP.

No runtime change follows from these data alone. Do not remove/ignore the guard, substitute the saved hash, or claim a cache-format defect. Do not invalidate or clear the user's caches while investigating.

## Bounded next work

1. Reproduce save/load across font-manager shutdown/reinitialization and separate processes using the same source, layout and fonts; current session fixture tests same-process cycles only and cannot by itself prove process-stable style hashing.
2. Decompose style-hash input on the host at save/check boundaries, comparing global/font-list/language/node components, before any second device build. Preserve the upstream hash and invalidation decisions.
3. Fix only a reproduced unstable or incorrectly ordered input, with stable-reopen and changed-layout/font/language/source negative regressions. If host reproduction fails, only then add narrowly justified component telemetry for device use; avoid another broad timing matrix.

No extra device run is requested now. Hybrid correctness gates, new full Monotype family and persistent menu-status toggle remain queued. About shared-wordmark fix is present, but the user's generic confirmation of this FB2 log run is not explicit visual acceptance of About.
