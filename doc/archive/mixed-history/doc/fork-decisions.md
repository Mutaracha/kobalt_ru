> АРХИВ: исторический контекст, не текущий план и не инструкция установки.
> Старые pending/next не возобновлять. Текущий источник — FB2_FORK_PLAN.md в корне.

# kobalt_ru — принятые решения

Этот документ хранит действующие согласованные решения, а не очередь работ.
Очередь и следующие действия: [FB2_FORK_PLAN.md](../../../../FB2_FORK_PLAN.md).
Инструкции агентам: [AGENTS.md](../../../../AGENTS.md). Проверки устройства:
[beta8-device-check.md](../../pre-beta10/doc/beta8-device-check.md).

Источник решений ниже — явные указания/подтверждения пользователя в сессии.
Перенос сюда не является новым согласованием или физической приёмкой.
Прежние отчёты и этапы доступны в истории Git; при конфликте действуют
решения этого реестра, затем последующие явные указания пользователя.

## D01 — интеграция upstream beta.8

**Решение:** при полном дублировании механизма брать цельную upstream-реализацию;
при отдельных изменениях — контекстно совмещать согласованные отличия. Standby
и сноски заменены механизмами beta.8, а не параллельными fork-подсистемами.
Сохранить нативный FB2, RU, comics и согласованные различия UI.

**Заменяет:** старые требования сохранять fork default-on notes/standby policy.
**Проверка:** beta.8 runtime интегрирован, build153 native/Qt5 прошли; отдельные
задачи перехода закрыты пользователем. Миграция завершена.

## D02 — физические кнопки Libra 2

**Решение:** upstream beta.8 positional/held() модель, equal actions, none и
частичные назначения — из базы. Оставить только Libra 2 landscape correction;
никакого второго actionRotation. Другие устройства не менять.
Libra2 polarity P/L/IP/IL = false/true/true/false; остальные — upstream.
Повторный code audit beta9/main: [отчёт](../../pre-beta10/doc/reports/beta9-physical-buttons.md).
В проверенном upstream correction не поглощён; поведение сейчас не менялось.

**Заменяет:** старую отдельную physical-action семантику и отклонённые общие
инверсии. Portrait-поведение сохранить. Проверки: четыре ориентации,
next/previous/none, меню/списки/preview и сохранённые назначения.

## D03 — режим чтения комиксов

**Решение:** per-book, default off; aspect-fit, без crop/stretch. Базовые поля,
колонтитулы и прочие параметры остаются редактируемыми и сохраняются; включённый
режим временно игнорирует их, выключение применяет последние значения.
Apply/cancel не должны сохранять временные нули в обычную раскладку.

Portrait — одна исходная страница. Landscape — две подходящие portrait-страницы;
wide, prose, unknown и standalone center страницы не объединять с соседними.
Portrait-dimension bitmap у standalone `rendition:page-spread-center` в landscape
вращается для заполнения; уже широкий bitmap не вращается. Сохранять пропорции
и вертикальное центрирование, единое отображение для numbering/bookmarks/hits.

**Заменяет:** одну страницу в обеих ориентациях и прежний общий запрет вращения.
**UI:** shared Light-panel checkbox, большой квадрат слева, те же отступы и
границы. Quick toggle рядом с dark toggle меняет тот же режим; в открытом Layout
работает с draft. Preview должен сразу обновлять тело, header/footer и progress
в обе стороны; после нового решения D04 — только штатная нижняя полоса beta9. Пользователь подтвердил полный
preview после build162; затем отдельно принял окончательную иконку.

## D04 — прогресс и меню чтения

**Последнее решение пользователя заменяет временное удержание fork progress:**
полностью убрать нашу модификацию сейчас: chapter ticks, настройку положения и
вариант сверху. Возвращается pinned beta9 stock bottom bar с его штатным on/off.
Удаляются отдельный renderer, fraction helper, model/JSON/property/strings wiring
и tests старого Top/Bottom варианта. Comics по-прежнему временно скрывает chrome;
не меняет настройки и не возвращает их с другими значениями.

Beta10 commit `4a9576ae017684d290dd2311f4f5195e5e043274` **не импортируется** в
этом изменении. Прежний план ждать beta10 перед удалением больше не действует.
Дальнейшая смена upstream progress входит только в отдельно согласованное обновление.

**Отдельная граница меню:** прежняя page-count/time-to-book-end полоса отменена
позднейшим указанием пользователя и удалена; вариант beta.10 не должен возвращать
её вместе с полосой страницы. Session minutes, clock и battery не относятся к
этому удалению. Старое требование отдельного счётчика «22 из 997» здесь больше
не действует.

## D05 — сноски EPUB/FB2

**Решение:** canonical `inpage_footnotes`, default false. Legacy Boolean
`fb2_inpage_footnotes` мигрируется, canonical Boolean имеет приоритет.
EPUB использует базовый beta.8 note pipeline; FB2 добавляет форматные вход/preview
и notes/comments selectors, а размещение выполняет общий CREngine.
Числовой FB2 title примечания должен идти в одной строке с его текстом
(нативный run-in, не отдельный block/inline перед абзацем).
Номер перед текстом нижней сноски — надстрочный; не менять ссылки основного текста
или произвольные числовые вхождения ради оформления номера.

**Заменяет:** старые отдельные EPUB/FB2 CSS-policy и default-on договорённости.
Эксперимент adjacent-footnotes из build157 отменён; позиционирование снова базовое.
**Приёмка:** пользователь завершил работу со сносками: индексы работают верно,
текущие отступы приняты без дальнейшей доработки. Это решение заменяет открытое
пожелание body-line gap. Задачи зазоров и дальнейшего исследования пагинации закрыты,
но отдельное исправление общего алгоритма пагинации не заявляется.
Не делать draw-only сдвиг, расходящийся с pagination/selection/hit testing;
не менять размер текста сносок и нижнее поле ради зазора без нового запроса.

## D06 — оформление, строки и локализация

**Решение:** общий baseline/action/row style, IBM Plex Sans UI / Instrument Serif
Kobalt; stock About не менять, fork UTC-row сохранить.
**Уточнение пользователя:** вход в существующее окно языка — только первый пункт
вкладки «Отладка», название «Язык интерфейса», общий стиль Settings action row.
Код UI находится в settings/debug_language_dialog, не в About. Прежний скрытый жест на строке «О программе» отменён.
Выбор English/Русский/Automatic, markers, async write и применение после
перезапуска не меняются.
Новые EN/RU тексты сначала согласовывать, длины RU учитывать в общей геометрии.
Locale Kobo пересчитывать на согласованной границе открытия книги, не делать
частичную live retranslation; explicit debug markers имеют приоритет над env.
C/POSIX не считать принудительным выбором языка. Русский — ExtraLocales на Kobo.

Согласованные формулировки (использовать существующие resource keys):
- «Капсула» вместо «Плашка»; не менять текст письма без отдельного согласования.
- «Режим чтения комиксов» / “Comics reading mode”; описание RU:
  «Убирает поля и колонтитулы, игнорируя текущие настройки раскладки.»;
  EN: “Hides margins, headers and footers, ignoring the current layout settings.”
- «Сноски внизу страницы», «Книга защищена» без точки, «ридер» не «читалка».
- «Резервные шрифты» без нового длинного пояснения; действующие family keys
  в группах/chooser, без фильтрации каталога по обещанной классификации.
- «Переключатель тёмной темы»; «Скрывает переключатель тёмной темы в меню чтения.
  Текущая тема сохраняется.» Скрывается кнопка, не сбрасывается тема.
- «синхронизация», «Стиль меню» без default, нижние вкладки не переименовывать;
  «Замок»; interval0 «Постоянно»/«Всегда» по текущим ключам;
  «Прогресс на устройствах актуален»;
  image «Просмотреть»/«Закрыть»/«Вписать». Standby warning — без изменений.

Sleep-дизайны приняты; сохранять shared ribbon newline/description.
Comics иконка — speech bubble: сплющенный овал и удлинённый загнутый
хвостик. Окончательная форма принята пользователем (`68b3ac16`).
canvas, общая толщина линии и hit target сохраняются. По новому запросу пользователя
(2026-10-26) выключенная иконка — серый контур, включённая — чёрное заполненное
облачко; ночью — соответствующие инверсные цвета (включённая белая). Это заменяет
прежнее правило «контур в обоих состояниях». Фон самой кнопки не заливается и не
инвертируется. Runtime 23e2d023 / CI 44574307848 native+Qt5 PASS; device pending. Занятость reflow не должна менять цвет иконки; скрытая функция
скрывает только кнопку, не режим и не настройку Layout.
Согласованы EN “Comics mode toggle” / “Hides the comics mode toggle in the reading
menu. The current reading mode is preserved.”; RU «Переключатель режима комиксов» /
«Скрывает переключатель режима комиксов в меню чтения. Текущий режим чтения сохраняется.»

## D07 — waveform, открытие и отложенное

**Решение:** сохранять успешную waveform policy; пользователь подтвердил отсутствие
ghosting. Не добавлять безусловные полные flashes или менять waveform ради UI.
Общая оптимизация открытия остановлена пользователем; не возобновлять без запроса.
Lazy Appearance выполнена; lazy Settings не объявлять выполненной или автоматически
продолжать. Standby protection — correctness, не доказательство ускорения/лечения
неустановленного зависания. Закрытие книги в device-сценариях означает выход в
систему; пользовательские A→B measurements могут идти в одном логе.

Master light-gesture switch соблюдать; completion feedback не путать с pause.
Quiet-frontlight fork удалён, quiet-gesture diagnostics отменены; не восстанавливать.
Очистка пользовательского cache закрыта без реализации, не возобновлять автоматически.
`.fb2.zip` не входит в согласованный scope.

## D08 — ownership материалов и приёмки

**Решение:** raw uploads/диагностика — `test & diagnostic/`, не `doc/`.
Инструкции и воспроизводимые контракты — тематические guides; accepted decisions
— этот реестр; задачи/статусы — план. AGENTS только English agent/build/environment
instructions. Сохранять bytes/provenance; CI, screenshots и подтверждение
пользователя — разные виды evidence. Приёмка одного аспекта не закрывает весь UI.

## D09 — завершение beta.8 и очистка материалов

**Источник:** пользователь принял иконку и индексы сносок, оставил текущие отступы,
завершил задачи beta.8 и закрыл все отложенные работы, кроме старого зависания
после frame load. Закрытые нереализованные предложения не являются обещаниями
будущего релиза. Единственный открытый пункт и необходимые данные — в плане.

**Очистка:** старые отчёты и интерфейсные скриншоты удалены из текущего дерева
(включая старые manual/device review-серии); при необходимости доступны в Git
history. Диагностические архивы и исходные книги сохранены без перепаковки.
Это явно заменяет прежнее требование держать все принятые скриншоты в checkout.


## D10 — приёмка исправлений FB2 после рефакторинга

**Источник:** после проверки сборки №181 пользователь сообщил: «проверил. можно
считать все поставленные по fb2 проблемы решенными».

**Приёмка:** все поставленные проблемы FB2 закрыты: наложение строк,
сохранение фрагмента предпросмотра при первой правке, ранее подтверждённые
Nickel progress и Smart chapter refresh. Финальный принятый runtime
`c0ff0115eae5557b4f093176b1ca9c100d4416d4`; CI №181 / 44154435468:
focused native EPUB/FB2 и Qt5 build/package/contents PASS, затем device-приёмка
пользователем. Это не all-variant проверка и не обещание поддержки `.fb2.zip`.
Старое зависание после frame load этим решением не закрыто. Закрытые FB2-задачи
не возобновлять без нового сообщения о проблеме или запроса пользователя.


## Reading-menu page counter strip

**Superseded by the latest explicit user instruction:** remove the complete page /
book-time-left strip from the reading menu. No hidden strip, labels or updater;
no replacement in the toolbar or another tab. A new implementation is deferred
until separately designed. Prior strip placement/formatting/dismissal requirements
no longer apply. Keep the existing centered session-minutes label, clock, battery
and book header/footer formatting; those are not the removed strip.

## Vertical spacing scope wording

User approved «Только зазоры от висячих строк» for
`settings.vertical_spacing_scope_protection`. Description and EN unchanged.

## B9-STRINGS — rendering and reading-time recovery translations

Accepted by the user in this session after the EN/RU table review. EN remains
pinned upstream beta9. RU uses «время последнего сеанса чтения», not «до сна»;
retry is «Сохранить снова». Rendering labels are «Движок рендеринга», «Гибридный»,
«Быстрее», «Точнее». Descriptions end with «Быстро», «Быстрее всего», «Медленнее»;
the accurate description is «Сразу рассчитывает всю книгу при её открытии.
Медленнее.». Exact accepted strings live in assets/ru.strings. The user will supply
interface screenshots to finalize wrapping/wording; visual acceptance is pending.
This approval covers upstream controls, not additional fork settings.

## B9-COUNTER-STRINGS — Hybrid page estimates

User approved before implementation: pinned EN `common.estimated_total` “~%1”
→ RU «~%1» and `header_footer.calculating` “Calculating…” → RU «Расчёт…».
These are count-state labels, not new settings. Estimated totals remain marked
until whole-book layout supplies counted pages. Device wrapping pending.

## B9-ACTIVITY-SLEEP-STRINGS — approved UI table

The user approved every EN/RU row in doc/beta9-strings-review.md before insertion
into assets. This covers Activity, custom sleep/position controls and chapter/book
word-count captions. Preserve the existing Letter wording and accepted sleep
design names. Final wrapping/length review remains by device screenshots.

## B9-DEVICE-REVIEW-STRINGS — approved screenshot refinements

After reviewing upload e82e1e1b (runtime 033d0be2), the user approved the complete
`device-review-translation` proposal. This supersedes the earlier RU rendering
labels only where specified: settings.rendering is «Режим рендеринга» and
settings.rendering_accurate is «Точный». activity.longest is «Самая длинная»
(the streak heading, not the session heading). EN labels and speed descriptions
are unchanged. Six approved EN/RU Activity unit keys supply book/books and
hour/hours with RU книга/книги/книг and час/часа/часов; use existing CLDR rules.
This changes presentation only, not recorded time, thresholds or totals.
Dates and grouped word counts follow the selected UI language without changing
Nickel's global process locale. Exact strings and the approved table are in
assets/*.strings and doc/reports/033d0be2-device-review.md.


## UPSTREAM-NEXT — граница подготовки следующего обновления

- Scope: cleanup beta.9, подготовка обновления и FB2 Hybrid.
- Decision: пользователь уточнил, что откладывал собственную оптимизацию в ожидании
  beta.9; при совпадении задач использовать готовую upstream реализацию, включая
  lazy panels. Это уточняет D07 и прежнее ограничение на автоматический перенос
  lazy Settings; не разрешает новые собственные оптимизации открытия.
- EPUB Hybrid остаётся upstream-owned: сейчас не менять его алгоритмы slicing,
  фоновой полной раскладки, adoption и safety fallback. Дальнейшие оптимизации
  ожидаются пользователем в beta.10; их наличие/результат пока не проверены.
- Параллельно cleanup разрешена подготовка FB2 к Hybrid: анализ, corpus и
  контрактные границы. Это не разрешение включить partial layout или изменить
  общий EPUB algorithm ради FB2. До доказанной корректности сохранять whole-book
  FB2 и защиту cross-section сносок; необходимые изменения общей архитектуры
  выносить на отдельное согласование.
- Source/approval: последние уточнения пользователя в этой сессии; план cleanup
  одобрен, дополнен подготовкой FB2 и анализом отказов beta.8 → beta.9.
- Validation: docs-only; runtime не менялся. Очередь в FB2_FORK_PLAN.md,
  причины отказов и gates — doc/reports/beta9-upgrade-retrospective.md.


## Annotation note editor strings

User explicitly approved the five RU note-editor strings through the session
choice: «Добавить заметку», «Изменить заметку», «Напишите заметку», «Не удалось
сохранить заметку. Ваш текст сохранён в редакторе.», «Заметка слишком длинная для
сохранения.». EN stays pinned upstream. Other new annotation wording is not
implicitly approved; existing common.cancel labels cancellation in this port.


## Upstream annotation interaction selector — approved translation

User approved six RU strings in the session choice: Выделение текста; Выберите,
как начинать выделение текста.; Переключаемый режим аннотаций; Включите режим
аннотаций в меню чтения, затем удерживайте текст для выделения. Вне этого режима
удержание текста открывает поиск в выбранном словаре.; Быстрый режим аннотаций;
Удерживайте текст, чтобы выделить его, добавить заметку или найти в словаре.
Кнопка аннотаций скрыта. EN remains pinned upstream. Toggle remains the default;
this is upstream alignment, not permission for an additional fork interaction mode.

User separately approved restoring the single pinned-upstream
`reader_annotation_drag` entry and meaning in platforms/checks/reader-properties.allow.
No general relaxation of source checks or additional allowlist entries approved.


### UPSTREAM-KOSYNC — unmodified synchronization policy

2026-10-26: user explicitly instructed full pinned upstream KOSync; the fork has
no independent KOSync policy. Whole model/coordinator/page/window handlers and
associated tests are upstream. Keep existing accepted RU wording; three new
login validation messages approved through `kosync_validation_strings`.
Sleep/storage logic follows upstream; retain accepted ribbon two-line text,
localized fitting and approved descriptions. CI/device evidence lives in the
queue; no device acceptance is implied. EPUB Hybrid algorithms remain excluded.

## B9-HYBRID-HANDOFF — повторная проверка на beta10

**Решение пользователя:** собственных исправлений EPUB Hybrid не вносить;
вернуться к проверке оценочного счётчика при переходе на beta10.
Проверенная цепочка расчёта/публикации/колонтитула не отличается от pinned beta9;
это не утверждение побайтового равенства всего движка и не доказательство
воспроизведения дефекта в чистой upstream-сборке.
Доказательства и границы: [отчёт](../../pre-beta10/doc/reports/beta9-hybrid-counter.md).

## B10-LIBRA2-OPTION — проверка замены локального mapping

По сообщению пользователя upstream beta10 содержит отдельную опцию, которая
должна заменить наше исправление Libra2. До сверки beta10 имя/API не утверждаются.
На обновлении проверить orientation × inversion; при покрытии сценария использовать
upstream и убрать локальную коррекцию. Не сохранять две инверсии одновременно.
[Текущая дельта и проверки](../../../reports/beta10-fork-map/sleep-refresh-cache-opening.md#f08a-libra2--проверить-заменяющую-опцию-beta10).

## B9-DELTA-REDUCTION — upstream policies вместо непроверенных поправок

По указанию пользователя: вернуть upstream beta9 ограничения анимации dark/i.MX,
BOOX refresh и sample-cache settings filter. Не сохранять эти три изменения как
feature при beta10. Старую first-open flash suppression тоже удалить; FB2/comics/
locale inputs оставить как необходимые функции, а не отдельную оптимизацию.
F07 ribbon/fitting пока не удалять: пользователь запросил места для проверки.
Текущий runtime2b33d231; CI результат в плане.
