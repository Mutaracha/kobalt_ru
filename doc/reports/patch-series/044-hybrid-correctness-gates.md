# Bounded Hybrid correctness gate: first test-only batch

## Cache investigation handoff

User explicitly confirms that the late fast A1/A2 pair in upload214bf213 followed their reboot. Record this together with the successful header-anchored disk-layout checks (retained=false, paginate9/34ms). The archive's single explicit startup remains a raw-log limitation, not a reason to reject the user's clarification or request another run.

No cache-read algorithm fix was made between the style-mismatch investigation and that successful device check.036 added context-check observations;037/038 added host tests;039 added current/saved input snapshots in existing document properties and optional one-time metadata priming on compatible old caches. Compatibility formulas/predicates, document identity and the disk-reader path were not bypassed. Interface fonts changed (055), affecting the environment under which an inventory hash may legitimately differ.056 changes optical interface sizing/wording, not cache reading. Earlier035 preload exclusion was an explicitly withdrawn experiment, restored before this last diagnostic series. Do not present the observed improvement as a newly fixed cache-read bug.

The cache work is now paused/accepted at the observed-lifecycle level: save/read and fast disk-layout reuse after user-confirmed reboot work on A1/A2. Initial rejection in this upload is precisely attributable to a changed primitive font_list aggregate, with node/other captured operands matching. The exact face/path/inventory initialization difference is still unknown. Keep that as a residual observation and reopen only on reproducible recurrence; no speculative removal of the font-list guard.

## Coverage audit before changes

Existing `tests/model/fb2_engine.cpp::coldPrefixExperiment` already verifies:

- cold prefix is partial, continuesAfter=true and does not claim a final whole-book count;
- first prefix frame equals a full Accurate reference frame;
- incomplete prefix cannot write the full-book disk cache;
- full build produces the Accurate whole-book page count;
- completed full-book cache is reopened without entering a prefix;
- promotion preserves a native anchor;
- distant-anchor and fraction jumps promote safely; saved middle position bypasses prefix.

Basic FB2 fixture additionally exercises metadata, section TOC, binary cover, encoding, resource-backed opening and EPUB-after-FB2 state isolation. This is not sufficient to claim every body-image/note/inline-footnote transition is tested. Generic EPUB/full-build tests in page_estimate cover parking/appearance changes/destruction, but they are not a substitute for an FB2-specific test actually run in the selected CI lane.

## New044 test-only patch

`044-fb2-hybrid-cancellation-tests.patch` adds bounded stop/cancel/close assertions to the existing FB2 prefix fixture:

1. Pre-start cancellation returns Stopped, preserving prefix position, visible frame, partial mode and unknown whole-book count.
2. A callback allowing five stop checks parks the full build, preserving prefix frame/position and still refusing incomplete full-cache save. No sleeps or measured timing threshold are used.
3. Resuming the same build completes and runs all pre-existing full-count/cache/adoption tests.
4. Destruction of a document with a parked build is followed by an independent open under the same temporary cache key. It cannot find a partial full-book cache, remains a cold prefix, and can build the full book to the Accurate reference count.

Only test source changes. No production reader/cache/shaping/preload code is altered, no experimental acceleration added, and no claim of physical power-loss recovery or host resource-lease revocation follows from object-destruction coverage.

## Validation

Revision **a3e1c24db65bf91791e8dd7b72d8dc5564ca84d5**, **CI45613247416 all jobs PASS**: ordered full apply/validation/inventory, native selected tests including direct execution of expanded fb2-engine, real A1/A2+uploaded-Kobo-font replay; Qt5 compile/check-package/configured refresh-qemu/package collection. The Hybrid assertions execute on the native host lane, not on physical Kobo. Selected CI pass is not the entire repository test matrix.

No device update is required for this test-only batch; keep the recommended7566324a visual package. Noto optical balance/new hint still await explicit device visual acceptance. No stopwatch or cache clearing request.

## Remaining bounded gate, not closed by this batch

- Targeted FB2 prefix→full note/body-image correctness, including inpage-footnote settings and safe fallback on structures unsuitable for prefix layout. Binary cover alone is insufficient.
- Resource-backed prefix/full host close/cancel with SourceLease revocation, distinguishing it from in-memory engine destruction.
- Check actual published final/pending page-count semantics at reader-session/UI boundary, beyond EngineDocument::wholeBookPageCount.

Use small deterministic fixtures and existing helpers. Add no sectional engine, prefix tuning or saved-middle acceleration. Menu-status toggle remains the next feature after these correctness checks; full-Monotype integration stays superseded by the accepted Noto subset scope.
