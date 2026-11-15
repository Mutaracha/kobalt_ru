# kobalt_ru — актуальный план

## Принятое решение — текущий этап интеграции

Все предложенные переводы согласованы пользователем, включая уточнённую группу.
258 строк EN/RU внесены в assets; это подготовка ресурсов, **не готовая beta.8**.
Новые функции ещё не доступны без завершения runtime merge.

Для кнопок использовать **позиционную модель upstream beta.8**: held(),
назначения одинаковых действий, none, частичные назначения и отображение
настроек — из базовой версии. От форка оставить только Libra 2 / Qt5 landscape
correction в определении поворота. Старый actionRotation убрать при интеграции,
не применять его поверх held(). Это новое решение заменяет прежнее требование
сохранять physical-action семантику для обоих-next/none.

Если механизмы дублируют друг друга (standby при открытии, notes и т.п.),
брать цельную реализацию upstream. Если изменены отдельные пункты общей функции,
делать контекстное слияние с сохранением согласованных fork-отличий. Текущая
runtime-база c4b8c1e не заменена пробным merge; бинарник beta.8 пока не выпущен.
Подробности незавершённой интеграции — в FB2_FORK_PLAN.md.

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
Production merge пока не опубликован: основная группа EN/RU согласована,
включая20 контекстных ключей. Подготовительные EN/RU assets применены;
runtime merge ещё не завершён.
Подробная карта конфликтов/семантических пересечений и проверки:
[doc/beta8-migration.md](doc/beta8-migration.md).

### Порядок работ

1. **Согласовать тексты** [EN/RU proposal](doc/beta8-strings-review.md):
   256 новых/изменённых upstream значений; 2 строки dark-toggle hiding уже
   согласованы пользователем. [Сводная таблица старого и нового RU](doc/beta8-strings-comparison.md)
   отделяет 8 прямых замен, новое имя настройки сносок и 247 новых upstream-ключей.
   Согласованные fork-подписи, нижние вкладки и standby warning не заменять
   механически. Основная группа принята; opening.protected — без точки,
   settings.inpage_footnotes — «Сноски внизу страницы», термин «ридер».
   [Контекстный аудит](doc/beta8-context-review.md) уточняет sleep.letter_*,
   pill, annotate/annotations, page_turn(s), from/to; эти20 строк также согласованы и внесены в assets.
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
   deferred Appearance и успешную waveform. Upstream held()/positional IDs
   пересекается с fork actionRotation: не применять оба преобразования.
   Для кнопок оставить только нашу Libra 2 landscape correction, а held(),
   both-next/none/частичные назначения и отображение брать из upstream
   по явному решению пользователя. Upstream Zen/presets не должны
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

В текущей итерации применены согласованные EN/RU ресурсы и закреплено правило
разрешения пересечений. Пробный runtime merge разобран, но ещё не завершён
и не опубликован. Новый пакет beta.8 не создавался.


### Проверки подготовительных переводов

- Применены258 согласованных EN/RU значений; общий словарь1421 ключей.
- CMake kobalt_check_string_parity, placeholders, генерация EN/RU и полный
  source-reference scan текущего src/ + Kobo adapters: PASS.
- platforms/strings-test.cmake с Ninja: PASS (включая invalid/duplicate/missing
  key, regeneration и locale-parity negative cases). Локальный Make-generator
  прогон упёрся в timestamp-sensitive fixture; код теста не ослаблялся.
- Это не проверка runtime beta.8: merge core/notes/UI ещё не завершён.

Подготовительные переводы `d4953148`: [CI43747021251](https://github.com/Mutaracha/kobalt_ru/actions/runs/43747021251)
PASS native focused + Kobo Qt5/offline/package. Qt5 artifact14175205488,
36 454 953 bytes; native-results14174718835. Qt6 skipped. Этот пакет всё ещё
beta.7-based runtime с новыми ресурсами, **не merged beta.8**, не выдавать его
за проверку upstream notes/schema/buttons. Последний подтверждённый пользователем
runtime остаётся c4b8c1e.

Следующий implementation checkpoint: заменить остатки fork notes в engine/UI/
settings/tests canonical upstream реализацией; совместить FB2/comics с новым
opening pipeline и page_content; завершить positional buttons с единственным
Libra 2 landscape guard; затем hidden dark toggle и полный relevant CI.
Пробное дерево с частично разрешёнными конфликтами не опубликовано, поскольку
оно ещё содержит несовместимые старые/новые символы. Ветка содержит только
согласованные ресурсы и документированный порядок интеграции, не сломанный merge.

### beta.8 runtime integration candidate — 2026-10-26

Integrated pinned upstream 679175046d8c806ea3b404525180fbc29a26fcd2 using
upstream notes/opening standby and positional button semantics; retained the
Libra2-only landscape correction and individual FB2/comics/progress changes.
Approved dark-toggle hidden feature wired without resetting the current theme.
Legacy FB2 notes preference migrates to the canonical key (canonical wins).
Offline checks and string-generation checks pass; native and Kobo Qt5 compilation
and runtime regressions must pass before this candidate is offered for installation.
Debug-language shutdown uses the existing `application quits` blocking reason;
no new allowlist exception or opening optimization was introduced.

### beta.8 integration verification — completed for device-test build

Runtime source: **dcc9ead5d25dc60e8ffa1923ccf1c82bef5cd1fa**.
Upstream 679175046d8c806ea3b404525180fbc29a26fcd2 is a merge ancestor.
CI **43755468526**, build **153**: SUCCESS.
- Native EPUB/FB2/comics/appearance/settings/lifecycle/button regressions: PASS.
- Additional supplied publisher EPUB note 4/5 regression: PASS with upstream notes.
- Kobo Qt5 compile, offline release checks and package validation: PASS.
- Artifact **14178457535**, `Kobalt-qt5-device`, 36,926,875 bytes.
- Artifact transport SHA-256: `345dcc7daa95083495912022cf04c66e70d7d48d2911be1639d0d735a8efe99c`.
  This is GitHub's artifact digest, not the inner installer ZIP checksum.
- Qt6 device build was intentionally skipped; desktop native tests use the pinned Linux SDK.

API and merge repairs include typed appearance patches, preset-cache shell
initialization, null initialization and safe teardown of deferred controls, and
updating legacy test assumptions to upstream default-off notes/modal separators.
Hidden-theme preference and Libra2-only four-orientation polarity have regression
coverage. No tests were disabled. Earlier preparation-only entries above are
historical and superseded by this completed runtime/CI checkpoint.

The build is ready for physical Libra2 testing, **not yet physically accepted**.
Keep the successful waveform policy; opening-speed optimization remains stopped.
The large footnote/body gap remains a separate physical review item, not declared
fixed by the merge. Backup/installation and checks: `doc/beta8-device-check.md`.

## Device fixes после build153 — WIP

- Согласовано: «Капсула» вместо «Плашка»; текст письма без изменений.
- Подготовлены изменения fitting sleep-текстов, draft comics viewport/chrome,
  компактного верхнего отступа Layout и промежутка перед примечаниями.
- Перед выпуском: проверить координаты ссылок/выделения, cache reopen,
  preview on/off без повторного открытия; native + Kobo Qt5 и скриншоты.
- Это не новая проверенная сборка. Рабочая база: build153.
