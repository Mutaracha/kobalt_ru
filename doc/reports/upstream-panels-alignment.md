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

CI pending. Focused selection includes existing lazyAppearance light/dark,
strip/comics and idempotence assertions; enabled upstream-derived themeChanges
and lightDarkModeShortcut tests, previously outside this focused workflow.
Settings/appearance lifecycle/preset checks retain their prior selection.
Local Qt/CMake/Docker unavailable; native/device NOT RUN locally.
Device approval for presets 9633d3fd does not approve this next runtime.
