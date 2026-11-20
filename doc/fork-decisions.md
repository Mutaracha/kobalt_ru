# kobalt_ru — принятые решения

Этот документ хранит действующие согласованные решения, а не очередь работ.
Очередь и следующие действия: [FB2_FORK_PLAN.md](../FB2_FORK_PLAN.md).
Инструкции агентам: [AGENTS.md](../AGENTS.md). Проверки устройства:
[beta8-device-check.md](beta8-device-check.md).

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
в обе стороны, включая top progress без header. Пользователь подтвердил полный
preview после build162; затем отдельно принял окончательную иконку.

## D04 — прогресс и меню чтения

**Решение:** Off/Bottom/Top = «Выключено»/«Внизу»/«Вверху»; legacy enabled → Bottom.
Top расположен **над** header, без нижнего резерва. Top ticks вниз, Bottom вверх;
отдельного выключателя chapter ticks нет. Comics временно скрывает chrome.

В верхней строке открытого меню отдельный счётчик «22 из 997» перед батареей;
строка «Вы читаете уже …» не удлиняется. Счётчик доступен без колонтитулов и
в comics, но не добавляется поверх обычной страницы. Zen сохраняет скрытие status.

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
Kobalt; stock About не менять, fork UTC-row и скрытый debug input сохранить.
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

- Scope: обычное меню чтения, не отдельная нижняя вкладка.
- Decision: по уточнению пользователя (2026-10-26) счётчик страниц размещается
  отдельной серой полосой под верхней панелью. Предпросмотр или другая панель,
  занимающая это место, замещает полосу. Это заменяет прежнее размещение страниц
  перед батареей. Время чтения центрируется по экрану, строки часов/батареи не меняются.
- Validation: runtime cb3fe8da, CI 44373349020 native/Qt5 PASS; device pending.


### Reading-menu strip: book estimate, italic captions and dismissal

User revised the decision on 2026-10-26: left caption shows time left in the
**book**, without the field-name prefix; right caption keeps book page count.
Both captions use italic interfaceFont (IBM Plex Sans), replacing the decorative
brand face after the user reported poor rendering; their background is transparent
so the header shading stays continuous. This
supersedes the earlier chapter-estimate/field-label rule. Reuse header/footer
BookTimeLeft formatting (including exact/approximate preference); capitalize
the first letter and leave the caption empty without an estimate.
The strip must disappear with the reading menu even if Preview never covered it.
No forced full-screen refresh or pagination reservation. Estimate preparation
must not depend on toolbar visibility: normal opening prepares status before show.
Runtime ef5424cf, CI 44397464051 native/Qt5 PASS; device acceptance pending.
The strip starts explicitly hidden, before its first menu placement.

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
