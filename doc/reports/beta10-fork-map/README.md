# Карта доработок форка: beta9 → beta10

## Зафиксированные базы и полнота

- Чистый upstream beta9: `be47258850ffdc3295257475afdf8b8f6cb53176`.
- Форк: `cb705bdd4fd72b8c1f48535204f54c4eb3264ede` — общий стиль первого пункта и перенос language UI в settings включены.
- Предыдущий проверенный binary: `ac21d582`, CI45067475543/build290 native + Qt5 PASS. Пользователь подтвердил открытие своих тестовых книг на устройстве; EPUB counter отдельно deferred.
- CI текущего runtime: [45152988058/build291](https://github.com/Mutaracha/kobalt_ru/actions/runs/45152988058), **native + Qt5/package PASS**. Предыдущая кнопка проверена пользователем; исправленная высота ещё не принята на устройстве. Документационный коммит с картой не меняет binary.

Сравниваются Git trees, не грязный checkout. **1556 путей: 1203 identical, 180 different, 173 fork-only, 0 upstream-only.** Это не число доработок или багов. Из 353 отличий: **194 runtime/build/assets/tests**, **159 reference** (документация, исходные загрузки пользователя, README и инструкции). Предыдущая версия карты входит в reference-пути; это дополнение и текущая регенерация добавлены после сравниваемого checkpoint.

- [Карта всех путей с OID, mode, kind и действием](all-files.tsv) — исчерпывающая основа.
- [1203 неизменённых файла](take-upstream.txt) — брать из beta10 без слияния наших правок.
- [Все 194 изменённых runtime/build/assets/tests пути](modified-files.md) — без сокращений и пропущенных тестов.
- [159 reference-путей](reference-files.tsv) — история и руководства, не runtime-доработки.
- [Машинные счётчики](summary.json); [скрипт воспроизведения](reproduce.py).

Ниже перечислены смысловые доработки и основные точки интеграции. Один shared файл может обслуживать несколько строк; исчерпывающий список находится в приложениях. Не считать весь отличный файл нашей реализацией: переносить только нужные hunks поверх beta10.

## Подробности по запросу пользователя

[Сноски/форматные стили и About/brand/UI/icons — подробная дельта](styles-and-presentation.md):
selectors, DOM markers, CSS, семантика FB2, checked-state иконок, checkbox geometry,
реальный About hook и границы общего upstream-кода.

## Смысловая карта

### F01. FB2: открытие, ресурсы, метаданные и платформенные входы

**Сохранять:** FB2 как формат, XML/ZIP resources, metadata/cover, маршрутизацию открытия; не включать FB2 partial-layout/Hybrid по аналогии с EPUB.

Файлы:
- `src/publication/fb2.h`, `publication_fb2.cpp`, `fb2_resources.cpp`, `.h`, `publication.cpp`, `.h`.
- `src/publication/local_resources.cpp`, `resource_cache.cpp`, `.h`, `image_request_trace.h` — shared ресурсные входы и diagnostics, переносить контекстно.
- `src/engine/blaze/adapter/reader_fb2.cpp`, `.h`, `reader_source.cpp`, `.h`, `engine_document.cpp`, `.h`; `src/reading/opening_job.cpp`.
- `platforms/kobo/common/book_menu.cpp`, `.h`, `reader_coordinator.cpp`; `platforms/kobo/nickel-qt5/native_publication.cpp`, `reader_controller.cpp`; аналоги `nickel-qt6`; `platforms/android/main.cpp`, `KobaltActivity.java`; `platforms/desktop/main.cpp`.

**Beta10:** брать upstream opener/lifecycle, добавлять форматные входы. Не ослаблять DRM/format gates EPUB и не превращать FB2 в ZIP. **Проверки:** FB2/FB2.ZIP, cover, metadata, note links, повторное открытие/позиция; обычный EPUB, protected/unsupported formats. Тесты: `tests/model/fb2.cpp`, `fb2_session.cpp`, `local_resources.cpp`, `resource_cache.cpp`, `nickel_book_menu.cpp`, `tests/desktop.cpp`.

### F02. Нижние сноски и форматные стили

**Сохранять:** canonical `inpage_footnotes` default false, миграцию legacy FB2 key; общий note pipeline, FB2 notes/comments selectors, run-in числовых заголовков и надстрочные номера нижних примечаний. Принятые интервалы не переделывать.

Файлы: `reader_fb2.cpp`, `reader_fork_styles.cpp`, `.h`, `reader_styles.h.in`, `reader_links.cpp`, `reader_alignment.cpp`, `reader_beautifier.cpp`, `reader_page_layout.cpp`, `engine_document.cpp`; `src/reading/appearance_settings.cpp`, `.h`; `src/app/reader/settings/appearance_panel.cpp`, `.h`, `fork_settings.cpp`, `.h`, `settings_panel.cpp`.

**Beta10:** совместить только форматные дополнения/принятые стили, общий EPUB note engine брать upstream. **Проверки:** EPUB и FB2, notes on/off, ссылки туда/назад, длинные/соседние примечания, отсутствие влияния на основной текст и сохранённые позиции. См. `doc/fb2-footnotes.md`; тесты `fb2.cpp`, `links.cpp`, `publisher_markup.cpp`, `fallback_styles.cpp`, `beautifier.cpp`.

### F03. Режим комиксов и изображения

**Сохранять:** per-book comics mode, отсутствие полей/колонтитулов без перезаписи обычных настроек, pairing/viewport/frame image, восстановление обычного чтения при выключении, кнопку toolbar.

Файлы: `reader_comics.cpp`, `.h`, `column_view_comics.cpp`, `column_view.h`, `engine_document_comics.cpp`, `page_draw_buffer.cpp`, `.h`, `engine_document.cpp`, `.h`, `reader_source.cpp`; `src/engine/engine.h`; `src/reading/reader_session.cpp`, `.h`, `appearance_settings.cpp`, `.h`; `src/app/reader/views/reading_page.cpp`, `.h`; `reader_settings.cpp`, `reader_build.cpp`, `reader_refresh.cpp`, `fork_reader_actions.cpp`, `reader_p.h`; `assets/icons/phosphor/dark-theme.svg` относится к соседней dark-кнопке, не к comic pipeline.

**Beta10:** сохранить fork-only helpers; в shared файлах переносить только comics hooks, не копировать весь Reader/engine. **Проверки:** single/double pages, portrait/landscape, image proportions, turn preview, on/off с appearance draft; ordinary EPUB. Тесты `comics.cpp`, `columns.cpp`, `figure_layout.cpp`, `alternate_layout.cpp`, `tests/desktop.cpp` (comicsQuickToggle/appearanceChromeDraft).

### F04. RU/CLDR и выбор языка

**Сохранять:** RU resource table, CLDR plural/forms/dates, Kobo locale entry point, приоритет явных markers, отсутствие частичной live retranslation. **Вход языка теперь только первым пунктом Debug**, не в About; прежнее окно English/Русский/Automatic и async markers остаются.

Файлы: `assets/ru.strings`, `en.strings`, `hyph/Russian_EnUS.pattern`; `src/support/locale.cpp`, `.h`, `debug_language.cpp`, `.h`, `string_locale.h`, `string_table.h`, `strings.h`; `platforms/strings.cmake`, `strings-test.cmake`; `src/app/reader/settings/debug_language_dialog.cpp`, `.h`, `src/app/reader/settings/settings_panel.cpp`; `platforms/kobo/common/reader_coordinator.cpp`, desktop/Android entry points.

Потребители CLDR/подписей: `src/app/library/library_view.cpp`, `panels/activity_view.cpp`, `chapter_contents.cpp`, `chapter_graph.cpp`, `search_panel.cpp`, `light_panel.cpp`, `reader_text_presets.cpp`, `src/reading/fork_text_preset_names.h`, `src/ui/controls/highlight_palette.h`, `reader_annotations.cpp`.

**Beta10:** переносить resource keys и локальные hooks поверх новых upstream consumers; новые переводы согласовывать. **Проверки:** EN/RU, 1/2/5/11/21, даты, длинные подписи, Automatic/English/Russian, отмена выбора, перезапуск. Тесты `strings_locale.cpp`, `ui.cpp`, `text_presets.cpp`; не возвращать five-tap жест About.

### F05. About, brand/UI presentation и иконки

**Сохранять:** fork UTC/build row, IBM Plex Sans UI/Instrument Serif brand, согласованную геометрию кнопок/checkboxes/иконок и return-title, подписи annotation palette.

Файлы: `src/app/about/about_page.cpp`, `fork_about.cpp`, `.h`; `src/support/build_info.h.in`; `src/ui/controls/buttons.cpp`, `.h`, `options.cpp`, `.h`, `icons.cpp`, `.h`, `fork_icon_geometry.cpp`, `.h`, `return_title_text.h`, `highlight_palette.h`; `assets/CMakeLists.txt`, `manifest.json`, `acknowledgments.json`, `notices/*` (точные отличия в TSV); `reader_build.cpp`, `reader_annotations.cpp`.

**Beta10:** сохранять небольшие визуальные hooks; не заменять общие layout/row APIs устаревшими копиями. Файлы шрифтов могут совпадать с beta9 и поэтому отсутствовать в списке отличий — сохранить именно настройку использования, не дублировать данные. **Проверки:** RU/EN light/dark, scale, label fit, checkbox rows, modal action geometry, About без языкового жеста; `ui.cpp`, `tests/desktop.cpp`.

### F06. Toolbar, навигация, session minutes и gestures

**Сохранять:** dark/comics toolbar quick actions, согласованные caption/hooks, центрированные session minutes и независимый invert-page-swipes. Старую page-count/time-to-end menu strip не возвращать.

Файлы: `src/app/reader/window/fork_reader_actions.cpp`, `reader.cpp`, `reader_build.cpp`, `reader_p.h`, `reader_refresh.cpp`, `reader_settings.cpp`; `src/app/reader/views/fork_navigation.cpp`, `.h`, `navigation.cpp`, `.h`; `src/reading/page_navigation.h`, `features.h`, `appearance_settings.cpp`, `.h`; `reading_page.cpp`, `.h`.

**Beta10:** upstream session/lifecycle/layout сохранять; переносить только согласованные actions и presentation. **Проверки:** darkThemeToggle, comicsQuickToggle, меню open/close, tap/swipe/button directions, session minutes; `tests/desktop.cpp`, `reader_ui.cpp`, `reader_lifecycle.cpp`.

### F07. Сон: ribbon, переносы текста и fitting

Файлы: `src/app/reader/views/sleep_designs.cpp`, `sleep_ribbon_text.cpp`, `sleep_screen.cpp`, `.h`.

**Сохранять:** принятые sleep captions/newline/ribbon/text fitting; shared save/standby/wake machinery брать upstream. **Проверки:** cover и другие designs, длинные RU строки, orientation, sleep/wake с сохранением позиции. `tests/reader_lifecycle.cpp`, `tests/model/ui.cpp`; общий persistence не считать fork-only реализацией.

### F08. Kobo: Libra2 buttons и display/refresh; BOOX

Файлы: `platforms/kobo/common/page_button_mapping.h`, `nickel-qt5/native_ui.cpp`; `platforms/kobo/display/page_animation.cpp`, `page_transition.h`, `nickel-qt5/refresh.cpp`; `src/platform/api/page_animation_support.h`; `platforms/android/app/src/org/kobalt/android/BooxRefresh.java`.

**Beta10:** это защищённые платформенные исключения, не случайная устаревшая дельта. Сверить новые upstream изменения по hunks; не перезаписать утверждённые mapping/waveform. **Проверки на устройстве:** Libra2 все ориентации и инверсия, dark/light, full/partial refresh, анимации и ghosting; BOOX отдельно. Тесты `native_dialog_overlay.cpp`, `page_animation.cpp`, `page_transition_native.cpp`, `tests/pewter/refresh_qt5.cpp` и `nickel*`. CI не заменяет waveform acceptance.

### F09. Cache образцов оформления: transient diagnostics

`src/reading/style_sample.cpp`: служебный `whole_book_failed` исключён из sample settings/cache key наряду с frame diagnostics. Исходный `tests/model/style_sample_cache.cpp` сохранён, PASS build289.

**Beta10:** проверить upstream реализацию и необходимость двухстрочной дельты; не переносить повторно, если upstream уже исключает это поле. Не связывать с собственным исправлением Hybrid — алгоритм Hybrid этим не менялся.

### F10. Build, регистрация ресурсов/тестов, provenance

Файлы: `.github/workflows/ci.yml`, `kobo-qt5-device.yml`, `.clang-format-ignore`; `platforms/{desktop,kobo,android}/CMakeLists.txt`, `platforms/kobo/{build.sh,sources.cmake}`, `platforms/shared-inputs.cmake`, `platforms/checks/reader-properties.allow`; `platforms/ci/{check-source-archive.sh,cleanup-runs.sh,upstream_delta.py,test_upstream_delta.py}`; остальные wiring-файлы перечислены в [приложении](modified-files.md).

**Сохранять:** FB2/comics/RU source/asset/test registration, Qt5 package/fork identity/provenance и принятые validation gates. Upstream SDK/dependencies/archive helpers переносить вместе с callers. Не импортировать старый CI только из-за отсутствия merge conflicts. **Проверки:** source/offline checks, resource generation, native suite, Qt5 package contents, source/provenance; Qt6/Android отдельно, не выдавать skip за PASS.

### F11. Тестовая инфраструктура, fixtures и доказательства

59 отличающихся test-путей перечислены в [полном списке](modified-files.md#tests), включая model, desktop, lifecycle, Pewter, fonts и manual EPUB. Это не 59 пользовательских функций. Сохранить покрытие FB2/comics/RU/platform и все восстановленные upstream сценарии; fixtures/macros/target registration переносить одновременно. `tests/desktop.cpp` и `tests/model/ui.cpp` смешанные — переносить наши проверки, не заменять upstream suite целиком. Оригинальные загрузки `test & diagnostic/` — evidence, не часть binary.

## Что НЕ является доработкой для сохранения

- Fork progress renderer, chapter ticks, position/Top setting удалены. Нижняя on/off полоса и header/footer model — upstream beta9. Не возвращать старую модификацию из history или сохранённого `progress_top` в пользовательских preferences.
- EPUB Hybrid algorithms — upstream-owned. Проверенная цепочка совпадает с beta9; симптом `~` после полного расчёта deferred до beta10. [Точный отчёт и границы вывода](../beta9-hybrid-counter.md). Это не доказательство работы/неработы чистого upstream на устройстве.
- Восстановленные quiet/lazy Settings/Reader/BookStatus/percentages/font loader/KOSync/lifecycle consumers не переносить как собственные альтернативные реализации. Их неизменённые файлы входят в take-upstream.
- Отменённая page-count/time-to-end menu strip, quiet/frontlight diagnostics, FB2 Hybrid и собственный freeze fix не включать в перенос. Исторический TODO не является действующим требованием.

## Порядок обновления на beta10

1. Зафиксировать **полный SHA** beta10 и новый fork checkpoint; прочитать release/source delta beta9→beta10, включая storage, dependencies и изменения API. Эта карта пока не сравнивает beta10.
2. Для путей в `take-upstream.txt` брать соответствующий результат beta10 **без слияния наших правок**. Если путь удалён/переименован — следовать upstream, а не восстанавливать файл по старому списку. Не выполнять слепой массовый checkout поверх пользовательской работы.
3. В изменённых shared файлах начинать с beta10 и переносить только hunks F01–F10. Fork-only файлы адаптировать к новым APIs, проверить upstream replacement и удалить дубликат, если обязанность уже покрыта.
4. Новые beta10 файлы также брать upstream; среди удалённых/новых файлов проверить helpers/callers сохранившихся fork-only модулей. Автоматически чистый merge не доказывает совместимость.
5. Перенести тесты/ресурсы/build registration и согласовать новые RU строки. Прогнать offline/native/Qt5; затем device acceptance. Повторить Hybrid counter test из отчёта, не писать собственный обход заранее.
6. Получить новую карту от обновлённого runtime. Исторический beta9 baseline и этот checkpoint не перетирать; добавлять новый отчёт.

## Воспроизведение и проверка полноты

Из корня репозитория (оба commit доступны локально):

```sh
python doc/reports/beta10-fork-map/reproduce.py /tmp/beta10-fork-map
# semantic diff одного файла:
git diff --no-renames be47258850ffdc3295257475afdf8b8f6cb53176 \
  cb705bdd4fd72b8c1f48535204f54c4eb3264ede -- src/reading/reader_session.cpp
```

Скрипт сравнивает mode/kind/OID и проверяет точное множество nonidentical через `git diff --no-renames --name-only -z`. TSV содержит все пути, включая Unicode, без git quote-экранирования. Группировка по назначению — навигационная подсказка, не разрешение переносить модуль целиком.
