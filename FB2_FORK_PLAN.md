# kobalt_ru — актуальный план

Этот файл — действующая очередь, не накопительный журнал прошлых сборок.
Требования к реализации: [AGENTS.md](AGENTS.md). Пользовательская инструкция:
[doc/fork-kobalt_ru.md](doc/fork-kobalt_ru.md).

## 1. Подтверждённая база

- Последний проверенный runtime: **`c4b8c1e`**, beta.7-based fork.
- [CI43645562443](https://github.com/Mutaracha/kobalt_ru/actions/runs/43645562443):
  native focused regressions и Kobo Qt5 build/package/contents PASS.
- [Qt5 artifact14134257020](https://github.com/Mutaracha/kobalt_ru/actions/runs/43645562443/artifacts/14134257020),
  36 441 133 bytes; native-results14133572838. Qt6 intentionally skipped.
- Пользователь подтвердил корректность comics: отдельные широкие страницы,
  портретный center-bitmap поворачивается clockwise только в landscape,
  широкие center-bitmap не вращаются, половины независимо центрированы по
  вертикали и появляются корректно. Код `3e810b7`; CI43502596739 PASS.
- Пользователь подтвердил Libra 2 во всех четырёх ориентациях (`c4b8c1e`):
  портретные направления сохранены относительно `3e810b7`, оба landscape
  направления исправлены. Явные назначения учитывают поворот, none отключён,
  настройки не переписываются. Предыдущая общая инверсия76852b5/e0b7c1f отменена.
- R1/R2/R3 beta7 выполнены, native/Qt5 PASS. Название «Резервные шрифты»
  согласовано; дополнительное длинное пояснение не добавлять.
- Appearance deferred реализована (`e21c3de`, CI43202483204 PASS).
- Открытие защищено от обычного standby (`e31dcdd`/`61dbfaa`, CI43213319349 PASS);
  успешное открытие подтверждено диагностикой. Это correctness fix, не speedup.
- Рабочая waveform policy сохранена; пользователь подтвердил отсутствие ghosting.

Старые пометки «ожидает проверки» для перечисленных исправлений закрыты.
Это не означает, что выполнены все возможные платформенные/миграционные тесты.

## 2. Активный этап — переход на upstream beta.8

Переход разрешён пользователем. Цель: `v1.0-beta.8`,
**`679175046d8c806ea3b404525180fbc29a26fcd2`**.
База начала: fork **`130091f`**; общий ancestor — beta.7 `e86533c`.

**Статус:** подготовка начата; пробное трёхстороннее объединение выполнено без
изменения рабочего кода. 433 upstream-changed файла, **31 конфликтующий путь**.
Production merge пока не опубликован: сначала согласование новых EN/RU строк.
Подробная карта конфликтов/семантических пересечений и проверки:
[doc/beta8-migration.md](doc/beta8-migration.md).

### Порядок работ

1. **Согласовать тексты** [EN/RU proposal](doc/beta8-strings-review.md):
   256 новых/изменённых upstream значений; 2 строки dark-toggle hiding уже
   согласованы пользователем. [Сводная таблица старого и нового RU](doc/beta8-strings-comparison.md)
   отделяет 8 прямых замен, новое имя настройки сносок и 247 новых upstream-ключей.
   Согласованные fork-подписи, нижние вкладки и standby warning не заменять
   механически. До согласования runtime assets не менять.
2. **Перенести beta.8 целиком**, контекстно разрешить конфликты и проверить
   автоматически объединённые файлы. Не выбирать ours/theirs для общих модулей.
3. **Заменить fork сноски upstream-реализацией**: canonical `inpage_footnotes`,
   default false, DOM markers, links, selection/hit testing, appearance/state,
   cache и upstream tests. Legacy value мигрировать явно; один переключатель,
   без параллельных fork CSS/notes веток. EPUB — как в базовой beta.8;
   нативную поддержку FB2 сохранить и проверить отдельно.
4. **Standby — единая upstream policy**: openingWorkPending,
   `busy && !run.fallbackAlert`, независимый save veto и существующие regressions.
   Не восстанавливать fork power policy/Wi-Fi saver.
5. **Добавить скрытие переключателя тёмной темы** в upstream Hidden features.
   Скрывается кнопка меню чтения; активная тема и сохранённый выбор остаются.
   По умолчанию кнопка видима. Название и описание согласованы; текст — в EN/RU proposal.
6. **Сохранить fork поведение**: FB2, RU locale/plurals, stock About и fork UTC,
   comics/spread/complete-pair publication/hits, progress Top над header,
   Bottom/Off и ticks, durable/effective geometry, Libra 2 rotation mapping,
   deferred Appearance и успешную waveform. Upstream Zen/presets не должны
   перезаписывать обычные значения comics/chrome/margins.
7. **Storage**: перенести schema upgrade и исправление duplicate-ID native save,
   сохранив FB2 percentage/time-only positions. Актуализировать doc/storage.md.
8. **Проверить и выпустить тестовый Qt5 package**: offline/EN-RU parity,
   placeholders, assets/checksums, native relevant suites, adapters/package;
   затем отдельная приёмка новой версии на устройстве.

### Резервная копия перед первым запуском beta.8

Полный backup `.adds/kobalt/` и штатной БД Kobo обязателен. Beta.8 обновляет БД;
старые beta не читают новую схему. Откат — только с восстановлением согласованного
backup, не установкой старого бинарника поверх migrated profile. Репагинация
ожидаема. До проверенного пакета и backup миграцию пользовательских данных не запускать.

## 3. Единый список отложенных задач

| Задача | Статус / условие возобновления |
|---|---|
| **Большой зазор перед нижними сносками** | Открытый дефект. Исправлять после замены механизма на beta.8; перенос upstream сам по себе его не закрывает. |
| **Ориентационно-зависимая схема кнопок в настройках** | Предложение, код не написан. Поворачивать устройство/кнопки, размещать подписи у соответствующих кнопок и показывать effective действия после преобразования, не изменяя сохранённые назначения. |
| **Общая оптимизация открытия** | Остановлена пользователем; автоматически не возобновлять. |
| **Ленивая сборка Settings** | Не реализована; часть остановленной оптимизации, не смешивать с переносом beta.8. |
| **Очистка пользовательского кэша** | Отложена; не возвращать автоматически при merge. |
| **Очистка старых Actions runs/artifacts** | Не выполнена: DELETE вернул403. Helper есть; нужны Actions write rights. Не удалять active/successful evidence. |
| **Причина прежнего зависания61dbfaa** | Не установлена. Последний frame/load завершены, затем лог обрывается до reboot. Успешный CI и текущие визуальные fixes не доказывают устранение причины. При повторе нужны system/kernel logs или stack snapshot. |

### Требование к зазору перед сносками

Зазор между последней строкой основного текста и сноской должен соответствовать
обычному зазору между соседними body lines. Не вводить фиксированный pixel offset
или draw-only сдвиг, расходящийся с pagination, selection и hit testing.

Новые скриншоты **базовой beta.8** подтверждают: большой промежуток остаётся и
при меньшем основном шрифте. Их источник — пользователь; точный binary SHA и
байты книги не переданы. Это visual evidence, не автоматический pixel test.
Файлы: [test & diagnostic/beta8-baseline](test%20%26%20diagnostic/beta8-baseline/).

Проверки исправления: FB2 и EPUB, одна/несколько/длинная split note, разные
font sizes и интервалы100/150/200%, cached reopen, preview, popup/tap/selection.
`FOOTNOTE_MARGIN_REM=1` — root-font reservation, не обычный body line spacing;
не считать изменение одной CSS-константы доказанным решением.

## 4. Приёмка новой beta.8 — не старые незакрытые исправления

Эти проверки относятся к **будущему объединённому runtime**, а не отменяют
подтверждения текущего `c4b8c1e`:

- backup/schema/legacy preference migration; reopen и native/local progress save;
- EPUB/FB2 notes on/off/default, ссылки/popup, selection, continuation, cache;
- hidden features + dark toggle: hide/show, сохранение темы, reopen, Zen;
- comics on/off, portrait/landscape/wide/odd page, backward, anchor/viewer/hits,
  original bitmap identity, отсутствие crop/stretch и прежняя waveform;
- progress Top/Bottom/Off: header/footer, reflow, ticks, preview, без второго reserve;
- Settings до Text/Layout; theme/interface font до первого открытия Appearance;
  повторное открытие без дублирования/reset;
- Libra 2: четыре ориентации, custom next/previous/none, native menus/lists/preview;
- standby success/cancel/error/fallback/restart и независимый save veto;
- EN/RU clipping новых текстов, font sizes и representative UI variants.

Полная native/all-variant release matrix не подменяется focused branch CI.
Kobo Qt5 — приоритет. Desktop/Android не blockers, но общий код не ломать.

## 5. История и evidence

Прежний подробный план/журнал сохранён в Git на
[`130091f:FB2_FORK_PLAN.md`](https://github.com/Mutaracha/kobalt_ru/blob/130091f/FB2_FORK_PLAN.md).
Его старые «pending» — история этапов, не действующая очередь.
Сырые diagnostics и uploads принадлежат `test & diagnostic/`, не `doc/`.
Curated `tests/manual/` fixtures и publisher-check arguments не перемещать.

В этой подготовительной итерации изменены только план, инструкции, proposal
и supplied evidence. Новый runtime/merge/package ещё не создавался.
