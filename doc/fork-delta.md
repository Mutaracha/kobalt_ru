# Текущая граница выравнивания beta.9

Baseline: upstream `be472588`, fork runtime `8f1fdf4d` (focused CI 44932175305 PASS; preset/version fixes device pending).
Полный актуальный inventory ещё planned; историческая карта beta.8 ниже не
является доказательством завершённости beta.9 consumer migration.

- Vendor `src/engine/blaze/crengine` не менялся при cleanup.
- Удалена старая external engine patch series; build recipe/cache key не менялись.
- На checkpoint 9633d3fd `reader_text_presets.cpp` был byte-identical upstream beta.9; общий
  style_sample/cache API и tests перенесены вместе. Старая gallery prerender
  очередь и её opening/idle wiring удалены.
- Сохранён fork transient `whole_book_failed` в sample settings sanitation;
  текущий styling cache consumer передаёт font catalog в общий ключ.
- EPUB Hybrid algorithms не менялись. FB2 whole-book contract сохранён;
  подготовка partial режима — [отдельный анализ](reports/fb2-hybrid-preparation.md).
- Runtime `613df9ba`: ensureAppearance перенесён из fork_reader_actions в общий
  reader_build; themeWidget/applyTheme взяты из pinned upstream. Удалён отдельный
  palette/icon loop поздних контролов. CI 44808379234 PASS, device pending.
- Runtime `77564e9e`: lazy Settings + upstream background preparePanels,
  KOSync model/page split, null-safe consumers и lifecycle harness. CI 44815357384
  PASS native + Qt5; theme и Settings ожидают общей device-проверки.
- Аннотации/keyboard/остальные KOSync policy/sleep остаются в очереди выравнивания;
  перенос model/page не означает завершения всех KOSync upstream изменений.

**Полное выравнивание не завершено.** Нет свежего exhaustive inventory,
который подтверждает отсутствие необоснованных различий. После переноса presets
в общий reader_text_presets добавлены nullable guards и fork-local presentation
вызовы локализации имён; byte-identical для текущего файла больше не заявляется.
Пользователь принял чек-лист panels/buttons; последующий freeze формы вертикальных
интервалов открыт отдельно. Остаток очереди — FB2_FORK_PLAN.md; исторический
beta.8 inventory ниже не заменяет итоговый beta.9 аудит.

Подробности/проверки: [cleanup report](reports/upstream-cleanup-stage1.md),
текущая очередь: [план](../FB2_FORK_PLAN.md).

---

# Карта расхождений kobalt_ru с upstream beta.8

Это поддерживаемая карта владения кодом для следующих обновлений, не новый
список функциональных задач. Принятые решения — [реестр](fork-decisions.md),
текущий статус проверки рефакторинга — [план](../FB2_FORK_PLAN.md).

## Точная база сравнения

- Upstream: `nicoverbruggen/kobalt`, beta.8,
  `679175046d8c806ea3b404525180fbc29a26fcd2`.
- Форк до этого рефакторинга: `4101ef45` (полный SHA доступен в Git).
- Сравнены содержимое и наличие **всех файлов** экспортов двух деревьев,
  а не только конфликтные hunks или ранее составленные описания.
- Исходная дельта: **138 файлов: 93 изменённых, 45 добавленных, 0 удалённых**.
  `src`: 69; `platforms`: 19; `assets`: 9; `tests`: 22; `doc`: 10;
  `test & diagnostic`: 6; workflow: 1; AGENTS и план: 2.
  Это сравнение baseline до рефакторинга; новые файлы ниже меняют эти числа.
- Зависимости CREngine/Harfbuzz и остальной manifest, patches, SDK pins совпадают
  с beta.8. Наличие CREngine или upstream notes само по себе не отличие форка.

## Пользовательские и технические отличия

| Область | Что добавлено/изменено относительно beta.8 | Владелец и необходимые точки интеграции |
|---|---|---|
| Нативный FB2 | Распознавание, публикация, ресурсы/обложка/метаданные, preview, FictionBook CSS; открытие из библиотеки Kobo. `.fb2.zip` исключён. | `publication/fb2.h`, `publication/fb2_resources.*`, `engine/crengine/reader_fb2.*`; подключения в publication/resources, reader_source, engine_document и Kobo routing. |
| Сноски FB2 | `notes/comments`, числовой run-in title, надстрочные labels, format salt v8, миграция legacy Boolean. EPUB остаётся на общем beta.8 pipeline, labels подняты. | `reader_fb2.*`, `reader_fork_styles.*`; выбор stylesheet и lifecycle cache в engine_document/reader_source, migration в appearance_settings. Индексы и текущие отступы приняты, доработка не открывается. |
| Семантика FB2 | Заголовки/стихи/эпиграфы/bylines исключаются из нормализации alignment/beautifier; дополнительные inline tags и line-height selectors. | Предикаты в `reader_fb2.*`; по одному вызову в upstream passes. CSS selectors остаются на общей границе применения stylesheet. |
| RU | Полный словарь, plural one/few/many, русский перенос с EN, English fallback. Locale при каждом принятом открытии книги, ExtraLocales/C/POSIX, debug markers. | `support/string_locale.h`, `string_table.h`, `locale.*`, `debug_language.*`, короткие hooks в `strings.h`, `assets/ru.strings`, `platforms/strings*.cmake`; platform entry points. |
| About/debug | Дополнительная строка fork + UTC build time; скрытый выбор языка. Stock version/commit сохранены. | `app/about/fork_about.*`, `fork_debug.*`; вызов из about_page, build_info и build definitions. |
| Comics | Per-book/default off, эффективное подавление полей/chrome без потери значений, preview, landscape пары, отдельные wide/center страницы, центровка/поворот portrait center, hit bounds и resource readiness. | `reader_comics.*`, `column_view_comics.cpp`, `engine_document_comics.cpp`; состояние/общая отрисовка и позиционирование подключены в ColumnView/EngineDocument, publication/resources/session/UI. |
| Меню чтения | Быстрые dark/comics кнопки; comics state ink + filled checked bubble, запрет действия при busy без серого мигания; скрываются только кнопки. Полоса book-time/page-count полностью удалена в cba1e229 по новому решению пользователя; замена deferred. | `fork_reader_actions.cpp`, `views/fork_navigation.*`, короткие toolbar wiring/refresh/navigation hooks, features/settings. Ни режим, ни Layout control не отключаются скрытием. |
| Прогресс | Выключено/Внизу/Вверху, top выше header, зеркальные chapter ticks, legacy enabled→Bottom. | `reading_page_progress.cpp`, reading_page geometry, header_footer model/panel, chapter fractions и session/navigation refresh. |
| Shared UI | Settings-row presentation, compact checkbox Light/Layout, корректный общий baseline RU tabs, return-title typography, fork glyphs. | `fork_settings.*`, options/buttons, `return_title_text.h`, navigation, `fork_icon_geometry.*`. Не дублировать экраны Settings. |
| Сон | Подгонка длинных надписей, две строки remaining в ribbon, fit оценок widgets; общая standby-логика beta.8 не заменяется. | `sleep_ribbon_text.cpp`, узкие изменения sleep_screen/sleep_designs. |
| Lazy Appearance | Upstream beta9 lazy Settings/Appearance, preparePanels/settled/inputQuiet; поздние children получают актуальную palette/refresh. Собственной второй оптимизации нет. | `fork_reader_actions.cpp`, reader window state/build/refresh/settings. Не новая оптимизация в этом рефакторинге. |
| Направление свайпов | Отдельный глобальный `invert_page_swipes`, default false; направление учитывает RTL, не меняет зоны касаний/свет/кнопки. | `reading/page_navigation.h`, Settings и ReadingPage input wiring. |
| Kobo Libra 2 | Только landscape polarity correction поверх positional/held модели beta.8; другие устройства — upstream. | `platforms/kobo/common/page_button_mapping.h`, Qt5 native_ui; ReaderController lifecycle hooks. |
| Kobo/FB2 прогресс | FB2 не использует EPUB-specific native position; сохраняются собственные source positions, в Nickel время/проценты. | Kobo native_publication, ReaderController, publication/opening_job/reader_session. |
| E-ink comics | Graphics/repair policy по submitted damage; диагностика refresh. Не принудительный whole-screen full flash. | reading_page property и `nickel-qt5/refresh.cpp`. Согласованную waveform не менять. |
| Диагностика cache/resources | Причина отказа сохранения parsed cache, трассировка comic visible resources и приоритет pending images; без возобновления остановленной оптимизации открытия. | EngineDocument/ReaderSession и ResourceCache. Данные профиля не сбрасываются. |
| Build/CI | Qt5-first workflow, RU generation/parity, fork name/time, FB2 CSS staging, native focused regressions, one-off Qt6 и helper очистки Actions. | `platforms`, assets manifest/CMake, `.github/workflows/kobo-qt5-device.yml`; не менять pins без причины. |
| Проверки/данные/docs | FB2/comics/RU/regression suites, opt-in книги и diagnostics; регламент обновления и storage contract. | `tests`, `test & diagnostic`, `doc`, AGENTS/план. Это не runtime-отличия. |

## Граница рефакторинга

Специфические алгоритмы вынесены **по слоям**, а не в общий каталог с зависимостями
на весь UI и движок:

- `engine/crengine/reader_comics.{h,cpp}` — comic CSS, OPF center metadata,
  bitmap classification. `reader_page_layout.*` снова отвечает за обычные leading images.
- `engine/crengine/column_view_comics.cpp` — spread rebuild/count, standalone/rotated
  image identity, fit rect, offsets. В `column_view.h` остаются API, состояние и
  небольшие подключения к общему mapping страниц/координат.
- `engine/crengine/engine_document_comics.cpp` — отрисовка single/spread artwork,
  tracking/union/prioritization видимых image requests и синхронизация перед
  публикацией кадра. Общие cache/normalization/ordinary-column loop не копируются.
- `engine/crengine/page_draw_buffer.*` — единственная shared реализация прежнего
  upstream буфера, EquationImage и IllustrationProbe. Вынесена из анонимного
  namespace EngineDocument, чтобы ordinary pages, covers и fork comics использовали
  один drawing primitive. Это не fork-only policy: при обновлении отдельно сравнивать
  соответствующий upstream блок из engine_document.cpp, а не замораживать его.
- `engine/crengine/reader_fb2.*` — также владеет format-specific предикатами
  upstream alignment/beautifier passes; общие passes не заморожены копиями.
- `app/reader/window/fork_reader_actions.cpp` — действие comics с draft/busy guard
  и существующий lazy Appearance lifecycle. `reader_build.cpp` только подключает действие.
- `app/reader/views/reading_page_progress.cpp` — painter top/bottom/chapter progress;
  резервирование места и header/footer geometry перенесены туда же в этапе 4.
- `ui/controls/fork_icon_geometry.*` — bubble и Fluent dark geometry. Generated
  `reader_icon_vectors.h` возвращён к beta.8 без fork-геометрии.

Второй этап дополнительно выносит:
- `support/string_locale.h` — RU table lookup, locale atomics/normalization и plural
  suffix policy; `support/string_table.h` — общий тип Entry и бинарный поиск.
  `strings.h` оставляет English fallback/key display и публичный plural wrapper.
  Header-only контракт сохранён, поэтому standalone targets не получают новой
  зависимости на linked library.
- `engine/crengine/reader_fork_styles.*` — FB2 line-height/note selectors и общие
  принятые superscript labels. Common notes CSS остаётся upstream, расширения
  вставляются в прежних byte boundaries, без изменения порядка cascade.
- `publication/fb2_resources.*` — bounded worker transfer одной FB2 virtual resource.
  `local_resources.cpp` оставляет один форматный dispatch; lifetime/worker ownership
  не меняется. Desktop явно включает новый `.cpp`; Kobo/Android берут его source glob.

Существующие FB2, locale/debug, About и Settings модули остаются на своих местах.
Новый generic plugin/hook framework, подмена upstream экранов, vendor snapshot
в репозитории и второй механизм notes/standby не вводятся. Engine/UI `.cpp` подхватывают существующие source globs Kobo/desktop/Android;
publication helper добавлен также в явный desktop core source list. Отдельные
platform-specific копии не нужны.

**Что намеренно остаётся в upstream-файлах:** интеграция формата в Publication и
resource lifecycle, сохранённые optional fields/миграции, эффективные настройки
EngineDocument, dispatch обычных колонок/comics, mapping page/DOM/hit,
UI wiring/layout/refresh и Kobo hooks. Полностью неизменный upstream с новыми
форматами/настройками без изменения его API не обещается. Дальнейшее сокращение
этих участков требует отдельной проверки; эта карта не называет их уже вынесенными.

## Проверки и правила следующего обновления

1. Сверять новый upstream с указанной базой полностью, затем просматривать каждую
   интеграционную точку по таблице. Не переносить целые старые upstream-файлы.
2. Раздельно проверять изменение поведения upstream и механический перенос fork кода.
3. Не менять keys/defaults/cache salts, CSS порядок, направление кнопок, glyph
   coordinates, hit targets и waveform ради структуры файлов.
4. Запустить focused FB2/links/comics/resources/appearance/header-footer/chapter-progress,
   component.ui, desktop comicsQuickToggle/lazyAppearance и lifecycle appearance;
   затем Kobo Qt5 package. Объём текущего CI определён в device workflow.
5. Визуальная приёмка иконки/сносок относится к прежнему runtime; она не заменяет
   compile/regression проверки перенесённых translation units. Функциональные задачи
   beta.8 не открываются заново из-за рефакторинга.

## Проверенный результат первого этапа

Runtime после выноса: `d9536352eb0264127f1a53815b7533d413bb4884`.
[CI44089641669](https://github.com/Mutaracha/kobalt_ru/actions/runs/44089641669) —
расширенный focused native (в том числе alignment, beautifier, page-layout,
columns) и Kobo Qt5 build/package/contents **PASS**.
Source-checks и strings self-tests также прошли. Проверка устройства и all-variant
не выполнялись. Тела 13 перенесённых функций сверены отдельно; generated vector
catalogue byte-identical upstream. Отдельные semantic predicates проверяются
существующими FB2/alignment/beautifier regressions.

Это первый проверенный этап, не заявление о полном выносе всей дельты.
Оставшиеся смешанные участки перечислены выше и в плане.
Пользователь подтвердил запуск первого этапа на устройстве; это не полный
повторный визуальный или all-variant прогон.

Второй этап проверен: runtime `69c70aa357209d0ef9ffce5afc68b42d74492a5f`,
[CI №171 / 44096144408](https://github.com/Mutaracha/kobalt_ru/actions/runs/44096144408)
**PASS**: expanded focused native, offline checks, Kobo Qt5 package/contents.
Локально source-checks и strings self-tests (Ninja) прошли. Полный note CSS byte-identical предыдущему этапу; digests фрагментов
проверяются новыми native assertions. `model.local-resources` добавлен в focused
workflow для FB2 cancellation/size/error/sink boundaries. Пользователь затем подтвердил запуск второго этапа и сообщил о двух FB2-недочётах
без описания. Исправлять их после рефакторинга; это не новая полная приёмка FB2.

## Третий этап — CI verified

Вынесены fork paint и visible-image resource policies; общий PageDrawBuffer перенесён
одним экземпляром реализации. До переноса его код и helpers byte-identical pinned
upstream beta.8; тела методов и помощников сохранены. Fork painter использует этот
же буфер, без отдельной scaling/inversion/normalization реализации.
`image()` сохраняет обычный upstream pipeline и маленький tri-state dispatch
(NotHandled/Painted/Failed), включая освобождение tracking на отказе allocation.
`loadNextImage()` делегирует приоритет visible comics batch и сохраняет fallback
на ordinary queue. Лимиты 2 visible resources / 8 request identities не меняются.

Новый contract-test буфера выполняется в engine.comics; focused native дополнен
engine.image-proportions и engine.mathml для shared image/night/equation поведения.
Локальные source-checks и strings self-tests прошли.
Runtime `9e03e25d31bfc000a88048c01f102b16e57d6a8f`,
[CI №172 / 44100128546](https://github.com/Mutaracha/kobalt_ru/actions/runs/44100128546)
**PASS**: expanded focused native, offline checks, Kobo Qt5 package/contents.
Физическая проверка этого этапа, отдельный RTL-сценарий и all-variant не выполнялись. Два новых FB2-недочёта — отдельная очередь
после рефакторинга, не повод менять поведение в этом механическом переносе.

## Четвёртый этап — завершающая граница модулей

`reader_comics.*` содержит effective layout overlay; `EngineDocument` применяет
результат, сохраняя исходные margins/columns. `publication_fb2.cpp` содержит raw
FB2 inspection/hash/metadata с tri-state dispatch из Publication. `ImageRequestTrace`
инкапсулирует worker screen trace, не меняя cancellation/rebind/queue lifecycle.
`fork_navigation.*` содержит translated ink bounds и toolbar style/restore;
`fork_reader_actions.cpp` — presentation/refresh/page counter Reader. Общая ReadingPage
оставляет вызовы geometry, реализация находится рядом с progress painter.

Это завершение структурного прохода, не обещание неизменного upstream API.
Storage/defaults/strings/hash/CSS/waveform не меняются. Новые native assertions:
effective overlay и неизменность base, прежний FB2 length-prefix SHA256, trace end/reset.
Локально source-checks/strings self-tests/diff check PASS; CI PASS, устройство pending.
Пользователь проверяет этапы 3+4 вместе. Два FB2-замечания не исправлялись.


Runtime финального этапа: `0e3e9f98bddc9e1ac219b04fe76d895c30e11f4b`.
[CI №173 / 44102979954](https://github.com/Mutaracha/kobalt_ru/actions/runs/44102979954)
**PASS**: focused native EPUB/FB2 regressions, включая новые assertions layout/hash/trace,
offline checks и Kobo Qt5 build/package/contents. Qt6 пропущен.
[Пакет Qt5 для совместной проверки этапов 3+4](https://github.com/Mutaracha/kobalt_ru/actions/runs/44102979954/artifacts/14336659474).
[Native results](https://github.com/Mutaracha/kobalt_ru/actions/runs/44102979954/artifacts/14338460061).
Устройство, отдельный RTL и all-variant не проверены; два FB2-недочёта не исправлялись.

## Полный файловый инвентарь baseline

`M` — содержимое изменено, `A` — только в форке. Пути относительно корня.
Это исходный inventory **до** описанного выноса; назначения новых модулей выше.

### .github

- `A` `.github/workflows/kobo-qt5-device.yml`

### Корень

- `M` `AGENTS.md`
- `A` `FB2_FORK_PLAN.md`

### assets

- `M` `assets/CMakeLists.txt`
- `M` `assets/acknowledgments.json`
- `M` `assets/en.strings`
- `A` `assets/hyph/Russian_EnUS.pattern`
- `A` `assets/icons/phosphor/dark-theme.svg`
- `M` `assets/manifest.json`
- `A` `assets/notices/Fluent-MIT`
- `M` `assets/notices/Hyphenation-patterns`
- `A` `assets/ru.strings`

### doc

- `M` `doc/acknowledgements.md`
- `A` `doc/beta8-device-check.md`
- `A` `doc/fb2-footnotes.md`
- `A` `doc/fork-decisions.md`
- `A` `doc/fork-kobalt_ru.md`
- `A` `doc/fork-update-workflow.md`
- `A` `doc/forum-description.ru.bbcode.txt`
- `A` `doc/qt6-one-off.md`
- `M` `doc/storage.md`
- `M` `doc/validation/links.md`

### platforms

- `M` `platforms/android/CMakeLists.txt`
- `M` `platforms/android/main.cpp`
- `A` `platforms/ci/cleanup-runs.sh`
- `M` `platforms/desktop/CMakeLists.txt`
- `M` `platforms/desktop/main.cpp`
- `M` `platforms/kobo/CMakeLists.txt`
- `M` `platforms/kobo/build.sh`
- `M` `platforms/kobo/common/book_menu.cpp`
- `A` `platforms/kobo/common/page_button_mapping.h`
- `M` `platforms/kobo/common/reader_coordinator.cpp`
- `M` `platforms/kobo/nickel-qt5/native_publication.cpp`
- `M` `platforms/kobo/nickel-qt5/native_ui.cpp`
- `M` `platforms/kobo/nickel-qt5/reader_controller.cpp`
- `M` `platforms/kobo/nickel-qt5/refresh.cpp`
- `M` `platforms/kobo/nickel-qt6/native_publication.cpp`
- `M` `platforms/kobo/nickel-qt6/reader_controller.cpp`
- `M` `platforms/shared-inputs.cmake`
- `M` `platforms/strings-test.cmake`
- `M` `platforms/strings.cmake`

### src

- `M` `src/app/about/about_page.cpp`
- `A` `src/app/about/fork_about.cpp`
- `A` `src/app/about/fork_about.h`
- `A` `src/app/about/fork_debug.cpp`
- `A` `src/app/about/fork_debug.h`
- `M` `src/app/library/library_view.cpp`
- `M` `src/app/reader/panels/chapter_contents.cpp`
- `M` `src/app/reader/panels/light_panel.cpp`
- `M` `src/app/reader/settings/appearance_panel.cpp`
- `M` `src/app/reader/settings/appearance_panel.h`
- `A` `src/app/reader/settings/fork_settings.cpp`
- `A` `src/app/reader/settings/fork_settings.h`
- `M` `src/app/reader/settings/header_footer_panel.cpp`
- `M` `src/app/reader/settings/settings_panel.cpp`
- `M` `src/app/reader/views/navigation.cpp`
- `M` `src/app/reader/views/navigation.h`
- `M` `src/app/reader/views/reading_page.cpp`
- `M` `src/app/reader/views/reading_page.h`
- `M` `src/app/reader/views/sleep_designs.cpp`
- `A` `src/app/reader/views/sleep_ribbon_text.cpp`
- `M` `src/app/reader/views/sleep_screen.cpp`
- `M` `src/app/reader/views/sleep_screen.h`
- `M` `src/app/reader/window/reader.cpp`
- `M` `src/app/reader/window/reader_build.cpp`
- `M` `src/app/reader/window/reader_p.h`
- `M` `src/app/reader/window/reader_refresh.cpp`
- `M` `src/app/reader/window/reader_settings.cpp`
- `M` `src/engine/crengine/column_view.h`
- `M` `src/engine/crengine/engine_document.cpp`
- `M` `src/engine/crengine/engine_document.h`
- `M` `src/engine/crengine/reader_alignment.cpp`
- `M` `src/engine/crengine/reader_beautifier.cpp`
- `A` `src/engine/crengine/reader_fb2.cpp`
- `A` `src/engine/crengine/reader_fb2.h`
- `M` `src/engine/crengine/reader_links.cpp`
- `M` `src/engine/crengine/reader_page_layout.cpp`
- `M` `src/engine/crengine/reader_page_layout.h`
- `M` `src/engine/crengine/reader_source.cpp`
- `M` `src/engine/crengine/reader_source.h`
- `M` `src/engine/crengine/reader_styles.h.in`
- `A` `src/publication/fb2.h`
- `M` `src/publication/local_resources.cpp`
- `M` `src/publication/publication.cpp`
- `M` `src/publication/publication.h`
- `M` `src/publication/resource_cache.cpp`
- `M` `src/publication/resource_cache.h`
- `M` `src/reading/appearance_settings.cpp`
- `M` `src/reading/appearance_settings.h`
- `M` `src/reading/chapter.h`
- `M` `src/reading/features.h`
- `M` `src/reading/header_footer.cpp`
- `M` `src/reading/header_footer.h`
- `M` `src/reading/opening_job.cpp`
- `M` `src/reading/page_navigation.h`
- `M` `src/reading/reader_session.cpp`
- `M` `src/support/build_info.h.in`
- `A` `src/support/debug_language.cpp`
- `A` `src/support/debug_language.h`
- `A` `src/support/locale.cpp`
- `A` `src/support/locale.h`
- `M` `src/support/strings.h`
- `M` `src/ui/controls/buttons.cpp`
- `M` `src/ui/controls/buttons.h`
- `M` `src/ui/controls/icons.cpp`
- `M` `src/ui/controls/icons.h`
- `M` `src/ui/controls/options.cpp`
- `M` `src/ui/controls/options.h`
- `M` `src/ui/controls/reader_icon_vectors.h`
- `A` `src/ui/controls/return_title_text.h`

### test & diagnostic

- `A` `test & diagnostic/1a56b68/diagnostics 1a56b68.zip`
- `A` `test & diagnostic/61dbfaa/Sin City vol 4.kepub.epub`
- `A` `test & diagnostic/README.md`
- `A` `test & diagnostic/SHA256SUMS`
- `A` `test & diagnostic/diagnostics 61dbfaa.zip`
- `A` `test & diagnostic/diagnostics a170389.zip`

### tests

- `M` `tests/desktop.cpp`
- `M` `tests/kobo_shutdown.cpp`
- `A` `tests/manual/README.md`
- `A` `tests/manual/books/comics/README.md`
- `A` `tests/manual/books/comics/spice-single-page.epub`
- `A` `tests/manual/books/publisher/Кокаиновые+короли.epub`
- `A` `tests/manual/materials.sha256`
- `M` `tests/model/appearance_settings.cpp`
- `M` `tests/model/chapter_progress.cpp`
- `A` `tests/model/comics.cpp`
- `A` `tests/model/fb2.cpp`
- `M` `tests/model/header_footer.cpp`
- `M` `tests/model/links.cpp`
- `M` `tests/model/local_resources.cpp`
- `M` `tests/model/nickel_book_menu.cpp`
- `M` `tests/model/reading_book.cpp`
- `M` `tests/model/resource_cache.cpp`
- `A` `tests/model/strings_locale.cpp`
- `M` `tests/model/ui.cpp`
- `M` `tests/reader_lifecycle.cpp`
- `A` `tests/support/anchor_preview.h`
- `M` `tests/support/reader_source_index_probe.h`

## Приёмка завершающего рефакторинга

Пользователь принял runtime `0e3e9f98` по совместному checklist этапов 3+4:
FB2/EPUB открытие, comics/ориентация, восстановление полей, top/bottom progress
и панель чтения. Структурный рефакторинг закрыт. Новые ошибки FB2 ведутся
отдельно в плане и [первичном разборе](reports/fb2-post-refactor.md).


## Проверка границы полосы меню (2026-10-26)

Полоса переиспользует ListHeader, interfaceFont, readerStyle и единый formatter
headerfooter::BookTimeLeft/Values; не имеет собственного расчёта скорости, хранения,
строк, screen-refresh policy или места в пагинации. Presentation construction,
formatting, placement/visibility и toolbar-Hide observer сосредоточены в существующем
views/fork_navigation.cpp. В reader_build остаётся создание через factory, в
Navigation::update — один dispatch, в fork_reader_actions — передача chromeValues.
Navigation не получает ReadingBook/ReaderSession: используются готовые Values.
Это устраняет разросшиеся fork-only блоки в upstream build/navigation, не копируя
общую реализацию ListHeader или header/footer. Checked comics fill использует общий
icon engine/cache/state и прежнюю fork geometry, не отдельную кнопку/bitmap.
CI/device для последнего изменения — см. FB2_FORK_PLAN.md.


## Beta.9 migration: adapter path boundary

Этап 2 использует src/engine/blaze/adapter для прежнего full-book adapter и наших
reader_fb2/reader_comics/fork_styles/render модулей. Engine worker/activity и общие
links/word_counts contracts размещены в src/engine по границе upstream beta.9.
39 переносов проверены на неизменность тел вне include directives; это ещё не
новые реализации Engine API/ReaderSession/Hybrid. Сборочные globs исключают только
vendor crengine/extensions, не adapter. Nullable Settings/Appearance predicate
полосы вынесен и проверен отдельно. Следующий перенос обязан заменить реализации
общего adapter/session на beta.9, сохранив узкие format-specific hooks, а не
заморозить старый whole-book EPUB pipeline. Текущий статус — в FB2_FORK_PLAN.md.


## Annotation foundation checkpoint — candidate 4e014dc5

Nine shared upstream files restored byte-for-byte: rows h/cpp, highlight_store
h/cpp, annotation_database.cpp, highlight_session h/cpp, bookmark_list h/cpp.
Reader annotationRows/bookmarkRows and annotationDetails hydration bodies use
upstream book-page/note metadata. No new schema or duplicate writer; model/UI
retry contract is upstream's. Labeled fork highlight palette unchanged.
[Consumer matrix](reports/beta9-annotation-port-matrix.md) tracks the remaining
NoteEditor/actions/gesture/lifecycle integration. This is not a complete annotation
port or a declaration that all other beta9 differences are intentional.

Intermediate 29487359 / CI 45033710524: Qt5 PASS, native failed exactly at
`Compact excerpts did not carry their notes`. SQLite excerpt consumer was still
old; whole upstream annotation_database.cpp restored in 4e014dc5. The assertion
was retained. Final run 45033749297 SUCCESS: native 9m55s + Qt5 5m01s; device pending. Intermediate artifact is not the
verified handoff package. EPUB Hybrid/frameImage/comics, Libra2 mapping and the
fork palette remain byte-identical to pre-checkpoint 34946cf2.
