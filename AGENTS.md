# Working on Kobalt

Read [Building](doc/building.md) before running a build. This branch is rebuilding the development tooling incrementally. Do not restore retired runners wholesale.

## Build ownership

Use CMake and presets for application builds. Do not add Python tooling or an internal build-support framework. Keep platform build definitions, presets and toolchains under `platforms/`. Shared application source remains under `src/`. Shared asset inputs live under root `assets/`; stage runtime bundles inside each platform output. Do not read runtime assets from another checkout.

Kobo and Android builds use prepared `kobuild` images. Use native ARM64 Android containers on ARM64 hosts when the selected image supports them. The compiler, sysroot, Qt modules, Android SDK, NDK and JDK belong to kobuild. Fix missing SDK components in kobuild rather than building Qt modules or repairing SDKs in the application. Never compile Qt or any Qt module as part of a Kobalt build.

The Mac app uses an existing native Qt SDK. A Kobo cross SDK cannot replace the native Mac SDK. Application-specific dependencies such as CREngine remain application inputs, separate from Qt SDK production.

The macOS and Linux desktop clients are tools for development and testing on desktop. They are not release versions of the app. Keep desktop builds, native tests and screenshot galleries local. Do not add desktop release packages or desktop CI jobs unless the user explicitly requests them.

Desktop development accepts Qt 6.11 or newer Qt 6 from one prepared native SDK. Linux uses the matching `linux`, `linux-light-tests`, `linux-fast-tests`, `linux-tests` and screenshot presets in `platforms/desktop/`; keep outputs under `build/desktop-linux/` and `build/screenshots/linux*/`. Desktop builds default to sensorless manual orientation; enable `KOBALT_DESKTOP_SENSORS` only when validating the real sensor adapter with a matching prepared Qt Sensors module. Mac development uses the `mac` presets. From `platforms/desktop/`, build `mac-tests` and run `ctest --preset mac` for native headless checks. Build `mac-screenshots` for the current captures in `build/screenshots/mac/`. Both paths use the same compiled desktop implementation as the app. Keep test profiles isolated and remove temporary fixture and capture state when a run ends. See [headless tests and screenshots](doc/building.md#headless-tests-and-screenshots) for scope and commands.

Restore portable regression assertions from `develop` with only necessary path, fixture and SDK adaptations. The `mac-tests` preset builds desktop, model, engine, session, resource and component checks; use `ctest --preset mac -L model` for the restored model group. Reuse compiled application objects. Native checks run through `platforms/desktop/run-test.cmake` so failure and timeout cleanup does not depend on test destructors. Keep the Fusion style used by the app and the original component suite. Await asynchronous UI completion before sending the next action. A page center can be covered by a panel; target exposed paper when dismissing it.

Use the checked-in Trevelyan's Type Tester fixture for general reading checks and screenshots. It includes the chapter-heading CSS fix documented in [Native validation](doc/validation.md#book-fixtures). Keep synthetic books for focused regressions, not as a replacement for the general book fixture. The full screenshot gallery also uses the checksum-pinned international chapters and Gutenberg editions documented in Native validation. Keep those fixtures, their provenance and their scenes when extending the tour. Gallery captures are frameless production reader or Library images at 300 PPI, at six-inch and seven-inch sizes, in the light theme, the dark theme and the light theme with 150% interface text. Each gallery card represents one screen; size, variant and page choose its raw file. Require all three variants at both sizes for each screen, with matching light/dark pages at each size; 150% text may add pages. Keep device frames in the HTML viewer only, and retain filenames and checksums in the manifest for comparisons.

Use [Native validation](doc/validation.md) for the test inventory, feature coverage and explicit gaps. The routine UI loop is `mac-light-tests` followed by `ctest --preset mac-light`; `mac-light-screenshots` captures the primary views. Keep the light lane focused on primary views and features. It does not replace full release validation. `mac-fast-tests` and `ctest --preset mac-fast` retain the broader fast checks; `mac-tests` and `ctest --preset mac` run the full native set. `mac-screenshots` produces the current image set, checksum manifest and local gallery. Set `KOBALT_GALLERY_SCENE` to capture only the named gallery cards into `build/screenshots/mac-scenes/`; it never replaces the full gallery. Use `ctest --preset mac -L lifecycle` for reader lifecycle and annotation-frame checks, `-L ui` for shared canvas and paint checks, `-L platform` for portable adapter checks and `-L corpus` for the representative literary corpus. All four groups belong to the integration lane. Keep them in the integration lane and link the existing production objects; do not restore the old test-host application or scroll-only canvas. Keep public validation independent of private runtime services. Public Nickel compatibility checks are deferred until the user chooses an approach.

Use [Release evaluation](doc/release-evaluation.md) to compare a candidate with a baseline: `cmake -DRECORD_BASELINE=ON -P platforms/eval/run.cmake` on the baseline, `cmake -P platforms/eval/run.cmake` on the candidate, then `cmake -P platforms/eval/compare.cmake`. The quick profile is the default; `-DPROFILE=release` runs the full suite and gallery. Keep these scripts thin: they call the presets and the performance mode, and they never replace them. End-to-end performance scenarios live only in `platforms/eval/scenarios.json` and run through the built-in scenario player on every platform; add a scenario there, not as C++ test code. The player is compiled only into the test executable and into evaluation packages (`build.sh qt5-eval` or `qt6-eval`), never into release packages. Device timings come from an evaluation package on a Kobo, collected with `platforms/kobo/install-eval.cmake` and `collect-eval.cmake` or through an external runner passed with `DEVICE_RUNNER`. Keep that contract generic: everything must also work without a private runner. Never re-record a baseline to make a report pass. Real-device runs kept for comparing devices live in `platforms/eval/devices/`; see [Device benchmarks](doc/device-benchmarks.md).

Flag suboptimal approaches before extending them. Identify duplicated build logic, application-side SDK workarounds, dependence on an old worktree, unnecessary rebuilds and unclear ownership. Explain the issue and propose a simpler approach.

Do not record fixed test, case, assertion or screenshot totals in documentation or these instructions. Describe coverage and verification commands instead. Use `ctest --preset mac -N`, with optional `-L` or `-R` filters, for the current test inventory. Keep meaningful fixture sizes, limits and measured durations.

When adding a UI screen that needs screenshot coverage, include its scenes in the full gallery in the same change. When adding or removing screenshot scenes, update the full-gallery expected count in `tests/desktop.cpp` in that change. Run `cmake --build --preset mac-screenshots` from `platforms/desktop/` to verify that the complete collection matches that count.

The root `./start.sh` builds and opens the Qt developer launcher on Linux and Mac. Keep launcher sources and presets under `platforms/launcher/`. Startup checks must explain missing dependencies in the terminal, keep Docker optional for desktop work, and never install tools or pull SDK images automatically. Its actions delegate to the existing platform presets, Kobo build script and autonomous scenario player. Keep visible scenarios separate from the full headless assertion suite. Preserve run-owned process, container and scratch cleanup when changing launcher actions.

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

## Execution and cleanup

For repeated development deployments, it is imperative to assess only the changes since the last validated build and reuse applicable validation results. Do not automatically rerun the full test suite or a smoke test for every minor change. Small UI spacing or styling adjustments may need only a build and visual inspection. Use focused checks when the change warrants them, and do not delay an explicitly requested deployment with redundant validation. Keep full release validation separate. State which checks were run, reused or skipped.

Run native Qt commands outside the agent sandbox on the development Mac. Headless Qt tools still need CPU capabilities that the sandbox can hide. Container commands need access to the engine socket. Keep outputs and caches in the authorized workspace.

Do not open a visible app or run visible UI tests without authorization. Prefer native headless validation while editing, then validate Kobo and Android integrations last. Reuse prepared SDKs and verified dependencies.

Treat storage IO on Kobo as expensive. Devices store data on eMMC or, on older models, an SD card, and their speed differs widely. Measure storage-bound work, such as syncs and saves, on both kinds, and judge changes by the slower one.

Stop and reap run-owned helper processes and remove scratch profiles, staging directories and temporary containers when a run ends, including on failure or interruption. Preserve the user's app state, active runs, SDKs and incremental caches. Retain useful current results and unresolved failure evidence rather than every successful raw log indefinitely.

## Fork workflow (kobalt_ru)

This fork follows `FB2_FORK_PLAN.md` in the repository root. Keep that plan current: mark every stage and checklist item with its progress status (planned / in progress / done) in the same change as the work it describes, so the plan alone is enough to resume after the sandbox or local checkout is wiped.

Always commit completed work and push it to the current working branch (`git push origin <current-branch>`). The local repository and sandbox can be cleared at any time, so unpushed work is not considered safe. Never switch, create or push to another branch.

The priority target is the Kobo **Qt5** package (`Kobalt-qt5.zip`, firmware 4.x). Desktop (mac) and Android are not priorities for this fork: do not block Kobo work on them, but keep shared code portable and avoid knowingly breaking their builds. Section 6 of `FB2_FORK_PLAN.md` (Russian interface localization) is implemented as a second compiled string table with an English fallback: when touching UI strings, update both `assets/en.strings` and `assets/ru.strings` — the build fails on any key or `%n` slot mismatch.

Every fork feature stays behind an explicit flag or in a separate file so the EPUB/upstream code paths keep their original behavior; the full change map and conflict-resolution rules are section 7.2 of `FB2_FORK_PLAN.md`. Read it before rebasing onto a new upstream release, and bump the shared DOM-cache marker (`:fb2-vN:` in `reader_fb2.h`, used by `reader_source.cpp` and `reader_model.cpp`) if FB2 parsing or default FB2 CSS changes. FB2-only override CSS lives beside this marker in `reader_fb2.h`; keep popup `footnote` unconditional, and gate bottom-page `footnote-inpage` CSS on the default-on `fb2_inpage_footnotes` appearance key. Numeric note-title normalization belongs to the FB2-only helper; never strip brackets in the main text. Include the on/off mode in FB2 cache policy, appearance equality/copy and global defaults. The Kobo Qt5 SDK predates Qt 5.10: do not use APIs such as `QStringView`.

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
  changes, not for releases or code moves. Preserve the empty EPUB salt.
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

## Repository practice

Read [Storage compatibility](doc/storage.md) before changing settings, book state, bookmarks, annotations or rendering caches. This also applies to backup tools and test database inspection: follow the backup contract and never open the live exclusive profile through an external SQLite connection. Keep saved reading data separate from disposable caches. App releases must not reset either format merely because the release version changed. Compatible optional data fields keep the current format version and must load older records with explicit defaults. Preserve supported optional fields when editing records. An incompatible saved-data change requires a format-version bump, a migration that preserves existing user data, and regression tests before it ships. Never overwrite or delete the original files of unsupported or damaged saved data. Profile preparation moves them into `backups/` and starts from defaults; other reads reject them and leave them in place. Do not assume older builds preserve unknown fields or can safely write newer schemas.

Bump the format version in `documentCacheChecksum` when parser or preprocessing changes make parsed-book caches incompatible. Bump the inspection-cache format separately when verdict rules change incompatibly. Cache invalidation must rebuild disposable data only; it must never clear settings, bookmarks or annotations.

Keep Markdown in `doc/`, except the root `README.md` and `AGENTS.md`. The user owns root README edits. Keep these instructions and build commands current in the same change as tooling updates.

In this fork, pushing completed work to the current working branch is pre-authorized and required (see [Fork workflow](#fork-workflow-kobalt_ru)); other publications (tags, releases, other branches) still need explicit authorization. Signed operations require an advance warning for 1Password prompts. Never add an agent coauthor: the sandbox may install a `commit-msg` hook that appends a `Co-authored-by: arena-agent` trailer, so create fork commits with `git commit --no-verify` and verify with `git log --grep=arena-agent` that the history stays trailer-free. Preserve unrelated changes and the original checkout.

Work on a feature branch by default. Before the first edit, create a dedicated branch and a Git worktree for it. The branch `feature/name-of-feature` always lives in the worktree `worktrees/name-of-feature` in the repository: the folder name is the branch name without the `feature/` prefix. The `feature/` prefix is required. Git ignores the `worktrees/` folder. This applies to small changes too, and also when no other worker seems active. Work directly in the shared `develop` checkout only when the user says so for that task. Otherwise, other agents and the user can edit `develop` at any time: only read and investigate there. If you find that you started work in `develop` without being asked, stop and move only your own changes into a worktree. Leave every other change in `develop` as it is. When authorized to integrate a feature into `develop`, cherry-pick its commits or merge it as a fast-forward. Never create a merge commit.

One conversation thread usually means one worktree and one branch. When the user asks for several features in one thread, check whether they would change the same code. If they would, ask whether the user wants a separate worktree for each feature before the first edit.

After successful integration, normally remove the worker's worktree and feature branch unless the user asks to keep them. Before removal, verify that the worktree has no uncommitted changes and that all intended changes are integrated. Preserve any needed test evidence or other artifacts before removing the worktree.

Use lowercase release-stage words in commit messages, including `alpha` and `beta`, even when part of a release name. The abbreviation `RC` may remain uppercase.

## beta6 integration scope

Use upstream frontlight gesture feedback, without the fork quiet-light UI, key or migration. Cache cleanup is deferred until after beta; do not restore its action, helper, worker retirement, lifecycle barriers or special tests. Never delete device caches as part of removing the feature. Use upstream progress/save, retaining necessary FB2 integration. Keep FB2 formatting/settings aligned with the shared EPUB implementation as upstream evolves; do not freeze a separate formatting implementation.
