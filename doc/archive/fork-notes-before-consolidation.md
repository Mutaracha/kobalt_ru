# Historical fork notes before documentation consolidation

Archive only. These records contain superseded requirements and intermediate
statuses. Do not use them as current agent instructions or a work queue.
Current decisions: [fork-decisions.md](../fork-decisions.md).
Current tasks: [FB2_FORK_PLAN.md](../../FB2_FORK_PLAN.md).
Original ordering is preserved for provenance; later explicit decisions supersede
older entries. CI success is not physical-device acceptance.

## Former AGENTS status and integration notes

## Текущие device-fixes после build153

Пользователь согласовал sleep.design_pill = «Капсула». Шаблон письма не менять
без согласования. Исправить немедленный preview comics (viewport + chrome),
лишний верхний отступ Layout без уменьшения hit target и fitting sleep-текстов.
Перед нижними сносками нужен обычный межстрочный зазор основного текста;
не менять размер примечаний и настройки нижнего поля. Не возобновлять
оптимизацию открытия и не менять рабочие waveform / beta8 button semantics.
Тестовая сборка157 (5edbf197): native и Qt5 CI прошли; устройство ещё не проверено.

<!-- Current migration/acceptance status is maintained in FB2_FORK_PLAN.md. -->

## Принятое решение — текущий этап интеграции

Все предложенные переводы согласованы пользователем, включая уточнённую группу.
258 строк EN/RU внесены в assets; это подготовка ресурсов, **не готовая beta.8**.
Новые функции ещё не доступны без завершения runtime merge.

Для кнопок использовать **позиционную модель upstream beta.8**: held(),
назначения одинаковых действий, none, частичные назначения и отображение
настроек — из базовой версии. От форка оставить только Libra 2 / Qt5 landscape
correction в определении поворота. Старый actionRotation убрать при интеграции,
не применять его поверх held(). Это новое решение заменяет прежнее требование
сохранять physical-action семантику для обоих-next/none.

Если механизмы дублируют друг друга (standby при открытии, notes и т.п.),
брать цельную реализацию upstream. Если изменены отдельные пункты общей функции,
делать контекстное слияние с сохранением согласованных fork-отличий. Текущая
runtime-база c4b8c1e не заменена пробным merge; бинарник beta.8 пока не выпущен.
Подробности незавершённой интеграции — в FB2_FORK_PLAN.md.

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

### beta7 integration — archived implementation/CI record

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

Use FB2_FORK_PLAN.md for the completed screenshot findings R1/R2 and
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

Follow FB2_FORK_PLAN.md and FB2_FORK_PLAN.md.
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
- Portrait: одна исходная страница; landscape: две portrait-страницы рядом, wide spread отдельно без поворота/crop/stretch.
- Отметки глав без отдельной настройки, только на существующей progress bar.
- Название и description EN/RU согласованы; использовать текущие строки без новых вариантов.

Сохранять этот контракт; проверять edit→apply
при активном режиме, close/reopen и выключение с восстановлением обновлённых
значений; отдельно edit→cancel не должен менять сохранённую базу.

Comics implementation: see FB2_FORK_PLAN.md and engine.comics regression.
Never claim device verification from a synthetic regression or report unmeasured speedups.


## Build106 device feedback

Build106 follow-up: use FB2_FORK_PLAN.md. Do not confuse KOReader CRe image-coverage refresh with its KOpt/Kaleido path. Tie comics quality refresh to committed frames, not repaint loops; preserve base refresh preferences. Footnote root-font margin is not body line spacing or page remainder. Caption/top-bottom wording was approved and implemented.


### Diagnostics 5bbff30 and approved wording

The comics description EN/RU in FB2_FORK_PLAN.md is approved; no further wording approval needed. Off/Bottom/Top is also approved. Preserve editable base settings. For diagnostics, distinguish parsed-cache vs layout-cache hits and open-end vs reader-shown. Do not assign unnamed openings to user benchmark books, sum nested counters or infer every save=false is an I/O error.


## Progress/comics follow-up implementation

Added approved description and content-measured compact row; Off/Bottom/Top
selector with legacy bottom default; top progress reuses fill/ticks with compact
reservation and header snapshot invalidation. Qt5 comics uses graphics repair
waveform on submitted damage, preserving screen-refresh preferences; user confirmed
correct rendering and no ghosting (no general latency speedup claim). Added opening_id and cache-save wrapper
reasons, not guessed parsed-cache miss reasons. No cache cleanup or memory-budget
increase. Subsequent native/Qt5 CI passed; current status: FB2_FORK_PLAN.md.

Incomplete explicitly: exact body-to-footnote gap (requires renderer/hit-test
coordination) and lazy construction of Settings. Appearance was subsequently
made lazy; general opening optimization remains stopped. New approved EN/RU pairs: comics mode
description, progress Bottom/Внизу and Top/Вверху; no other UI text changes.


## Build109 device follow-up (partial implementation)

Top progress must be ABOVE the header, the vertical mirror of bottom progress;
top ticks point down and bottom ticks up. Shared square checkmarks scale their
path and pen to the actual indicator canvas (40px compact row included).
A pixel-mirror reader regression covers the bar, fill and chapter marks.
Device opening tests exit to Nickel between attempts; combined A→B logs are valid.
User confirms comics refresh now resembles Nickel: preserve its waveform policy.

Still OPEN: landscape comics must pair portrait source pages but keep wide source
spreads whole without rotation/crop/stretch. The former one-column-in-both-
orientations rule is superseded; simply forcing two columns would shrink wide
spreads into half a screen and does not satisfy the requirement. No comics layout
change in this partial patch. Footnote gap and lazy panels also remain unfinished.
This patch is not yet native/Qt5-validated and is not a verified device package.


## Comics safety and landscape follow-up

Device report: NO ghosting artifacts. Preserve the successful Qt5 graphics
refresh policy; do not add unconditional full-screen flashing.
The FB2 screenshot's overprinted text comes from the comics stylesheet's global
line-height:0. Removed that override for all formats, retaining readable prose.
Comics cache policy v2 invalidates parsed checkpoints containing the old layout.

Landscape comic screens now group adjacent known portrait-image-only engine
pages. Wide/unknown/multiple-image/prose pages stay separate. Portrait images
fit half the landscape viewport, wide art fits the whole viewport, no rotation.
Numbering, source bookmarks and hit coordinates use the same screen mapping;
rebuild it after pagination, never on page turns. Base typography/layout choices
remain saved and are restored when comics is disabled. Added mixed-art rotation
and FB2/HTML prose geometry regressions. Subsequent native/Qt5 CI passed.

Also corrected the missing area label/comment for the previous progress-mirror
regression, which prevented native CI configuration (Qt5 packaging itself passed).
Footnote gap and lazy Settings remain deferred; lazy Appearance was completed. No opening speedup claim.


## Verified comics follow-up and deferred Appearance slice

c2786cd: native EPUB/FB2 regressions and Kobo Qt5 package both PASS in run
43106796126. Artifact 13895871347 (Kobalt-qt5-device, 36,433,212 bytes).
Device confirmation of the new landscape geometry is still separate from CI.
No ghosting reported; keep the successful refresh policy unchanged.

Next slice defers Appearance controls until first Text/Layout opening; cheap
sheet shells and Settings remain eager. Chrome loading and reader refresh must
not instantiate controls. First use builds once, applies current palette and
font context, then synchronizes current session/preferences. Diagnostics:
deferred-panel-build(panel=appearance, elapsed_ms); debug state
appearance_controls_built. Startup build_appearance_ms now measures only the
eager slice, not total cost of eventually opening Appearance. First opening
pays deferred construction; no speedup claimed before hardware measurement.
Regression covers Settings before Appearance, absent startup controls, first
construction and reuse in light/dark. e21c3de native and Qt5 verification PASS
in run 43202483204 (Qt6 intentionally skipped). Settings lazy construction
and the footnote gap remain unfinished;
Settings currently owns KOSync initialization and cannot simply be skipped.


## Opening interrupted by ordinary standby: targeted defect fix

Device e21c3de diagnostics show ~29.895s suspended during preparation before
open-start; monotonic reader-shown total 13.112s excludes it. Native lifecycle
now checks openingWorkPending before permitting CPU standby. Coordinator binds
it to existing busy state, except a fallback/error prompt awaiting user input.
Thus preparation, engine loading and presentation are covered without a global
hold flag or kernel wake lock. Existing success/cancel/error/disposal paths
release the gate through busy; fallback prompts do not hold the CPU awake.
Explicit sleep, USB and power-off transitions retain their existing paths.
Added native lifecycle regression for idle-engine opening, terminal outcomes,
fallback and independent save veto. Runtime 61dbfaa verified by run
43213319349: native regressions including platform.shutdown and Qt5 package /
offline / package-content checks PASS; Qt6 skipped. General opening optimization
remains stopped; this is a correctness fix, no startup speedup promise. Device
confirmation of uninterrupted opening remains outstanding.

## beta.8 preparation (not integrated)

Canonical current state and archived-report provenance: FB2_FORK_PLAN.md, section
«Подготовка beta.8». Completed doc uploads have been removed from the current tree,
not from Git history. Do not recreate raw vendor binaries or diagnostic logs in doc/.
Keep permanent instructions and tests/manual fixtures. General opening optimization
is stopped; Settings remains eager; preserve the device-confirmed refresh policy.
Upstream beta.8 already contains openingWorkPending; reconcile it once, not twice.
Back up the whole Kobo profile before database migration; do not claim rollback
compatibility. Preserve fork default-on footnotes and legacy appearance-key mapping.
Documentation-only pushes are excluded from the fork device workflow; mixed source
pushes and workflow_dispatch still build. Actions cleanup needs actions:write;
never treat a 403 as a successful deletion. See platforms/ci/cleanup-runs.sh.

## New device evidence and beta.8 replacement decision

Raw user uploads now belong to `test & diagnostic/` (64930c0), not doc/. Preserve
original bytes and checksums; curated tests/manual stays where its workflow paths
expect it. Data-only uploads in this directory must not trigger package builds.
A source bitmap's aspect ratio is not sufficient for comic pairing: OPF
rendition:page-spread-center is standalone even when the bitmap is portrait-shaped.
Honor it through parsing, cache reuse, comics toggling and orientation changes;
never rotate artwork automatically or change the device-confirmed waveform.
The new 61dbfaa archive ends after completed page65 submission and image loading,
then shows a new boot. Cause of the reported freeze is unproven; do not claim an
opening standby repair also fixes reading freezes.
User now chooses upstream beta8 standby AND footnotes instead of fork mechanisms.
This supersedes earlier default-on/custom bottom-note rendering requirements for
the migration. Adopt full upstream note pipeline, not only its CSS; one canonical
inpage_footnotes key (default false), explicit legacy-value migration and full
links/selection/hit-test regressions. General beta8 merge is not performed yet.

## Landscape comics follow-up (supersedes the no-rotation center rule)

For a170389 diagnostic follow-up see FB2_FORK_PLAN.md. A displayed portrait pair
must load its actually requested visible image resources as one bounded batch,
not capture/repaint the whole spread after each half or consume the neighbour
queue first. Preserve normal one-image behavior, cancellation, optional failures,
encoded-resource memory budget, source integrity and the verified waveform.
The user now requests quarter-turn rotation for portrait-shaped page-spread-center
art in landscape. Rotate clockwise, aspect-fit, on isolated image-only/non-linked
center pages; already-wide art and portrait view stay unchanged. Preserve image
viewer hit identity, letterbox exclusion, anchor preview and orientation roundtrip.
Do not claim source downsampling from a smaller output buffer; PNG, SVG and JPEG
have different decoder paths. This is a page-rendering correction, not permission
to resume the stopped general opening-speed optimization.


### Landscape comics: follow-up to device diagnostics 1a56b68

Portrait bitmap center pages rotate clockwise only in landscape; already-wide
bitmaps remain unrotated. Each portrait half is vertically centered independently,
with matching image-viewer bounds and no crop/stretch. Standalone image-only pages
are aspect-fit and centered. Publication of a visible pair now resolves its pending
resources before capture, including cold and partially cached frames; neighbour
preparation stays deferred. Request identity survives cached draws and neighbour
map pressure. `comic-frame-images` records pending counts around publication.
Regression coverage includes unequal-height halves, hit/letterbox bounds, cold and
mixed-ready frames, neighbour pressure and wide center artwork. Waveform unchanged;
opening-speed work remains stopped. Native and Kobo Qt5 CI passed for runtime `3e810b7` (run
[43502596739](https://github.com/Mutaracha/kobalt_ru/actions/runs/43502596739));
User confirmed correct comics rendering, including the visible pair and centering.


### Libra 2 rotation-only correction (supersedes 76852b5/e0b7c1f)

Restore the 3e810b7 portrait polarity; invert ONLY landscape and inverted-landscape
native directions on Libra 2 / Qt5 (`Device::isIo`). Explicit physical-button
reading assignments now follow the same rotation transform after selection:
portrait +1, landscape -1, inverted portrait -1, inverted landscape +1.
No stored preferences are migrated or overwritten; none stays disabled. Native
fallback still honors system inversion. Other models and Qt6 retain existing
mapping/assignment behavior, as do arrow keys and touch navigation. Menu/list/
preview use native directions, not custom reading actions. UI action labels remain
canonical assignments. Native and Kobo Qt5 CI passed for `c4b8c1e`
([run43645562443](https://github.com/Mutaracha/kobalt_ru/actions/runs/43645562443));
User confirmed physical Libra 2 behavior in all four orientations.
User example (button1 assigned next, button2 previous): portrait +/-, opposite
portrait -/+, buttons-top landscape -/+, buttons-bottom landscape +/-.


## Active beta.8 migration checkpoint

The user confirmed the preceding comics fixes and Libra 2 rotation-only mapping;
do not retain pending-device labels for those changes. The concise active plan is
FB2_FORK_PLAN.md; older build-stage notes are historical, not a work queue.
Migration to upstream679175046 is authorized and in preparation (31 conflicting
paths identified); no new runtime is published yet. Approve the EN/RU proposal
in doc/beta8-strings-review.md before UI string changes. Integration decisions and
validation gates are in doc/beta8-migration.md.

Extend upstream Hidden features with a switch for the reading-menu dark-theme
toggle, with an explicit description. Hiding must preserve the active/saved theme;
it is not a global dark-mode prohibition. New names/descriptions require approval.
Replace fork notes with the full upstream beta.8 mechanism (canonical setting,
links/selection/hits/cache/tests) while preserving native FB2 support. EPUB screenshots
from stock beta.8 show the body-to-note gap is still excessive even with a smaller
font; keep that defect open independently of notes migration. Supplied images and
checksums: test & diagnostic/beta8-baseline/. General opening optimization remains
stopped. All deferred items are grouped in FB2_FORK_PLAN.md, not separate queues.


### Beta.8 translation review update

The user approved features.dark_mode and features.dark_mode_description exactly
as proposed. This does not approve the remaining upstream translation proposal.
Use doc/beta8-strings-comparison.md for old RU vs proposed RU, same-key changes,
the renamed footnotes label, new keys and Russian plural/context review notes.
Do not rewrite existing RU for English-only capitalization or stylistic changes
without approval. No runtime strings or code changed in this documentation step.


### Beta.8 contextual review decisions

The user accepted the main translation batch with opening.protected without a
period, settings.inpage_footnotes = «Сноски внизу страницы», and «ридер» instead
of «читалка». Twenty context-reviewed keys (sleep.letter_*, design_pill,
navigation.annotate/annotations, stats.page_turn/page_turns, kosync.from/to)
remain a revised proposal in doc/beta8-context-review.md. Upstream beta.8
button positions use held() and differ from the confirmed physical-action
rotation for equal actions/none/partial assignments. Never double-transform or
silently migrate the meaning of saved button preferences. Keep Libra 2 behavior
and use a single selected transform path. This audit is not runtime validation.

Beta.8 integration validation: notes and opening standby follow upstream; retain
only the Libra2 landscape correction on top of upstream positional buttons.
Dark-theme hidden feature must hide the toolbar control without resetting theme.
Do not treat the runtime integration candidate as device-validated before CI and
physical Kobo checks; opening-speed optimization remains stopped.

### Отзыв build157 / открытые дефекты

- Сноски: пользователь подтвердил проблемы; задача **не завершена**, нужна
  доработка. Прохождение CI не означает визуальную приёмку.
- Comics preview: при выключении из активного режима пустые места вместо
  chrome; после повторного открытия правильно. Причина: preview использовал
  headerPixels/footerPixels применённой страницы. Генерировать полосы заново
  из draft-состояния, включая верхний progress без header; проверить остальные
  настройки chrome (header/footer enabled, положение/видимость progress, размер).
- Строка comics: большой квадратный checkbox слева, общий размер settings,
  границы/отступы как в Light через общий stylePanelCheckbox; полный hit target.

Build159 `3633da45`: native + Qt5 SUCCESS, preview chrome regression включён
в CI. Физическая приёмка не выполнена; сноски — подтверждённая незавершённая задача.

### Отзыв screenshots008–020 после build159

Карточки сна017–020 зафиксированы как контрольный результат. EPUB notes вернуть
к upstream beta8: убрать общий adjacent-footnotes patch и восстановить noteTop
и тестовые координаты. Эксперимент затрагивал оба формата, его отмена тоже общая.
FB2 использует общий CREngine pagination/notes, а не отдельный самописный layout;
fork добавляет native FB2 input, preview и selector для notes/comments.
Сноски не считать визуально принятыми. Предложение page indicator в меню чтения
«22 из 997» отдельно от времени — ждёт согласования, пока не реализовывать.
Скриншоты и подробности: test & diagnostic/build159-review/README.md.

### Согласованные page counter и raised note labels

Пользователь подтвердил контрольные sleep screenshots и отдельный счётчик
«22 из 997» в верхней строке меню перед батареей, без изменения фразы времени.
Счётчик доступен и при comics/скрытых колонтитулах; в Zen сохраняем скрытие status.
Номер перед нижней сноской надстрочный: targeted note-label CSS только внутри
inpage notes, FB2 title и EPUB a.label/_idFootnoteAnchor. Позиционирование блока
сносок остаётся upstream после отмены adjacent-footnotes. Нужна проверка на устройстве.

Build162 `ba309f79`: native + Qt5 SUCCESS; меню page counter и raised note
labels готовы к device-проверке, визуальная приёмка не выполнена.

### Build162 device feedback / comics shortcut / note report

Пользователь подтвердил по screenshots003–004 правильный полный preview comics.
Добавлена отдельная quick toolbar кнопка comics (панели страницы), тот же
per-book comics_mode; при открытой раскладке менять draft через canonical checkbox.
Новых строк нет, используется согласованное название режима.
Сноски остаются открытыми: сравнение CREngine lvpagesplitter.cpp из beta7 и
текущей зависимости даёт идентичный исходник. addLinesFootnotesToPage() явно
откладывает note при нехватке первой строки, не откатывая reference-bearing body
line. Стилевые различия могут менять threshold, конкретный CSS виновник не доказан.
English report: doc/reports/epub-note-reference-pagination.md (без ссылок на
коммиты/код форка). Не выдавать этот анализ за уже исправленную pagination.

Build163 `5b6c8afe`: comics quick toggle native regression + Qt5 SUCCESS.
English pagination report ready; notes not fixed. Preview device feedback positive.


## Former accumulated plan

# kobalt_ru — актуальный план

## Принятое решение — текущий этап интеграции

Все предложенные переводы согласованы пользователем, включая уточнённую группу.
258 строк EN/RU внесены в assets; это подготовка ресурсов, **не готовая beta.8**.
Новые функции ещё не доступны без завершения runtime merge.

Для кнопок использовать **позиционную модель upstream beta.8**: held(),
назначения одинаковых действий, none, частичные назначения и отображение
настроек — из базовой версии. От форка оставить только Libra 2 / Qt5 landscape
correction в определении поворота. Старый actionRotation убрать при интеграции,
не применять его поверх held(). Это новое решение заменяет прежнее требование
сохранять physical-action семантику для обоих-next/none.

Если механизмы дублируют друг друга (standby при открытии, notes и т.п.),
брать цельную реализацию upstream. Если изменены отдельные пункты общей функции,
делать контекстное слияние с сохранением согласованных fork-отличий. Текущая
runtime-база c4b8c1e не заменена пробным merge; бинарник beta.8 пока не выпущен.
Подробности незавершённой интеграции — в FB2_FORK_PLAN.md.

Этот файл — действующая очередь, не накопительный журнал прошлых сборок.
Требования к реализации: [AGENTS.md](AGENTS.md). Пользовательская инструкция:
[doc/fork-kobalt_ru.md](doc/fork-kobalt_ru.md).

## 1. Подтверждённая база

- Последний проверенный runtime: **`c4b8c1e`**, beta.7-based fork.
- [CI43645562443](https://github.com/Mutaracha/kobalt_ru/actions/runs/43645562443):
  native focused regressions и Kobo Qt5 build/package/contents PASS.
- [Qt5 artifact14134257020](https://github.com/Mutaracha/kobalt_ru/actions/runs/43645562443/artifacts/14134257020),
  36 441 133 bytes; native-results14133572838. Qt6 intentionally skipped.
- Пользователь подтвердил корректность comics: отдельные широкие страницы,
  портретный center-bitmap поворачивается clockwise только в landscape,
  широкие center-bitmap не вращаются, половины независимо центрированы по
  вертикали и появляются корректно. Код `3e810b7`; CI43502596739 PASS.
- Пользователь подтвердил Libra 2 во всех четырёх ориентациях (`c4b8c1e`):
  портретные направления сохранены относительно `3e810b7`, оба landscape
  направления исправлены. Явные назначения учитывают поворот, none отключён,
  настройки не переписываются. Предыдущая общая инверсия76852b5/e0b7c1f отменена.
- R1/R2/R3 beta7 выполнены, native/Qt5 PASS. Название «Резервные шрифты»
  согласовано; дополнительное длинное пояснение не добавлять.
- Appearance deferred реализована (`e21c3de`, CI43202483204 PASS).
- Открытие защищено от обычного standby (`e31dcdd`/`61dbfaa`, CI43213319349 PASS);
  успешное открытие подтверждено диагностикой. Это correctness fix, не speedup.
- Рабочая waveform policy сохранена; пользователь подтвердил отсутствие ghosting.

Старые пометки «ожидает проверки» для перечисленных исправлений закрыты.
Это не означает, что выполнены все возможные платформенные/миграционные тесты.

## 2. Активный этап — переход на upstream beta.8

Переход разрешён пользователем. Цель: `v1.0-beta.8`,
**`679175046d8c806ea3b404525180fbc29a26fcd2`**.
База начала: fork **`130091f`**; общий ancestor — beta.7 `e86533c`.

**Статус:** подготовка начата; пробное трёхстороннее объединение выполнено без
изменения рабочего кода. 433 upstream-changed файла, **31 конфликтующий путь**.
Production merge пока не опубликован: основная группа EN/RU согласована,
включая20 контекстных ключей. Подготовительные EN/RU assets применены;
runtime merge ещё не завершён.
Подробная карта конфликтов/семантических пересечений и проверки:
[doc/beta8-migration.md](doc/beta8-migration.md).

### Порядок работ

1. **Согласовать тексты** [EN/RU proposal](doc/beta8-strings-review.md):
   256 новых/изменённых upstream значений; 2 строки dark-toggle hiding уже
   согласованы пользователем. [Сводная таблица старого и нового RU](doc/beta8-strings-comparison.md)
   отделяет 8 прямых замен, новое имя настройки сносок и 247 новых upstream-ключей.
   Согласованные fork-подписи, нижние вкладки и standby warning не заменять
   механически. Основная группа принята; opening.protected — без точки,
   settings.inpage_footnotes — «Сноски внизу страницы», термин «ридер».
   [Контекстный аудит](doc/beta8-context-review.md) уточняет sleep.letter_*,
   pill, annotate/annotations, page_turn(s), from/to; эти20 строк также согласованы и внесены в assets.
2. **Перенести beta.8 целиком**, контекстно разрешить конфликты и проверить
   автоматически объединённые файлы. Не выбирать ours/theirs для общих модулей.
3. **Заменить fork сноски upstream-реализацией**: canonical `inpage_footnotes`,
   default false, DOM markers, links, selection/hit testing, appearance/state,
   cache и upstream tests. Legacy value мигрировать явно; один переключатель,
   без параллельных fork CSS/notes веток. EPUB — как в базовой beta.8;
   нативную поддержку FB2 сохранить и проверить отдельно.
4. **Standby — единая upstream policy**: openingWorkPending,
   `busy && !run.fallbackAlert`, независимый save veto и существующие regressions.
   Не восстанавливать fork power policy/Wi-Fi saver.
5. **Добавить скрытие переключателя тёмной темы** в upstream Hidden features.
   Скрывается кнопка меню чтения; активная тема и сохранённый выбор остаются.
   По умолчанию кнопка видима. Название и описание согласованы; текст — в EN/RU proposal.
6. **Сохранить fork поведение**: FB2, RU locale/plurals, stock About и fork UTC,
   comics/spread/complete-pair publication/hits, progress Top над header,
   Bottom/Off и ticks, durable/effective geometry, Libra 2 rotation mapping,
   deferred Appearance и успешную waveform. Upstream held()/positional IDs
   пересекается с fork actionRotation: не применять оба преобразования.
   Для кнопок оставить только нашу Libra 2 landscape correction, а held(),
   both-next/none/частичные назначения и отображение брать из upstream
   по явному решению пользователя. Upstream Zen/presets не должны
   перезаписывать обычные значения comics/chrome/margins.
7. **Storage**: перенести schema upgrade и исправление duplicate-ID native save,
   сохранив FB2 percentage/time-only positions. Актуализировать doc/storage.md.
8. **Проверить и выпустить тестовый Qt5 package**: offline/EN-RU parity,
   placeholders, assets/checksums, native relevant suites, adapters/package;
   затем отдельная приёмка новой версии на устройстве.

### Резервная копия перед первым запуском beta.8

Полный backup `.adds/kobalt/` и штатной БД Kobo обязателен. Beta.8 обновляет БД;
старые beta не читают новую схему. Откат — только с восстановлением согласованного
backup, не установкой старого бинарника поверх migrated profile. Репагинация
ожидаема. До проверенного пакета и backup миграцию пользовательских данных не запускать.

## 3. Единый список отложенных задач

| Задача | Статус / условие возобновления |
|---|---|
| **Большой зазор перед нижними сносками** | Открытый дефект. Исправлять после замены механизма на beta.8; перенос upstream сам по себе его не закрывает. |
| **Ориентационно-зависимая схема кнопок в настройках** | Предложение, код не написан. Поворачивать устройство/кнопки, размещать подписи у соответствующих кнопок и показывать effective действия после преобразования, не изменяя сохранённые назначения. |
| **Общая оптимизация открытия** | Остановлена пользователем; автоматически не возобновлять. |
| **Ленивая сборка Settings** | Не реализована; часть остановленной оптимизации, не смешивать с переносом beta.8. |
| **Очистка пользовательского кэша** | Отложена; не возвращать автоматически при merge. |
| **Очистка старых Actions runs/artifacts** | Не выполнена: DELETE вернул403. Helper есть; нужны Actions write rights. Не удалять active/successful evidence. |
| **Причина прежнего зависания61dbfaa** | Не установлена. Последний frame/load завершены, затем лог обрывается до reboot. Успешный CI и текущие визуальные fixes не доказывают устранение причины. При повторе нужны system/kernel logs или stack snapshot. |

### Требование к зазору перед сносками

Зазор между последней строкой основного текста и сноской должен соответствовать
обычному зазору между соседними body lines. Не вводить фиксированный pixel offset
или draw-only сдвиг, расходящийся с pagination, selection и hit testing.

Новые скриншоты **базовой beta.8** подтверждают: большой промежуток остаётся и
при меньшем основном шрифте. Их источник — пользователь; точный binary SHA и
байты книги не переданы. Это visual evidence, не автоматический pixel test.
Файлы: [test & diagnostic/beta8-baseline](test%20%26%20diagnostic/beta8-baseline/).

Проверки исправления: FB2 и EPUB, одна/несколько/длинная split note, разные
font sizes и интервалы100/150/200%, cached reopen, preview, popup/tap/selection.
`FOOTNOTE_MARGIN_REM=1` — root-font reservation, не обычный body line spacing;
не считать изменение одной CSS-константы доказанным решением.

## 4. Приёмка новой beta.8 — не старые незакрытые исправления

Эти проверки относятся к **будущему объединённому runtime**, а не отменяют
подтверждения текущего `c4b8c1e`:

- backup/schema/legacy preference migration; reopen и native/local progress save;
- EPUB/FB2 notes on/off/default, ссылки/popup, selection, continuation, cache;
- hidden features + dark toggle: hide/show, сохранение темы, reopen, Zen;
- comics on/off, portrait/landscape/wide/odd page, backward, anchor/viewer/hits,
  original bitmap identity, отсутствие crop/stretch и прежняя waveform;
- progress Top/Bottom/Off: header/footer, reflow, ticks, preview, без второго reserve;
- Settings до Text/Layout; theme/interface font до первого открытия Appearance;
  повторное открытие без дублирования/reset;
- Libra 2: четыре ориентации, custom next/previous/none, native menus/lists/preview;
- standby success/cancel/error/fallback/restart и независимый save veto;
- EN/RU clipping новых текстов, font sizes и representative UI variants.

Полная native/all-variant release matrix не подменяется focused branch CI.
Kobo Qt5 — приоритет. Desktop/Android не blockers, но общий код не ломать.

## 5. История и evidence

Прежний подробный план/журнал сохранён в Git на
[`130091f:FB2_FORK_PLAN.md`](https://github.com/Mutaracha/kobalt_ru/blob/130091f/FB2_FORK_PLAN.md).
Его старые «pending» — история этапов, не действующая очередь.
Сырые diagnostics и uploads принадлежат `test & diagnostic/`, не `doc/`.
Curated `tests/manual/` fixtures и publisher-check arguments не перемещать.

В текущей итерации применены согласованные EN/RU ресурсы и закреплено правило
разрешения пересечений. Пробный runtime merge разобран, но ещё не завершён
и не опубликован. Новый пакет beta.8 не создавался.


### Проверки подготовительных переводов

- Применены258 согласованных EN/RU значений; общий словарь1421 ключей.
- CMake kobalt_check_string_parity, placeholders, генерация EN/RU и полный
  source-reference scan текущего src/ + Kobo adapters: PASS.
- platforms/strings-test.cmake с Ninja: PASS (включая invalid/duplicate/missing
  key, regeneration и locale-parity negative cases). Локальный Make-generator
  прогон упёрся в timestamp-sensitive fixture; код теста не ослаблялся.
- Это не проверка runtime beta.8: merge core/notes/UI ещё не завершён.

Подготовительные переводы `d4953148`: [CI43747021251](https://github.com/Mutaracha/kobalt_ru/actions/runs/43747021251)
PASS native focused + Kobo Qt5/offline/package. Qt5 artifact14175205488,
36 454 953 bytes; native-results14174718835. Qt6 skipped. Этот пакет всё ещё
beta.7-based runtime с новыми ресурсами, **не merged beta.8**, не выдавать его
за проверку upstream notes/schema/buttons. Последний подтверждённый пользователем
runtime остаётся c4b8c1e.

Следующий implementation checkpoint: заменить остатки fork notes в engine/UI/
settings/tests canonical upstream реализацией; совместить FB2/comics с новым
opening pipeline и page_content; завершить positional buttons с единственным
Libra 2 landscape guard; затем hidden dark toggle и полный relevant CI.
Пробное дерево с частично разрешёнными конфликтами не опубликовано, поскольку
оно ещё содержит несовместимые старые/новые символы. Ветка содержит только
согласованные ресурсы и документированный порядок интеграции, не сломанный merge.

### beta.8 runtime integration candidate — 2026-10-26

Integrated pinned upstream 679175046d8c806ea3b404525180fbc29a26fcd2 using
upstream notes/opening standby and positional button semantics; retained the
Libra2-only landscape correction and individual FB2/comics/progress changes.
Approved dark-toggle hidden feature wired without resetting the current theme.
Legacy FB2 notes preference migrates to the canonical key (canonical wins).
Offline checks and string-generation checks pass; native and Kobo Qt5 compilation
and runtime regressions must pass before this candidate is offered for installation.
Debug-language shutdown uses the existing `application quits` blocking reason;
no new allowlist exception or opening optimization was introduced.

### beta.8 integration verification — completed for device-test build

Runtime source: **dcc9ead5d25dc60e8ffa1923ccf1c82bef5cd1fa**.
Upstream 679175046d8c806ea3b404525180fbc29a26fcd2 is a merge ancestor.
CI **43755468526**, build **153**: SUCCESS.
- Native EPUB/FB2/comics/appearance/settings/lifecycle/button regressions: PASS.
- Additional supplied publisher EPUB note 4/5 regression: PASS with upstream notes.
- Kobo Qt5 compile, offline release checks and package validation: PASS.
- Artifact **14178457535**, `Kobalt-qt5-device`, 36,926,875 bytes.
- Artifact transport SHA-256: `345dcc7daa95083495912022cf04c66e70d7d48d2911be1639d0d735a8efe99c`.
  This is GitHub's artifact digest, not the inner installer ZIP checksum.
- Qt6 device build was intentionally skipped; desktop native tests use the pinned Linux SDK.

API and merge repairs include typed appearance patches, preset-cache shell
initialization, null initialization and safe teardown of deferred controls, and
updating legacy test assumptions to upstream default-off notes/modal separators.
Hidden-theme preference and Libra2-only four-orientation polarity have regression
coverage. No tests were disabled. Earlier preparation-only entries above are
historical and superseded by this completed runtime/CI checkpoint.

The build is ready for physical Libra2 testing, **not yet physically accepted**.
Keep the successful waveform policy; opening-speed optimization remains stopped.
The large footnote/body gap remains a separate physical review item, not declared
fixed by the merge. Backup/installation and checks: `doc/beta8-device-check.md`.

## Device fixes после build153 — WIP

- Согласовано: «Капсула» вместо «Плашка»; текст письма без изменений.
- Подготовлены изменения fitting sleep-текстов, draft comics viewport/chrome,
  компактного верхнего отступа Layout и промежутка перед примечаниями.
- Перед выпуском: проверить координаты ссылок/выделения, cache reopen,
  preview on/off без повторного открытия; native + Kobo Qt5 и скриншоты.
- Это не новая проверенная сборка. Рабочая база: build153.

### Проверочная сборка157

Runtime: `5edbf1975e869bf1495823d53ffbaa5d7b2b6958`.
CI: https://github.com/Mutaracha/kobalt_ru/actions/runs/43765998650 — native
regressions и Kobo Qt5 package SUCCESS. Устройство и визуальные результаты
ещё не подтверждены. Артефакт Kobalt-qt5-device: 14182328674.

### Отзыв build157 / открытые дефекты

- Сноски: пользователь подтвердил проблемы; задача **не завершена**, нужна
  доработка. Прохождение CI не означает визуальную приёмку.
- Comics preview: при выключении из активного режима пустые места вместо
  chrome; после повторного открытия правильно. Причина: preview использовал
  headerPixels/footerPixels применённой страницы. Генерировать полосы заново
  из draft-состояния, включая верхний progress без header; проверить остальные
  настройки chrome (header/footer enabled, положение/видимость progress, размер).
- Строка comics: большой квадратный checkbox слева, общий размер settings,
  границы/отступы как в Light через общий stylePanelCheckbox; полный hit target.

### Проверочная сборка159 — preview chrome

Runtime `3633da452b02b9cdfdcd0811e9ed427d2265d765`. Native (включая
`desktop.appearanceChromeDraft`) и Kobo Qt5 SUCCESS:
https://github.com/Mutaracha/kobalt_ru/actions/runs/43870347038
Пакет: https://github.com/Mutaracha/kobalt_ru/actions/runs/43870347038/artifacts/14227993687
Исправлен stale chrome cache preview, включая top progress без header;
Light и comics используют общий stylePanelCheckbox. Физическая приёмка не
выполнена. **Сноски остаются проблемными, в этой сборке не дорабатывались.**

### Отзыв screenshots008–020 после build159

Карточки сна017–020 зафиксированы как контрольный результат. EPUB notes вернуть
к upstream beta8: убрать общий adjacent-footnotes patch и восстановить noteTop
и тестовые координаты. Эксперимент затрагивал оба формата, его отмена тоже общая.
FB2 использует общий CREngine pagination/notes, а не отдельный самописный layout;
fork добавляет native FB2 input, preview и selector для notes/comments.
Сноски не считать визуально принятыми. Предложение page indicator в меню чтения
«22 из 997» отдельно от времени — ждёт согласования, пока не реализовывать.
Скриншоты и подробности: test & diagnostic/build159-review/README.md.

### Согласованные page counter и raised note labels

Пользователь подтвердил контрольные sleep screenshots и отдельный счётчик
«22 из 997» в верхней строке меню перед батареей, без изменения фразы времени.
Счётчик доступен и при comics/скрытых колонтитулах; в Zen сохраняем скрытие status.
Номер перед нижней сноской надстрочный: targeted note-label CSS только внутри
inpage notes, FB2 title и EPUB a.label/_idFootnoteAnchor. Позиционирование блока
сносок остаётся upstream после отмены adjacent-footnotes. Нужна проверка на устройстве.

### Build162 для устройства

Runtime `ba309f791508a926f036dfc66040d2cdfd8f2eb6`: native regressions и Qt5
SUCCESS, включая проверку заполнения rrMenuPage при открытии меню.
CI: https://github.com/Mutaracha/kobalt_ru/actions/runs/43905062782
Пакет: https://github.com/Mutaracha/kobalt_ru/actions/runs/43905062782/artifacts/14244978065
Проверить: счётчик перед батареей при comics/без колонтитулов, отсутствие
наложения на время чтения, надстрочные номера внизу EPUB/FB2. Позиционирование
сносок upstream; физическая приёмка открыта.

### Build162 device feedback / comics shortcut / note report

Пользователь подтвердил по screenshots003–004 правильный полный preview comics.
Добавлена отдельная quick toolbar кнопка comics (панели страницы), тот же
per-book comics_mode; при открытой раскладке менять draft через canonical checkbox.
Новых строк нет, используется согласованное название режима.
Сноски остаются открытыми: сравнение CREngine lvpagesplitter.cpp из beta7 и
текущей зависимости даёт идентичный исходник. addLinesFootnotesToPage() явно
откладывает note при нехватке первой строки, не откатывая reference-bearing body
line. Стилевые различия могут менять threshold, конкретный CSS виновник не доказан.
English report: doc/reports/epub-note-reference-pagination.md (без ссылок на
коммиты/код форка). Не выдавать этот анализ за уже исправленную pagination.

### Build163: comics quick toggle

Runtime `5b6c8afe622b26c85244ef48230f2d4705c7e790`: native including
`desktop.comicsQuickToggle` and Qt5 package SUCCESS. Physical acceptance pending.
CI https://github.com/Mutaracha/kobalt_ru/actions/runs/43948942647
Package https://github.com/Mutaracha/kobalt_ru/actions/runs/43948942647/artifacts/14266591934
Footnote pagination was analysed, not modified or declared fixed.
