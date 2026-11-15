# Beta.8 migration — integration checkpoint

## Принятое решение — текущий этап интеграции

Все предложенные переводы согласованы пользователем, включая уточнённую группу.
258 согласованных строк EN/RU внесены в assets. Runtime merge выполнен;
проверки и пакет для устройства фиксируются в текущем результате ниже.

Для кнопок использовать **позиционную модель upstream beta.8**: held(),
назначения одинаковых действий, none, частичные назначения и отображение
настроек — из базовой версии. От форка оставить только Libra 2 / Qt5 landscape
correction в определении поворота. Старый actionRotation убрать при интеграции,
не применять его поверх held(). Это новое решение заменяет прежнее требование
сохранять physical-action семантику для обоих-next/none.

Если механизмы дублируют друг друга (standby при открытии, notes и т.п.),
брать цельную реализацию upstream. Если изменены отдельные пункты общей функции,
делать контекстное слияние с сохранением согласованных fork-отличий.
Слияние upstream оформлено коммитом 190031dc с последующими исправлениями
API/инициализации и регрессионными проверками. Результаты CI и пакет
для физической проверки — в FB2_FORK_PLAN.md.

- Fork: `130091fb93e455727d00acb77c312710ad8a9290`.
- Target: `v1.0-beta.8`, `679175046d8c806ea3b404525180fbc29a26fcd2`.
- Common ancestor: beta.7 `e86533c80e8aa41bfc7d79c9e445cbd4a6405039`.
- Scope: 433 upstream-changed files; three-way trial reports 31 conflicting paths.
- Method: `git merge-tree --write-tree <fork> v1.0-beta.8`; index/worktree untouched.
- **No runtime merge published yet.** Trial tree is not a distributable build.
- First gate: approve `beta8-strings-review.md` before applying new EN/RU strings.

## Conflict resolution checklist

| Path | Required resolution |
|---|---|
| `assets/en.strings` | Approve proposal; preserve fork overrides; regenerate EN/RU parity and placeholder checks. |
| `doc/storage.md` | Combine storage and validation contracts; record DB backup/rollback and explicit legacy preference migration. |
| `doc/validation/links.md` | Combine storage and validation contracts; record DB backup/rollback and explicit legacy preference migration. |
| `platforms/android/CMakeLists.txt` | Combine upstream targets/assets with fork About/locale/FB2 tests; no Python in CMake; Qt5 first. |
| `platforms/desktop/CMakeLists.txt` | Combine upstream targets/assets with fork About/locale/FB2 tests; no Python in CMake; Qt5 first. |
| `platforms/kobo/CMakeLists.txt` | Combine upstream targets/assets with fork About/locale/FB2 tests; no Python in CMake; Qt5 first. |
| `platforms/kobo/common/native_ui.h` | Keep confirmed Libra 2 rotation and button-action transform; add upstream API without reverting it. |
| `platforms/kobo/nickel-qt5/native_ui.cpp` | Keep confirmed Libra 2 rotation and button-action transform; add upstream API without reverting it. |
| `src/app/reader/panels/bookmark_list.cpp` | Contextual merge: upstream features plus fork locale/About/comics/progress invariants; no blanket ours/theirs. |
| `src/app/reader/settings/settings_panel.cpp` | Contextual merge: upstream features plus fork locale/About/comics/progress invariants; no blanket ours/theirs. |
| `src/app/reader/settings/settings_panel.h` | Contextual merge: upstream features plus fork locale/About/comics/progress invariants; no blanket ours/theirs. |
| `src/app/reader/views/navigation.cpp` | Contextual merge: upstream features plus fork locale/About/comics/progress invariants; no blanket ours/theirs. |
| `src/app/reader/views/reading_page.cpp` | Contextual merge: upstream features plus fork locale/About/comics/progress invariants; no blanket ours/theirs. |
| `src/app/reader/views/sleep_screen.h` | Contextual merge: upstream features plus fork locale/About/comics/progress invariants; no blanket ours/theirs. |
| `src/app/reader/window/reader_build.cpp` | Keep deferred Appearance, fork About/dark toggle/comics/progress; add upstream features and new dark-toggle visibility gate. |
| `src/app/reader/window/reader_p.h` | Contextual merge: upstream features plus fork locale/About/comics/progress invariants; no blanket ours/theirs. |
| `src/app/reader/window/reader_refresh.cpp` | Contextual merge: upstream features plus fork locale/About/comics/progress invariants; no blanket ours/theirs. |
| `src/app/reader/window/reader_settings.cpp` | Preserve canonical physical-button assignment UI; integrate hidden features, presets, privacy and single notes switch. |
| `src/engine/crengine/column_view.h` | Retain centered comic bounds/hits, portrait-only center rotation and spread mapping; integrate upstream selection geometry. |
| `src/engine/crengine/engine_document.cpp` | Replace fork notes with upstream canonical inpage_footnotes/default false; retain FB2/comics/frameImage/cache policy and waveform. |
| `src/engine/crengine/engine_document.h` | Replace fork notes with upstream canonical inpage_footnotes/default false; retain FB2/comics/frameImage/cache policy and waveform. |
| `src/reading/appearance_settings.cpp` | One canonical notes key; explicit legacy migration; preserve comics/progress durable vs effective settings; upstream presets/Zen. |
| `src/reading/appearance_settings.h` | One canonical notes key; explicit legacy migration; preserve comics/progress durable vs effective settings; upstream presets/Zen. |
| `src/reading/reader_session.cpp` | Keep complete-pair capture/publication and cancellation; integrate upstream session/notes APIs. |
| `src/reading/reading_book.h` | Contextual merge: upstream features plus fork locale/About/comics/progress invariants; no blanket ours/theirs. |
| `tests/desktop.cpp` | Combine regressions, not choose one side. Replace obsolete fork notes expectations with upstream contract and FB2 coverage. |
| `tests/model/appearance_settings.cpp` | One canonical notes key; explicit legacy migration; preserve comics/progress durable vs effective settings; upstream presets/Zen. |
| `tests/model/document_cache.cpp` | Combine regressions, not choose one side. Replace obsolete fork notes expectations with upstream contract and FB2 coverage. |
| `tests/model/links.cpp` | Combine regressions, not choose one side. Replace obsolete fork notes expectations with upstream contract and FB2 coverage. |
| `tests/model/ui.cpp` | Combine regressions, not choose one side. Replace obsolete fork notes expectations with upstream contract and FB2 coverage. |
| `tests/reader_lifecycle.cpp` | Combine regressions, not choose one side. Replace obsolete fork notes expectations with upstream contract and FB2 coverage. |

## Changes that may auto-merge but still require semantic review

- `reader_links.cpp`, `reader_source.cpp`, `reader_styles.h.in`: remove parallel fork note machinery; carry upstream DOM markers/link/selection recognition, preserve native FB2 source handling.
- `page_buttons.h`: preserve actionRotation and explicit action semantics verified on Libra 2.
- Standby/coordinator: one upstream gate with openingWorkPending and busy && !run.fallbackAlert; no parallel power policy or resumed opening optimization.
- Kobo native progress: duplicate-ID upstream correction, without losing FB2 percentage/time-only save semantics.
- Schema/profile/storage: upstream DB upgrade; backup before first run, rollback by restoring matching backup only.
- Features: extend upstream features registry with disable_dark_mode; apply to reading-menu toggle only, preserve active theme and respect Zen/hidden feature composition.

## Test and delivery gates

1. Approval of proposed EN/RU labels; keep approved standby warning and existing fork wording.
2. Resolve all 31 paths and review clean auto-merges; no conflict markers, duplicate switches or retired fork notes branch.
3. Offline strings/assets/checksums/YAML/shell checks; native notes/FB2/EPUB/links/selection/cache/settings/hidden-features/Zen/presets/Libra/comics/lifecycle/storage coverage.
4. Kobo Qt5 adapter build/package/content checks; run full relevant suite, not merely the historical focused regex.
5. Back up device profile and Kobo DB; migration/reopen/save, notes on/off and links, themes/hidden toggle, comics/progress, physical keys and standby on device.
6. Keep the note-gap issue OPEN independently of successful upstream notes migration.

Current code remains the previously verified beta.7-based runtime c4b8c1e.
This checkpoint changes documentation/evidence only, so it does not produce a new binary.


## Context audit after translation review

See [beta8-context-review.md](beta8-context-review.md) for code-backed letter
placeholders, action-vs-list labels, session turn counter, sync positions and
functional overlaps beyond notes. The beta.8 page-button change is a semantic
collision, not a clean equivalent: positional held() swapping differs from the
fork physical-action rotation for both-next, none and partial assignments.
Never apply both transforms or silently reinterpret saved assignments. Preserve
the confirmed Libra 2 contract until a separate positional migration is approved.

Main translation batch accepted by user with fixed labels/reader terminology;
20 context-reviewed keys are a new proposal awaiting confirmation. No runtime
merge or assets were published by this documentation-only step.
