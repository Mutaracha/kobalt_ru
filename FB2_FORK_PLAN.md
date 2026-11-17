# kobalt_ru — актуальное состояние

Принятые решения: [doc/fork-decisions.md](doc/fork-decisions.md).
Инструкции агентам: [AGENTS.md](AGENTS.md).

## Завершено

- Переход на upstream beta.8 и задачи после интеграции — **done** по решению пользователя.
- Comics: иконка со сплющенным овалом и длинным загнутым хвостиком принята пользователем.
- Работа со сносками завершена: индексы работают верно, текущие отступы оставлены
  без изменений. Дальнейшая доработка зазоров и пагинации не запланирована;
  закрытие задачи не означает отдельного исправления общего алгоритма пагинации.
- Последний CI-проверенный runtime: `9e03e25d31bfc000a88048c01f102b16e57d6a8f`.
  [CI №172 / 44100128546](https://github.com/Mutaracha/kobalt_ru/actions/runs/44100128546):
  расширенный focused native и Kobo Qt5 package/contents **PASS**. Не all-variant.
  [Qt5 artifact](https://github.com/Mutaracha/kobalt_ru/actions/runs/44100128546/artifacts/14336795926).
  Пользователь подтвердил запуск первого и второго этапов на устройстве.
  Третий этап на устройстве пока не проверен. При чтении FB2 на втором этапе
  найдены два недочёта без описания; вернуться к ним после рефакторинга.
- Старые отчёты, снимки интерфейса и review-материалы удалены из текущего дерева;
  история Git сохранена. Диагностические архивы и исходные книги сохранены.

## Рефакторинг сопровождения (новый запрос)

**done / CI verified**, устройство этапов 3+4 pending. Первый этап **done / CI verified**:
составлена [карта отличий beta.8](doc/fork-delta.md), вынесены fork algorithms по
слоям: comics/ColumnView/frame readiness, FB2 semantic predicates, progress painter,
glyph geometry и window actions/lazy Appearance. Generated vectors совпадают с
beta.8. Тела 13 перенесённых функций сверены без изменения токенов/выражений
(кроме форматирования/комментариев). Defaults, CSS, waveform и storage не менялись.

Проверки: source-checks и strings generation/self-tests PASS. Расширенный CI
`44089641669` PASS: прежний focused набор плюс alignment/beautifier/page-layout/columns,
Kobo Qt5 build и package contents. Локальный Kobo syntax check не выполнен из-за
отсутствия SDK/Docker; локальный offline helper exit 0 сопровождался source-export
ошибками архивного checkout, поэтому чистый offline PASS берётся только из CI.
Новые поведенческие тесты не добавлялись: перенос проверен существующими suites.
All-variant не выполнялся; пользователь затем подтвердил запуск на устройстве.

**Второй этап done / CI verified:**
- RU таблица, состояние locale и plural suffix policy вынесены в `string_locale.h`,
  структура/поиск таблицы — в `string_table.h`. Общий `strings.h` сохраняет English
  fallback, key display и короткий plural wrapper; API и generated tables прежние.
- FB2 selectors и принятые EPUB/FB2 labels вынесены в `reader_fork_styles.*`.
  Полный собранный inpage CSS сверён byte-for-byte: SHA256
  `0ebdec60d9d7c756cfdadf966e4c52c31311f5b712bb9b34e3c4bfd96abdb86a`.
  Порядок cascade, font sizes, отступы и cache salts не меняются.
- Worker-only single-file transfer вынесен в `publication/fb2_resources.*`;
  lease cancellation, timeout, bounded chunks, first error и sink accounting сохранены.
  Добавлены assertions CSS fragments в FB2 и transfer boundaries в local-resources;
  соответствующий suite добавлен в focused CI.
- Локально source-checks PASS, strings self-tests PASS с Ninja. Make-версия
  self-test не заметила regeneration; причина локального сбоя не установлена.
  При Ninja прошла без изменения теста. Kobo syntax по-прежнему blocked без Qt5 SDK/Docker.
- [CI44096144408](https://github.com/Mutaracha/kobalt_ru/actions/runs/44096144408)
  **PASS**: весь focused набор, `engine.fb2` с CSS digest assertions,
  `model.strings-locale` (обе таблицы/fallback/plurals/key mode),
  `model.local-resources` с transfer boundaries; Qt5 package/contents и offline checks.
  All-variant и повторная проверка устройства не выполнялись.
Следующий этап: отделить оставшиеся fork-участки effective layout/paint и
view/resource lifecycle без дублирования upstream PageDrawBuffer/Settings/notes.

**Третий этап done / CI verified:**
- В `engine_document_comics.cpp` вынесены paint одиночного/парного artwork,
  начало/завершение request tracking и приоритет visible-image batch. В общем
  `image()` остаются dispatch, ordinary columns, pixel normalization и caching.
- Общий `PageDrawBuffer` с EquationImage/IllustrationProbe перенесён в
  `page_draw_buffer.*` одной реализацией. До переноса блок byte-identical
  upstream beta.8 `679175046`; тела методов/помощников сохранены. Это shared
  primitive, не fork-копия и не новый механизм инверсии.
- Порядок рисования, fit/rotation/RTL/centering, metadata/hits, batching/лимиты,
  cleanup при allocation failure и waveform сохранены. Настройки/схемы/salts/CSS
  и строки не меняются; два найденных FB2-недочёта не исправляются этим изменением.
- Добавлен прямой contract-test буфера (normalization, clipping, text/image metadata,
  publisher ink), native CI расширен `engine.image-proportions` и `engine.mathml`.
  Имеющиеся comics tests покрывают cold/cached spreads, neighbour pressure,
  visible priority, letterbox/hits, rotation и сохранение базовой раскладки.
- Локально source-checks, strings self-tests (Ninja) и проверка diff PASS;
  Kobo syntax blocked без Qt5 SDK/Docker.
- [CI44100128546](https://github.com/Mutaracha/kobalt_ru/actions/runs/44100128546)
  **PASS**: прежний focused native набор плюс image-proportions/mathml и новый
  buffer contract в comics; Qt5 build/package/contents, offline checks.
  Физическая проверка нового runtime, отдельный RTL-сценарий и all-variant
  не выполнялись. Следующий этап — завершающий проход по effective layout,
  UI wiring и оставшимся format/resource lifecycle участкам по карте дельты.

**Четвёртый, завершающий этап: done / CI verified.**
- `reader_comics.*`: effective columns/margins overlay, одинаковый для открытия и
  applyAppearance; базовые сохранённые значения не перезаписываются.
- `publication_fb2.cpp`: распознавание raw FB2, прежний fingerprint и virtual metadata.
- `image_request_trace.h`: worker-only trace; общий ResourceCache lifecycle сохранён.
- `fork_navigation.*` и `fork_reader_actions.cpp`: tab ink/toolbar presentation,
  refresh и отдельный счётчик страниц; ReadingPage geometry рядом с progress painter.
- Добавлены assertions overlay/base immutability, FB2 hash compatibility и trace reset.
  Локально source-checks, strings self-tests (Ninja), diff check PASS.
- Узкие format dispatch, persisted fields, worker ownership, mapping/hit и UI wiring
  остаются в общих классах намеренно: это интеграционные API, не копии fork алгоритмов.
  Нулевой diff с upstream не является целью. Runtime/CSS/strings/storage/waveform
  намеренно не меняются. Проверка этапов 3+4 на устройстве — совместно пользователем.


Runtime финального этапа: `0e3e9f98bddc9e1ac219b04fe76d895c30e11f4b`.
[CI №173 / 44102979954](https://github.com/Mutaracha/kobalt_ru/actions/runs/44102979954)
**PASS**: focused native EPUB/FB2 regressions, включая новые assertions layout/hash/trace,
offline checks и Kobo Qt5 build/package/contents. Qt6 пропущен.
[Пакет Qt5 для совместной проверки этапов 3+4](https://github.com/Mutaracha/kobalt_ru/actions/runs/44102979954/artifacts/14336659474).
[Native results](https://github.com/Mutaracha/kobalt_ru/actions/runs/44102979954/artifacts/14338460061).
Устройство, отдельный RTL и all-variant не проверены; два FB2-недочёта не исправлялись.

## После завершения рефакторинга

| Задача | Статус | Условие начала |
|---|---|---|
| Два новых недочёта при чтении FB2 | planned | Пользователь сообщил о наличии двух проблем, детали ещё не переданы. Вернуться после завершения рефакторинга; не угадывать причину/содержание и не смешивать bugfix с переносом кода. |

## Открытая диагностика

| Задача | Статус | Следующее действие |
|---|---|---|
| Причина старого зависания после завершённого frame load | blocked | Данных для отладки нет. При повторе нужны system/kernel logs или stack. Сохранённые diagnostics — в `test & diagnostic/`. Не объявлять исправленной standby-патчем. |

## Закрыто без реализации дальнейших работ

По указанию пользователя все прежние отложенные задачи закрыты, а не перенесены
на следующую beta: оптимизация открытия и lazy Settings, очистка пользовательского
кэша, ориентационная схема физических кнопок в Settings, удаление старых
Actions runs/artifacts. Автоматически не возобновлять. Закрытие задач не означает,
что эти функции реализованы или удалённые Actions-артефакты очищены.

Для будущих установок/откатов сохраняется [backup/schema contract](doc/storage.md).
