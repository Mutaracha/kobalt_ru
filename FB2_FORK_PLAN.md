# kobalt_ru — актуальное состояние

Принятые решения: [doc/fork-decisions.md](doc/fork-decisions.md).
Инструкции агентам: [AGENTS.md](AGENTS.md).

## Завершено

- Переход на upstream beta.8 и задачи после интеграции — **done** по решению пользователя.
- Comics: иконка со сплющенным овалом и длинным загнутым хвостиком принята пользователем.
- Работа со сносками завершена: индексы работают верно, текущие отступы оставлены
  без изменений. Дальнейшая доработка зазоров и пагинации не запланирована;
  закрытие задачи не означает отдельного исправления общего алгоритма пагинации.
- Последний runtime: `68b3ac16d592207b467f0778cf1f6742867844a1`.
  [CI44075570386](https://github.com/Mutaracha/kobalt_ru/actions/runs/44075570386):
  focused native и Kobo Qt5 package **PASS**. Это не all-variant проверка.
  [Qt5 artifact](https://github.com/Mutaracha/kobalt_ru/actions/runs/44075570386/artifacts/14325995040).
- Старые отчёты, снимки интерфейса и review-материалы удалены из текущего дерева;
  история Git сохранена. Диагностические архивы и исходные книги сохранены.

## Рефакторинг сопровождения (новый запрос)

**in progress**: составлена [карта отличий beta.8](doc/fork-delta.md), выполнен
вынос fork algorithms по слоям: comics/ColumnView/frame readiness, FB2 semantic
predicates, progress painter, glyph geometry и window actions/lazy Appearance.
Это механическое разделение без изменения defaults, CSS, waveform и storage.
Интеграционные участки и невынесенные обязанности явно отмечены в карте.

Проверки: source-checks PASS; strings generation/parity/self-tests PASS.
Kobo syntax локально **blocked** (нет подготовленного Qt5 SDK/Docker).
Offline helper suite завершилась с exit 0, но source-export под архивным checkout
выдал ошибки отсутствующих validation-record файлов: не считать чистым PASS.
Focused CI расширен существующими alignment/beautifier/page-layout/columns suites.
Нужны native/Qt5 CI для новых translation units; тесты поведения используются
существующие, это не новый all-variant или device pass.
Следующее действие: получить результат push CI, исправить ошибки, записать runtime.
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
