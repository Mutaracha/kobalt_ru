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
| Нативный FB2 | Распознавание, публикация, ресурсы/обложка/метаданные, preview, FictionBook CSS; открытие из библиотеки Kobo. `.fb2.zip` исключён. | `publication/fb2.h`, `engine/crengine/reader_fb2.*`; подключения в publication/resources, reader_source, engine_document и Kobo routing. |
| Сноски FB2 | `notes/comments`, числовой run-in title, надстрочные labels, format salt v8, миграция legacy Boolean. EPUB остаётся на общем beta.8 pipeline, labels подняты. | `reader_fb2.*`; CSS/выбор stylesheet и lifecycle cache в engine_document/reader_source, migration в appearance_settings. Индексы и текущие отступы приняты, доработка не открывается. |
| Семантика FB2 | Заголовки/стихи/эпиграфы/bylines исключаются из нормализации alignment/beautifier; дополнительные inline tags и line-height selectors. | Предикаты в `reader_fb2.*`; по одному вызову в upstream passes. CSS selectors остаются на общей границе применения stylesheet. |
| RU | Полный словарь, plural one/few/many, русский перенос с EN, English fallback. Locale при каждом принятом открытии книги, ExtraLocales/C/POSIX, debug markers. | `support/locale.*`, `debug_language.*`, `strings.h`, `assets/ru.strings`, `platforms/strings*.cmake`; platform entry points. |
| About/debug | Дополнительная строка fork + UTC build time; скрытый выбор языка. Stock version/commit сохранены. | `app/about/fork_about.*`, `fork_debug.*`; вызов из about_page, build_info и build definitions. |
| Comics | Per-book/default off, эффективное подавление полей/chrome без потери значений, preview, landscape пары, отдельные wide/center страницы, центровка/поворот portrait center, hit bounds и resource readiness. | `reader_comics.*`, `column_view_comics.cpp`, `engine_document_comics.cpp`; состояние/общая отрисовка и позиционирование подключены в ColumnView/EngineDocument, publication/resources/session/UI. |
| Меню чтения | Быстрые dark/comics кнопки; comics state-only ink, запрет действия при busy без серого мигания; скрываются только кнопки. Счётчик страниц перед батареей. | `fork_reader_actions.cpp`, toolbar wiring/refresh/navigation, features/settings. Ни режим, ни Layout control не отключаются скрытием. |
| Прогресс | Выключено/Внизу/Вверху, top выше header, зеркальные chapter ticks, legacy enabled→Bottom. | `reading_page_progress.cpp`, reading_page geometry, header_footer model/panel, chapter fractions и session/navigation refresh. |
| Shared UI | Settings-row presentation, compact checkbox Light/Layout, корректный общий baseline RU tabs, return-title typography, fork glyphs. | `fork_settings.*`, options/buttons, `return_title_text.h`, navigation, `fork_icon_geometry.*`. Не дублировать экраны Settings. |
| Сон | Подгонка длинных надписей, две строки remaining в ribbon, fit оценок widgets; общая standby-логика beta.8 не заменяется. | `sleep_ribbon_text.cpp`, узкие изменения sleep_screen/sleep_designs. |
| Lazy Appearance | Уже существовавшее отложенное построение Appearance с palette/refresh для поздних children. Lazy Settings не реализована, задача закрыта. | `fork_reader_actions.cpp`, reader window state/build/refresh/settings. Не новая оптимизация в этом рефакторинге. |
| Kobo Libra 2 | Только landscape polarity correction поверх positional/held модели beta.8; другие устройства — upstream. | `platforms/kobo/common/page_button_mapping.h`, Qt5 native_ui; ReaderController lifecycle hooks. |
| Kobo/FB2 прогресс | FB2 не использует EPUB-specific native position; сохраняются собственные source positions, в Nickel время/проценты. | Kobo native_publication, ReaderController, publication/opening_job/reader_session. |
| E-ink comics | Graphics/repair policy по submitted damage; диагностика refresh. Не принудительный whole-screen full flash. | reading_page property и `nickel-qt5/refresh.cpp`. Согласованную waveform не менять. |
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
- `engine/crengine/engine_document_comics.cpp` — синхронизация видимых graphic
  ресурсов перед публикацией кадра. Общий cache/paint loop не копируется.
- `engine/crengine/reader_fb2.*` — также владеет format-specific предикатами
  upstream alignment/beautifier passes; общие passes не заморожены копиями.
- `app/reader/window/fork_reader_actions.cpp` — действие comics с draft/busy guard
  и существующий lazy Appearance lifecycle. `reader_build.cpp` только подключает действие.
- `app/reader/views/reading_page_progress.cpp` — painter top/bottom/chapter progress;
  резервирование места остаётся частью общей ReadingPage geometry.
- `ui/controls/fork_icon_geometry.*` — bubble и Fluent dark geometry. Generated
  `reader_icon_vectors.h` возвращён к beta.8 без fork-геометрии.

Существующие FB2, locale/debug, About и Settings модули остаются на своих местах.
Новый generic plugin/hook framework, подмена upstream экранов, vendor snapshot
в репозитории и второй механизм notes/standby не вводятся. Новые `.cpp` подхватывают
существующие source globs Kobo/desktop/Android; отдельные platform-specific копии не нужны.

**Что намеренно остаётся в upstream-файлах:** интеграция формата в Publication и
resource lifecycle, сохранённые optional fields/миграции, эффективные настройки
EngineDocument, смешанная отрисовка обычных колонок/comics, mapping page/DOM/hit,
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
