<!-- beta9 handoff: see doc/reports/beta9-delta/README.md for immutable divergence evidence. -->
# kobalt_ru — актуальное состояние

Принятые решения: [doc/fork-decisions.md](doc/fork-decisions.md).
Инструкции агентам: [AGENTS.md](AGENTS.md).

## Активная очередь — выравнивание с upstream перед следующим обновлением

**in progress: подготовка кода; cleanup и upstream presets — CI verified; пресеты device accepted.**
Baseline форка `4e3a33b9`, upstream beta.9 `be472588`.
[Разбор отказов beta.8 → beta.9 и протокол обновления](doc/reports/beta9-upgrade-retrospective.md).
Решение: [UPSTREAM-NEXT](doc/fork-decisions.md#upstream-next--граница-подготовки-следующего-обновления).

| Блок | Статус | Следующий результат / gate |
|---|---|---|
| Уборка артефактов и incidental diff | in progress | Удалены список и 26 старых engine patches: consumers отсутствуют, build использует in-tree Blaze. Cache recipe/filter оставлены byte-identical, чтобы не менять dependency key. Прочий incidental diff ещё не сводился; CI 44800640114 PASS. |
| Общие реализации beta.9 | in progress: пресеты device accepted; panel/theme device checklist accepted; freeze deferred до нового примера | Контекстно свести пресеты, chapter lookup, аннотации/keyboard, KOSync, sleep/storage. Использовать upstream lazy panels вместо собственной оптимизации; проверить shared helper/API и protected fork behavior. |
| Актуальная карта дельты | planned | Обновить fork-delta по фактическим файлам; для каждого retained delta указать причину и gate, upstream-only файлы проверить отдельно. |
| Замена полосы прогресса beta.10 | planned; после текущего выравнивания | Полностью заменить fork progress bar на upstream `4a9576ae017684d290dd2311f4f5195e5e043274`, удалить старую реализацию/parallel wiring и заменённые тесты; не поддерживать два пути. Новые строки требуют согласования. [D04](doc/fork-decisions.md#d04--прогресс-и-меню-чтения). Сейчас runtime полосы не меняется. |
| Подготовка FB2 Hybrid | in progress: source analysis | [Граница и матрица](doc/reports/fb2-hybrid-preparation.md) описаны по коду. Далее corpus, sections/notes/resources/anchors и проверяемый whole-book fallback. Реализация и включение partial layout не разрешены автоматически. |
| Следующее обновление | planned; blocked до фиксации нового upstream SHA для runtime-переноса | Использовать протокол отчёта; сначала завершить cleanup и составить dependency/consumer/test matrix. Beta.10 ещё не анализировалась. |

**Граница:** сейчас не меняем алгоритмы EPUB Hybrid (slicing, background full
layout, adoption, fallback) базовой версии и не пишем собственные оптимизации.
Восстановление upstream consumers/UI не является разрешением менять эти алгоритмы.
Существующие narrow hooks, включая comics frameImage, сохраняются по назначению.
FB2 whole-book остаётся безопасным поведением до отдельного проверенного решения.

**Текущий проверенный runtime:** `34811f2f42b64c73a557fa532a453c34851953db`,
Qt5 build **266**, [CI 45048708215](https://github.com/Mutaracha/kobalt_ru/actions/runs/45048708215)
**SUCCESS**: native 9m06s; Qt5/offline/package/contents 7m37s.
[Qt5 package](https://github.com/Mutaracha/kobalt_ru/actions/runs/45048708215/artifacts/14764857536)
(37408180 bytes). [Combined checklist](doc/validation/beta9-kosync-sleep.md).
Device/Android/Qt6/full upstream suite NOT RUN. Theme/lazy Settings and presets
accepted ранее; freeze deferred. Это не свидетельство complete-repository parity.

**Annotation interaction/drag — done / CI verified, device pending:** runtime
`c81928517f13859cb5baaf7f1927da5b41e19fae`, CI `45043939833` SUCCESS
(native + Qt5/offline/package/contents), artifact `14762084755` (37382310 bytes).
Quick/toggle, auto-open/reopen 1500 ms, pressed handles и Qt5 direct-partial drag /
one-shot REAGL cleanup; fork palette/headers сохранены. Устройство NOT RUN.

**KOSync + sleep/storage — done / CI verified, device pending:** по прямому
указанию пользователя KOSync whole upstream: модель, page, coordinator, Reader
handlers, network/login retries, final exchanges и соответствующие тесты/helpers.
Whole-book percentage, deferred Android API и native modal-input resume подключены.
Три RU validation messages утверждены. Sleep/USB lifecycle и asset verification
upstream; `.cache-blaze`, optional-assets handling и boot/remount gates перенесены.
Сохранены fork ribbon/newline/text fitting, note migration и lazy-Appearance report
flag; нет второго save pipeline. Upstream cleanPreferences восстановлен целиком
с note-migration hook, EPUB algorithms не менялись. CI 45048708215 SUCCESS.

**Wi-Fi idle / sync-exclusion — implementation / CI PASS, device pending:**
полные pinned-upstream Qt5 correction, verified callback adapter и vendor detours
NickelFix d735967 с MIT notice; unknown firmware и установленный NickelFix
сохраняют штатное поведение/владение. Upstream diagnostics build option default
OFF, не включается в обычном пакете. Никаких обещаний экономии батареи или
исправления sleep drain. Sync exclusion применяет/откатывает upstream receipt,
сохраняет пользовательский regex и правки после установки; Qt5/Qt6 wiring.
Upstream tests/helper/build targets перенесены вместе; offline/source gates PASS,
native/Qt5 CI45050356339 PASS, runtime `2c5d2c55`, build268. Device pending.
[Handoff](doc/reports/beta9-wireless.md). Полоса прогресса/EPUB Hybrid unchanged.

**Итоговый semantic inventory — in progress, не закрыт:**
[актуальная карта](doc/reports/beta9-inventory.md) и TSV охватывают все nonidentical
production/build/assets/tests paths. Остаются не только UI отличия: независимые
platform/Reader consumers (Wi-Fi-idle patches, sync-exclusion, quiet preload,
status/debug/percentage/chapter-navigation и platform-specific helpers/tests).
Их нельзя объявить косметикой или закрыть фактом успешного CI этой группы.
Следующее действие — disposition/целостный перенос оставшихся consumer groups,
без новых fork features; EPUB Hybrid и freeze остаются исключениями.

**Редактор заметок — implemented / CI verified, device pending:** upstream NoteEditor h/cpp целиком,
Reader saved/draft actions и save completion/error callbacks, note popover;
upstream card grid для дополнительных действий с сохранением fork section headers
и подписей palette. Quick annotation/drag reopen timer — отдельная следующая группа,
не включена скрытно. Helper handle exclusions перенесён вместе с dot/hit regions,
чтобы области handles не перекрывали кнопки нового popover. EPUB Hybrid unchanged.
Пять новых RU строк приняты пользователем через выбор; EN из pinned upstream.
Для отмены изменений используется существующий common.cancel, не новая RU copy.
Тесты: upstream passage lines, новый save/error/retry/close state, note integration
на базе upstream (без блока ещё не перенесённых drag/reopen gestures), прежние
annotation/selection/grip и palette label проверки включены в focused CI.
Runtime `09307e421d649527cdcaa6854255e335f9861409`, build **257**,
[CI 45037956822](https://github.com/Mutaracha/kobalt_ru/actions/runs/45037956822) **SUCCESS**: native 11m00s;
Kobo Qt5/offline/package/contents 4m51s.
[Пакет](https://github.com/Mutaracha/kobalt_ru/actions/runs/45037956822/artifacts/14760328232)
(37377574 bytes). Qt6 skipped, Android/full suite/device NOT RUN.
[Чек-лист NoteEditor](doc/validation/beta9-note-editor.md).
Следующий upstream consumer group: quick annotation / drag reopen и их input/
refresh dependencies, затем оставшиеся KOSync policy/sleep/storage и semantic gate.
До feedback устройства не считать нативную multiline keyboard проверенной.


**Пакет 254 — implemented / CI verified, device pending:** upstream shared ListRow и
SegmentedRow, HighlightStore/HighlightSession, BookmarkList восстановлены целиком.
Подключены upstream annotationRows/bookmarkRows и hydration callback — note preview,
whole-book page metadata и pending row navigation используют общую цепочку.
Восстановлены note model assertions; новый annotationNoteRows проверяет note
hydration, page reuse/очистку, geometry и pending selection light/dark × density.
Whole upstream annotation_database.cpp также восстановлен: compact note flag,
excerpt note/валидация. Schema unchanged. Focused CI теперь запускает также
choiceSheet/modal dividers/list regressions, annotation-database и highlight-store. Это НЕ полная интеграция NoteEditor/quick gestures: Reader editor
ownership и действия редактирования заметок остаются следующим consumer group.
EPUB Hybrid, freeze deferred и Libra2 correction не менялись.
Runtime `4e014dc57c04ec35a5d3dd3e0cba7f73fa91b698`,
[CI 45033749297](https://github.com/Mutaracha/kobalt_ru/actions/runs/45033749297) **SUCCESS**:
native 9m55s, Kobo Qt5/offline/package/contents 5m01s.
[Пакет 254](https://github.com/Mutaracha/kobalt_ru/actions/runs/45033749297/artifacts/14758934067)
(37359704 bytes). Qt6 skipped, Android/full suite/device NOT RUN.
[Чек-лист устройства](doc/validation/beta9-annotation-foundation.md).
Следующее действие после device feedback: NoteEditor + Reader actions/ownership/
input/save/close consumers по матрице; не считать весь annotation port завершённым.


**Последнее указание — implemented / CI verified, device pending:** принята RU строка
«Только зазоры от висячих строк»; полоса страниц/времени до конца книги полностью
удалена (создание, layout, visibility, updater, state/API); проверка lazyAppearance
теперь требует отсутствия виджетов. Session minutes/clock/battery сохранены.
Предыдущие требования полосы superseded, новую реализацию пока не делать.
[Аудит физических кнопок](doc/reports/beta9-physical-buttons.md): upstream main
по API всё ещё be472588; Qt5 Libra2 correction НЕ поглощён этим кодом, пока оставлен.
[Полный path inventory](doc/reports/beta9-inventory.md) — есть, semantic gate открыт.
CI 45025613466: Qt5 PASS, native FAIL — UI tests были выбраны, но executable
не был включён в focused build target list. Target добавлен; это не доказательство
ошибки клавиатуры и не успешный native run. Follow-up **CI 45027682492 PASS**
для runtime `cba1e229`: native 9m52s + Qt5 4m56s; Qt6 skipped, Android/device
NOT RUN. [Qt5 пакет](https://github.com/Mutaracha/kobalt_ru/actions/runs/45027682492/artifacts/14756567766). Следующий dependency group annotations:
[dependency/consumer matrix](doc/reports/beta9-annotation-port-matrix.md) готова.
ReaderSession location API уже есть; highlight geometry/paint byte-identical.
Далее shared ListRow + storage/session note metadata с upstream tests, затем
BookmarkList/NoteEditor/Reader consumers. Fork labeled highlight palette сохранять.

**Следующее действие:** разобрать замечания upload `68c77d8`:
[отчёт](doc/reports/77564e9-device-issues.md). EPUB Hybrid подтверждён логом
(частичный старт → full-document-ready → adoption); алгоритмы не меняются.
Fix версии manifest beta.9 и отображение EN/RU имён встроенных пресетов —
done / CI verified runtime `8f1fdf4d`,
[CI 44932175305](https://github.com/Mutaracha/kobalt_ru/actions/runs/44932175305)
PASS native + Qt5; device pending. Этот пакет НЕ содержит исправления freeze
или нового шрифта экрана открытия. Freeze Settings/Styling — **deferred по решению пользователя до нового примера**. Контекст:
последнее действие уточнено — закрытие формы выравнивания по высоте после
изменения параметров; касания и пользовательский сон перестали работать,
потребовалось долгое нажатие питания. Нет stack trace/адресата касаний;
Новых попыток исправления до нового примера не предпринимать.
Event loop в части лога оставался активен, speculative
standby/renderer fixes запрещены. Opening RU typography — planned: подобрать
размер/начертание по образцу EN, не менять глобальный brand/font fallback.
**Текущий dependency group — keyboard native/Qt5 CI verified:** общий keyboard API
QWidget (QLineEdit/QTextEdit), widget/native Qt5/Qt6 keyboards и upstream modal /
ChoiceSheet paging. Whole-file upstream implementations, согласованные factories;
новый multiline regression, расширенные choice paging/vertical form teardown
проверки. В focused native запуск входят widgetKeyboard*, lazyAppearance и
verticalJustification; choiceSheetListFillsCard/modalListDividers в этой выборке
не запускались, хотя расширение теста и исходники компилируются. Freeze deferred,
не объявлен исправленным. Затем annotations, остальные
KOSync policy/sleep и семантический inventory. Подробности —
[panel alignment](doc/reports/upstream-panels-alignment.md).

**Validation:** runtime `9633d3fd4b2df250c97ac77d1ff5e0827db35465`,
[CI 44801198903](https://github.com/Mutaracha/kobalt_ru/actions/runs/44801198903)
**PASS**: focused native EPUB/FB2, presets gallery/cache через settings/model
selection; Kobo Qt5/offline/package/contents. Qt6 skipped, full upstream suite,
Android и устройство NOT RUN. Локально нет CMake/Qt SDK/Docker; source checks
и publication diff --check PASS. Raw native artifact download из Arena недоступен
(redirect EOF); CI job success подтверждён GitHub API, сырые результаты не изучены.
[Пакет](https://github.com/Mutaracha/kobalt_ru/actions/runs/44801198903/artifacts/14661747164),
[короткий device checklist](doc/validation/upstream-presets-device.md).
Это checkpoint только presets/build cleanup, не завершение всей очереди.
Исторические этапы ниже — evidence, их промежуточные pending не переоткрывают задачи.
Старое зависание после frame load не объявлено исправленным.

## Последние результаты устройства и выполненных этапов

**Device follow-up cfb8f51c / upload 487416ff:** получены 2 снимка Activity
и лог четырёх открытий. [Разбор](doc/reports/cfb8f51c-device-followup.md).
Подтверждён Hybrid chapter request → `page-note-layout` fallback → whole-book
при включённых нижних сносках. Cold до reader-shown: Hybrid 16,538 с, Accurate
18,302 с (разные disk-cache состояния); repeat 5,021 / 4,996 с с retained model.
Снимки подтверждают RU даты/заголовок серии, сохранность показанных исторических
итогов и согласованность видимых чисел с округлением. Полная база не предоставлена.
Повтор того же прогона не нужен; новых runtime правок/сборки нет. Защита сносок
и закрытая задача оптимизации открытия не изменены. Это не полный device PASS.

**Device review 033d0be2 / upload e82e1e1b:** пользователь кратко проверил
FB2/EPUB/comics, прислал 51 снимок и три ZIP диагностики. [Разбор](doc/reports/033d0be2-device-review.md):
warm Accurate/Hybrid ~4,8 с с retained model; cold ~16,28 с, причина whole-book
в старом логе не наблюдаема. Оптимизация открытия не возобновлена. Исправлены
пустой заголовок hidden Navigate и принудительные EN даты/word totals в RU;
добавлены диагностические mode/fallback события без изменения алгоритма.
Технический CI `44775124583` PASS. Пользователь одобрил весь перевод/склонения
из review: три RU уточнения и шесть EN/RU unit keys внесены. Runtime
`cb9afca9`, CI `44778323964`: Qt5 PASS; native FAIL только
activityUnitForms (empty history скрывала Overview). Исправлен fixture реальным
reading day и проверкой видимости без ослабления сравнений. Повторный runtime `cfb8f51c`, CI `44781746091` **PASS**: native
EPUB/FB2 (включая activityUnitForms), Kobo Qt5/offline/package/contents.
[Короткий повторный прогон](doc/validation/033d0be2-followup.md).
Локальные source/parity/Ninja string self-tests ранее PASS.
[Новый пакет Qt5](https://github.com/Mutaracha/kobalt_ru/actions/runs/44781746091/artifacts/14652058784).
Числа Activity согласуются с округлением/порогами; база профиля не предоставлена,
точность исторических итогов не объявлена подтверждённой.

**Готово к device-тесту Kobo Qt5:** runtime `033d0be278f2f7afecd5a9a1825d07ffb5a8e13a`,
[CI 44731173763](https://github.com/Mutaracha/kobalt_ru/actions/runs/44731173763) **PASS**: focused native EPUB/FB2,
Activity/Stats, sleep/Custom/editor/privacy, profile/writer/preview reconciliation;
Kobo Qt5/offline/package/contents. [Пакет](https://github.com/Mutaracha/kobalt_ru/actions/runs/44731173763/artifacts/14626623140).
[Чек-лист устройства](doc/validation/beta9-device.md). Это не physical acceptance,
не полный upstream/all-variant suite и не обещание исправления старого freeze.

Sleep custom/positions и startup preview migration подключены. Первый sleep CI
нашёл anonymous checklistOption; helper опубликован в общем Settings API как
upstream. Activity smoke ждёт загруженный Overview и исчезновение welcome;
Stats test проверяет перенесённые общие word labels. Финальный CI выше проверил
исправления. Сохранены fork fit и двухстрочная sleep-лента, strip/comics/font
policy. G1–G9: focused автоматическое покрытие PASS; визуальная и аппаратная
части остаются NOT RUN до устройства. Дальше — пользовательский прогон и снимки
для длины/переносов утверждённых RU строк; новых переводов не вводилось.

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

**done / focused CI verified — wake/save recovery:** sleep теперь сохраняет progress
перед drain native time, а поздний result обрабатывается после wake. Подключены
upstream SleepSave/epoch callback/clear pending Exit и повторная попытка без закрытия
книги. Согласованные EN/RU строки внесены; оценка переносов по будущим скриншотам
пользователя. Upstream close/sleep order, stale pre-sleep completion и wake retry
regressions перенесены вместе с fixture и включены в focused CI (старые обратные
order tests заменены, не ослаблены). Source checks и strings parity/generation
PASS. CI `44712598821`: Qt5/package PASS; native compile нашёл недостающий
QSemaphore include у перенесённого regression. Добавлен явный include; runtime
проверка новых wake regressions PASS в CI `44713551666` (`17ae835a`),
как и Qt5/offline/package. Это не проверка на устройстве.

**done / focused CI verified — host rendering selector/application:** подключены approved selector
Hybrid/Accurate и debug-only Fast (по существующему debug_mode preference, без
нового debug switch), load/save/reset/caption и requestAppearance с mode override.
Host standalone/Kobo применяют preference до opening. Строки ранее согласованы;
строка расположена в существующем разделе Opening, без нового заголовка/строк.
Используется общий styleListAction; визуальная проверка на устройстве pending.
CI `44719630736` (`cedd10e9`): native и Kobo Qt5/offline/package PASS.
Новый routed regression settingsRendering добавлен следующим navigation slice:
проверяет caption/ListAction, save/reopen, position retention и отсутствие Fast.

**done / focused CI verified — Hybrid navigation handoff:** PageScrubber/SettingSlider перенесены
целиком из pinned upstream (без fork policy). Reader использует chapter bar,
preview adjacent source pages/bookmarks и goToChapter/turn, затем сохраняет
Navigate при adoption whole-book layout. Fork chapter ticks используют bookPages,
progress strip — session progressFraction. ReadingPace получает upstream source
position observation, чтобы локальная нумерация главы не теряла forward turns.
Добавлен upstream component regression range handoff; component.ui уже включён
в focused CI. Source checks и strings self-tests (Ninja) PASS; CI `44720881637` (`7014d57e`) native и Qt5/package PASS, включая
settingsRendering и component.ui handoff.
Whole-book chrome/counters/graphs и history/activity consumers ещё pending.

**done / focused CI verified — bookmark local/display coordinates:** BookmarkSession перенесён
целиком из pinned upstream: separate local marking/merging и whole-book display,
slice/layout revision validation и сохранение mark при prepared-neighbour update.
Reader bookmark rows используют bookPage для подписи и local page для current;
sleep progress/preview используют whole-book progressPercent. Добавлен upstream
prepared-bookmark regression; session.bookmark-session включён в focused CI.
CI `44722105686`: Qt5/package PASS; native compile выявил недостающий
KOBALT_TEST_ZIP у нового fixture. Условие compile definitions перенесено из
upstream (`8040a9f9`); повторный CI `44723018339` PASS (native и Qt5/package, включая prepared bookmark). Новых строк/настроек нет.

**done / focused CI verified — whole-book counters/chrome:** перенесены upstream Values estimate
flags, percent fallback и pageTotal formatting без debug controls/новых настроек.
Approved `~%1`/«Расчёт…» добавлены EN/RU. Chrome не подставляет chapter-local
номера вместо whole-book, fork menu strip использует те же флаги/формат total;
ToC и Stats получают whole-book pages/fraction. Upstream estimated/no-pages
header/footer regressions добавлены к уже выбранному model.header-footer.
Source checks, EN/RU parity/code-reference generation и strings self-tests (Ninja)
PASS. CI `44723102693` (`b3253e6b`): native и Qt5/offline/package PASS,
включая no-pages/estimated header-footer и strip assertions. Activity/history/sleep positions
ещё не закончены, package пока не готов для device handoff.

**done / focused CI verified — daily history/opening accounting:** ReadingHistory и ReadingBook
теперь exact pinned upstream (включая per-day/per-hour records, opening ID,
new-book preset selection и checkpoint sleep_positions field). Reader загружает
чужие opening totals в фоне и прибавляет текущий day из памяти, без удвоения
queued save и без переноса вчерашнего чтения на сегодня. Переход полуночи/DST,
sleep-gap и failed transaction/retry regressions импортированы; model.reading-history
включён в focused CI. Схема уже 4; новые записи optional format 1, schema bump нет.
Sleep positions host consumers и Activity UI пока pending; новых строк нет.
Source checks PASS; CI `44724222957` (`40d3948a`): native и Qt5/offline/package
PASS, включая новые midnight/DST/sleep/failed-day-transaction regressions.

**in progress — Activity records/model:** импортированы exact upstream
ActivityRecordStore и reading_activity summary: async read/hide/remove,
opening exclusion, streak/calendar/hour aggregation и optional hidden-books record.
UI и его строки ещё не подключены. Exact upstream model.reading-activity
regressions добавлены в focused CI. CI `44725474629` failed before acceptance: test area label missing and
RecordWriter Delivery::All dependency not yet imported. Ported exact upstream
RecordWriter/profile boundary plus preference normalization/update primitives;
added writer FIFO/launch-failure regressions and area registration. Approved
UI table inserted EN/RU. Source checks и approved EN/RU parity/generation PASS. CI `44728203966`
(`3ee4a622`): native/Qt5/offline/package PASS, включая FIFO/launch-failure
writer tests и model.reading-activity.

**done / focused CI verified — Activity/sleep/preview integration:** ActivityView,
ChapterGraph/StatsCell, reader_activity и CoverThumbnails взяты из pinned upstream.
Host связывает Stats → Activity, load/edit/map/cover, welcome, input barrier и
resize. Preference deltas/FIFO не теряют соседние изменения. Custom sleep/editor,
sleep/off wallpaper, per-book places и follow-reading wiring используют общую
реализацию. Startup удаляет только generated legacy PNG, сохраняет scan marker
и завершает его после native doneProcessing; есть exactly-once regression.
Source checks и EN/RU parity/reference generation PASS. Финальный CI и пакет
указаны в начале. Device visual review NOT RUN; это следующий этап.


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

## Полоса счётчика в меню чтения — superseded: удалена

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

## История удалённой полосы меню: закрытие и время до конца главы

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

## История удалённой полосы меню: оценка книги и курсив

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

## История удалённой полосы меню: фон, шрифт и первый показ

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
Повторный runtime `cfb8f51c`, CI `44781746091` **PASS**: native
EPUB/FB2 (включая activityUnitForms), Kobo Qt5/offline/package/contents.

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

## Вход для следующего upstream update

[Журнал остатка beta.9 → beta.10](doc/reports/beta9-delta/README.md):
439 tracked расхождений на fork `3cc7ee19`, полные OID/modes и conservative review
статусы; 14 semantic handoff групп, воспроизводимый audit script и regression tests.
Это evidence/ownership log, не вторая очередь задач. Сначала закрывать открытые
consumers выравнивания; после выхода beta.10 зафиксировать release SHA и сравнить
beta.9→beta.10 отдельно от старой fork delta. REVIEW не означает одобрение.

**Продолжение implementation alignment:**
[delta-current](doc/reports/beta9-delta-current/README.md), runtime `ec9eee6c`:
Reader lazy/search/chapter consumers, Android/standalone opening lifecycle,
diagnostics/scenario/desktop reporting, новые upstream engine tests, opaque-copy
и dead native-save helper cleanup. Полный gate OPEN, CI45053881024 pending.
[Оставшиеся тексты](doc/beta9-remaining-strings-review.md) не применены до согласования.
