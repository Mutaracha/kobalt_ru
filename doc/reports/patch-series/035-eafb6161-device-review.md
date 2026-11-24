# eafb6161: device review 46ecb8f and bounded preload correction

Source upload: `46ecb8f67e551c40af1b13f701d3eaa39ef3e82c`, separate `diagnostics H.zip` and `diagnostics A.zip` plus two original screenshots under `test & diagnostic/eafb6161/`. Device UTC timestamps are recorded evidence, not an independently inferred date. Runtime startup identifies unmodified eafb6161.

## User observations and timing boundaries

All measured openings were manual from the UI; all stopwatch readings were manual. No measured run is labelled an automatic UI opening. User reports initial cold H1/H2, reboot, then diagnostics enabled: H1 4.70 s, H2 5.31 s. The pre-logging cold openings are not reconstructible from these archives. The two logged H opens are **disk-cache hits, retained=false, startup_slice=false**, not a new cold-prefix benchmark.

| Measured run | Stopwatch, s | Session reading, s | Model load, ms | Parse / paginate, ms | Disk / retained |
|---|---:|---:|---:|---:|---|
| H1 | 4.70 | 4.161 | 962 | 70 / 10 | true / false |
| H2 | 5.31 | 4.730 | 1083 | 169 / 30 | true / false |
| A1 | 14.81 | 14.438 | 4757 | 93 / 4513 | true / false |
| A1 immediate repeat | 3.80 | 2.630 | 80 | inherited stats, not new work | false / true |
| A2 | 5.20 | 4.821 | 1016 | 154 / 27 | true / false |
| A1 after next reboot | 20.49 | 20.225 | 4727 | 92 / 4488 | true / false |
| A2 after next reboot | 5.08 | 4.843 | 1023 | 155 / 28 | true / false |

Stopwatch and session totals have different boundaries and are not averaged together. In particular the retained repeat's resource stats preserve the previous parse/paginate times; they must not be counted as repeated 4.5 s pagination.

A archive also contains successful `all_books=true` cache eviction at 19:38:07–08, followed by two full uncached opens (8.593 / 26.951 s). These are preparation/background context, not the user's measured cached A1/A2 pair. No source/position deletion is inferred. All 12 cache-save events across A/H have saved=true. This confirms save/read lifecycle improvement, not consistently fast A1 layout reuse.

## Confirmed source of the large A1 wait: background preload, not automatic UI open

The logs explicitly distinguish `book-preload-*` from `startup-opener` (the manual accepted UI request):

1. 19:40:05.897 `book-preload-start`; worker open 05.899. Manual request 19.219 cancels preload at 19.222. The worker does not finish its interrupted opening until 26.647. New requested open starts 26.823 and finishes 31.735; session reading 33.662. Cancellation-to-worker-finish is **7425 ms**, and request-to-new-worker-start is **7604 ms**.
2. 19:43:35.584 preload; manual request 43.189, cancelled 43.192. Worker finishes interrupted open at 56.362, new open 56.532–19:44:01.424; session reading 03.416. Cancellation-to-finish **13170 ms**, request-to-new-worker-start **13343 ms**.

The difference of about 5.8 s in these queue waits closely matches the difference between the two A1 session totals, while requested A1 model load stays ~4.7 s. Later 19:49/19:51 ~19.8 s engine opens explicitly end with `book-preload-end`; they have no visible session reading and are not additional manually measured A1 timings.

Source: `ReaderCoordinator::preloadLastBook()` inspects the last local publication and opens a ReaderSession speculatively at Home. `begin()` cancels it when the user taps a book. The former EPUB-only path also admits FB2 after raw-format integration. Cancelling the lease does not instantly release all ongoing Render work. The observed wait is real, not evidence the user opened books automatically or tapped twice.

## Remaining distinct issue

Requested A1 has a real parsed-cache hit but ~4.49–4.51 s pagination; H1 has 10 ms and A2 27–28 ms. The exact layout incompatibility has not been established. Do not bypass compatibility hashes, return stale pages or claim removing preload eliminates that 4.5 s work. The overall delay has a substantial local component, so a bounded fix is worthwhile; further cached-layout work remains scoped investigation.

## Correction 035 and wording

035 excludes raw `.fb2` (case insensitive) only from speculative Home preload, before source inspection. Manual routing, parsed-cache identity/saving, saved positions, full fallback and upstream EPUB preload remain unchanged. A format-policy regression covers FB2/FB2 upper case rejection and EPUB/KEPUB/`.fb2.epub` continued eligibility. Existing routing assertions continue to require manual FB2 support. This avoids a background task rather than changing engine cancellation or EPUB algorithms. Tradeoff: FB2 no longer benefits from a speculative preload that fully completed before a tap; ordinary same-process retained reuse remains. The repeated 7–13 s blocking in this scenario justifies that conservative policy. It does not promise an exact total after the fix.

RU cache-eviction action descriptions and both confirmation messages now explicitly say Kobalt closes. This is a wording correction, not a new cleanup feature; EN upstream strings are unchanged. Updated both canonical RU strings and reusable 010 overlay, with presentation regression for all four messages.

## Acceptance status

- Exit wordmark: user explicitly accepts the screenshot; mark done/device accepted.
- Logging: detailed recording works and startup verbose=true survives several reboots. Explicit OFF-after-reboot acceptance was not separately reported; do not invent it or treat absence of earlier detailed events as proof.
- Cache save/read: all observed saves successful; disk hits for both books in measured series. A1 latency is not accepted as fixed.
- 035/wording: full ordered apply PASS. Native/Qt5 CI and device verification pending at initial publication.

## Next device check after a successful 035 build

No new cold corpus or broad A/B. Keep logs ON and the same layout. Close A1 last, reboot, manually open A1; export logs with local time/stopwatch and confirm position/count. Repeat once after closing A2 last (reboot, manually open A1): this covers both preceding-last-book cases. Expected: `book-preload-skipped` for an FB2 last book, no FB2 `book-preload-start` occupying the worker before the tap. Compare queue wait separately from A1 pagination. A2 quick check is optional unless it regresses. Open (then cancel) the two cache-cleanup confirmations to inspect wording; do not clear caches for this timing test.
