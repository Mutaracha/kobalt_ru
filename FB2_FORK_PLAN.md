# kobalt_ru — актуальное состояние

Принятые решения: [doc/fork-decisions.md](doc/fork-decisions.md).
Инструкции агентам: [AGENTS.md](AGENTS.md).

## Завершено

- Переход на upstream beta.8 и задачи после интеграции — **done** по решению пользователя.
- Comics: иконка со сплющенным овалом и длинным загнутым хвостиком принята пользователем.
- Работа со сносками завершена: индексы работают верно, текущие отступы оставлены
  без изменений. Дальнейшая доработка зазоров и пагинации не запланирована;
  закрытие задачи не означает отдельного исправления общего алгоритма пагинации.
- Последний CI-проверенный runtime: `69c70aa357209d0ef9ffce5afc68b42d74492a5f`.
  [CI №171 / 44096144408](https://github.com/Mutaracha/kobalt_ru/actions/runs/44096144408):
  расширенный focused native и Kobo Qt5 package/contents **PASS**. Не all-variant.
  [Qt5 artifact](https://github.com/Mutaracha/kobalt_ru/actions/runs/44096144408/artifacts/14333753710).
  Пользователь подтвердил запуск пакетов первого и второго этапов на устройстве.
  После проверки FB2 сообщил о двух недочётах: исправлять после рефакторинга;
  содержание и воспроизведение ещё не описаны, исправления сейчас не предпринимаются.
- Старые отчёты, снимки интерфейса и review-материалы удалены из текущего дерева;
  история Git сохранена. Диагностические архивы и исходные книги сохранены.

## Рефакторинг сопровождения (новый запрос)

**in progress**, первый этап **done / CI verified**:
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

**Третий этап in progress / CI pending:**
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
  visible priority, letterbox/hits, rotation, RTL и сохранение базовой раскладки.
- Локально source-checks, strings self-tests (Ninja) и проверка diff PASS;
  Kobo syntax blocked без Qt5 SDK/Docker. Следующее действие: native/Qt5 CI.

**Остаток полного запроса:** effective layout/configuration EngineDocument,
Publication/ResourceCache state/lifecycle и ReadingPage/navigation/settings wiring.
Не копировать upstream классы ради искусственного нулевого diff; оставлять узкие
явные интеграционные точки. Закрытые продуктовые задачи beta.8 не возобновляются.

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
