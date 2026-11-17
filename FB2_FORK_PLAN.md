# kobalt_ru — актуальное состояние

Принятые решения: [doc/fork-decisions.md](doc/fork-decisions.md).
Инструкции агентам: [AGENTS.md](AGENTS.md).

## Завершено

- Переход на upstream beta.8 и задачи после интеграции — **done** по решению пользователя.
- Comics: иконка со сплющенным овалом и длинным загнутым хвостиком принята пользователем.
- Работа со сносками завершена: индексы работают верно, текущие отступы оставлены
  без изменений. Дальнейшая доработка зазоров и пагинации не запланирована;
  закрытие задачи не означает отдельного исправления общего алгоритма пагинации.
- Последний проверенный runtime: `d9536352eb0264127f1a53815b7533d413bb4884`.
  [CI44089641669](https://github.com/Mutaracha/kobalt_ru/actions/runs/44089641669):
  расширенный focused native и Kobo Qt5 package/contents **PASS**. Не all-variant.
  [Qt5 artifact](https://github.com/Mutaracha/kobalt_ru/actions/runs/44089641669/artifacts/14331407822).
  Приёмка прежней иконки/сносок не является новой физической проверкой рефакторинга.
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
All-variant и повторная проверка на устройстве не выполнялись.

**Остаток полного запроса:** смешанные участки EngineDocument (CSS/effective layout/
paint), Publication/ResourceCache lifecycle, inline RU policy в strings.h,
ReadingPage/navigation/settings wiring. Они не объявлены полностью вынесенными.
Следующий этап — сокращать эти участки через узкие format/locale/view helpers,
не копируя upstream классы и не меняя принятый функционал; сверять по карте и CI.
Закрытые продуктовые задачи beta.8 не возобновляются.

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
