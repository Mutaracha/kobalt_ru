> АРХИВ: исторический контекст, не текущий план и не инструкция установки.
> Старые pending/next не возобновлять. Текущий источник — FB2_FORK_PLAN.md в корне.

# Журнал расхождений с beta.9 → вход для beta.10

## Неизменяемый checkpoint

- Upstream beta.9: `be47258850ffdc3295257475afdf8b8f6cb53176`.
- Fork: `3cc7ee19c59a34cdba9651d534192a0a3c79cc9e` (handoff docs поверх runtime
  `2c5d2c554b50181b1d03aa4ee2e14779c3866104`).
- Проверенный runtime: Qt5 build268, [CI45050356339](https://github.com/Mutaracha/kobalt_ru/actions/runs/45050356339),
  native и Qt5 SUCCESS; device acceptance pending. Текущий отчёт/скрипт не меняют runtime.
- Это журнал **остаточных расхождений**, а не сертификат полного выравнивания.
  Релиз beta.10 здесь не проверен. Его SHA нельзя подменять moving HEAD.

## Полный список — не только знакомые конфликты

[files.tsv](files.tsv) содержит **все 439** неидентичных tracked путей:
247 different, 158 fork-only, 34 upstream-only; ещё 1102 идентичны.
146 записей относятся к документации/исходным диагностическим материалам;
остальные **293** — не документация (включая root config), а не «293 бага».
Предыдущая сокращённая таблица на 290 строк исключала root config и документацию.
[summary.json](summary.json) хранит SHA, счётчики и группы;
[changes.stat](changes.stat) — размер реальной дельты.

Для каждой записи есть оба blob/gitlink OID и mode, группа, консервативный
статус разбора, действие при обновлении. `upstream-only` означает **нет в форке**,
а не «новое в beta.10». `REVIEW` означает, что перенос/сохранение ещё не принят.
Группировка по пути — только навигация: она намеренно не объявляет файл
«намеренной особенностью» и не заменяет разбор hunks. В mixed файлах сохранять
fork-функцию не значит сохранять весь старый upstream-файл.

## Семантический журнал / handoff

| ID | Остаток относительно beta.9, доказательство / consumers | Решение и следующий шаг |
|---|---|---|
| B9-01 | **Закрытая implementation group:** `src/sync/`, `reader_kosync.cpp`, общие sleep preferences/snapshot; после Wi-Fi переноса `wifi-idle/`, installer, hooks, sync_exclusion. Сверять identity отдельно от mixed controllers. | Upstream — единственный владелец KOSync/common sleep/Wi-Fi policy. Не восстанавливать старые fork-версии. Проверки runtime268 зелёные, физическая сеть/сон не подтверждены. [Handoff](../beta9-wireless.md), [sleep](../../validation/beta9-kosync-sleep.md). |
| B9-02 | **OPEN:** `reader_coordinator.{cpp,h}` существенно расходятся; upstream quiet/preload, opening state, private lifecycle consumers отсутствуют/изменены. Одновременно здесь принятые FB2 routing и locale на каждое открытие. | Переносить связную upstream coordinator/opening реализацию с зависимостями; вернуть узкие FB2/locale hooks, не копировать whole upstream controller вслепую. `previewBook`/format gating не эквивалентны EPUB-only predicate. Не считать preload уже перенесённым по наличию lazy Settings. |
| B9-03 | **OPEN:** `search_panel.{cpp,h}` создаёт keyboard в constructor вместо upstream lazy factory; caption/detail readiness не учитывают upstream chapter/text contract. `chapter_contents.cpp` не содержит upstream `setPages`, `reader_build/refresh/settings` имеют связанные отличия. | Связать панели, Reader callbacks, chapter/navigation model и тесты. `SearchHit` уже имеет chapter, но consumer parity от этого не появляется. Не добавлять только метод без caller. RU plural/общий стиль строк сохранить. Недостающие строки отдельно согласовать. |
| B9-04 | **OPEN:** `dictionary_panel` eager keyboard + другой save action; `dictionaries_page` отсутствует upstream stale/showEvent; также light/settings/header-footer consumers. | Импортировать upstream lazy behavior целиком, отдельно сохранить принятый fork row/action/icon style. Не трактовать все отличия save checkbox как policy. Тестировать первое/повторное открытие, hide/resize до создания keyboard. |
| B9-05 | **OPEN:** `reader_debug.cpp` отсутствует, mixed `reader.cpp/reader_p/reader_dialogs/settings/refresh` содержат неперенесённые status/debug/percentage consumers. | Определить весь upstream consumer graph, не объявлять отсутствующие diagnostics косметикой. Не возвращать удалённую page-count/time-to-book-end полосу меню и quiet/frontlight diagnostics. `percentageFormatKey` в sanitizer не доказывает перенос UI/поведения. |
| B9-06 | **OPEN:** `native_save_trace.h` отсутствует, fork `native_event_save.h` и `native_reading_save.h` присутствуют; `progress_policy`/Qt native adapters отличаются. | Сопоставить save ordering и FB2 native progress контракт, затем убрать заменяемый путь, не поддерживать двух writers. Соответствующие nickel trace/save тесты переносить вместе. Общая sleep/storage group не означает parity этих платформенных consumers. |
| B9-07 | **OPEN, portability:** Android PickerDelivery/ScreenBrightness/library_open и tests отсутствуют; Java Activity/BOOX, CMake, manifest смешанные. | Переносить delivery/brightness/lifecycle API и wiring/tests вместе, не только файлы helpers. Kobo Qt5 приоритет; отсутствие Android проверки явно фиксировать. |
| B9-08 | **OPEN, tooling:** desktop engine_monitor, scenario resources, eval resource gate отсутствуют; scenario/desktop build consumers отличаются. | Перенести reporting + build/test consumers одним блоком. Отсутствие этих путей — не одобренное fork-поведение. Desktop/Android не блокируют Kobo-пакет, но остаются в журнале parity. |
| B9-09 | **OPEN, tests:** upstream chapter_parity, fast_shaping/surfaces, layout_interrupt, page_estimate, text_sample и fixtures отсутствуют. Существующие model tests тоже отличаются. | Сопоставить с переносимым production API, зарегистрировать и запускать tests/fixtures вместе. Нельзя считать native SUCCESS доказательством запуска отсутствующих тестов. Замороженные EPUB изменения не импортировать скрыто под видом тестов. |
| B9-10 | **PRESERVE HOOKS, mixed hunks REVIEW:** FB2 parser/resources/styles/notes, comics geometry/resources/hit bounds, `reader_session` frameImage, engine model/adapter. | Сохранять утверждённые FB2/comics. Базовые EPUB Hybrid алгоритмы сейчас не менять. Это граница работы, не оправдание всех 21 engine delta. [FB2 Hybrid preparation](../fb2-hybrid-preparation.md); `.fb2.zip` исключён. |
| B9-11 | **PRESERVE PRESENTATION, mixed hunks REVIEW:** RU/CLDR/ExtraLocales/C-POSIX, fonts/brand/About UTC, half-filled dark icon, shared rows, sleep ribbon newline, comics/dark button hiding, swipe inversion, Libra2 polarity. | Только узкие fork hooks поверх upstream; не тащить старые полные панели. Новые EN/RU строки согласовывать. [Решения](../../../../../fork-decisions.md). Device waveform/ghosting не подтверждены CI. |
| B9-12 | **REPLACE-BETA10, план:** fork `reading_page_progress.cpp` + hooks reading_page geometry/build/refresh/settings, header_footer model/panel и tests. | Полностью удалить старый renderer и заменяемое wiring при переносе **4a9576ae017684d290dd2311f4f5195e5e043274**. Не поддерживать параллельный старый путь. До этого runtime unchanged. D04: это не разрешение вернуть меню page/time-to-end. |
| B9-13 | **REVIEW:** assets/build/acknowledgments и CI provenance, cache keys, source inventories, generated RU/FB2 CSS. | Проверять inventory источников, notices и licenses вместе с production; не менять SDK/dependency pins ради снятия конфликтов. NickelFix MIT уже bundled в268; отдельная acknowledgments запись ещё не добавлена (не считать завершённым UI attribution). |
| B9-14 | **DEFERRED:** старый freeze после frame load; EPUB Hybrid beta.10 ожидания. | Не заявлять исправление и не смешивать со структурными переносами. Freeze ждёт нового примера. FB2 Hybrid — подготовка, не автоматическое включение. |

Эта таблица даёт решения/известные причины по группам. **Пофайловый semantic
sign-off ещё не завершён:** все непрочитанные hunks остаются REVIEW, в том числе
в строках с разрешённым fork feature. Отсутствие строки в краткой таблице не
исключает файл из exhaustive ledger.

## Как использовать при выходе beta.10

1. Зафиксировать точный release SHA beta.10; получить объект без смены текущей
   ветки. Для beta.9 объект можно получить командой ниже. Не брать свежий HEAD
   вместо релиза и не создавать новую рабочую ветку в этой сессии.
2. Сначала прочитать upstream дельту **beta.9 → beta.10**, затем пересечь её пути
   с files.tsv. Так отделяются новые upstream изменения от оставшегося долга beta.9.
   Чистые от fork delta файлы тоже проверить на новые API/callers.
3. На каждый пересекающийся consumer разобрать три состояния: pinned beta.9,
   текущий fork и pinned beta.10. При выборе upstream удалять заменяемую fork
   реализацию, её источники/опции/tests; не строить второй pipeline.
4. Закрывать semantic пункты ссылкой на implementation commit и проверку;
   intentional fork hooks — ссылкой на решение и узкие hunks. Остальные REVIEW
   оставлять видимыми. Не переносить лишь конфликтующие файлы без callers/tests.
5. Повторно сгенерировать ledger на новом immutable fork commit, diff старого и
   нового files.tsv: что исчезло, что добавилось, какие OID изменились. Сохранить
   этот beta.9 checkpoint как исторический вход; не перетирать его beta.10 данными.
6. Запустить offline/source checks, Qt5 adapter tests/ARM build/package validation,
   native regressions; отдельно перечислить device acceptance и непроверенные
   Android/Qt6. Зелёный пакет не закрывает semantic gate.

## Воспроизведение и просмотр конкретного изменения

Из корня репозитория; Python нужен только offline audit, **не CMake/build**:

```sh
git fetch --no-tags https://github.com/nicoverbruggen/kobalt.git be47258850ffdc3295257475afdf8b8f6cb53176
python3 platforms/ci/upstream_delta.py \
  --upstream be47258850ffdc3295257475afdf8b8f6cb53176 \
  --fork 3cc7ee19c59a34cdba9651d534192a0a3c79cc9e \
  --output /tmp/beta9-delta-check
python3 platforms/ci/test_upstream_delta.py
git diff --no-ext-diff --no-renames \
  be47258850ffdc3295257475afdf8b8f6cb53176 \
  3cc7ee19c59a34cdba9651d534192a0a3c79cc9e -- src/app/reader/panels/search_panel.cpp
```

Скрипт читает Git objects, а не dirty checkout/mtime; сравнивает также modes,
symlinks/gitlinks, бинарные файлы, корректно записывает TSV с пробелами в путях.
Совпадающие файлы не теряются из счётчика. Отсутствующий объект — ошибка, а не
пустое дерево. Тесты проверяют immutable input, повторяемость, binary/mode delta,
добавление/отсутствие файла и консервативность статусов. Автоматического semantic
одобрения, merge или запуска build скрипт не делает.
