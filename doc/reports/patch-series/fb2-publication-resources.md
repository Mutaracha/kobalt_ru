# FB2: подготовлены publication и local resources

## Результат и границы

В текущей ветке подготовлены **два настоящих source patch**, последовательно поверх
upstream beta10 → 010-ru:

1. `020-fb2-publication.patch`: raw-byte FictionBook probe, PublicationFormat,
   inspection raw FB2, виртуальное имя/размер, fingerprint полного файла;
   регистрация desktop source и проверки в существующем local_resources test.
2. `021-fb2-resources.patch`: bounded streaming исходных байтов FB2 через local
   resource adapter, lease/cancellation/revocation и регистрация source/tests.

Это этапы **одного общего переноса FB2**, не самостоятельные пользовательские
профили. Патчи имеют статус `prepared`, не `ready`: clean apply и структурные
проверки выполнены, native/Qt5 CI ещё требуется. Основной runtime форка не заменён.

**Пока отсутствует 022**: engine source/document, metadata/cover, FB2.ZIP, форматные
стили/сноски и platform input routing. Поэтому нельзя объявлять FB2 рабочим форматом
в новой полной сборке, предлагать этот префикс пользователю как готовый пакет или
пропускать 022 и переходить к визуальной приёмке комиксов.

## Адаптация к beta10, а не копия старых файлов

При пробном трёхстороннем переносе обнаружены конфликты в Publication, ResourceCache,
ReaderSource, EngineDocument и ColumnView. Конфликтные результаты НЕ включены в
готовые патчи. Engine/Comic hunks из этого пробного переноса пока не перенесены.

Главное отличие Publication beta10 — новый `zip::readDirectory`/ZIP64. Старый fork
parser ZIP32 не возвращён. В beta10 добавлен только ранний FB2 probe и состояние
формата; исходная ZIP64/EPUB/protection ветка сохранена. После повторной inspection
состояние FB2 сбрасывается вместе с исходными полями.

В local resources сохранены upstream ZIP stream, CRC/XML/DRM/resource policy.
Raw FB2 branch допускается только для одноэлементной виртуальной resource map.
Это важная адаптация: реальный EPUB может иметь обычный ZIP member с именем
`__kobalt_document__.fb2`. Совпадение имени само по себе не должно перенаправлять
чтение всего EPUB-файла в sink. Добавлен regression case для stored/deflated EPUB.

Raw XML не декодируется в UTF-8 на этом слое: сохраняется encoding declaration
и исходные байты (в тесте есть Windows-1251). XML/layout выполнят последующие слои.
Никакого собственного Hybrid, flash suppression, preload/quiet opening accelerator
или изменения физических кнопок в этих патчах нет.

## Проверки

Локально:
- 010 → 020 → 021: clean apply на закреплённый upstream — PASS.
- SHA-256 каждого patch сверяется series loader — PASS.
- EN/RU 1808 ключей, placeholders, percent contracts, production references,
  generated tables и asset manifest через validate.cmake — PASS.
- 9 Python regression tests series loader, включая проверку реального вложенного
  export и блокирование неполного стека — PASS.

Добавлены C++-проверки в существующий `tests/model/local_resources.cpp`:
- настоящий/поддельный root, delimiter/entity guards;
- format/name/size/fingerprint и same-length edits, state reset после failure;
- raw bytes и ограничение размера каждого chunk;
- изменившийся размер файла, sink rejection, revocation до/во время transfer;
- full localResourceFactory path;
- ZIP-member collision с виртуальным FB2-именем, stored и deflated варианты.

Эти новые C++-проверки **ещё не объявлены выполненными локально**: Qt toolchain
отсутствует. В workflow Ordered patch series добавлены отдельные проверки
подготовленного префикса: pinned Linux SDK строит/выполняет local-resources,
zip-resource и plain-archive; pinned Qt5 SDK компилирует и проверяет пакет.
Простое apply не заменяет их результат. Qt6 hardware/Android/device acceptance
не выполнялись. Тестовые пакеты префикса не публикуются как полный форк.

## Исправление сборщика серии

Найден и исправлен отдельный дефект `apply.py`: если export находится внутри
`build/` родительского git checkout, обычный git apply из этого подкаталога может
найти родительский .git и молча пропустить все пути вне его prefix. Внешний temp
check из предыдущего шага не выявлял этот сценарий.

Теперь git discovery ограничен родителем export, унаследованные GIT_DIR/WORK_TREE/
INDEX_FILE/COMMON_DIR не используются. Regression test проверяет фактическое
наличие RU, FB2 source и C++ test changes внутри вложенного export, а не только
код возврата git apply. Не затрагивает runtime reader.

## Дальше

После native/Qt5 результатов исправить обнаруженные проблемы и продолжить 022
поверх этих двух патчей. Не выносить общие helpers в отдельные библиотеки ради
автономности: применять единую последовательность, совместно проверять полный стек.
