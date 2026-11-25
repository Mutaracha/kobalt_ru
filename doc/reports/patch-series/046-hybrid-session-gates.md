# Hybrid FB2: remaining bounded session gates

Ordered046 changes only `tests/model/fb2_cache_session.cpp`. It is executed in the
selected native CI `model.fb2-cache-session` lane, unlike generic reader-session tests.

## Pending/final whole-book count publication

A real local-resource FB2 session opens a long native prefix in Hybrid mode. The
opening callback asserts continuesAfter and paginationPending; its whole-book count
must be unknown or explicitly estimated, never a short-prefix count labelled final.
`requestMenuPagination()` requests the existing idle full build. The test waits for
paginationPending=false, then asserts whole_book_ready, an unestimated final count
matching an independently opened Accurate EngineDocument, and unchanged displayed
image/position. The next page turn adopts full layout without regressing count or
leaving a continuation flag. These are the ReaderSession APIs consumed by the menu
and stats UI, not a screenshot/font-fitting test or a measurement of device latency.
A30s deadline prevents a hung test; no speed threshold is used as an acceptance claim.

## Cancel/revoke with queued idle work and close/drain

For separate fresh local-resource FB2 sessions, the opening callback requests full
pagination then immediately revokes or cancels the SourceLease, while prefix/idle
work is still pending. After the worker drain barrier: original first-cause termination
is preserved, no new turn is accepted, no updated callback or changed state/frame
is published. Deleting the session and draining releases the observed ResourceCache
(weak reference expires). The test does not claim to deterministically stop a worker
at a chosen machine instruction or prove power-loss recovery.

Direct mid-transfer coverage is already present and **executed** in
`tests/model/local_resources.cpp`: FB2 transfer sink revokes after the first block,
no next block is delivered; an already-revoked lease never reaches the sink. There
are separate tests for sink cancellation and resource-size changes. This complements
the new session-level stale-publication/close gate rather than substituting EPUB tests.
The engine prefix park/destruction/resume tests from044 remain in fb2-engine.

## Outcome and boundary

Initial full series revision10882bdb, CI45625881182 all jobs PASS, including both new
session gates, expanded FB2 engine tests, real A1/A2+device-font cache replay, Qt5
build/check-package/configured refresh-qemu. Follow-up QSS refinements/font-sample
path consolidation are validated in the final handoff CI recorded in047.

The bounded Hybrid correctness queue is complete at automated host-test level:
anchors/counts/notes/body-image parity/cancel/park/close/resource revocation. It does
not assert every FB2, generic reader-session scenario, or physical device lifecycle
has been tested. No new reader/cache algorithm, cache admission bypass, experimental
acceleration, preload exclusion or user timing matrix. Menu-status toggle can follow.
