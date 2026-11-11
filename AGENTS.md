# Working on Kobalt

Read [Building](doc/building.md) before running a build. Do not restore retired runners wholesale.

## Build ownership

Use CMake and presets for application builds. Do not add Python tooling or an internal build-support framework. Keep platform build definitions, presets and toolchains under `platforms/`. Shared application source remains under `src/`. Shared asset inputs live under root `assets/`; stage runtime bundles inside each platform output. Do not read runtime assets from another checkout.

Kobo and Android builds use prepared `kobuild` images. Use native ARM64 Android containers on ARM64 hosts when the selected image supports them. The compiler, sysroot, Qt modules, Android SDK, NDK and JDK belong to kobuild. Fix missing SDK components in kobuild rather than building Qt modules or repairing SDKs in the application. Never compile Qt or any Qt module as part of a Kobalt build. Pin the Qt5, Qt6, Android and Linux images in `platforms/sdk-image.sh` from one and the same kobuild release tag, each by digest, and name that tag in [Building](doc/building.md).

The Mac app uses an existing native Qt SDK. A Kobo cross SDK cannot replace the native Mac SDK. Application-specific dependencies such as CREngine remain application inputs, separate from Qt SDK production.

Compiled third-party dependencies and their downloads live in the shared dependency cache, `build/dependency-cache/` in the main checkout, keyed by their content. It belongs to no worktree and is the only build state that worktrees share; using it is not dependence on another worktree. Application objects and all other outputs stay under each worktree's `build/`. Never edit, move or partly delete a published cache entry: change a pin, recipe or patch to get a new key. Release evaluation exports use their own cache. See [Dependency cache](doc/building.md#dependency-cache).

The macOS and Linux desktop clients are tools for development and testing on desktop. They are not release versions of the app. Keep desktop builds and screenshot galleries local. CI runs the full native suite on Linux in the pinned kobuild Linux image (`platforms/ci/native-tests.sh`), and the Kobo and Android builds wait for it. Branch CI uses representative variants; tagged CI requires all variants. Release approval requires an all-variant pass (`KOBALT_TEST_ALL_VARIANTS=1`), recorded separately from representative coverage. Do not add desktop release packages or other desktop CI jobs unless the user explicitly requests them.

Desktop development accepts Qt 6.11 or newer Qt 6 from one prepared native SDK. Linux uses the matching `linux`, `linux-light-tests`, `linux-fast-tests`, `linux-tests` and screenshot presets in `platforms/desktop/`; keep outputs under `build/desktop-linux/` and `build/screenshots/linux*/`. Desktop builds default to sensorless manual orientation; enable `KOBALT_DESKTOP_SENSORS` only when validating the real sensor adapter with a matching prepared Qt Sensors module. Mac development uses the `mac` presets. From `platforms/desktop/`, build `mac-tests` and run `ctest --preset mac` for native headless checks. Build `mac-screenshots` for the current captures in `build/screenshots/mac/`. Both paths use the same compiled desktop implementation as the app. Keep test profiles isolated and remove temporary fixture and capture state when a run ends. See [headless tests and screenshots](doc/building.md#headless-tests-and-screenshots) for scope and commands.

Restore portable regression assertions from `develop` with only necessary path, fixture and SDK adaptations. The `mac-tests` preset builds desktop, model, engine, session, resource and component checks; use `ctest --preset mac -L model` for the restored model group. Reuse compiled application objects. Native checks run through `platforms/desktop/run-test.cmake` so failure and timeout cleanup does not depend on test destructors. Keep the Fusion style used by the app and the original component suite. Await asynchronous UI completion before sending the next action. A page center can be covered by a panel; target exposed paper when dismissing it. New tests must not use fixed waits over 100 ms; wait for a condition or use an injected clock.

Use the checked-in Trevelyan's Type Tester fixture for general reading checks and screenshots. It includes the chapter-heading CSS fix documented in [Native validation](doc/validation.md#book-fixtures). Keep synthetic books for focused regressions, not as a replacement for the general book fixture. The full screenshot gallery also uses the checksum-pinned international chapters and Gutenberg editions documented in Native validation. Keep those fixtures, their provenance and their scenes when extending the tour. Gallery captures are frameless production reader or Library images at 300 PPI, at six-inch and seven-inch portrait sizes, six-inch landscape and a phone in portrait and landscape, in the light theme, the dark theme and the light theme with 150% interface text. Each gallery card represents one screen; size, variant and page choose its raw file. Require light and dark variants at every viewport and 150% text at every e-reader viewport. Phone mode supports 100% interface text only. Require matching light/dark pages at each viewport; 150% text may add pages. Keep device frames in the HTML viewer only, and retain filenames and checksums in the manifest for comparisons.

Use [Native validation](doc/validation.md) for the commands, guards, test inventory, feature coverage and explicit gaps. For one feature area, read only its file under `doc/validation/` and run its label with `ctest --preset mac -L <area>`; for reader behavior, read only the matching file under `doc/reader/`. Both index files list their areas. The routine UI loop is `mac-light-tests` followed by `ctest --preset mac-light`; `mac-light-screenshots` captures the primary views. Keep the light lane focused on primary views and features. It does not replace full release validation. `mac-fast-tests` and `ctest --preset mac-fast` retain the broader fast checks; `mac-tests` and `ctest --preset mac` run the full native set. `mac-screenshots` produces the current image set, checksum manifest and local gallery. Set `KOBALT_GALLERY_SCENE` to capture only the named gallery cards into `build/screenshots/mac-scenes/`; it never replaces the full gallery. Use `ctest --preset mac -L lifecycle` for reader lifecycle and annotation-frame checks, `-L '^ui$'` for shared canvas and paint checks (the anchors keep `-L` from also matching the `build` label), `-L platform` for portable adapter checks and `-L corpus` for the representative literary corpus. All four groups belong to the integration lane. Keep them in the integration lane and link the existing production objects; do not restore the old test-host application or scroll-only canvas. Keep public validation independent of private runtime services. Public Nickel compatibility checks are deferred until the user chooses an approach.

When behavior changes, identify all affected test suites. Update the tests where the expected behavior changed, and run those suites before handing off.

Use [Release evaluation](doc/release-evaluation.md) to compare a candidate with a baseline: `cmake -DRECORD_BASELINE=ON -P platforms/eval/run.cmake` on the baseline, `cmake -P platforms/eval/run.cmake` on the candidate, then `cmake -P platforms/eval/compare.cmake`. The quick profile is the default; `-DPROFILE=release` runs the full suite and gallery. Keep these scripts thin: they call the presets and the performance mode, and they never replace them. End-to-end performance scenarios live only in `platforms/eval/scenarios.json` and run through the built-in scenario player on every platform; add a scenario there, not as C++ test code. The player is compiled only into the test executable and into evaluation packages (`build.sh qt5-eval` or `qt6-eval`), never into release packages. Device timings come from an evaluation package on a Kobo, collected with `platforms/kobo/install-eval.cmake` and `collect-eval.cmake` or through an external runner passed with `DEVICE_RUNNER`. Keep that contract generic: everything must also work without a private runner. Never re-record a baseline to make a report pass. Real-device runs kept for comparing devices live in `platforms/eval/devices/`; see [Device benchmarks](doc/device-benchmarks.md).

Only the performance evaluation needs a quiet machine. It holds a lock that all worktrees share, waits for running Kobo builds before it measures, and makes new Kobo builds wait. Kobo build containers use at most `KOBALT_DOCKER_CPUS` CPUs. Do not lower the priority of the native tests or the evaluation. See [Machine load](doc/release-evaluation.md#machine-load).

Flag suboptimal approaches before extending them. Identify duplicated build logic, application-side SDK workarounds, dependence on an old worktree, unnecessary rebuilds and unclear ownership. Explain the issue and propose a simpler approach.

Direct Kobo builds enable verbose diagnostics by default; tagged CI release builds explicitly disable them. Branch, nightly and interim CI builds retain verbose diagnostics. `KOBALT_VERBOSE_DIAGNOSTICS=OFF` selects release logging for a direct build. Kobo diagnostic packages always enable verbose diagnostics and use the `qt5-diagnostic` and `qt6-diagnostic` presets. See [page-turn diagnostics](doc/validation/diagnostics.md#page-turn-diagnostics) for opt-in markers, event meanings and timing limits. They use the regular reader without the evaluation player.

Do not record fixed test, case, assertion or screenshot totals in documentation or these instructions. Describe coverage and verification commands instead. Use `ctest --preset mac -N`, with optional `-L` or `-R` filters, for the current test inventory. Keep meaningful fixture sizes, limits and measured durations.

When adding a UI screen that needs screenshot coverage, include its scenes in the full gallery in the same change. A scene's card title in `tests/support/screenshot_gallery.h` declares it, and a screen added to `primaryViews` also goes in `primaryScreens` in `tests/desktop.cpp`. The gallery check derives the expected captures from the declared scenes, viewports and variants, so there is no count to update. For fixes to specific views, use `KOBALT_GALLERY_SCENE` to recapture only the affected cards and all their current pages. Keep unrelated captures and review feedback. Do not regenerate the full gallery for a targeted fix; reserve the complete coverage check for full-gallery validation.

The root `./start.sh` builds and opens the Qt developer launcher on Linux and Mac. Keep launcher sources and presets under `platforms/launcher/`. Startup checks must explain missing dependencies in the terminal, keep Docker optional for desktop work, and never install tools or pull SDK images automatically. Its actions delegate to the existing platform presets, Kobo build script and autonomous scenario player. Keep visible scenarios separate from the full headless assertion suite. Preserve run-owned process, container and scratch cleanup when changing launcher actions.

For an optional check against both prepared Kobo SDKs, use `cmake --build --preset mac-kobo-checks`. It also builds and runs the Kobo adapter tests against pewter, the fake Nickel in `tests/pewter/`. CI runs the same tests in its Kobo build jobs. Ordinary Mac builds stay independent of Docker and Kobo checks. Run both SDK checks before integration. When an adapter change alters what Kobalt asks Nickel to do, update pewter and its tests in the same change; see [Pewter](doc/validation/kobo.md#pewter-the-fake-nickel). See [Kobo compatibility checks](doc/building.md#kobo-compatibility-checks). After the first edit of a slice to shared code, run `cmake -P platforms/kobo/syntax.cmake` to find Qt 5.2 and Qt 6.5 errors before the Kobo builds. Add `-DEVAL=ON` when the change touches code of the evaluation package. See [Kobo syntax check](doc/building.md#kobo-syntax-check).

## The page turn is sacred

While the user can turn a page or otherwise interact with the reader, the main thread must not write storage or wait for another thread. This includes Nickel's database, profile and preference saves, the diagnostic log, sleep snapshots and synchronous calls to the profile writer thread. Queue such work on a worker thread and report the result back to the main thread. A slow SD card or a busy Nickel database must never delay the next tap.

Block the main thread only where it is absolutely necessary: where no input can arrive before the wait ends, such as sleep, USB export, closing, power-off and a host checkpoint. Mark each such scope with `mainthread::AllowBlocking` and state the reason. Do not use it to silence a violation on a path that runs while the user reads. The current known exceptions are listed in [the main-thread guard](doc/validation.md#main-thread-guard); remove them rather than add to them.

`src/support/main_thread.h` enforces this rule. Every operation that writes storage or waits for another thread must call `mainthread::blocking()`, including a new storage helper or a new firmware call. The native tests run with `KOBALT_STRICT_MAIN_THREAD=1` and abort on a violation. Run them with `KOBALT_STRICT_MAIN_THREAD=trace` to list every violation with its call stack. Kobo logs a `main-thread-blocked` event instead. See [the main-thread guard](doc/validation.md#main-thread-guard).

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

Review visible UI changes with the user through [Screen review](doc/screen-review.md). After such a change, capture the affected screens, create a round, start the review server and send the user the link. Read the feedback with the `notes` command, fix the screens that need changes and the pinned notes, then create the next round. Use an annotation-first review: moving on records the screen as seen, seen screens without annotations need no changes, and notes or pins mean Needs changes. Allow pins on either screenshot and arrow navigation outside text boxes. Repeat until the requested fixes are reviewed. Only the user supplies review notes; the tool records seen status from the user’s navigation. Respect questions, accepted behavior and explicitly deferred ideas in the feedback. Keep review data in the worktree's `build/review/`. Stop the server when the review ends.

## Execution and cleanup

For repeated development deployments, it is imperative to assess only the changes since the last validated build and reuse applicable validation results. Do not automatically rerun the full test suite or a smoke test for every minor change. Small UI spacing or styling adjustments may need only a build and visual inspection. Use focused checks when the change warrants them, and do not delay an explicitly requested deployment with redundant validation. Keep full release validation separate. State which checks were run, reused or skipped. Run the full native suite with `cmake -DPRESET=mac -P platforms/validation-record.cmake` from the repository root, so that a pass is recorded. The launcher's full-suite action, the release evaluation, the full gallery and Kobo package builds record their own passes. The [validation guard](doc/validation.md#validation-guard) hook refuses a full suite, full gallery or Kobo package run when the tree already passed it; documentation edits do not count as changes. Run focused label or regex checks instead. Use `KOBALT_REVALIDATE=1` only when a rerun is necessary, and tell the user why.

Run native Qt commands outside the agent sandbox on the development Mac. Headless Qt tools still need CPU capabilities that the sandbox can hide. Container commands need access to the engine socket. Keep outputs and caches in the authorized workspace.

Do not open a visible app or run visible UI tests without authorization. Prefer native headless validation while editing, then validate Kobo and Android integrations last. Reuse prepared SDKs and verified dependencies.

Treat storage IO on Kobo as expensive. Devices store data on eMMC or, on older models, an SD card, and their speed differs widely. Measure storage-bound work, such as syncs and saves, on both kinds, and judge changes by the slower one.

Stop and reap run-owned helper processes and remove scratch profiles, staging directories and temporary containers when a run ends, including on failure or interruption. Preserve the user's app state, active runs, SDKs and incremental caches. Retain useful current results and unresolved failure evidence rather than every successful raw log indefinitely.

## Fork workflow (kobalt_ru)

This fork follows the permanent update procedure in [Fork update workflow](doc/fork-update-workflow.md) and `FB2_FORK_PLAN.md` in the repository root. Keep that plan current: mark every stage and checklist item with its progress status (planned / in progress / done) in the same change as the work it describes, so the plan alone is enough to resume after the sandbox or local checkout is wiped.

The fork workflow takes precedence over generic upstream feature/worktree instructions below: in Arena, stay in the assigned session branch and checkout. Never create another branch or worktree.

Always commit completed work and push it to the current working branch (`git push origin <current-branch>`). The local repository and sandbox can be cleared at any time, so unpushed work is not considered safe. Never switch, create or push to another branch.

The priority target is the Kobo **Qt5** package (`Kobalt-qt5.zip`, firmware 4.x). Desktop (mac) and Android are not priorities for this fork: do not block Kobo work on them, but keep shared code portable and avoid knowingly breaking their builds. Section 6 of `FB2_FORK_PLAN.md` (Russian interface localization) is implemented as a second compiled string table with an English fallback: when touching UI strings, update both `assets/en.strings` and `assets/ru.strings` — the build fails on any key or `%n` slot mismatch.

Every fork feature stays behind an explicit flag or in a separate file so the EPUB/upstream code paths keep their original behavior; the full change map and conflict-resolution rules are section 7.2 of `FB2_FORK_PLAN.md`. Read it before rebasing onto a new upstream release, and bump the shared DOM-cache marker (`:fb2-vN:` in `reader_fb2.h`, used by `reader_source.cpp` and `engine_document.cpp`) if FB2 parsing or default FB2 CSS changes. FB2-only override CSS lives beside this marker in `reader_fb2.h`; keep popup `footnote` unconditional, and gate bottom-page `footnote-inpage` CSS on the default-on `fb2_inpage_footnotes` appearance key. Numeric note-title normalization belongs to the FB2-only helper; never strip brackets in the main text. Include the on/off mode in FB2 cache policy, appearance equality/copy and global defaults. The Kobo Qt5 SDK predates Qt 5.10: do not use APIs such as `QStringView`.

Interim device builds come from the push-triggered `.github/workflows/kobo-qt5-device.yml` (artifact `Kobalt-qt5-device`); a local SDK build is `KOBALT_FORK_NAME=kobalt_ru bash platforms/kobo/build.sh qt5`. `KOBALT_FORK_NAME` defaults to empty, which hides the fork row on the About page and preserves the stock layout; only the fork workflow sets it. The fork row is a single added label (`kobalt_ru · <UTC build time>` from `KOBALT_BUILD_TIME`); never change the stock version/commit rows. Before pushing, run the offline checks that need no Qt SDK: `cmake -DTEST_TMP_ROOT=$(mktemp -d) -P platforms/strings-test.cmake`, the en/ru string parity check, `bash platforms/ci/test.sh` when `jq` is available, and `git diff --check`. On a Mac, also run the `model` ctest group (`fb2`, `strings_locale`, `local_resources`, `nickel_book_menu`, `reading_book`). The complete pre-push checklist is section 7.5 of the plan. User-facing behavior of the fork is documented in Russian in `doc/fork-kobalt_ru.md`; update it together with user-visible changes.

## Refactoring contracts for new fork work

These are maintenance requirements, not just a historical changelog. Preserve
them when adding features or rebasing onto upstream; see section 8 of
`FB2_FORK_PLAN.md` for decisions and validation evidence.

- Keep upstream frames/classes in place where a small hook suffices. For the
  mixed-font exit title, reuse `return_title_text.h` for our brand runs; do not
  replace upstream ReturnTitle or its palette/large-scale layout/painting frame.
- FB2 head recognition stays in `publication/fb2.h`; do not run the extra full
  FB2 inspection on the normal ZIP/EPUB path. FB2 preview and numeric note-title
  preprocessing stay in `reader_fb2.cpp`, with explicit concrete engine includes.
  Do not modify main-text reference labels or remove DOM nodes/IDs to restyle notes.
- Keep FB2 CSS/cache-policy changes together in `reader_fb2.h`. Popup hints are
  unconditional; in-page CSS is conditional on the default-on appearance key.
  Update policy in its single helper only when the parsed result/default CSS
  changes, not for releases or code moves. Keep the separate EPUB footnote policy
  salt in its own helper; do not restore the obsolete empty EPUB salt.
- Register new layout-affecting choices in appearance state, clean/default/apply,
  copy/equality and retained-layout checks. `afterContentChange()` is a narrow
  whitelist: never substitute all appearance keys or retain stale source-bound
  positions when a book fingerprint changes. Test missing keys and explicit false.
- Acquire locale string tables/counts before calling lookup in a separate
  statement; never rely on C++ function-argument evaluation order. Keep canonical
  English diagnostics and the Russian fallback/key-display behavior unchanged.
- Main Settings rows must receive their shared Presentation through the existing
  construction factory (and explicit shared styling for embedded external rows).
  Do not restore the fragile late registry or change global widget defaults.
- Hidden diagnostics must be isolated and inactive by default. The fork About
  label remains an InterfaceLabel with its original text, alignment, font,
  stylesheet, geometry and paint path. The five-tap handler in `fork_debug.cpp`
  opens only a standard modal; never add visible hints/buttons to that row.
  Language marker selection is startup-only, not a partial live retranslation.
  Use hidden `.lang_en`/`.lang_ru`; explicit markers beat the environment.
  Kobo must resolve again at the accepted new-book boundary before any UI is
  built, not just early Nickel module initialization; never retranslate a live
  reader. Read legacy names only as fallback and remove them on UI selection.
  ModalButton must use adaptiveActions for measured text/padding height. Avoid
  duplicate rules at RadioList/action boundaries via the list's final-edge
  option, preserving default dividers for existing callers. Test actual action
  height before and after saving, not only callback or marker success.
  Serialize marker writes off the GUI thread, weakly guard completion, and wait
  for workers only at application shutdown. Marker files are diagnostics, not
  preferences migrations or book data. Do not touch other diagnostics files.
- New helpers must have explicit owners/lifetime/threading and focused tests.
  Validate the actual UI presentation, not merely clicks and persistence. State
  separately which checks were executed, added-but-unrun, reused or still need
  device review. Update the plan, both locales and user instructions together.

## Arena environment notes (verified 2026-10-29)

Arena runs this checkout in a Linux sandbox, not on the development Mac described above. At the last check `git`, `gh`, `python3`, `g++` and `jq` were available; `cmake`, `qmake` and `docker` were not on PATH. Do not assume a native Qt SDK or Kobo container engine is available. Recheck tools with `command -v` every session. If needed, install CMake in a disposable venv (e.g. `/tmp/kobalt-tools`); this is agent-only validation tooling, never an application build dependency. Native desktop/UI tests need a suitable Qt host; the push-triggered Kobo Qt5 workflow verifies the device build.

Files and Git metadata can be restored independently between turns. In observed sessions the files contained the pushed fork changes while local HEAD was back at the base commit, making the whole fork appear as uncommitted changes. First inspect `git status -sb`, `git log -5 --oneline` and, after `git fetch origin`, compare HEAD, origin's working branch and the actual file diff. Do not blindly commit that apparent delta again. Only when the reset is confirmed and the files have been inspected, `git reset --mixed origin/arena/9e626d86-kobalt-ru` can realign the current branch/index **without overwriting working files**. Never use `--hard`, switch branches or discard unrelated edits. The session branch is fixed to `arena/9e626d86-kobalt-ru`; push only there.

`/tmp` tools, patched validation scripts and downloaded artifacts from earlier turns may disappear; their paths in conversation history are not evidence they still exist. Recreate scratch tooling when needed. The strings incremental-build test has shown a local filesystem timestamp flake; if reproduced, keep any workaround in a scratch copy and report it, rather than weakening the repository test. Check GitHub CI separately and report native tests as unrun when the SDK is unavailable. Commit/push finished work with the plan update, bypass the coauthor-injecting hook as documented below, and verify the remote result.
Remove run outputs and scratch state with `cmake -DPATHS="<path1>;<path2>" -P platforms/cleanup.cmake`, not with `rm -rf` on a variable path. It removes only paths inside the worktree's `build/` folder, outside `build/dependency-cache/`, or inside an agent scratch folder. It refuses everything else and fails when it refuses a path.

## Repository practice

Format first-party C and C++ code with the checked-in `.clang-format` (Qt project style) before committing. See [Code formatting](doc/building.md#code-formatting).

Read [Storage compatibility](doc/storage.md) before changing settings, book state, bookmarks, annotations or rendering caches. This also applies to backup tools and test database inspection: follow the backup contract and never open the live exclusive profile through an external SQLite connection. Keep saved reading data separate from disposable caches. App releases must not reset either format merely because the release version changed. Compatible optional data fields keep the current format version and must load older records with explicit defaults. Preserve supported optional fields when editing records. An incompatible saved-data change requires a format-version bump, a migration that preserves existing user data, and regression tests before it ships. Never overwrite or delete the original files of unsupported or damaged saved data. Profile preparation moves them into `backups/` and starts from defaults; other reads reject them and leave them in place. Do not assume older builds preserve unknown fields or can safely write newer schemas.

Bump the format version in `documentCacheChecksum` when parser or preprocessing changes make parsed-book caches incompatible. Bump the inspection-cache format separately when verdict rules change incompatibly. Cache invalidation must rebuild disposable data only; it must never clear settings, bookmarks or annotations.

Keep Markdown instructions in `doc/`, except root `README.md`, `AGENTS.md`, the canonical `FB2_FORK_PLAN.md`, and provenance/index READMEs beside test materials. The user owns root README edits. Keep these instructions and build commands current in the same change as tooling updates.

In this fork, pushing completed work to the current working branch is pre-authorized and required (see [Fork workflow](#fork-workflow-kobalt_ru)); other publications (tags, releases, other branches) still need explicit authorization. Signed operations require an advance warning for 1Password prompts. Never add an agent coauthor: the sandbox may install a `commit-msg` hook that appends a `Co-authored-by: arena-agent` trailer, so create fork commits with `git commit --no-verify` and verify with `git log --grep=arena-agent` that the history stays trailer-free. Preserve unrelated changes and the original checkout.

Work on a feature branch by default. Before the first edit, create a dedicated branch and a Git worktree for it. The branch `feature/name-of-feature` always lives in the worktree `worktrees/name-of-feature` in the repository: the folder name is the branch name without the `feature/` prefix. The `feature/` prefix is required. Git ignores the `worktrees/` folder. This applies to small changes too, and also when no other worker seems active. Work directly in the shared `develop` checkout only when the user says so for that task. Otherwise, other agents and the user can edit `develop` at any time: only read and investigate there. If you find that you started work in `develop` without being asked, stop and move only your own changes into a worktree. Leave every other change in `develop` as it is. When authorized to integrate a feature into `develop`, cherry-pick its commits or merge it as a fast-forward. Never create a merge commit.

One conversation thread usually means one worktree and one branch. When the user asks for several features in one thread, check whether they would change the same code. If they would, ask whether the user wants a separate worktree for each feature before the first edit.

After successful integration, normally remove the worker's worktree and feature branch unless the user asks to keep them. Before removal, verify that the worktree has no uncommitted changes and that all intended changes are integrated. Preserve any needed test evidence or other artifacts before removing the worktree.

Use lowercase release-stage words in commit messages, including `alpha` and `beta`, even when part of a release name. The abbreviation `RC` may remain uppercase.

## beta6 integration scope

Use upstream frontlight gesture feedback, without the fork quiet-light UI, key or migration. Cache cleanup is deferred until after beta; do not restore its action, helper, worker retirement, lifecycle barriers or special tests. Never delete device caches as part of removing the feature. Use upstream progress/save, retaining necessary FB2 integration. Keep FB2 formatting/settings aligned with the shared EPUB implementation as upstream evolves; do not freeze a separate formatting implementation.

## Maintainability during active upstream development

For the beta6 integration and subsequent updates, optimize for maintenance effort,
not the smallest diff or fewest lines. Additional focused code is acceptable when
it isolates a fork responsibility and reduces repeated conflict resolution.
Keep fork-specific policy in cohesive modules with explicit ownership, and use a
small number of deliberate integration points in upstream code. Follow upstream's
new directory boundaries rather than retaining parallel obsolete directories.
Do not build a generic plugin framework, scatter conditional hooks, or copy whole
upstream classes/settings pages to avoid a merge conflict. In particular, FB2
must use evolving shared EPUB formatting/settings and upstream progress/save;
isolate only format-specific detection, preprocessing, resource and position
adaptation, footnote policy and cache salt. Keep localization/debug policy out of
unrelated UI and lifecycle code. Preserve the approved About rendering and shared
UI styling contracts. Document each integration point's responsibility and focused
regression coverage; distinguish code moves/refactoring from behavior changes.

### beta6 integration points

- EngineDocument is the upstream engine owner. Keep reader_fb2 preprocessing/CSS
  and FB2 cache policy there, using the same normalization/appearance pipeline as
  EPUB. Do not restore ReaderModel or the retired interfaces/ source directory.
- fork_settings owns Settings row presentation; app/about/fork_about owns the shared About attachment;
  invoke row styling in check() so future upstream options inherit it. Never
  reintroduce a late manually enumerated registry. app/about/fork_about::appendForkBuildRow must retain
  the approved InterfaceLabel rendering and attach debug input without repainting.
- sleep_ribbon_text owns only translated caption splitting; sleep screen modes,
  remaining-time calculation and lifecycle belong to upstream beta6.

FB2 must not bypass markBeautifierContent/markChapterLeadingImages: those markers
feed the shared settings. Preserve FB2 semantic blocks and secondary notes/comments
bodies in the same classifier; allow FB2 inline/image vocabulary. Only Kobo link
decoration is EPUB-only. Keep the focused sharedNormalization model regression,
and do not claim it ran when only the device package was cross-compiled.


### EPUB / FB2 bottom-footnote integration

Use the standard KOReader CSS profiles on our pinned CREngine, not a parallel
EPUB DOM classifier or private markers. reader_notes.h owns semantic popup
helpers and EPUB in-page CSS. Popup FOOTNOTE/FOOTNOTE_IGNORE and in-page
FOOTNOTE_INPAGE are independent; do not auto-promote popup hints/classes to
in-page notes. NOTEREF_IGNORE is evaluated on source links by CREngine.
Use late/inpage-footnote CSS for the legacy EPUB 80% size. FB2 descendant
sections receive hints, with sizes only on the top-level sections (75% notes,
85% comments), never repeated shrinkage at each nesting level. Preserve the
agreed superscript numeric labels and default-on FB2 comments as explicit fork
choices. Do not enable optional KOReader classic/Wikipedia/extension/flattening
profiles globally. See doc/fb2-footnotes.md for the maintained contract, pinned sources and scope; completed review history is in FB2_FORK_PLAN.md#archive-koreader.

Keep fb2_inpage_footnotes and global/default compatibility. Apply PROP_FOOTNOTES
at configure and live reflow, so off disables publisher in-page hints too,
without disabling popup. Keep initial-load/live-toggle mode salts identical;
EPUB v3 and FB2 v6 invalidate earlier rendering styles, not reading data.
Select fb2Document_ before configure() installs the parse-time stylesheet.
Use getUnboxedParent() for FB2 run-in title normalization. No page splitter,
dependency or save/progress fork is needed. Native engine checks and Qt5
package CI do not replace complete UI/lifecycle tests or Kobo visual comparison.

InDesign EPUB 2 support uses the narrow CSS structure
`._idFootnotes > ._idFootnote`. Never mark the entire group or the
`_idFootnoteLink` / `_idFootnoteAnchor` links, and never hard-code book paths,
IDs or note numbers into production. This is a publisher-profile extension,
not KOReader's per-spec default. Keep the synthetic InDesign matrix portable;
`--publisher-notes PATH` is a separate opt-in real-book validation owned by the
fork device workflow. Do not copy commercial book text/fonts into new fixtures
or runtime assets. EPUB cache v3 invalidates earlier per-spec-only layouts.

### beta7 integration — approved RU, CI passed, device gate pending

Use FB2_FORK_PLAN.md, including its complete Beta7 approved integration archive,
for the e86533c integration. The checked binary is dddace7 / CI 42500439074;
focused native + publisher and Kobo Qt5 passed. This is not device or full desktop
validation. Documentation/format-only commits do not identify a new checked binary. The user approved the complete proposal, including
the screenshot-driven refinements; these are applied. Further wording changes
require new approval.
The user confirmed InDesign footnotes work on-device in ab6d9a0; preserve that
baseline, EPUB v3/FB2 v6 policies, the standalone publisher-book regression,
and shared formatting/progress. Adapt fork About/debug to upstream's common
app/about/AboutPage for Library and Settings; do not retain an old About screen
or obsolete SettingsPanelInputs solely for the fork label. Check temporary
anchor-based preview drawing/caches on pages with notes. Match the new shared
row/description geometry through the common factory, not old copied metrics.
Keep standby default-off with upstream confirmation; no revival of cache cleanup
or unrelated Wi-Fi work. Initial merge-tree review is not a completed merge,
compile, native run or visual check. Record those stages separately.

### Documentation and manual material ownership

Keep permanent instructions and active proposals/plans in doc/. Consolidate completed
work reports into FB2_FORK_PLAN.md before deleting them, preserving approvals,
sources, risks, CI/artifact identities and unrun checks. Fix references in the same
change. Do not turn a deleted work log into a differently named duplicate in doc/.
Keep stable technical contracts in their area guide as well as historical evidence.

User device captures and opt-in publisher books live under tests/manual/; read its
README for provenance and materials.sha256 for byte-preservation checks. Portable
synthetic/public-domain fixtures remain in tests/fixtures/. Move originals without
editing images or repacking books, update all workflow arguments, and bind each
capture set to its actual binary SHA. A screenshot commit is not a binary build.
Record a per-screen context audit in the canonical plan, propose wording changes
for approval, and never infer beta7 layout validation from beta6 captures.

About fork metadata/debug now belongs under src/app/about/, used by both Library
and reader Settings. Do not restore build/debug fields in SettingsPanelInputs.
The only addition to upstream AboutPage is its fork-row helper call. Keep hidden
debug input out of the stock version/commit labels and preserve their painting.
Run the extended fork CI selection (shared About/settings/buttons/lifecycle plus
locale, EPUB/FB2 and cache); its focused coverage is not the full native suite.
Keep anchor_preview.h regressions on semantic/long/InDesign EPUB and FB2 notes:
preview must restore source position, pagination, bottom fragments and reading image.

### One-off Qt6 package policy

Qt5 remains the fork's primary ongoing target. The explicit manual
`build_qt6_once=true` input of the device workflow permits an occasional Qt6
adapter/package check; do not add Qt6 to automatic pushes on that basis.
See doc/qt6-one-off.md. Record binary SHA/artifact separately from device proof.

### Beta7 device review and author proposals

Use doc/beta7-ui-review.md for the pending screenshot findings R1/R2 and
per-frame evidence. Do not treat the saved-language confirmation as proof of
an actual language change after reopening. Preserve original captures and
provenance in doc/dddace7/. External language packs and a graphics-reading
profile in the author inventory are proposals only. Detailed cache cleanup
notes are historical; never revive that removed lifecycle state machine
without a new explicit implementation request. New Russian wording still
requires approval before editing assets.

### Approved beta7 review fixes

R1/R2 are approved: paragraph help must call the visible option «Из книги»;
the debug-language radio selection must represent persisted DebugLanguage,
not the currently resolved UI locale. Opening/cancelling must not write markers
or change the live locale. Keep async persistence and next-opening application.
R3 is approved using the existing-key fallback: group «Резервные шрифты»;
existing serif/sans/mono keys name reserve fonts in both rows and chooser titles.
Do not add translation keys, a long help message, RU-only title branching,
prefix-stripping hacks or catalog filtering. Serif is a fallback/family role,
not a promise that every listed font is classified as serif.


### Comics/progress constraints and opening performance

Follow doc/comics-progress-proposal.md and doc/coolreader-opening-analysis.md.
New captures establish zero configured margins/chrome with residual bottom space;
do not claim a proven 18px engine margin or introduce magic cropping. Existing
engine native header is already disabled. Comics must be an effective override,
never persist its zeroed geometry into ordinary layout values. The single EN/RU title and per-book scope are approved. Keep base layout
controls editable while the effective comics profile ignores their values. Reuse upstream
TOC/page progress and invalidate footer ticks on reflow; never parse DOM in paint.
The public Vlasovsoft source is from 2015 and not verified as the 2022/2023
binary source. Measure existing opening phases before claiming optimization.


## Согласованный контракт режима комиксов

Пользователь утвердил поведение перед реализацией:
- Режим сохраняется отдельно для каждой книги; default off.
- Изображение вписывается целиком с сохранением пропорций, без crop/stretch.
- Поля и колонтитулы НЕ блокируются: пользователь может менять их обычным
  способом, значения сохраняются, но при активном режиме не применяются.
  После выключения применяются последние сохранённые значения, а не snapshot
  на момент включения. Apply/cancel сохраняет обычную семантику настроек.
- Одна исходная страница на экран в обеих ориентациях; без auto-spread.
- Отметки глав без отдельной настройки, только на существующей progress bar.
- Название EN/RU принято; новых descriptions не требуется.

Сохранять этот контракт; проверять edit→apply
при активном режиме, close/reopen и выключение с восстановлением обновлённых
значений; отдельно edit→cancel не должен менять сохранённую базу.

Comics implementation: see doc/comics-progress-proposal.md and engine.comics regression.
Never claim device verification from a synthetic regression or report unmeasured speedups.
