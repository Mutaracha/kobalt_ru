# 4d9985e2: no-preload experiment and brand-wordmark correction

Input: user upload 7bbb6aef, `test & diagnostic/4d9985e2/diagnostics.zip`, and original screenshot `screen_001-24.png` from the conversation. User reports manual A1 11.20 s / A2 22.32 s after opening A1, A2 and rebooting. All measured UI openings remain manual; no automatic UI opening is inferred.

## Evidence

The archive contains 7368 events including rotated files. Startup explicitly identifies 4d9985e2. At 20:20:37, 20:23:36 and 20:30:59 UTC it logs `book-preload-skipped` for FB2, confirming 035 worked as an exclusion experiment. No occupying FB2 preload/cancel chain explains these subsequent opens.

| Device UTC session reading | Book by resource size | Session total, s | Model load, ms | Parse, ms | Paginate, ms | Parsed cache hit |
|---|---|---:|---:|---:|---:|---|
| 20:21:46.659 | A1 / 1101046 bytes | 8.800 | 4676 | 112 | 3660 | true |
| 20:22:42.157 | A2 / 3913359 bytes | 21.272 | 17630 | 361 | 16441 | true |
| 20:24:55.129 | A1 / 1101046 bytes | 10.262 | 6094 | 118 | 3676 | true |
| 20:25:52.217 | A2 / 3913359 bytes | 21.333 | 17731 | 362 | 16536 | true |
| 20:31:20.969 | A1 / 1101046 bytes | 10.571 | 6178 | 139 | 3643 | true |

All these phase events have retained=false; all five saves succeed. The manually reported single post-reboot pair is preserved separately, not averaged with these session totals or assigned by exact identity without per-tap timestamps. The 20:24/25 pair is consistent in scale/order but the archive also has another A1 observation. Direct layout reuse is not confirmed: A2 now spends ~16.5 s paginating despite the parsed-cache hit. Therefore blocking speculative work was one confirmed source of the prior waiting, not the sole cached-opening defect.

At 20:30:52.744 startup has `verbose=false`, followed by ordinary warning-level/compact operation. Combined with the user's explicit confirmation that OFF survives reboot, persistent disabling is device accepted. User also explicitly accepts cache-cleanup wording. Exit wordmark was accepted in the previous review. About screenshot separately reports the localized decorative wordmark, which is not accepted.

## User decision and scope

The user values preload and requests that it remain available to FB2 in the final configuration. 035 is **withdrawn from the active ordered series**, preserved only under `doc/archive/beta10-experiments/`. This restores the pre-existing FB2-capable Home preload rather than introducing a new switch. The no-preload 4d9985e2 binary remains an experimental comparison, not the final default. Prior long cancellation waits remain real evidence; restoring preload does not claim to fix its cancellation latency or cache layout reuse.

This result does not justify a broad cancellation/renderer rewrite. Keep the bounded investigation separate: identify cache render-context invalidation before touching compatibility/source guards. Time potentially at stake is substantial for A2, but the safe remedy is not established. Do not promise a speed ratio after restoring preload.

## About font correction

`wordmarkFont()` incorrectly returned the localized Serif family even though its documented role is the literal application name. About's `styles::wordmark` used it directly; the exit title had a local Instrument Serif override, hence only that title was corrected previously.

The shared literal-wordmark role now selects regular Instrument Serif independently of UI language, including About, opening and exit. The exit-only family override is removed. RU heading/caption serif remains Monotype Corsiva; classic Sans, reading fonts and explicit choices are unchanged. Instrument Serif is already present in RU Qt registration/private renderer from the exit fix, so this is not just an unavailable family label. Warm-up selects the actual Instrument face for the Latin wordmark. Added RU/EN assertions for the shared role and the resolved About label, while retaining locale-heading assertions.

## Validation and next step

Local complete-series application to pinned pristine beta10 passes. Active series has no 035 gate and retains preloadLastBook. Native/Qt5 CI and About visual acceptance pending at initial publication. Full new Corsiva family and menu-status toggle are still separate pending tasks, not included by implication.

A new broad timing matrix is not requested. After a verified next package, visually check About Kobalt and preserve preload behavior. Existing diagnostics suffice to establish remaining cached repagination; only request another narrowly targeted run if new instrumentation or a concrete remedy requires it.
