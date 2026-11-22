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

## Next checkpoint — lazy Settings and background preparation (CI pending)

Implemented the dependency group described above:
- buildKosync now creates the model/coordinator/status/timer independently;
  buildKosyncPage binds controls and replays the latest status. Existing account,
  native sync, upload and progress policies are unchanged in this step.
- buildSettings(parent), ensureSettings and preparePanels follow pinned beta9;
  dictionary language is initialized without opening Settings. Settled/input-quiet
  wiring prepares one hidden panel at a time, with the upstream quiet interval.
- Settings-dependent visibility, hiding, navigation bounds, hover blocking,
  host refresh, dark-option/error and shutdown paths now handle absent controls.
  Navigation retains the fork page strip while using upstream settingsOpen logic.
- New settings controls start null. On-demand open uses ensureSettings; theme
  application uses the already-verified shared late-widget boundary.
- The settled callback is cleared when the Reader window is destroyed: a retained
  ReaderSession must not keep a callback to the departed window.

New lifecycle.lazyPanels tests observe construction synchronously, intercept the
public session settled callback in the fixture (no runtime test flag), and release
it explicitly. Light/dark rows check absent controls, on-demand creation, buffered
KOSync status, Appearance-first/background Settings preparation, hidden panel and
single-instance reuse. lazyPanelsCloseUnbuilt checks shutdown without either panel.
The desktop strip/comics test keeps geometry/visibility/idempotence assertions;
its old fork-only never-prebuild assumption is replaced by the explicit lifecycle
boundary checks, since beta9 legitimately builds panels after settling.

Focused CI also now builds/runs kosync, sync-coordinator and text-presets model
tests and lifecycle.kosync* alongside the existing EPUB/FB2/settings/theme matrix.
Changed C++ ranges formatted with repository .clang-format (tool installed only
in excluded scratch storage; no Python/CMake application dependency added).
No engine/session algorithm, schema, cache version, string or waveform changes.
Device review requested by the user is combined with the preceding theme step.


Initial CI 44813033108 stopped native configure: the two new lifecycle tests
were registered/selected but missing the settings area pattern. Added the area
and one-line test descriptions; no tests removed or assertions weakened. This
repeats a known migration preflight omission, not a rendering failure. Follow-up
must pass the area/selection check before rerunning CI.

Follow-up CI 44813639385: Qt5/offline/package PASS; native completed compilation
and ran 475 selected tests. Six KOSync tests failed because their old harness
accessed Settings controls before creating the now-lazy page (null status/sync/
automatic controls, including test-side dereferences). The new lazyPanels tests
and previous focused checks were not listed as failures. Restored pinned beta9
openSettings/buildSettings fixture helpers and its small call-site hunks that
materialize controls before UI assertions. Request-count assertions surrounding
those calls are retained: constructing the UI must not initiate network work.
No synchronization policy or test expectation was changed to hide these failures.


## Verified combined checkpoint

Runtime `77564e9e11d2049cf82cfd5355704ae93e1cffa8`: [CI 44815357384](https://github.com/Mutaracha/kobalt_ru/actions/runs/44815357384)
SUCCESS. Kobo Qt5 build/offline/package/contents and focused native job PASS;
Qt6 intentionally skipped. No local Qt runtime/device or full-suite/Android claim.
The prior failed native run selected 475 tests; this successful run's raw XML/log
was not downloaded (GitHub log redirect EOF), so no raw result-count claim is made.
Artifacts API confirms Qt5 artifact 14669920633 (37360968 bytes) and native results
14667809370 (14888 bytes). [Device package](https://github.com/Mutaracha/kobalt_ru/actions/runs/44815357384/artifacts/14669920633).

Local static checks PASS: preparePanels/ensureSettings equal pinned beta9;
ReaderSession and engine source/adapter algorithms unchanged from 02b4ddfc;
CMake test-area preflight, focused selection/build-target check, strings generation
and rebuild script, explicit staged diff --check. Scratch-only CMake was installed
for script preflights, not used to claim a local Qt build. Combined device checks:
doc/validation/upstream-panels-device.md. Settings background construction and
first-open theme are now one package; presets have prior user approval only.


## Next dependency group: keyboard and choice sheets (CI pending)

Pinned beta9 keyboard/controller interface, widget keyboard, native keyboard
header and Qt5/Qt6 adapters restored as whole files. Supports QLineEdit and
QTextEdit before annotation note consumers migrate. All platform factories and
model-test factories widened consistently to QWidget; no platform dictionary,
light, sync or input method policy copied incidentally. Android's own controller
holds QWidget as upstream; Android/Qt6 runtime not tested by Qt5-first CI.

Pinned beta9 ModalOverlay and ChoiceSheet restored as whole files: long choice
lists own row pagination and refit on resize. Existing RadioList paging API is
already present; fork-only divider/checkbox presentation remains intact. This is
not the vertical-spacing form's implementation and is NOT a claimed freeze fix.

Added widgetKeyboardMultiline (selection replacement, deletion, submit, hide /
bounds), extended choiceSheetListFillsCard with short-landscape paging/refit,
and strengthened verticalJustificationTour to await destruction of the dismissed
spacing overlay while Settings is visible/enabled. Focused CI now selects all
widgetKeyboard* and the existing desktop.verticalJustification tour. The new test
keeps the existing keyboard area label; no test removal or weaker assertions.

Static investigation: showStylingChoices/dismissSheet and the vertical-spacing
close callback are unchanged from pinned beta9. No evidence yet justifies a
fork-specific close workaround. Need actual touch-path/lifecycle reproduction;
a desktop pass cannot establish that the device freeze is solved.

First CI 45025211358 Qt5 failed compiling dictionary_panel.h: the generalized
keyboard factory no longer includes QLineEdit transitively. Restored upstream's
explicit forward declaration in the DictionaryPanel header, preserving all fork
presentation. No runtime success claimed for that first run.


## Follow-up scope and native harness

CI 45025613466 Qt5/package passed; native failed before launching the six
widgetKeyboard tests (executable absent from focused build target list). Added
kobalt-reader-ui-tests to that list, keeping test selection/assertions intact.
Freeze investigation is now deferred by the user until a new example; the generic
form teardown regression remains, but is not a freeze fix or active investigation.

Annotation dependency review: BookmarkList currently lacks pagePending/hasNote/
note fields, excerptFont public API and note-aware pagination. Whole upstream
BookmarkList restoration needs ListRow::setNote/noteHeight and note rendering,
plus highlight annotationDetails that supplies note text. ListRow also shares
upstream SegmentedRow::prepend / unlabeled segmented controls changes. Reader
annotation actions need NoteEditor, Reader private fields, highlights note API,
state restoration/event filters/save completion and tests as one coordinated
consumer group, not a standalone pasted reader_annotations.cpp. None of these
annotation UI/storage changes is falsely marked completed by the keyboard port.


## Verified follow-up

Runtime cba1e229d25187c9934de18fb48359491d58b27f:
[CI 45027682492](https://github.com/Mutaracha/kobalt_ru/actions/runs/45027682492)
SUCCESS: native 9m52s, Qt5/offline/package/contents 4m56s, Qt6 skipped.
Qt5 artifact 14756567766 (37359133 bytes). No device/Android test claimed.
The focused selection ran widgetKeyboard* (including multiline), lazyAppearance
(including absent retired strip), verticalJustification and existing EPUB/FB2 /
settings/lifecycle regressions. ChoiceSheet/modalListDividers tests compile in
the UI executable but are not selected by the current focused CTest regex; their
runtime pass is NOT claimed. Overall source/semantic alignment remains incomplete.
