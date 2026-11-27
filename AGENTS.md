# Agent instructions — kobalt_ru

## Document ownership and note format

Keep this file **in English**. It contains durable agent/build/test instructions
and verified environment caveats, not feature decisions, a changelog or a task log.

- `FB2_FORK_PLAN.md`: the single active work queue (Russian is allowed). Update
  status, next action, blockers and validation state in the same change as the work.
  Use `planned`, `in progress`, `blocked`, `done` or `deferred`; distinguish an
  implementation, CI verification and user/device acceptance. Do not append
  chronological build diaries or duplicate approved requirements here.
- `doc/fork-decisions.md`: accepted product/implementation decisions. Each entry
  has an ID, scope, decision, approval/source and validation or supersession note.
  Edit the canonical entry when a decision changes; name the superseded rule.
  Proposals are not approvals. Do not turn a test result into a product decision.
- `doc/validation/` and `doc/validation/beta10-device.md`: reproducible checks and current
  device acceptance/evidence. Record runtime source, CI run/artifact, scope,
  outcome and **unrun checks**. A docs-only commit is not a new tested binary.
- `doc/reports/`: focused technical investigations/author-facing reports. Separate
  observation, confirmed mechanism, hypothesis and proposed remedy.
- `doc/`: maintained technical/user guides; update the affected area guide for
  user-visible changes. The user owns root `README.md` edits.
- `test & diagnostic/`: retained CI FB2 input archive and its checksums; historical screenshots/uploads/logs were removed after acceptance and remain in Git at `40ec2610`;
  preserve bytes and add a context/provenance README. Curated `tests/manual/`
  materials and portable `tests/fixtures/` keep their existing ownership/paths.
- `doc/archive/`: historical records only, explicitly labeled superseded. Never
  copy old pending statuses back into the live queue. The pre-consolidation
  archive preserves former approvals, sources, build IDs and intermediate states.

Do not paste the same note into AGENTS, the plan and the decision register.
Link to its owner. Keep completed evidence grouped; keep the active queue short.
For a handoff, record the current baseline, next action and unverified items in
the plan, not a new competing status file.

## Session branch and publishing

Stay on the assigned Arena branch and checkout. Never create/switch branches or
worktrees. Push only the assigned branch; tags, releases, PRs and other branches
need explicit permission. Preserve unrelated work. Completed work and its plan
update must be committed and pushed to that branch. Disable the coauthor-injecting
hook with `git -c core.hooksPath=/dev/null commit ...`; never add agent coauthors.
Use lowercase release-stage words (`alpha`, `beta`) in commit messages.

## Environment and recovery

Arena is Linux, not the Mac described in upstream build examples. Recheck
`command -v git gh cmake qmake docker jq` each session. Do not assume a Qt SDK or
container engine exists. Local offline checks cannot replace native/Qt5 CI.
Never introduce Python into CMake or application build dependencies.

Files and Git metadata can be restored independently: HEAD may return to the
session base while files reflect later pushed work. Inspect `git status -sb`,
`git log -5 --oneline`, fetch the assigned branch and compare file contents to
its remote tree before editing/committing. Do not blindly commit the whole
apparent delta or discard it. If justified by inspection, align only the index/
branch with a mixed reset, preserving working files; an isolated index based on
the remote tree can publish an explicit file list without staging unrelated data.
Never use a hard reset as recovery. Confirm the remote result after publishing.

Scratch files, `/tmp` uploads and excluded directories such as `.arena`/`.cache`
may disappear across turns. Do not rely on them for handoff evidence; preserve
requested deliverables and original uploads in their documented locations.
Large generated artifacts/builds do not belong in Git. Do not repeat unchanged
commands after timeout; use managed process tools for CI watches/servers.

## Fork test-build workflow

Kobo **Qt5 / Libra 2** is the priority. Keep shared code portable; desktop and
Android are not release blockers for this fork. Qt5 uses old Qt 5.2 APIs: avoid
newer overloads such as the two-argument `QScopedValueRollback` constructor.

- Read `doc/building.md`, `doc/storage.md` and the affected validation guide.
- Local SDK build: `KOBALT_FORK_NAME=kobalt_ru bash platforms/kobo/build.sh qt5`.
- The current ordered-series build uses `.github/workflows/patch-series.yml`;
  its artifact is `Kobalt-ru-beta10-series-qt5`. The legacy device workflow is not
  proof that the complete ordered series was applied. Native regressions and package validation must both pass
  before offering a checked build. Do not claim a full/all-variant run from the
  focused workflow selection. Run relevant added tests in that selection.
- `gh run list/view/watch` and checks annotations identify results; download/log
  redirects may be inaccessible from Arena. Do not call a failed fetch a pass.
  A manual workflow dispatch or cleanup can return 403; use the push-triggered
  run and report the permission limit instead of requesting credentials.
- Qt6 is one-off/manual only. Historical artifacts are archived, not current builds.
  Do not enable it on every push.
- Run available offline checks: `git diff --check`, the shared EN/RU string and
  placeholder checks (`platforms/strings.cmake`, `platforms/strings-test.cmake`),
  and `bash platforms/ci/test.sh` when its dependencies exist. Report missing tools.
- For device handoff give build number, runtime source, artifact and test scope;
  physical validation remains separate. Follow the full-profile backup/schema
  contract before installing or rolling back a migrated profile. Do not restore an old binary
  over a migrated profile and call it a safe rollback.

## Shared-code and compatibility rules

Use `doc/fork-delta.md` and `platforms/patch-series/series.json` for current
beta10 ownership. Always export pristine pinned upstream and apply the complete
ordered series. Do not treat the retained dirty root source tree as that export.
Keep format behavior in focused adapters; preserve upstream policy ownership.
Retired features in `doc/retired-decisions.md` are closed, not a backlog. Historical
beta9/earlier records and mixed chronological plans live in `doc/archive/`;
never reinstate their pending tasks or superseded approvals automatically.

Blaze library sources live in `src/engine/blaze/{crengine,extensions,data}` and
are built by `dependencies/local-engine.cmake` into each platform output. Do not
compile vendor sources again through application source globs or apply the retired
external CREngine patch series to this directory. Preserve inherited formatting
and license headers. The shared dependency cache supplies only external libraries,
not Blaze objects. The application adapter and fork FB2/comics modules live under
`src/engine/blaze/adapter`; the worker, activity, links and word-count contracts
live directly under `src/engine`. Application globs must exclude only the library
subtrees (`blaze/crengine` and `blaze/extensions`), not the adapter. See
`doc/building.md` for the current engine build boundary.

Compiled third-party dependencies and their downloads live in the shared dependency cache, `build/dependency-cache/` in the main checkout, keyed by their content. It belongs to no worktree and is the only build state that worktrees share; using it is not dependence on another worktree. Application objects and all other outputs stay under each worktree's `build/`. Never edit, move or partly delete a published cache entry: change a pin, recipe or patch to get a new key. Release evaluation exports use their own cache. See [Dependency cache](doc/building.md#dependency-cache).

The macOS and Linux desktop clients are tools for development and testing on desktop. They are not release versions of the app. Keep desktop builds and screenshot galleries local. Upstream CI can run the full native suite on Linux in the pinned kobuild Linux image (`platforms/ci/native-tests.sh`), and the Kobo and Android builds wait for it. Branch CI uses representative variants; tagged CI requires all variants. Release approval requires an all-variant pass (`KOBALT_TEST_ALL_VARIANTS=1`), recorded separately from representative coverage. Do not add desktop release packages or other desktop CI jobs unless the user explicitly requests them.

Desktop development accepts Qt 6.11 or newer Qt 6 from one prepared native SDK. Linux uses the matching `linux`, `linux-light-tests`, `linux-fast-tests`, `linux-tests` and screenshot presets in `platforms/desktop/`; keep outputs under `build/desktop-linux/` and `build/screenshots/linux*/`. Desktop builds default to sensorless manual orientation; enable `KOBALT_DESKTOP_SENSORS` only when validating the real sensor adapter with a matching prepared Qt Sensors module. Mac development uses the `mac` presets. From `platforms/desktop/`, build `mac-tests` and run `ctest --preset mac` for native headless checks. Build `mac-screenshots` for the current captures in `build/screenshots/mac/`. Both paths use the same compiled desktop implementation as the app. Keep test profiles isolated and remove temporary fixture and capture state when a run ends. See [headless tests and screenshots](doc/building.md#headless-tests-and-screenshots) for scope and commands.

Restore portable regression assertions from `develop` with only necessary path, fixture and SDK adaptations. The `mac-tests` preset builds desktop, model, engine, session, resource and component checks; use `ctest --preset mac -L model` for the restored model group. Reuse compiled application objects. Native checks run through `platforms/desktop/run-test.cmake` so failure and timeout cleanup does not depend on test destructors. Keep the Fusion style used by the app and the original component suite. Await asynchronous UI completion before sending the next action. A page center can be covered by a panel; target exposed paper when dismissing it. New tests must not use fixed waits over 100 ms; wait for a condition or use an injected clock.

Use the checked-in Trevelyan's Type Tester fixture for general reading checks and screenshots. It includes the chapter-heading CSS fix documented in [Native validation](doc/validation.md#book-fixtures). Keep synthetic books for focused regressions, not as a replacement for the general book fixture. The full screenshot gallery also uses the checksum-pinned international chapters and Gutenberg editions documented in Native validation. Keep those fixtures, their provenance and their scenes when extending the tour. Gallery captures are frameless production reader or Library images at 300 PPI, at six-inch and seven-inch portrait sizes, six-inch landscape and a phone in portrait and landscape, in the light theme, the dark theme and the light theme with 150% interface text. Each gallery card represents one screen; size, variant and page choose its raw file. Require light and dark variants at every viewport and 150% text at every e-reader viewport. Phone mode supports 100% interface text only. Require matching light/dark pages at each viewport; 150% text may add pages. Keep device frames in the HTML viewer only, and retain filenames and checksums in the manifest for comparisons.

Use [Native validation](doc/validation.md) for the commands, guards, test inventory, feature coverage and explicit gaps. For one feature area, read only its file under `doc/validation/` and run its label with `ctest --preset mac -L <area>`; for reader behavior, read only the matching file under `doc/reader/`. Both index files list their areas. The routine UI loop is `mac-light-tests` followed by `ctest --preset mac-light`; `mac-light-screenshots` captures the primary views. Keep the light lane focused on primary views and features. It does not replace full release validation. `mac-fast-tests` and `ctest --preset mac-fast` retain the broader fast checks; `mac-tests` and `ctest --preset mac` run the full native set. `mac-screenshots` produces the current image set, checksum manifest and local gallery. Set `KOBALT_GALLERY_SCENE` to capture only the named gallery cards into `build/screenshots/mac-scenes/`; it never replaces the full gallery. Use `ctest --preset mac -L lifecycle` for reader lifecycle and annotation-frame checks, `-L '^ui$'` for shared canvas and paint checks (the anchors keep `-L` from also matching the `build` label), `-L platform` for portable adapter checks and `-L corpus` for the representative literary corpus. All four groups belong to the integration lane. Keep them in the integration lane and link the existing production objects; do not restore the old test-host application or scroll-only canvas. Keep public validation independent of private runtime services. Public Nickel compatibility checks are deferred until the user chooses an approach.

When behavior changes, identify all affected test suites. Update the tests where the expected behavior changed, and run those suites before handing off.

Use [Release evaluation](doc/release-evaluation.md) to compare a candidate with a baseline: `cmake -DRECORD_BASELINE=ON -P platforms/eval/run.cmake` on the baseline, `cmake -P platforms/eval/run.cmake` on the candidate, then `cmake -P platforms/eval/compare.cmake`. The quick profile is the default; `-DPROFILE=release` runs the full suite and gallery. Keep these scripts thin: they call the presets and the performance mode, and they never replace them. End-to-end performance scenarios live only in `platforms/eval/scenarios.json` and run through the built-in scenario player on every platform; add a scenario there, not as C++ test code. The player is compiled only into the test executable and into evaluation packages (`build.sh qt5-eval` or `qt6-eval`), never into release packages. Device timings come from an evaluation package on a Kobo, collected with `platforms/kobo/install-eval.cmake` and `collect-eval.cmake` or through an external runner passed with `DEVICE_RUNNER`. Keep that contract generic: everything must also work without a private runner. Never re-record a baseline to make a report pass. Real-device runs kept for comparing devices live in `platforms/eval/devices/`; see [Device benchmarks](doc/archive/pre-beta10/doc/device-benchmarks.md).

Only the performance evaluation needs a quiet machine. It holds a lock that all worktrees share, waits for running Kobo builds before it measures, and makes new Kobo builds wait. Kobo build containers use at most `KOBALT_DOCKER_CPUS` CPUs. Do not lower the priority of the native tests or the evaluation. See [Machine load](doc/release-evaluation.md#machine-load).

Flag suboptimal approaches before extending them. Identify duplicated build logic, application-side SDK workarounds, dependence on an old worktree, unnecessary rebuilds and unclear ownership. Explain the issue and propose a simpler approach.

The current full-series Qt5 package uses KOBALT_VERBOSE_DIAGNOSTICS=OFF so the Debug
checkbox can persistently switch detailed recording. OFF retains compact warning/error logs.
Direct Kobo builds otherwise enable verbose diagnostics by default; tagged CI release builds explicitly disable them. Branch, nightly and interim CI builds retain verbose diagnostics. `KOBALT_VERBOSE_DIAGNOSTICS=OFF` selects release logging for a direct build. Kobo diagnostic packages always enable verbose diagnostics and use the `qt5-diagnostic` and `qt6-diagnostic` presets. See [page-turn diagnostics](doc/validation/diagnostics.md#page-turn-diagnostics) for opt-in markers, event meanings and timing limits. They use the regular reader without the evaluation player.

Do not record fixed test, case, assertion or screenshot totals in documentation or these instructions. Describe coverage and verification commands instead. Use `ctest --preset mac -N`, with optional `-L` or `-R` filters, for the current test inventory. Keep meaningful fixture sizes, limits and measured durations.

When adding a UI screen that needs screenshot coverage, include its scenes in the full gallery in the same change. A scene's card title in `tests/support/screenshot_gallery.h` declares it, and a screen added to `primaryViews` also goes in `primaryScreens` in `tests/desktop.cpp`. The gallery check derives the expected captures from the declared scenes, viewports and variants, so there is no count to update. For fixes to specific views, use `KOBALT_GALLERY_SCENE` to recapture only the affected cards and all their current pages. Keep unrelated captures and review feedback. Do not regenerate the full gallery for a targeted fix; reserve the complete coverage check for full-gallery validation.

The root `./start.sh` builds and opens the Qt developer launcher on Linux and Mac. Keep launcher sources and presets under `platforms/launcher/`. Startup checks must explain missing dependencies in the terminal, keep Docker optional for desktop work, and never install tools or pull SDK images automatically. Its actions delegate to the existing platform presets, Kobo build script and autonomous scenario player. Keep visible scenarios separate from the full headless assertion suite. Preserve run-owned process, container and scratch cleanup when changing launcher actions.

For an optional check against both prepared Kobo SDKs, use `cmake --build --preset mac-kobo-checks`. It also builds and runs the Kobo adapter tests against pewter, the fake Nickel in `tests/pewter/`. CI runs the same tests in its Kobo build jobs. Ordinary Mac builds stay independent of Docker and Kobo checks. Run both SDK checks before integration. When an adapter change alters what Kobalt asks Nickel to do, update pewter and its tests in the same change; see [Pewter](doc/validation/kobo.md#pewter-the-fake-nickel). See [Kobo compatibility checks](doc/building.md#kobo-compatibility-checks). After the first edit of a slice to shared code, run `cmake -P platforms/kobo/syntax.cmake` to find Qt 5.2 and Qt 6.5 errors before the Kobo builds. Add `-DEVAL=ON` when the change touches code of the evaluation package. See [Kobo syntax check](doc/building.md#kobo-syntax-check).

## The page turn is sacred

While the user can turn a page or otherwise interact with the reader, the main thread must not write storage or wait for another thread. This includes Nickel's database, profile and preference saves, the diagnostic log, sleep snapshots and synchronous calls to the profile writer thread. Queue such work on a worker thread and report the result back to the main thread. A slow SD card or a busy Nickel database must never delay the next tap.

Block the main thread only where it is absolutely necessary: where no input can arrive before the wait ends, such as sleep, USB export, closing, power-off and a host checkpoint. Mark each such scope with `mainthread::AllowBlocking` and state the reason. Do not use it to silence a violation on a path that runs while the user reads. The current known exceptions are listed in [the main-thread guard](doc/validation.md#main-thread-guard); remove them rather than add to them.

`src/support/main_thread.h` enforces this rule. Every operation that writes storage or waits for another thread must call `mainthread::blocking()`, including a new storage helper or a new firmware call. The native tests run with `KOBALT_STRICT_MAIN_THREAD=1` and abort on a violation. Run them with `KOBALT_STRICT_MAIN_THREAD=trace` to list every violation with its call stack. Kobo logs a `main-thread-blocked` event instead. See [the main-thread guard](doc/validation.md#main-thread-guard).

The [source checks](doc/validation.md#source-checks) in `platforms/checks/` fail on storage writes in interface code, unlisted `AllowBlocking` reasons, unlisted `reader_*` names, short timers and unlisted `reading/` headers in the folders below `reading/`. Run `cmake -P platforms/checks/source-checks.cmake` after such a change. A change to an allowlist there is a decision for the user: say so and wait.

## UI integration and verification

Before changing UI, identify the closest existing control or screen. Trace its construction, shared styling, metrics and later presentation adjustments. Reusing its widget class alone is not enough.

Preserve the existing typography, caption/value alignment, padding, dividers and interaction behavior unless the user requests a change. Use established preview content when suitable.

For this fork's main Settings rows, use the local `check()` factory in `settings_panel.cpp`: it applies `styleSettingsOption()` at construction. An embedded page's option constructed elsewhere must explicitly receive that same presentation. Do not reintroduce a second hand-maintained list of controls to style later. `DescribedOption` alone defaults to a toggle with different insets and typography; never change its global defaults to fix one screen. Modal radios, previews, light-panel options and other surfaces keep their own presentation.

Every new Settings option must be compared with its nearest existing row, including square indicator/size, `chrome` label and description metrics, leading-icon alignment, horizontal insets, row height, bottom divider and pressed state. Check automatic paging and text wrapping in the surrounding screen, not just an isolated widget. Add assertions on the actual `Presentation` in component/routed tests; successful toggling or persistence alone is insufficient. If a native SDK or device visual comparison is unavailable, explicitly report it as unverified rather than declaring visual consistency.

Keep new fork behavior isolated behind its format/feature flag or in a focused helper, preserve upstream paths and saved-data compatibility, and update both EN/RU strings, `FB2_FORK_PLAN.md`, agent guidance and user instructions when relevant. Do not mix unrelated architecture or lifecycle changes into a visual correction. Continue refactoring in small testable steps after the correction, recording completed work separately from remaining debt.

For new layout controls, make the units understandable without reading implementation details. The preview must visibly demonstrate the setting.

Share an early screenshot for a new or substantially redesigned control. Include both its entry in the surrounding UI and its opened state. Do this before broad polishing and gallery generation.

Before declaring the UI complete, compare it directly with the closest existing example. Verify the actual font, size, alignment and spacing. Passing functional tests and generating screenshots do not establish visual consistency.

When reviewers are requested, give them these specific comparisons. Require findings or explicit verification for both the surrounding screen and the new control.

Review visible UI changes with the user through [Screen review](doc/screen-review.md). Before changing existing screens, capture them at the base with `platforms/review/baseline.cmake`, so the review can compare each screen with how it was before the feature. After such a change, capture the affected screens, create a round, start the review server and send the user the link. When the user presses Finish review, or says the review is done, read the feedback with the `notes` command, fix the screens that need changes and the pinned notes, then create the next round. Use an annotation-first review: moving on records the screen as seen, seen screens without annotations need no changes, and notes or pins mean Needs changes. Allow pins on either screenshot and arrow navigation outside text boxes. Repeat until the requested fixes are reviewed. Only the user supplies review notes; the tool records seen status from the user’s navigation. Respect questions, accepted behavior and explicitly deferred ideas in the feedback. Answer a note that needs no change with the `reply` command, so that the next round shows the answer instead of the same open note. To ask the user about specific screens, queue them with the `ask` command and a question; the page then offers only those screens first, so the user does not have to go through the whole gallery. Keep review data in the worktree's `build/review/`. Stop the server when the review ends.

## Execution and cleanup

For repeated development deployments, it is imperative to assess only the changes since the last validated build and reuse applicable validation results. Do not automatically rerun the full test suite or a smoke test for every minor change. Small UI spacing or styling adjustments may need only a build and visual inspection. Use focused checks when the change warrants them, and do not delay an explicitly requested deployment with redundant validation. Keep full release validation separate. State which checks were run, reused or skipped. Run the full native suite with `cmake -DPRESET=mac -P platforms/validation-record.cmake` from the repository root, so that a pass is recorded. The launcher's full-suite action, the release evaluation, the full gallery and Kobo package builds record their own passes. The [validation guard](doc/validation.md#validation-guard) hook refuses a full suite, full gallery or Kobo package run when the tree already passed it; documentation edits do not count as changes. Run focused label or regex checks instead. Use `KOBALT_REVALIDATE=1` only when a rerun is necessary, and tell the user why.

Run native Qt commands outside the agent sandbox on the development Mac. Headless Qt tools still need CPU capabilities that the sandbox can hide. Container commands need access to the engine socket. Keep outputs and caches in the authorized workspace.

Do not open a visible app or run visible UI tests without authorization. Prefer native headless validation while editing, then validate Kobo and Android integrations last. Reuse prepared SDKs and verified dependencies.

Treat storage IO on Kobo as expensive. Devices store data on eMMC or, on older models, an SD card, and their speed differs widely. Measure storage-bound work, such as syncs and saves, on both kinds, and judge changes by the slower one.

Stop and reap run-owned helper processes and remove scratch profiles, staging directories and temporary containers when a run ends, including on failure or interruption. Preserve the user's app state, active runs, SDKs and incremental caches. Retain useful current results and unresolved failure evidence rather than every successful raw log indefinitely.
