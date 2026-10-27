# Working on Kobalt

Read [Building](doc/building.md) before running a build. This branch is rebuilding the development tooling incrementally. Do not restore retired runners wholesale.

## Build ownership

Use CMake and presets for application builds. Do not add Python tooling or an internal build-support framework. Keep platform build definitions, presets and toolchains under `platforms/`. Shared application source remains under `src/`. Shared asset inputs live under root `assets/`; stage runtime bundles inside each platform output. Do not read runtime assets from another checkout.

Kobo and Android builds use prepared `kobuild` images. Use native ARM64 Android containers on ARM64 hosts when the selected image supports them. The compiler, sysroot, Qt modules, Android SDK, NDK and JDK belong to kobuild. Fix missing SDK components in kobuild rather than building Qt modules or repairing SDKs in the application. Never compile Qt or any Qt module as part of a Kobalt build.

The Mac app uses an existing native Qt SDK. A Kobo cross SDK cannot replace the native Mac SDK. Application-specific dependencies such as CREngine remain application inputs, separate from Qt SDK production.

Mac development uses Qt 6.11.3. From `platforms/desktop/`, build `mac-tests` and run `ctest --preset mac` for native headless checks. Build `mac-screenshots` for the current captures in `build/screenshots/mac/`. Both paths use the same compiled desktop implementation as the app. Keep test profiles isolated and remove temporary fixture and capture state when a run ends. See [headless tests and screenshots](doc/building.md#headless-tests-and-screenshots) for scope and commands.

Restore portable regression assertions from `develop` with only necessary path, fixture and SDK adaptations. The `mac-tests` preset builds desktop, model, engine, session, resource and component checks; use `ctest --preset mac -L model` for the restored model group. Reuse compiled application objects. Native checks run through `platforms/desktop/run-test.cmake` so failure and timeout cleanup does not depend on test destructors. Keep the Fusion style used by the app and the original component suite. Await asynchronous UI completion before sending the next action. A page center can be covered by a panel; target exposed paper when dismissing it.

Use the checked-in Trevelyan's Type Tester fixture for general reading checks and screenshots. It includes the chapter-heading CSS fix documented in [Native validation](doc/validation.md#book-fixtures). Keep synthetic books for focused regressions, not as a replacement for the general book fixture.

Use [Native validation](doc/validation.md) for the test inventory, feature coverage and explicit gaps. The everyday loop is `mac-fast-tests` followed by `ctest --preset mac-fast`; `mac-tests` and `ctest --preset mac` run the full native set. `mac-screenshots` produces the current image set, checksum manifest and local gallery. Use `ctest --preset mac -L lifecycle` for reader lifecycle and annotation-frame checks, `-L ui` for shared canvas and paint checks, `-L platform` for portable adapter checks and `-L corpus` for the representative literary corpus. All four groups belong to the integration lane. Keep them in the integration lane and link the existing production objects; do not restore the old test-host application or scroll-only canvas. Keep public validation independent of private runtime services. Public Nickel compatibility checks are deferred until the user chooses an approach.

Flag suboptimal approaches before extending them. Identify duplicated build logic, application-side SDK workarounds, dependence on an old worktree, unnecessary rebuilds and unclear ownership. Explain the issue and propose a simpler approach.

## Execution and cleanup

Run native Qt commands outside the agent sandbox on the development Mac. Headless Qt tools still need CPU capabilities that the sandbox can hide. Container commands need access to the engine socket. Keep outputs and caches in the authorized workspace.

Do not open a visible app or run visible UI tests without authorization. Prefer native headless validation while editing, then validate Kobo and Android integrations last. Reuse prepared SDKs and verified dependencies.

Stop and reap run-owned helper processes and remove scratch profiles, staging directories and temporary containers when a run ends, including on failure or interruption. Preserve the user's app state, active runs, SDKs and incremental caches. Retain useful current results and unresolved failure evidence rather than every successful raw log indefinitely.

## Fork workflow (kobalt_ru)

This fork follows `FB2_FORK_PLAN.md` in the repository root. Keep that plan current: mark every stage and checklist item with its progress status (planned / in progress / done) in the same change as the work it describes, so the plan alone is enough to resume after the sandbox or local checkout is wiped.

Always commit completed work and push it to the current working branch (`git push origin <current-branch>`). The local repository and sandbox can be cleared at any time, so unpushed work is not considered safe. Never switch, create or push to another branch.

The priority target is the Kobo **Qt5** package (`Kobalt-qt5.zip`, firmware 4.x). Desktop (mac) and Android are not priorities for this fork: do not block Kobo work on them, but keep shared code portable and avoid knowingly breaking their builds. Section 6 of `FB2_FORK_PLAN.md` (Russian interface localization) is implemented as a second compiled string table with an English fallback: when touching UI strings, update both `assets/en.strings` and `assets/ru.strings` — the build fails on any key or `%n` slot mismatch.

Every fork feature stays behind an explicit flag or in a separate file so the EPUB/upstream code paths keep their original behavior; the full change map and conflict-resolution rules are section 7.2 of `FB2_FORK_PLAN.md`. Read it before rebasing onto a new upstream release, and keep the DOM-cache marker (`:fb2-vN:` in `reader_source.cpp`/`reader_model.cpp`) in sync if FB2 parsing changes. The Kobo Qt5 SDK predates Qt 5.10: do not use APIs such as `QStringView`.

Interim device builds come from the push-triggered `.github/workflows/kobo-qt5-device.yml` (artifact `Kobalt-qt5-device`); a local SDK build is `KOBALT_FORK_NAME=kobalt_ru bash platforms/kobo/build.sh qt5`. `KOBALT_FORK_NAME` defaults to empty, which hides the fork row on the About page and preserves the stock layout; only the fork workflow sets it. The fork row is a single added label (`kobalt_ru · <UTC build time>` from `KOBALT_BUILD_TIME`); never change the stock version/commit rows. The "Clear book cache" action may only delete disposable caches via `storage::clearRenderingCaches()` (`.cache/` and `cache/reopen-layout.json`) — never the database, settings, annotations or per-book state — and must keep the confirm-then-exit flow. Before pushing, run the offline checks that need no Qt SDK: `cmake -DTEST_TMP_ROOT=$(mktemp -d) -P platforms/strings-test.cmake`, the en/ru string parity check, `bash platforms/ci/test.sh` when `jq` is available, and `git diff --check`. On a Mac, also run the `model` ctest group (`fb2`, `strings_locale`, `local_resources`, `nickel_book_menu`, `reading_book`). The complete pre-push checklist is section 7.5 of the plan. User-facing behavior of the fork is documented in Russian in `doc/fork-kobalt_ru.md`; update it together with user-visible changes.

## Repository practice

Read [Storage compatibility](doc/storage.md) before changing settings, book state, bookmarks, annotations or rendering caches. Keep saved reading data separate from disposable caches. App releases must not reset either format merely because the release version changed. Compatible optional data fields keep the current format version and must load older records with explicit defaults. Preserve supported optional fields when editing records. An incompatible saved-data change requires a format-version bump, a migration that preserves existing user data, and regression tests before it ships. Reject unsupported saved-data versions without overwriting the original files. Do not assume older builds preserve unknown fields or can safely write newer schemas.

Bump the format version in `documentCacheChecksum` when parser or preprocessing changes make parsed-book caches incompatible. Bump the inspection-cache format separately when verdict rules change incompatibly. Cache invalidation must rebuild disposable data only; it must never clear settings, bookmarks or annotations.

Keep Markdown in `doc/`, except the root `README.md` and `AGENTS.md`. The user owns root README edits. Keep these instructions and build commands current in the same change as tooling updates.

In this fork, pushing completed work to the current working branch is pre-authorized and required (see [Fork workflow](#fork-workflow-kobalt_ru)); other publications (tags, releases, other branches) still need explicit authorization. Signed operations require an advance warning for 1Password prompts. Never add an agent coauthor: the sandbox may install a `commit-msg` hook that appends a `Co-authored-by: arena-agent` trailer, so create fork commits with `git commit --no-verify` and verify with `git log --grep=arena-agent` that the history stays trailer-free. Preserve unrelated changes and the original checkout.

Use lowercase release-stage words in commit messages, including `alpha` and `beta`, even when part of a release name. The abbreviation `RC` may remain uppercase.
