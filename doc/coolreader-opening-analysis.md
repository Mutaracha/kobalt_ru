# Открытие книг: CoolReader Vlasovsoft и Kobalt

Статус: первичный source/configuration audit и план измерений. Пользователь
наблюдает более быстрое открытие в CoolReader при сходном перелистывании.
Это пока qualitative device observation, не воспроизводимый timing benchmark.
Оптимизацию открытия добавляем в план; изменения page-turn path не требуются
без измеренного основания.

## Что фактически загружено

В `doc/CoolReader/vlasovsoft 2022.5.19/` и `vlasovsoft 2023.10.21/` находятся
готовые ARM программы, Qt 4 библиотеки, настройки, stylesheets и данные.
Файлов C/C++ исходников в этих каталогах не обнаружено. `cr3/cr3` — executable,
не исходный файл. Эти материалы полезны для сопоставления настроек и runtime
поведения, но не дают точный source diff двух выпусков.

Найден публичный проект [sergeyvl/vlasovsoft](https://github.com/sergeyvl/vlasovsoft).
Изучены `cr3/cr3widget.cpp`, `cr3/main.cpp`, `cr3/cr3.pro` на commit
[`5f799936b020f5d87055f1837956524ca1324be1`](https://github.com/sergeyvl/vlasovsoft/tree/5f799936b020f5d87055f1837956524ca1324be1).
Его commit датирован 2015-01-14 («CR3 files added»). **Не установлено**, что эти
исходники соответствуют бинарникам 2022/2023; точные движок/build flags этих
бинарников нельзя приписывать им автоматически. Engine source referenced by
cr3.pro лежит вне дерева этого публичного проекта.

Kobalt использует KOReader CREngine
`f335f7a97e240f505bb8a0dfb721fcb7912a7824` с документированными patches.
Совпадение названия движка не означает идентичность его ревизии, настроек,
шейпинга, кэша или frontend startup работы.

## Конкретные находки

### Настройки и кэш Vlasovsoft

В 2023 `cr3/cr3.ini` видны, в частности:

```
crengine.cache.filesize.min=300000
crengine.cache.forced.filesize.min=65536
crengine.doc.embedded.fonts.enabled=0
crengine.doc.embedded.styles.enabled=0
crengine.image.scaling.zoomin.block.mode=2
crengine.page.header.chapter.marks=1
```

Сходные cache/embedded flags есть в `2022/cr3/data/cr3.ini`; не смешивать этот
файл с небольшим `2022/cr3/cr3.ini`, в котором параметры launcher/application.
Наличие рядом сохранённого `.cr3` cache показывает, что пакет содержит данные
для прогретого сценария, а не доказывает постоянный cache hit на любой книге.

Отключённые embedded fonts/styles могут заметно менять объём работы и результат
верстки. Нельзя просто скопировать их в Kobalt как «ускорение без изменений»:
это может потерять publisher layout, графику или оформление сносок.
Отметки глав на референсе согласуются с включённым chapter-marks property,
но Kobalt рисует свою полосу и отключает native engine header. Переносить весь
native header ради tick marks не нужно.

### Публичный frontend CoolReader

В `CR3View::loadDocument()` виден короткий путь:
`savePosition()` → `LoadDocument(filename)` → `swapToCache()` →
`restorePosition()` → `update()`.

Инициализация font manager и регистрация файлов шрифтов находятся в
`InitCREngine()` / startup. В qmake project включены own DOM memory management,
UTF-8 DOM storage и release defines. Это направления для проверки, **не доказанные
причины** преимущества скорости: отсутствуют точные engine sources/flags
пользовательских бинарников и измерения на одинаковой конфигурации.
Не делать вывод, что всё открытие у CoolReader ленивое или что parsing бесплатен
только потому, что wrapper короткий.

### Kobalt: есть готовые границы измерения

В `ReaderSession` диагностируется `opening-phases`:
`directory_ms`, `hash_ms`, `layout_check_ms`, `model_load_ms`,
`retained_book_hit`, `layout_cache_hit`, `document_cache_hit`, а также `open-end`.

В `reader_source.cpp` добавляются model phase timings:
`model_prepare_ms`, `model_parse_ms`, `model_paginate_ms`, `model_cache_hit`.
На свежем DOM выполняются classification/normalization markers, stylesheet
reinitialization и `Render()`. На cache hit эти markers специально не ставятся
повторно. Это важная существующая оптимизация, не предлагается её «добавлять»
ещё раз или удалять ради необоснованного упрощения.

Внешняя наблюдаемая задержка включает и host handoff, source acquisition,
font discovery, profile/storage, first-frame composition и e-ink presentation.
`open-end` и первый физически показанный текст — не обязательно один timestamp.

## План оптимизации — отдельная задача

1. **Зафиксировать исходные условия.** Один Kobo/firmware, та же книга и байты,
   формат/кодировка, размер шрифта, поля, сноски, hyphenation, embedded fonts/
   styles и близкое число страниц. Отдельно запуск процесса и открытие книги.
2. **Разделить cold/warm/retained.** Первая книга после старта, повторное открытие
   с дисковым cache, повторное открытие retained document, другая книга. Не
   использовать запрещённую cache-cleanup UI функцию и не удалять profile;
   тестовые isolated caches создавать контролируемым benchmark окружением.
3. **Измерять до first usable frame**, полный layout ready и фоновую готовность
   статистики отдельно. Повторять условия, фиксировать разброс, а не выбирать
   лучший запуск. Отделять UI progress от реально готовой страницы.
4. **Сопоставить штатные phase markers.** Найти dominant stage: acquisition/hash,
   discovery/fonts, parse/styles, pagination, cache compatibility, layout
   inspection, first draw или host/display wait. Не добавлять второй trace
   механизм, если существующих diagnostic/evaluation hooks достаточно.
5. **Проверить cache misses.** Policy/fingerprint, density, effective appearance,
   engine/DOM version, embedded fonts и file changes. Сначала объяснить, почему
   ожидаемый cache hit не произошёл; не выкидывать валидность/защиту данных.
6. **Проверить повторную работу.** Повторная font discovery/registration,
   повторное чтение/хеширование, двойной stylesheet/DOM pass и синхронное
   ожидание необязательной статистики. Устранять только подтверждённые дубли;
   не менять immutable identity при изменённой книге.
7. **Разграничить обязательное и отложенное.** Только если trace докажет задержку
   first frame на необязательной операции, перенести её на существующий worker
   с cancellation/ownership, не в paintEvent/main-thread storage.
8. **Проверить функциональные инварианты:** position/save/notes/preview/cache,
   FB2 UTF-8/CP1251, EPUB embedded resources, повторное открытие и source edits.
   Не переносить старую модель close/progress из CR вместо текущего upstream.
9. **Измерить результат тем же протоколом.** Report до/после, representative
   text и image-heavy books, память и worst cases. Page-turn timing не должен
   регрессировать, даже если оптимизируется только opening.

До таких измерений нельзя обещать определённый коэффициент ускорения.
Точная matching source revision Vlasovsoft и файл affected book остаются
полезными входными данными. В эту итерацию не добавляются/обновляются engine
зависимости и не копируются vendor binaries в distribution.


## Уточнение FB2: не выполнять EPUB embedded-font работу

Пользователь верно уточнил: стандартный FictionBook 2 не поддерживает встроенные
шрифты. В openSource сейчас есть общий `embeddedFontCount()` даже для FB2;
после первого вызова count хранится в ReaderSource. Сам метод перечисляет
registered document fonts/embedded list. Для FB2 логично явно возвращать ноль
без EPUB-specific enumeration, включая cache/preview/reused-engine paths.
Это безопасная граница для проверки/оптимизации, но не доказанный источник
заметной задержки — список пуст, а результат memoized.

Не пропускать общую initializeFonts(), регистрацию доступных внешних fonts,
выбор serif/sans/mono fallback или hyphenation: это необходимые ресурсы
отрисовки FB2, а не embedded fonts. Проверить, что при EPUB→FB2 переиспользовании
engine не протекает чужой count, а EPUB с embedded fonts не регрессирует.
Измерить разницу через штатные opening phases; обещание ускорения пока не даётся.
