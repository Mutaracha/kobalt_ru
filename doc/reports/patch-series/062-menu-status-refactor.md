# R05 — сериализация и переключение статуса меню

Статус: implemented; local series/native CI/Qt5/package PASS; device pending.
База — b2cdb129 (runtime 2258d58b). Аппаратная приёмка R04 отдельно не подтверждена;
новый пакет включает R04, но продолжение работы не считается его приёмкой.

## Изменение

Существующий reading/menu_status.h уже определял showsPage и форматирование
страницы, но UI callback и диагностический снимок сами собирали значения
book_page/session. Теперь модуль также владеет value(preferences) и
чистой операцией toggledPreferences(preferences). UI передаёт результат
прежнему savePreferences и сохраняет тот же callback updateMenuStatus /
updateSessionGeometry; снимок запрашивает эффективное значение у модуля.

Входной QJsonObject не меняется. Сохранение по-прежнему обновляет live preferences
только при успехе writer. Не меняются ключ, допустимые значения, cleaning/schema,
глобальность выбора, fallback к session, геометрия, часы/батарея или формат страницы.
Это рефакторинг владения правилами состояния, не новый режим меню.

## Проверки

Расширен существующий model.appearance-settings native target: absent/null/bool/
number/array/object/empty/invalid/wrong-case/session/book_page; эффективная
сериализация, переключение, два переключения, сохранность входа и остальных
(включая nested) настроек, JSON round-trip и cleaning. Некорректное исходное
значение не восстанавливается после двух taps: восстанавливается эффективный
режим, как и раньше.

Только 040: три production paths и существующий native test. Четыре changed paths
записаны в explicit reference amendment; остальные 1548 путей/байтов/режимов
не менялись. Экспорт 1552 файлов и 15 локальных Python series/package/equivalence
regressions PASS. Локального Qt/cmake SDK нет; C++ cases требуют CI.
030/кэш/Hybrid и переводы не менялись. Beta11 cache note хранится отдельно:
[U11-FONT-CACHE](beta11-upgrade-checklist.md).

## Устройство после CI

1. Открыть меню, нажать на существующую строку статуса: время сеанса ↔ страница книги.
   Часы и батарея остаются на месте, правильная страница видна сразу.
2. Несколько раз переключить режим и закрыть/открыть меню.
3. Закрыть Kobalt и вручную открыть книгу; проверить сохранение выбранного режима.
4. Открыть другую книгу: режим глобальный, а номер страницы относится к этой книге.
5. Если R04 ещё не проверен, в этом же пакете проверить встроенные/свои названия
   стилей по [его чек-листу](061-preset-name-refactor.md); результат указать отдельно.

Новые замеры времени, сброс профиля, очистка кэша и изменение колонтитулов не нужны.

## CI и пакет

Runtime source: `7d13ec2c032307f553459536dacd52be51454d86`.
[CI45815527684](https://github.com/Mutaracha/kobalt_ru/actions/runs/45815527684):
все 3 jobs PASS — series/export/inventory, selected native/Linux (новые cases
в model.appearance-settings), Qt5/package и archived FB2 replay.
Новый C++ тест выполнен CI; локальный Qt SDK запуск не заявляется.

[Qt5 / Libra2 пакет](https://github.com/Mutaracha/kobalt_ru/actions/runs/45815527684/artifacts/15114663890):
`Kobalt-ru-beta10-series-qt5`, 45 438 239 bytes. Устройство R05 ещё не проверено;
R04 включён, его отдельная аппаратная приёмка также пока не подтверждена.
