# Upstream panel alignment — shared late-widget theme boundary

Baseline: fork 00be0150, upstream beta.9 be472588.

## Implemented prerequisite

Moved ensureAppearance out of fork_reader_actions into reader_build and restored
its pinned upstream implementation. Restored upstream themeWidget/applyTheme,
including retained themePalette. This removes a separate fork palette/icon loop
for late-built children. The common path clears the inherited theme property
while constructing controls, polishes new widgets, then applies the stored theme
palette. Native-colored descendants keep their exemption. There is one common
implementation for initial and late-created widgets; no pixel overrides.

Appearance construction still contains the accepted FB2/comics/chrome wiring;
opening a panel calls the existing syncAppearanceControls path. No changes to
EPUB engine/session/Hybrid, waveform, resources, persistence or strings.

## Dependency boundary not yet migrated

Settings remains eager in this checkpoint. Its old buildSettings constructs
both KOSync model and page. Making it lazy before separating those would disable
sync before opening Settings or dereference absent kosyncPage. Other refresh,
navigation, sleep and settings consumers still require nullable-control audit.
Next: port the upstream model/page split and null-safe control consumers as a
closed dependency group, then ensureSettings/preparePanels and their harness.
Do not introduce a fork-specific deferred builder to bypass these dependencies.

## Validation

Focused CI verified (see checkpoint below). Selection includes existing lazyAppearance light/dark,
strip/comics and idempotence assertions; enabled upstream-derived themeChanges
and lightDarkModeShortcut tests, previously outside this focused workflow.
Settings/appearance lifecycle/preset checks retain their prior selection.
Local Qt/CMake/Docker unavailable; native/device NOT RUN locally.
Device approval for presets 9633d3fd does not approve this next runtime.

## Closed dependency group for the next checkpoint

- `reader_kosync.cpp`: upstream buildKosync/buildKosyncPage split, buffered status
  replay and null-safe async completions; build model before building Settings.
  Keep existing native cloud-sync policy. Audit book-vs-fragment coordinates
  separately against the shared progress contract; do not alter EPUB slicing.
- `reader_p.h`: nullable Settings/page/control initialization and settingsVisible
  predicate; no uninitialized option may be read before ensureSettings.
- `reader_settings.cpp`: openSettings calls ensureSettings; fit/apply/host refresh
  handle an absent panel. `reader_refresh.cpp` guards darkOption and panel access.
- Other consumers: reader.cpp, reader_build, dialogs, annotations and navigation
  must retain behavior before Settings exists; geometry/raise paths run only for
  the built panel. Preserve strip visibility and hidden-action preferences.
- Only then connect upstream preparePanels/settled/input-quiet bookkeeping and
  transfer its test expectations. The current fork lazyAppearance test explicitly
  asserts no early construction, so an upstream background-preparation change
  requires a matching harness update, not removal of assertions on a failed CI.

This dependency inventory is analysis, not an implementation claim.


## Verified checkpoint

Runtime `613df9ba96ff6b4406398ae942d92045e72d6a5c`,
[CI 44808379234](https://github.com/Mutaracha/kobalt_ru/actions/runs/44808379234)
completed success. Native job 136538246110 includes the extended theme tests and
existing focused EPUB/FB2/settings/lifecycle matrix. Qt5 job 136538245875 passed
offline, build/package/contents. Package artifact 14665345524; native results
artifact 14666150878. Raw XML was not downloaded/inspected for this checkpoint;
job outcomes are confirmed via GitHub API. Full suite/Android/device NOT RUN,
Qt6 skipped. No measured performance or physical acceptance claim.

Optional device focus for this package: open Text/Layout first in a dark reader;
repeat after switching theme before first opening those panels. Check controls,
icons, comics switch and strip, then close/reopen the panel. Existing preset
acceptance remains attached to 9633d3fd, not automatically to this binary.
