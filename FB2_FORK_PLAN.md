# kobalt_ru — актуальное состояние

Принятые решения: [doc/fork-decisions.md](doc/fork-decisions.md).
Инструкции агентам: [AGENTS.md](AGENTS.md).

**Текущая работа — in progress:** переход на beta.9. Blaze, Engine/adapter и
реальные reading consumers подключены. Последний проверенный runtime:
`cb96eb65f80dc556f29bccbb40fd2ffd281121b9`, [CI `44705458156`](https://github.com/Mutaracha/kobalt_ru/actions/runs/44705458156)
**PASS**: focused native EPUB/FB2 regressions, Kobo Qt5/offline/package/contents.
Это не полный upstream/all-variant suite и не device acceptance.

**done / CI verified:** whole-book FB2 boundary, семантический preview, comics
pair readiness через Engine, bounded page-estimate checkpoints; ReaderSession,
OpeningJob и style-sample используют Engine/createEngine. Worker-thread deleter,
retained ownership и close barrier сохранены. Reading/annotation и UI search/page
headers больше не зависят от concrete Blaze adapter. Cache-save использует upstream
no-arg interface. Рендеринг, настройки и waveform на этом шаге не менялись.

Новый FB2 Engine-boundary тест выбирает body passage через search/goTo и строго
проверяет exact/one-based pages до и после смены viewport/шрифта, отсутствие
chapter/refine/full build и lifetime preview после удаления source. Initial
bookmark `/?xml[1].0` не является passage и не использован как эталон страницы;
initial-fallback renderer/bookmark поведение не исправлялось. CI `44615814323`
также PASS для adapter checkpoint до factory consumers. После отвязки UI header
явно подключены links.h + types.h; промежуточные include-ошибки закрыты финальным CI.

**done / focused CI verified — upstream Hybrid session lifecycle:**
ReaderSession cpp/h перенесены целиком от pinned beta9; единственный поведенческий
fork delta в capture — `frameImage()` для comics readiness. Подключены upstream
background estimate/full build, cancellation/yield, next-turn adoption, stepped
cache save, settled/retired ownership, layout generations и display/local page
контракты; opening больше не выполняет старый whole-book layout inspection.
Fork open сохраняет raw FB2 probe, legacy notes и per-book comics. Новые
rendering_mode helpers пока без Settings UI/host preference wiring; complete UI
потребители ещё pending, промежуточный runtime не для устройства.

Для цельного upstream session API подключены его зависимости: chapter equality,
cancellable StarDict preparation, wallpaper worker helpers и preview eviction.
Preview store теперь использует private `.preview`; startup PNG migration/scan
пока не включена, старые disposable tiles не удаляются. Новых UI строк/настроек
на этом шаге нет. Добавлены в focused CI upstream session Hybrid/adoption suite,
preview-store safety tests, dictionary-store и sleep-persistence.

**done / CI verified:** `7079c75f`, CI `44633797097`: focused native suite
и Qt5/package PASS. Предыдущие reading-book/highlight/markup/FB2 session failures
больше не воспроизводятся. Пропущенный `platforms/desktop/run-test.cmake` перенесён
byte-for-byte из pinned beta9: legacy checks запускаются с Accurate по умолчанию,
внешний env override сохранён. Assertions не менялись. Dedicated upstream
Hybrid/adoption suite, FB2 Accurate/Hybrid и fragment-finish regression сохраняют
явные mode overrides и также проходят. FB2 check запускается внутри app.exec,
чтобы aboutToQuit завершал worker до Qt teardown; это test lifecycle correction,
не изменение поведения приложения. Две test-infrastructure правки проверены
одним итоговым CI; отдельная причина прежнего FB2 process exit не изолировалась.

ReadingBook использует upstream whole-book progressPercent/bookPage/bookPages
для activity/native/checkpoint/time remaining; canFinish учитывает book_fraction
и continuesAfter. Regression отклоняет finish на последнем листе не-final
fragment и проверяет whole-book checkpoint percentage. Эти изменения и прежние
appearance exit/lifecycle проверки теперь CI verified.

**done / focused CI verified — checkpoint ownership:** перенесён upstream beta9
ReadingBook request-generation/deferred-checkpoint/position-save-epoch lifecycle,
включая close snapshots и защиту от stale completion. Добавлен upstream-derived
regression для close → новый выбор → forced durable save: только новый epoch
подтверждает позицию, close не теряет completion, поздний ответ не заменяет выбор.
Сохраняется upstream native failure diagnostic. CI `44635988130`: Qt5 PASS,
reading-book/new epoch regression и остальные selected checks PASS, кроме
advancedText(native): worker был заблокирован до завершения фоновой работы.
Перенесено отсутствовавшее upstream ожидание idle перед установкой gate;
строгий QVERIFY(!busy) при заблокированном worker сохранён. Source-checks PASS;
локальный Kobo syntax blocked: нет подготовленного SDK/Docker. Это не device
acceptance. Итоговый CI `44637222380` (`8e241eed`): native и Qt5/package PASS,
включая новый close/epoch regression и advancedText. Промежуточный failure
`44635988130` superseded этим итоговым прогоном.
API retry host bridge, activity/calendar, sleep positions и new-book preset policy
ещё не перенесены вместе с их consumers; не подменены локальными реализациями.

**done / focused CI verified — native transaction boundary:** NativeReadingSession,
NativeProgressExtra и ReadingBridge от pinned beta9 подключены для Qt5/Qt6.
При pause сессия Nickel остаётся до boundary; progress transaction завершает её
и сохраняет время вместе с позицией, failed commit оставляет seconds для следующей
попытки. Explicit/Background retry API перенесён через ReadingBook. Exit/USB
порядок операций взят из upstream; fork UI и unmapped FB2 policy сохранены.
Pewter дополнен проверкой extra apply/commit/refused begin/rollback.
CI `44705458156` (`cb96eb65`): native и Kobo Qt5/offline/package/contents PASS.
Source checks PASS; local Kobo syntax blocked отсутствием подготовленного SDK.
Qt6 source перенесён, но отдельная Qt6 package/device проверка не выполнялась.
Это не проверка на устройстве; wake/recovery consumers и host-mode/UI остаются pending.

**implemented / CI pending — wake/save recovery:** sleep теперь сохраняет progress
перед drain native time, а поздний result обрабатывается после wake. Подключены
upstream SleepSave/epoch callback/clear pending Exit и повторная попытка без закрытия
книги. Согласованные EN/RU строки внесены; оценка переносов по будущим скриншотам
пользователя. Upstream close/sleep order, stale pre-sleep completion и wake retry
regressions перенесены вместе с fixture и включены в focused CI (старые обратные
order tests заменены, не ослаблены). Source checks и strings parity/generation
PASS. Общий host-mode/UI перенос ещё pending.

**Следующий шаг:** связать host rendering-mode preference/application и выполнить
перенести оставшиеся wake/recovery consumers и подключение save epochs;
затем UI whole-book counts/progress, navigation/bookmark indexes и полосу.
Default Hybrid не считать готовым до этой интеграции. Не использовать chapter-local
номера как book pages. Новые настройки/строки на этом checkpoint не добавлялись.

G1–G9 остаются **pending** для полной интеграции. Промежуточный пакет не для
устройства. При итоговой выдаче заполнить runtime/CI и выдать пользователю
[device checklist](doc/validation/beta9-device.md).
Подробности adapter: [отчёт](doc/reports/beta9-engine-adapter.md).


## Завершено

- Переход на upstream beta.8 и задачи после интеграции — **done** по решению пользователя.
- Comics: иконка со сплющенным овалом и длинным загнутым хвостиком принята пользователем.
- Работа со сносками завершена: индексы работают верно, текущие отступы оставлены
  без изменений. Дальнейшая доработка зазоров и пагинации не запланирована;
  закрытие задачи не означает отдельного исправления общего алгоритма пагинации.
- Рефакторинг этапов 1–4 — **done / device accepted**. Пользователь проверил
  финальный runtime `0e3e9f98bddc9e1ac219b04fe76d895c30e11f4b`: FB2/EPUB открытие,
  comics/ориентация, восстановление полей, top/bottom progress и панель чтения.
  [CI №173](https://github.com/Mutaracha/kobalt_ru/actions/runs/44102979954) PASS.
  Это приёмка перечисленного сценария, не all-variant и не закрытие ошибок ниже.
- Старые отчёты, снимки интерфейса и review-материалы удалены из текущего дерева;
  история Git сохранена. Диагностические архивы и исходные книги сохранены.

## Рефакторинг сопровождения (новый запрос)

**done / CI verified**, этапы 3+4 device accepted. Первый этап **done / CI verified**:
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

## Ошибки FB2 после рефакторинга — закрыты

Пользователь проверил сборку №181 (runtime `c0ff0115`) и подтвердил: все
поставленные проблемы FB2 решены. Приёмка на устройстве завершена; новых
FB2-работ в очереди нет. Это не расширяет scope на `.fb2.zip` и не закрывает
старое зависание после frame load.

| Задача | Статус | Следующее действие |
|---|---|---|
| «Умное» обновление | done / device accepted | Пользователь пролистал всю главу 8 → 9 (>10 страниц), flash в начале 9 выполнен. Изменения refresh policy не нужны. |
| Наложение строк FB2 | done / device accepted | Получены скриншоты главы 8 и events.zip (8407bdb). Fira Sans 13.5 pt, weight +1, line 130%, word 80%, justify, widows allow, vertical normal, paragraph controls publisher. В 16:37 source 116024c, document_cache_hit=false; spacing уже выключен. CI 44132499310 на 73fd8369: Qt5 package PASS; native regression FAIL после английского preview — повторное форматирование русских абзацев даёт высоты на 94 px меньше сохранённых (658→564, 564→470, 5264→5170, 1128→1034). До preview, включая paint с weight +1, probe проходит. Подтверждено междокументное рассогласование геометрии; соответствие конкретному наложению главы 8 ещё не доказано. Реализована привязка языка FB2 к FictionBook DOM до pagination (явные lang сохранены), вместо зависимости paint/hits от глобального fallback, изменяемого preview. FB2 cache marker v9 требует нового parse; позиции не меняются. Добавлены проверки сохранения explicit lang и drift до/после repaint. Runtime 1b091187, CI №180 / 44135643021: focused native EPUB/FB2 PASS, Qt5 build/package/contents PASS. Replay проверяет оба направления ru↔en, spacing off/on и drift до/после repaint. [Qt5 artifact](https://github.com/Mutaracha/kobalt_ru/actions/runs/44135643021/artifacts/14350784926). Device-прогон пользователя (9b7748d): глава 8 пройдена с изменением настроек текста, наложение не возникло. Это успешная проверка данного сценария, не доказательство отсутствия дефекта во всех книгах. Новое наблюдение preview вынесено отдельной строкой. |
| Смена фрагмента в предпросмотре FB2 при первой правке | done / device accepted | Source/history + XML fixture check подтвердили потерю позиции при создании synthetic preview: чтение идёт с p/v #22, preview собирается с #0, settings.position удаляется. Выбранное начало совпадает с новым фрагментом скриншотов. Механизм существовал уже в 82eda3be, а не введён language fix; первый визуально затронутый билд не установлен. [Проверка](doc/reports/fb2-preview-position.md). Реализовано извлечение с текущего абзаца с отображением source offset (включая inline) в новый DOM. Добавлен native regression: середина длинного раздела, nonzero inline offset, толщина 0→1→2→0, source state/image неизменны. Runtime c0ff0115; [CI №181 / 44154435468](https://github.com/Mutaracha/kobalt_ru/actions/runs/44154435468) PASS: focused native EPUB/FB2 (включая новый previewPosition), Qt5 build/package/contents. [Пакет Qt5](https://github.com/Mutaracha/kobalt_ru/actions/runs/44154435468/artifacts/14360732277). Пользователь проверил сборку №181 на ридере и принял исправление вместе со всеми поставленными проблемами FB2. Новый UI interaction test не добавлялся. |
| Nickel не обновляет последнюю книгу/процент FB2 | done / device accepted | Пользователь подтвердил исправление runtime 116024c. Activity-only запись принята; точная позиция остаётся локальной. |


### Проверка исправления Nickel FB2

Runtime `5eb8a600f71b5f46794a8c469783577f2ca01eed`.
[CI №175 / 44111989597](https://github.com/Mutaracha/kobalt_ru/actions/runs/44111989597)
**PASS**: focused native, включая session.reading-book с FB2 identity и activity-only
writeback, model.screen-refresh, engine.fb2 с новым chapter/eligible Smart contract;
offline checks и Kobo Qt5 build/package/contents. Qt6 пропущен, all-variant не выполнен.
[Qt5 artifact](https://github.com/Mutaracha/kobalt_ru/actions/runs/44111989597/artifacts/14340793511).
[Native results](https://github.com/Mutaracha/kobalt_ru/actions/runs/44111989597/artifacts/14342042111).
На этапе №175 device-проверка ещё ожидалась; впоследствии пользователь принял
Nickel и Smart на runtime 116024c, а затем все поставленные FB2-проблемы на №181.
Синтетические тесты сами по себе не являлись device-приёмкой.
Предыдущий CI №174 остановился на старом bool assertion теста NativeSaveResult;
он исправлен, №175 полностью прошёл. Новых production rendering изменений нет.

Актуальный пакет с исправлением Nickel: runtime `116024cd`,
[CI №176 PASS](https://github.com/Mutaracha/kobalt_ru/actions/runs/44114141676),
[Qt5 artifact](https://github.com/Mutaracha/kobalt_ru/actions/runs/44114141676/artifacts/14343185395).
Проверка исходной книги и ограничения: [разбор FB2](doc/reports/fb2-post-refactor.md).

## Надёжность FB2 — дополнительный аудит по запросу пользователя

**in progress**. Это отдельное усиление интеграции, не отмена приёмки сборки №181.
Сравнение с KOReader и границы: [отчёт](doc/reports/fb2-reliability-koreader.md).
Исправлены root-probe (не принимать FictionBook из комментария чужого XML),
полный language tag/root override и строгий mapped offset в FB2 preview,
точная граница размера при transfer файла. Добавлены русские nested/verse/subtitle
preview/reflow cases и проверки grow/truncate из sink. EPUB, принятые сноски,
refresh, schema/cache format и строки не менялись. Runtime `09a7acdc`,
[CI №182](https://github.com/Mutaracha/kobalt_ru/actions/runs/44342485883) PASS:
focused native EPUB/FB2 и Qt5 build/package/contents.
[Qt5 artifact](https://github.com/Mutaracha/kobalt_ru/actions/runs/44342485883/artifacts/14443824738).
Следующий шаг: smoke-прогон FB2/предпросмотра на ридере. Новая device-приёмка
ожидается; приёмка №181 не переименована в приёмку №182.

## Сохранение оформления FB2 в предпросмотре

**implemented / CI PASS / device pending**. После №182 пользователь сообщил:
в начале главы при первой правке заголовок сохраняет позицию, но теряет
жирность/отступы. Причина — flatten в synthetic preview. Вместо извлечения строк
реализовано ограниченное копирование semantic поддерева с контекстом
title/poem/cite/section, без generated boxing/маркеров движка. Source bookmark
отображается по codepoint offset внутри блока; основной DOM не меняется.
Добавлены assertions заголовка на исходной книге («На другое утро») и synthetic
strong/title/margins при толщине 0→1→2→0. Начальный native FAIL оказался ошибкой
нового probe: getFont() вызывался на text node (CREngine возвращает null), а не
на родительском element. Probe исправлен; временный прямой запуск/gdb удалён.

Runtime `06cd5e129b2799a0a11cd90a9270f6fb8086b785`:
[CI 44362073516](https://github.com/Mutaracha/kobalt_ru/actions/runs/44362073516)
**PASS** — focused native matrix, reported FB2 fixture, Qt5 compile/package/contents.
[Qt5 device package](https://github.com/Mutaracha/kobalt_ru/actions/runs/44362073516/artifacts/14454752686).
Device-приёмка ожидается. EPUB, сноски и refresh не менялись.

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

## Полоса счётчика в меню чтения

**done / CI verified / device pending**. По уточнению пользователя счётчик
страниц убран из строки часов/батареи в отдельную серую полосу под верхней панелью
обычного меню чтения. Полоса не резервирует место в пагинации; предпросмотр и
заголовки других полноэкранных панелей замещают её. Подпись времени чтения снова
центрируется относительно экрана независимо от ширины индикаторов справа.
Используются существующие ListHeader и строка navigation.page_count, новых строк
и настроек нет. Расширен lazyAppearance test: положение/видимость полосы,
отдельный parent счётчика и центрирование session label.
Runtime `cb3fe8dad2e1db50d1b4c2cf96fdb60d0760126c`:
[CI 44373349020](https://github.com/Mutaracha/kobalt_ru/actions/runs/44373349020)
PASS — focused native matrix (включая lazyAppearance light/dark), offline checks,
Kobo Qt5 package/contents. [Пакет](https://github.com/Mutaracha/kobalt_ru/actions/runs/44373349020/artifacts/14460611469).
Device-проверка ожидается; waveform и пагинация не менялись.

## Полоса меню: закрытие и время до конца главы

**done / CI verified / device pending**. Пользователь подтвердил: при
закрытии меню не перекрытая предпросмотром полоса оставалась поверх книги.
Причина: sibling overlay не скрывался вместе с toolbar на путях без replay
Navigation::update. Navigation теперь наблюдает Hide toolbar и сразу скрывает
полосу. Добавлена регрессия открытия/закрытия обычного меню без Preview (light/dark).
Слева добавлена согласованная оценка до конца главы: chromeValues + общий formatter
ChapterTimeLeft и существующий fieldLabel; при отрицательной оценке слева пусто.
Новых строк, настроек, waveform и изменений пагинации нет.
Runtime `e1fcfa8cffd5b8d1a44f4ef3d40298eb818cce44`:
[CI 44377438368](https://github.com/Mutaracha/kobalt_ru/actions/runs/44377438368)
PASS — focused native matrix (включая plain-menu dismiss/reopen light/dark),
offline checks, Kobo Qt5 package/contents.
[Пакет](https://github.com/Mutaracha/kobalt_ru/actions/runs/44377438368/artifacts/14461174896).
Физическая device-проверка ожидается.

## Полоса меню: оценка книги и курсив

**done / CI verified / device pending**. Пользователь сообщил о пустой
левой подписи при открытии меню и изменил требование: остаток времени книги,
не главы; без названия поля; обе подписи курсивом. Причина пустой подписи:
updateMenuStatus выполняется до topControls->show(), а formatter был gated
видимостью toolbar. Gate удалён, используется BookTimeLeft из chromeValues,
без изменения общего расчёта/настройки точного или приблизительного формата.
Первая буква подписи заглавная. Подписи используют brandFont italic, не caption
с разрядкой. Дополнены проверки italic и сохранения подписи при reopen.
Runtime `76485448e023bc7c72a6b79baa78a38ce657f502`:
[CI 44383356110](https://github.com/Mutaracha/kobalt_ru/actions/runs/44383356110)
PASS: focused native matrix, offline checks, Kobo Qt5 build/package/contents.
[Пакет](https://github.com/Mutaracha/kobalt_ru/actions/runs/44383356110/artifacts/14464463554).
Device-приёмка ожидается; автоматическая fixture без reading history не доказывает
наличие ненулевой оценки на реальном устройстве, поэтому нужна пользовательская сверка.

## Полоса меню: фон, шрифт и первый показ

**done / CI verified / device pending**. Пользователь сообщил белые прямоугольники
под подписями, неудачный brand italic и фрагмент счётчика в (0,0) при открытии книги.
Полоса создавалась sibling-виджетом без initial hide; до первого Navigation::update
могла показываться с default geometry. Добавлен initial hide. Подписи переведены
на interfaceFont italic с явным transparent background вместо brand stylesheet,
который наследовал общий фон QLabel. Регрессии: полоса скрыта до первого меню,
семейство interfaceFont, italic и transparent background, прежние dismiss/reopen.
Новых строк, refresh policy и изменений пагинации нет.
Runtime `ef5424cf8067bad322ef97190baa02ca56f68544`:
[CI 44397464051](https://github.com/Mutaracha/kobalt_ru/actions/runs/44397464051)
PASS — focused native matrix (включая initial hidden, italic/interface family,
transparent stylesheet, dismiss/reopen light/dark), offline checks, Qt5 package/contents.
[Пакет](https://github.com/Mutaracha/kobalt_ru/actions/runs/44397464051/artifacts/14472252031).
Первый CI потребовал явный int cast TextPx для Qt6 QString::arg; исправлено.
Физическая device-проверка фона/шрифта и отсутствия артефакта ожидается.

## Comics fill и аудит полосы меню

**done / CI verified / device pending**. По запросу пользователя включённое
облачко comics заливается ink (белым ночью), выключенное контурное; фон кнопки,
контур/stroke/хвост, hit area, busy guard и refresh policy прежние. Использован
shared vector painter/state cache, geometry не дублируется. Добавлена проверка
alpha центра off/on light/dark и сохранены проверки state ink/фона.
Аудит выявил presentation блоки полосы в общих reader_build/navigation; они
перенесены в существующий views/fork_navigation.cpp. Общие файлы оставляют короткие
hooks. ListHeader, chromeValues/BookTimeLeft, styles, storage и page geometry общие.
Добавлен тест formatter с book=3h/chapter=15m до show и отрицательной оценкой.
Карта владения doc/fork-delta.md актуализирована.
Runtime `23e2d023e1ed732878a21cd2148baaddc24d50e6`:
[CI 44574307848](https://github.com/Mutaracha/kobalt_ru/actions/runs/44574307848)
PASS — focused native matrix, UI icon off/on light/dark, lazyAppearance и новый
formatter test, offline checks, Kobo Qt5 package/contents.
[Пакет](https://github.com/Mutaracha/kobalt_ru/actions/runs/44574307848/artifacts/14555672823).
Device-проверка включённой иконки в обеих темах и полосы после переноса ожидается.

## Подготовка перехода на upstream beta.9

**in progress / source preflight only**. По запросу пользователя начат аудит без
merge/переноса runtime. Точные SHA, полный diff и rename-aware static port map:
[doc/reports/beta9-preflight.md](doc/reports/beta9-preflight.md),
[таблица 193 fork paths](doc/reports/beta9-port-map.tsv).
Upstream beta.9 `be47258850ffdc3295257475afdf8b8f6cb53176`: 189 commits,
676 rename-aware entries. Static merge-file: 29 текстовых конфликтов; clean не
означает semantic/API compatibility. Высокие риски: Blaze/Hybrid, whole-book vs
local pages, FB2 EPUB-only boundaries, nullable lazy Settings, comics/sleep/Kobo
lifecycle и новые RU строки. Runtime остался 23e2d023, device-приёмка последней
иконки/переноса полосы не заявляется. Следующий шаг — детальный аудит новых
Engine/ReaderSource contracts и выбор безопасного FB2 path, затем отдельный этап
переноса. Native/Qt5/device beta.9 с нашими правками ещё не запускались.

### Beta.9: разбор контрактов Blaze завершён (перенос не начат)

**source audit done / integration pending**. Детали и gates G1–G9:
[doc/reports/beta9-engine-contracts.md](doc/reports/beta9-engine-contracts.md).
Начальная техническая граница: сохранить EPUB Hybrid upstream; FB2 — прямой
single-document whole-book path через новый adapter без искусственного EPUB/spine
и без глобального отключения Hybrid. Одного open guard недостаточно: смена
appearance вызывает setChapterMode, который для FB2 должен корректно применить
patch/size без slicing, а не вернуть false/ложный success. Preview API теперь
Engine&, FB2 dispatch нужен до DocFragment. Уточнение preflight: полоса уже
получает Values; переносить надо новый chromeValues целиком, не старый локальный
page/pages fallback. G1–G9 все pending. Следующий шаг — реализация новой базы и
адаптация publication/FB2 границ, затем compile/native/Qt5 проверки; docs-аудит
не является runtime/device-приёмкой. Последний runtime форка не менялся.

### Beta.9 этап 1 — подключение библиотеки Blaze

**done / focused CI verified / device not requested**. Реализация начата отдельно
от Hybrid/session/UI: vendored library crengine/extensions/data/CMake/COPYING
скопирована byte-for-byte из upstream be472588; adapter остаётся текущим
whole-book в src/engine/crengine. Это промежуточный compatibility gate, **не
завершённая beta.9 и не пакет для приёмки**. Подключена upstream local-engine
сборка для desktop/Kobo/Android; source globs исключают vendor library из app
objects; внешняя crengine download/patch сборка больше не вызывается. Старые
patch files пока оставлены как неиспользуемые исторические входы до полного
переноса. Hyphenation/CSS берутся из Blaze data, fork FB2 stylesheet сохранён.
Parser cache identity обновлён 11→13, rendering policy — upstream version12;
проверка cache identity обновлена, старые parsed layouts не принимаются за новые.
ACK/provenance и source-archive external-library probe актуализированы.
Native focused + Qt5 CI PASS; следующий этап — новый adapter/ReaderSession Hybrid.

Первый CI этапа 1 `44593194056`: Qt5 compile/package/contents и offline checks PASS;
native compile PASS, матрица остановилась на engine.page-layout: kept-block
chapter boundary дал пустую страницу. В Blaze drop-empty-pages — explicit adapter
policy; добавлен upstream setKoblazeDropEmptyPages(true) перед Render без правки
библиотеки/теста. Повторный прогон:
`866f5b05feb647a3a0c6e492ada774a665efb9c5`,
[CI 44594490713](https://github.com/Mutaracha/kobalt_ru/actions/runs/44594490713)
PASS — focused native matrix, reported FB2 fixture, offline checks, Qt5 build/package/contents.
Это не upstream beta.9 full-suite/Hybrid validation и не device-приёмка.
Подробности: [doc/reports/beta9-stage1-blaze.md](doc/reports/beta9-stage1-blaze.md).

### Beta.9 этап 2 — подготовка adapter/API boundaries

**done / focused CI verified / no device handoff**. Текущий whole-book adapter и
fork FB2/comics модули перемещены в src/engine/blaze/adapter; worker/activity/links/
word_counts — в engine root по структуре beta.9. Для всех перемещённых файлов
проверено отсутствие изменений тела вне #include. Includes, CMake configure,
source globs и perf corpus identities обновлены; vendor library по-прежнему не
компилируется как app objects. Новый upstream adapter/Hybrid/ReaderSession ещё не
заменяет текущую реализацию — это подготовка следующей интеграции.
Полоса получила nullable-panel predicate/stack anchor: не создаёт Settings или
Appearance ради проверки и не разыменовывает отсутствующую панель. Добавлены
проверки первого состояния с null Settings/Appearance и скрывающих условий.
По просьбе пользователя подготовлен draft device checklist:
[doc/validation/beta9-device.md](doc/validation/beta9-device.md). Выдавать его вместе
с конкретным финальным тестовым пакетом и заполненным CI/runtime, а не просить
проверять промежуточные этапы 1/2.
Runtime `5bab5b4d0dc64150a9845ba06ae5c785cf0c1d95`,
[CI 44597171704](https://github.com/Mutaracha/kobalt_ru/actions/runs/44597171704)
PASS: offline, focused native matrix с reported FB2 fixture/null-panel predicate,
Qt5 build/package/contents. Полный upstream suite/Hybrid/sanitizers/device не запускались.
Следующий шаг — новый upstream Engine API/adapter/ReaderSession, не ещё один этап
перемещения файлов. Реализация/CI evidence: doc/reports/beta9-stage2-adapter.md.

### Beta.9 — worker, shared contracts и принятые сохранения

**done / focused CI verified / no device handoff** (только указанная граница).
Общие types/query/font defaults,
worker и EngineDrain перенесены из beta.9; concrete whole-book adapter использует
общие типы. Ошибка CMake area labels исправлена в `6d491a6a`; CI `44603115596`
PASS: worker/contracts и прежний focused matrix, Qt5 build/package/contents.

Подключена upstream drain-граница к AnnotationDraft::prepareSave и цепочке
HighlightSession::save (range → overlap → chapter lookup → persist). AnnotationDraft
взят целиком из upstream; в HighlightSession перенесён только связанный drain-блок,
поскольку остальной API зависит от ещё не перенесённого Engine/ReaderSession.
Новые проверки: release на Ready/Rejected/Stale/dead owner и запрет hardware suspend
до завершения принятого сохранения. Они добавлены в focused CI вместе с существующим
highlight-session suite. Это не исправление старого зависания после frame load.
Следующий шаг после CI — фактический Engine/adapter/ReaderSession Hybrid с whole-book
FB2 по контрактам; G1–G9/device остаются pending.

Runtime с подключёнными drains `d4389deccd4f9fba8004d305e322b29d44307b04`,
[CI 44604383056](https://github.com/Mutaracha/kobalt_ru/actions/runs/44604383056)
PASS: focused native (включая annotation-draft/highlight-session/native-lifecycle,
worker/contracts, reported FB2), offline и Qt5 build/package/contents.
Qt6 skipped; Android, полный upstream suite, sanitizers и устройство не проверялись.

### Beta.9 — source locations в новом позиционном контракте

**done / focused CI verified / no device handoff** (позиционная часть).
В concrete adapter подключены
upstream Location/Restoration и XPointer ↔ source-XHTML mapping целиком из beta.9,
включая обход layout wrappers и исключение generated content. Это реализация
позиционной части будущего Engine API, не перенос Engine/ReaderSession целиком.
Кэш spine names сбрасывается при configure нового документа. На FB2 shared XHTML
location не создаётся: legacy XPointer остаётся прежним; искусственный EPUB не строится.
Сохранённые позиции/профиль/схемы не переписываются, reader пока использует прежний API.

В focused CI добавлен upstream location-round-trip corpus test с четырьмя
существующими EPUB fixtures. Дополнительные проверки: no-document/future-version/
unknown-href/invalid-path, неизменность reader state, round-trip после font/viewport
reflow и EPUB → FB2 → EPUB reuse без утечки spine cache.
Runtime `b0b9c916e3369b6a0d85dc1b3e35eb74c70c22ac`,
[CI 44607352261](https://github.com/Mutaracha/kobalt_ru/actions/runs/44607352261)
PASS: focused native с location corpus/boundaries, reported FB2, сохранениями и
прежними regressions; offline и Qt5 build/package/contents.
Qt6 skipped; full upstream suite/Android/sanitizers/device не запускались.
Новый adapter Hybrid и ReaderSession остаются следующим незавершённым участком.

### Beta.9 — реальный Engine API и Blaze adapter

**in progress / CI pending / no device handoff**. Перенесены upstream Engine
interface/factory, новый Blaze adapter (windows/slices/estimates/background full
layout/adoption/search/preview/text samples), PageMap и позиционный host codec.
Fork FB2/comics/style модули сохранены, пересечения сведены контекстно, библиотека
не менялась. В shared PageDrawBuffer перенесён beta.9 formatting tracker без второй
копии drawing primitive. Зависимости font inputs и interrupted-operation diagnostics
обновлены вместе с их потребителями. Текущий ReaderSession ещё concrete/whole-book;
автоматическая пользовательская Hybrid интеграция и новый UI остаются pending.

FB2 raw-stream dispatch остаётся до CSS/configure и EPUB-only проверок. На каждом
open сбрасываются window/slices; setChapterMode реально применяет patch/viewport/
anchor через whole-book replacement, не отвечает ложным no-op success. Preview
через Engine& dispatch-ится в semantic FB2 path до DocFragment/container lookup.
Comics geometry/hits/spreads остаются в fork-модулях, column grouping объединён с
upstream non-linear grouping; cache identity включает comics и layout/source state.

CI дополняется upstream engine-contract/spine-window/chapter-slices и FB2 Engine
boundary test: exact pages, отсутствие лишнего full build, mode/reflow/anchor,
preview lifetime. Native и Qt5 обязательны; G1–G9/device ещё не закрыты.

Первый CI adapter `44610120056` выявил compile dependency gaps, не runtime renderer
failure: отсутствовал ResourceCache::fresh для независимого reload/builder и Qt5
refresh-test double имел старую сигнатуру DiagnosticOperation. Подключён полный
upstream ResourceCache delta с сохранением fork image request trace/priority;
добавлены cold-state/fingerprint/revocation проверки. Mock синхронизирован с API.
Comics target-preview использует свою bounded pair loading без навигации source;
layout signature включает fork grouping/viewport. Добавлен реальный engine-only
chapter → full build → prepared preview → adoption smoke (без ReaderSession).
Повторный CI pending.

CI `44611131876`: Qt5 build/package/contents PASS; native linking выявил отсутствие
page_estimate.cpp в явном desktop core source list (Kobo собирает рекурсивно).
Файл добавлен без изменения списка/ослабления тестов. Так как новый state() уже
может отдавать derived page_estimate для whole-book EPUB, перенесён целиком
upstream persistedCheckpoint: drop display-only fields, trim read → estimate →
image views при byte limit; добавлен исходный upstream budget regression.
Sample cache исключает page/window/estimate transient state по upstream. Контракт
поля зафиксирован в doc/storage.md. Следующий CI проверяет adapter и persistence.

CI `44612383960`: Qt5 PASS; native compilation/linking PASS, CTest failure only
in new engine.fb2 boundary bookPages/current-page assertion. Другие выбранные
проверки прошли. Добавлены фактические mapped/current/source значения к ошибке;
причина пока не установлена, speculative mapping fix не вносился. Подробности
adapter boundary: doc/reports/beta9-engine-adapter.md. Device handoff нет.
