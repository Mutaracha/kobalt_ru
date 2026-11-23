# 022 — FB2 engine поверх beta10 + RU + publication/resources

Статус: **prepared**. Source patch создан и структурно проверен; новая native/Qt5
CI ещё требуется. 020/021 отдельно прошли CI45259254139. Ни тот результат, ни
успешный apply не являются проверкой новых C++-изменений 022.

## Что перенесено

- Raw FB2 memory input, resource-backed input через одноэлементную виртуальную
  map. Формат CREngine проверяется после LoadDocument; EPUB media type gate
  остаётся обязательным для всех остальных resource providers.
- Формат выбирается **до configure()**, чтобы stylesheet не наследовался от
  предыдущей книги. Default FB2 CSS берётся из того же pinned CREngine, что HTML CSS.
- Метаданные/обложка читаются CREngine из FB2, а не преобразуются в синтетический EPUB.
- DOM language binding, numeric note labels, FB2 vocabulary в alignment/beautifier,
  line-height selectors и безопасном popup HTML. Общий EPUB note pipeline не заменён.
- Форматные inpage note CSS и ранее принятые raised/run-in labels. Сам canonical
  `inpage_footnotes` остаётся upstream-настройкой, новые политики не добавлены.
- Семантический preview из FB2 с сохранением предков и текстового offset; без
  сериализации генерируемых boxing/pseudo nodes и без изменения reading document.
- FB2 не получает spine window/chapter slicing, даже с runtime keys от прежнего EPUB.
  Chapter-mode request всё ещё применяет настройки и делает whole-document reflow.
- Форматный маркер parsed cache и FB2 filename hint. Адаптированы **обе** ветки
  сохранения beta10: saveDocumentCache и новый saveDocumentCacheStep, а также eviction
  и diagnostic checksum. Старый форк не имел полного набора этих новых API.

## Что специально НЕ переносилось

- Старый ZIP32 memory archive parser: beta10 ZIP64/ZipResource путь сохранён.
- Comics PageDrawBuffer/ColumnView/helpers/frameImage — это 030, не часть 022.
- Старые opener accelerations, flash/refresh policy, Hybrid algorithms или Libra2 mapping.
- Platform open routing, library file filters/metadata и legacy note-setting
  migration: это оставшийся 023. Нельзя объявлять платформенный FB2 готовым только
  на основании прямого engine test.

## Структура patch

16 файлов: reader_source.cpp/.h, engine_document.cpp/.h, reader_fb2.cpp/.h,
reader_fork_styles.cpp/.h, reader_alignment/beautifier/links/page_layout,
reader_styles.h.in, shared-inputs.cmake, desktop CMake и новый fb2_engine.cpp.
Adapter cpp обнаруживаются существующими glob на платформах; отдельная ручная
регистрация дублирующих списков не добавлена. Новая test target и её area label
регистрируются в том же patch, не откладываются до 060.

## Проверки

Локально PASS:
- upstream → 010 → 020 → 021 → 022, включая реальное содержимое nested export;
- hashes всех patch; 9 Python regression tests series runner;
- CMake generation, production string references, EN/RU parity/percent contracts,
  asset manifest; 1808/1808 ключей, новый перевод этим patch не требуется.

В новом `engine.fb2-engine` заложены проверки:
- UTF-8/Windows-1251, metadata/title/author, TOC, embedded font count, binary cover;
- запрет raw FB2 в supportedArchive(EPUB-only), source state reset при FB2 → EPUB;
- exact whole-document reflow с унаследованными spine-window keys;
- preview/сохранение исходного anchor, memory и ResourceCache input;
- save, cache hit, clean incremental save и eviction FB2 parsed cache.

Новая CI строит/выполняет этот test вместе с local-resources, zip-resource и
plain-archive, затем отдельно компилирует Qt5 и check-package. Пока результат
не получен — это **план тестов, не PASS**. Qt6 hardware, Android и устройство
не проверялись. Полный стек всё ещё блокируется на 022/023/030–060.

## CI45261059977 и исправление регистрации fixture

Qt5 build/check-package и check-ready-prefix — PASS. Linux остановился на
компиляции tests/model/fb2_engine.cpp: book_fixtures.h использует KOBALT_TEST_ZIP,
но новый test target отсутствовал в существующем CMake-списке определения макроса.
Это не runtime failure и не выполненный engine-test. В 022 добавлен fb2_engine
в тот же список; отдельный regression assertion проверяет регистрацию после
применения патчей. Новая CI должна скомпилировать и выполнить тест; до этого
статус 022 остаётся prepared. Ошибка не обходится исключением теста из сборки.


## Повторная CI: причины и исправления

CI 45267339459: Qt5/package PASS; native compile PASS и три resource/archive теста PASS.
FB2 runtime остановился на cache-reuse assertion: исходный короткий fixture меньше
64 KiB, а upstream ParseDocument намеренно не ищет в cache такие документы.
Исправлен fixture: XML comment увеличивает только cache-тест до >128 KiB,
не раздувая rendered text. Порог production не менялся, assertion сохранён.
Повторный runtime ещё требуется; 022 остаётся prepared.
