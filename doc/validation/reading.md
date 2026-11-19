# Startup and reading checks

Label `reading`. Build `mac-tests`, then run `ctest --preset mac -L reading` from `platforms/desktop/`. `ctest --preset mac -N -L reading` lists its tests.

These checks cover empty-library startup, opening books, rendered pages, page turns, saved positions, opening failures and cancellation, the feature tour, reader sessions, the opening cover, local resources and archives. [Test inventory](../validation.md#test-inventory) lists the desktop checks.

`model.resource-cache` checks compacted resource tails with different transfer chunk sizes, reads across block boundaries, empty resources, many small chapters, mixed-size eviction and failed-load accounting. Near the budget, it checks that compaction keeps the original block instead of exceeding the limit.

`ui.openingCoverLayout` checks cover placement, the centred group with a cover of any shape and with the logo, the text's distance from the cover's bottom edge, immediate branding, first-frame tile covers, native handoff and missing-cover fallback. `ui.openingHandoffLayout` checks that protected and remembered native routes keep the normal cover, bar and status positions, complete the bar, and show the destination and redirect icon in both themes, including missing covers and small screens. `ui.openingCoverUpgrade` checks sharper cover selection and asynchronous replacement of a displayed thumbnail. `ui.openingCoverDamage` checks that identical displayed pixels do not repaint, changed covers damage only their own surface, and progress leaves the cover untouched. `ui.openingMessages` checks both initial messages, the shared delayed message and the centered, fixed status area. `engine.local-reader` checks that reuse notifications agree with retained and parsed-book cache hits, including cache invalidation and cancelled opening. The loading gallery includes preparing, reopening, delayed status, missing cover and native handoff. These checks do not measure physical Kobo flashes or cover-loading latency.

## FB2 whole-book session boundary (beta9 migration)

`session.fb2-session` runs the real ReaderSession in Accurate and Hybrid modes
with a generated multi-page raw FB2. It checks exact whole-book frame/progress
values, four turns, font/viewport reflow with the same source anchor, layout
revision changes, zero-based session bookmark pages and isolated semantic preview
ownership. Engine::bookPages remains one-based; ReaderSession's display labels
are zero-based. No device result follows from this check.

Build `kobalt-fb2-session-tests` and run `ctest --preset linux -R
'^session.fb2-session$' --output-on-failure` from platforms/desktop in the pinned
native SDK. The focused Kobo workflow also builds/runs it.

`session.reading-book` also forces a chapter-local EPUB session, visits its last
local page before the final book fragment, and rejects an early finish offer.
The saved library percentage must equal ReaderSession's whole-book percentage,
not the local chapter percentage. This is independent of a device finish-dialog
check and does not verify the native save-epoch lifecycle.
