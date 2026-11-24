# FB2 opening: device logs 6a82161 vs 08c66e4d

## Inputs

User confirmed that 08c66e4d opens FB2 and comics on the device, but FB2 feels much
slower than 6a82161. Supplied in commits 7c1d7acd / c80d66c3:

- `test & diagnostic/08c66e4d/diagnostics.zip`
- `test & diagnostic/6a82161/diagnostics.zip`
- `test & diagnostic/6a82161/diagnostics 2.zip`
- `Sorokin_Norma-Tridcataya-lyubov-Mariny-Goluboe-salo-Den-oprichnika-Saharnyy-Kreml.3nbKGQ.593957.fb2`

Book size: 4,097,199 bytes. SHA256:
`b9742fa797a1736b9a4adb4132f137e06445a8f1fc000c41dc46ca3d21a22c90`.
The book contents are not reproduced in this report.

The old second archive contains the same current events.jsonl as the first archive,
plus five rotations. Do not count the two current logs as independent trials.
The rotated old logs are essential: the old current log alone mostly covers EPUB.
Event paths/titles are not included in opening events. Association below uses
resource_count=1, transferred_bytes equal to the exact attached book size and the
adjacent opening/worker timeline. Final state.json in both archives refers to other
books, and must not be used to identify the earlier FB2 run.

## Direct observations

### New: completed reading sessions with the attached file's resource size

UTC values are log timestamps, not a claim about the analysis date.

| Session reading event UTC | logged total | load/layout | parse | paginate | model cache hit |
|---|---:|---:|---:|---:|---|
| 2026-11-24 07:44:41.745 | 25,794 ms | 22,848 ms | 1,510 ms | 19,247 ms | false |
| 2026-11-24 07:45:47.222 | 30,803 ms | 28,491 ms | 1,823 ms | 25,415 ms | false |

Resource reads in these records are about 231 and 434 ms, respectively, with one
loaded resource and no request failures. Most of the delay is pagination, not file
transfer, database eligibility or hashing. Opening-phases model_load_ms for these
runs is 21,788 / 27,396 ms. The following opening-phases record at 07:46:56.927
also has no document-cache hit and a 22,925 ms model load; it is not counted as a
third fully identified completed-reading-session measurement in the table above.

All 17 `document-cache-save` records in the new archive have `saved:false` and
`elapsed_ms:0`. This is an important symptom, not an error reason: the event does
not distinguish unkeyed/ineligible documents, name mismatch, cache unavailability
or other early-return conditions. Other books are present in the log too.

### Old: both expensive openings and fast repeat/cache paths are present

At 2026-11-23 13:42:15.717, opening-phases records document_cache_hit=true and
model_load_ms=662. The adjacent FB2 worker completion at 13:42:16.862 has the
attached file's exact resource size, model_cache_hit=true, model_parse_ms=148 and
model_paginate_ms=26. The open-end operation took about 1,123 ms; this is worker
opening time, not a complete user-visible tap-to-content stopwatch measurement.

Several old cache saves succeed (including 13:42:13.342 and 13:42:18.757); several
repeat openings have model_load_ms around 431–662 ms. There are also retained-book
hits, which are a separate in-memory reuse path and must not be equated with disk
parsed cache.

Old slow runs exist too: identified reading-session records at 2026-11-22
21:23:27.666 and 2026-11-23 13:32:48.095 have about 21.2 / 23.8 seconds logged total
and 16.3 / 16.2 seconds pagination, despite model_cache_hit=true. A parsed cache
hit is not a guarantee that the current layout can be reused without repagination.
These are not controlled equal-settings cold-start benchmarks.

## Conclusion and next diagnostic boundary

The logs support a concrete explanation for the reported repeat-opening gap:
08c66e4d takes the full parse/paginate path rather than the fast cached path seen
in 6a82161. They do NOT yet establish why cache save/reuse fails in the real
ReaderSession pipeline, nor prove a general beta10 cold-pagination regression.

Do not confuse opening-first-frame with readable content: it can be the loading
screen. Use session reading/worker and stage events with their own time origins.

Before 040, investigate the real resource-backed opening/cache-save lifecycle:

1. Trace setDocumentCache key/policy/bytes and save eligibility through OpeningJob,
   ReaderSession and raw-FB2 resource input, not just isolated open(bytes).
2. Identify the actual save early-return reason; do not weaken source-name or
   fingerprint checks merely to make saved=true.
3. Add a resource-backed/session cache save-close-reopen regression. The existing
   engine test proves an isolated byte-backed cache path, not all device callers.
4. Verify parsed-cache hits AND compatible-layout reuse on the supplied book and
   ordinary EPUB, while retaining changed-source rejection and cache eviction.

No runtime code changed as part of this log comparison. No old opening acceleration,
Hybrid change, new FB2.ZIP support or excluded Russian_EnUS.pattern is restored.
Existing native/Qt5 CI acceptance of 010–032 remains a compile/test milestone, not
acceptance of repeat-opening performance on hardware.
