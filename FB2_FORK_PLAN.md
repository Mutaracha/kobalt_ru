# kobalt_ru — актуальный план работ

Единая очередь задач и статус для продолжения работы. Принятые решения:
[doc/fork-decisions.md](doc/fork-decisions.md). Инструкции агентам:
[AGENTS.md](AGENTS.md). История до разделения документов:
[архив](doc/archive/fork-notes-before-consolidation.md).

## База и состояние

- Upstream beta.8 интегрирована; этап runtime merge **done**, не «подготовка».
- Последний проверенный runtime: **build163 / `5b6c8afe`**.
  [CI43948942647](https://github.com/Mutaracha/kobalt_ru/actions/runs/43948942647):
  focused native (включая comicsQuickToggle) и Kobo Qt5 package **PASS**.
  [Kobalt-qt5-device](https://github.com/Mutaracha/kobalt_ru/actions/runs/43948942647/artifacts/14266591934).
- Пользователь подтвердил полный preview comics при переключении режима.
  Новая quick-кнопка есть; её иконка требует визуальной доработки по скриншоту.
- Сноски **не приняты**. Эксперимент положения отменён, базовое поведение
  восстановлено, надстрочные labels добавлены. Pagination defect не исправлен.
- Полная all-variant/платформенная матрица и устройство не подменяются focused CI.

## Активные задачи

| Задача | Статус | Следующее действие / проверка |
|---|---|---|
| Разделить инструкции, план и решения | in progress | English AGENTS; компактный план; decision register; сохранить исторические evidence и проверить ссылки. |
| Облегчить иконку quick comics | in progress | Общая action-геометрия/толщина; native + Qt5, затем сравнение на Libra2. Поведение кнопки не менять. |
| Сноска целиком переносится после ссылки | planned | Использовать готовый English report; собрать контролируемую репродукцию, computed styles и regression reference+first fragment. Не менять renderer без отдельного задания. |
| Зазор перед/после нижних сносок, интервалы | planned | Визуальная проверка EPUB/FB2; одна/несколько/длинная note, 1/2 колонки, cache reopen, popup/links/selection. См. D05. |

## Выполненные текущие изменения (done)

| Область | Результат / граница проверки |
|---|---|
| beta.8 integration | build153: native/Qt5 PASS; upstream notes/standby/positional buttons + согласованные отличия. |
| Sleep text fitting / «Капсула» | build157 и последующие; screenshots017–020 сохранены как контрольный результат. |
| Полный comics preview / shared Light checkbox | build159; пользователь подтвердил chrome/progress/body preview. |
| Menu page counter / superscript note labels | build162 native/Qt5 PASS; общий статус notes остаётся открытым. |
| Comics quick toggle / English note report | build163 native/Qt5 PASS; новый запрос касается толщины иконки. |
| Предыдущие comics landscape, progress, Appearance, Libra2 | Реализованы; действующие ограничения и отдельные device confirmations в decision register и device evidence. |

## Отложено / blocked

| Задача | Статус / условие |
|---|---|
| Общая оптимизация открытия, lazy Settings | deferred — остановлена пользователем; без нового запроса не продолжать. |
| Очистка пользовательского кэша | deferred — после beta, не возвращать автоматически. |
| Ориентационная схема физических кнопок в Settings | deferred — предложение, реализации нет. |
| Удаление старых Actions runs/artifacts | blocked — DELETE403 / нужны права Actions; не удалять рабочую evidence. |
| Причина старого зависания после завершённого frame load | blocked — не установлена; при повторе нужны system/kernel logs или stack. Не объявлять исправленной standby-патчем. |

## Gate перед следующим device handoff

1. Проверить explicit diff относительно актуальной remote branch; не включить
   посторонние восстановленные файлы/старые версии SDK/workflow.
2. Offline checks, relevant native regression и Qt5 package/contents validation.
   Отдельно записать недоступные/непройденные проверки.
3. Зафиксировать runtime source, CI/build/artifact и область проверки в
   [device-check](doc/beta8-device-check.md), не добавлять новый журнал в AGENTS.
4. Сравнить comics иконку с search/light/night на Libra2, включая checked/unchecked
   и обе темы. Не менять размер hit target, раскладку или waveform.
5. Сноски остаются отдельной открытой приёмкой; успех icon/preview не закрывает их.

Перед установкой/откатом beta.8 соблюдать backup/schema contract из
[storage](doc/storage.md) и [device-check](doc/beta8-device-check.md).
