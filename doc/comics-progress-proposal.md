# Отметки глав, режим комиксов и новый пример остаточного поля

Статус: запрос пользователя принят, проведён первичный code/evidence review.
Исполняемый код и перевод пока не менялись. Для новой UI строки ниже приведён
EN/RU название уже принято пользователем; окончательная причина остаточного поля
проверяется на полученном reduced-book материале после подготовки fixture.
Qt5 остаётся основной целью, повторная сборка Qt6 не требуется.

## 1. Что подтверждают новые снимки

[Оригиналы и provenance](../tests/manual/screenshots/kobo-qt5/comics-report/README.md).

- `layout.png`: оба значения полей равны 0.
- `header-footer.png`: верхний, нижний колонтитулы и progress bar выключены.
- `kobalt.png`: внизу остаётся белая полоса; `nickel.png` показывает ту же
  композицию до нижнего края. Это уточняет прежний аудит, где comic screenshots
  отсутствовали. Нельзя по-прежнему объяснять случай только ненулевыми полями.
- PNG имеют одинаковый canvas 1264×1680. Диагностический замер снизу, считая
  строку почти белой при доле пикселей <220 менее/равно 0,5%, даёт 18 строк
  у Kobalt и 0 у Nickel. Это **замер PNG**, не извлечённое значение page margin.
  Его нельзя превращать в универсальный magic offset для всех книг/устройств.
- Уточнение пользователя: Kobalt build 94, `d7b7d55`; unpacked пример получен
  в `doc/Spice and Wolf v01.kepub.epub/`. Первоначально этих данных не было. Различия масштаба/ресэмплинга
  видны, но без оригинальной картинки и CSS нельзя доказать, что нужно именно
  изменить viewport, увеличить image fit или убрать inline line-box.

## 2. Code review: независимые уровни геометрии

1. `ReadingPage::topHeight()/bottomHeight()` резервируют chrome strips по
   header/footer/progress flags. Для показанных выключенных controls не следует
   добавлять ещё один условный «скрыть колонтитулы» и считать проблему решённой.
2. `Reader::bodySize()` вычитает safe area и высоты полос из размера viewport.
   Из предоставленных PNG конкретное значение safe area не устанавливается.
3. `EngineDocument::configure()` устанавливает `setPageMargins(...)` отдельно
   от chrome, но также явно вызывает `setPageHeaderInfo(0)`: штатный header
   CREngine уже отключён, его нельзя объявлять причиной без дополнительного trace.
4. Движку передаётся `Resize(width,height)`; одноколоночный drawColumn рисует
   в QImage размера viewport. На этом уровне нет найденного явного правила
   «всегда оставить 18px снизу».
5. `applyStyleSheet()` при normalization сбрасывает margin/padding на
   html/body/DocFragment, а не на всех вложенных контейнерах. Существуют special
   full-page SVG/table rules и leading-image boundary в один физический pixel.
   Последний недостаточен, чтобы сам по себе объяснить всю нижнюю полосу.
6. CSS baseline/line-height у inline image, paragraph box, image max-height,
   исходный aspect ratio и собственные белые края ресурса — отдельные кандидаты.
   Нормальные legacy image zoom-in modes отключены для защиты inline картинок
   и формул от двойного масштабирования. Профиль графики не должен ломать их
   глобальным изменением этих флагов для обычных книг.
7. Используются upstream patches `crengine-image-proportions.patch`,
   `crengine-page-layout.patch` и `crengine-render-overflow.patch`; замена
   движка/отмена этих исправлений без regressions не предлагается.

### Как доказать точную причину

Получены SHA и исходные XHTML/CSS/PNG. Требуется подготовить валидный
минимальный package из этого материала, не переписывая исходник пользователя. Зафиксировать viewport, chrome
strips, effective margins, image source size/box/baseline и rendered page rect.
Сравнить: исходный resource → engine buffer → GUI screenshot → Nickel.
Проверить изолированно inline img vs display:block, wrapper padding/line-height,
SVG и aspect-fit. Не лечить white strip захардкоженным crop/растяжением всего
кадра: это может обрезать artwork, сноски или текст.

## 3. Нижняя полоса прогресса: отметки границ глав

Требование пользователя: аналог делений в верхней полосе CoolReader, но на
существующей **нижней** progress bar Kobalt. Это отметки начала глав, не равные
деления процентов и не ещё один счётчик страниц.

- Источник — уже доступные `session->chapters()` и число страниц. Они уже
  передаются в page scrubber. Не добавлять второй parser оглавления.
- Извлечь валидные page positions; удалить дубли одного начала, отбрасывать
  неразрешённые/вне диапазона anchors. Привязать к той же page-progress шкале,
  которой рисуется текущая заливка. Не вводить word-count vs page-count drift.
- `ReadingPage::paintFooter()` сейчас рисует только линию и filled fraction.
  Передавать туда подготовленные позиции отметок, а не обращаться к DOM/engine
  из paintEvent. Обновлять при pagination, смене книги и появлении TOC.
- Включить данные отметок в footer invalidation/snapshot state: иначе частичная
  перерисовка может оставить старые отметки после reflow.
- Короткие контрастные ticks, без подписей и дополнительной высоты footer.
  Схлопывать совпавшие pixel positions на плотном TOC. Без оглавления — прежняя
  полоса без делений. Учитывать текущую светлую/тёмную палитру.
- Рисовать только при включённой progress bar. В comics profile она скрыта.
  Отдельный переключатель/новая строка перевода для ticks не предлагаются.

Acceptance: неравномерные/дублированные главы, пустой/поздний TOC, invalid pages,
reflow и переход книг, разные viewport/PPI, dark mode, частичное обновление и
скрытая progress bar. На устройстве сравнить с исходным референсом cr.png.

## 4. Режим чтения комиксов — контракт до реализации

Новая опция в **Раскладке**, default off. Предлагается сохранять её **для
текущей книги**, чтобы включение на манге не меняло следующую текстовую книгу.
Это уточнение проекта, а не утверждение о существующем поведении.

### Effective overlay вместо перезаписи настроек

- Пока включена: effective engine margins = 0; effective header/footer/progress
  выключены; режим одной графической страницы без текстовых колонок.
- Исходные поля, preset/custom header slots, размеры, progress preference,
  normal rotation и layout values не заменяются нулями в durable settings.
- Выключение восстанавливает именно сохранённую раскладку, не заводские defaults.
  Режим можно сохранить отдельно, не сохраняя его effective overrides как базу.
- Внимание к `rememberPageMargins`, `rememberTextLayout`, engine state,
  checkpoint, appearance preview/apply/cancel, close/sleep/reopen и source-change
  filtering: простой `saved[margin]=0` приведёт к потере пользовательских значений.
- Нужно включить effective profile в cache/layout compatibility. Не использовать
  cache, рассчитанный с другими geometry, и не требовать ручной очистки данных.
- UI полей/колонтитулов остаётся доступным: изменения сохраняются в базовой
  раскладке, но не применяются до выключения режима. Не добавлять длинных
  пояснений; checkbox через existing common presentation.

### Отображение изображения

Цель — использовать весь доступный экран, без искусственной нижней полосы,
по образцу Nickel. На image-only страницах учитывать full-page wrappers и
inline baseline, не масштабировать вслепую все иллюстрации в текстовых книгах.
Сохранять aspect ratio: «без добавленных полей» не гарантирует заполнения
обоих измерений при другой форме картинки. Для таких случаев crop/fill должен
быть отдельным явным решением, а не скрытой порчей пропорций.

Существующий ImageViewer с fit/zoom/pan сохраняется; режим чтения комикса не
должен требовать открывать его для каждой страницы. Новые CBZ/CBR форматы в
этот запрос не включаются. Нижние сноски и стандартная FB2/EPUB верстка при
выключенном режиме остаются без изменений.

### Новая UI строка — принята пользователем

| Ключ (проект) | EN | RU | Default |
|---|---|---|---|
| reader.comics_reading_mode | Comics reading mode | Режим чтения комиксов | Off |

RU заголовок дан самим пользователем. Новых descriptions/dialog texts не
предлагается. Название принято. Все перечисленные ниже решения согласованы с поправкой: настройки полей
и колонтитулов остаются доступными для изменения и сохранения.

Acceptance: toggle on/off с ненулевыми базовыми полями и custom chrome;
apply/cancel и ошибка сохранения; close/reopen/sleep; переход comic→text;
portrait/landscape; image-only raster/SVG и обычный inline image; aspect-ratio
mismatch; cache hit/miss; position invariants; actual affected book и Kobo Qt5.

## 5. Следующий шаг

1. Согласовать единственную новую пару EN/RU и per-book scope.
2. Получить affected publication/minimal fixture и установленный SHA — для
   доказательства причины strip и регрессионного воспроизведения.
3. Реализовать ticks и effective comics overlay отдельными сопровождаемыми
   helpers, не копируя экран/EPUB pipeline. Добавить unit/routed/engine checks.
4. Qt5/native/package CI, конкретный artifact и новые device screenshots.

Измерения и первичный source audit уже выполнены; реализация/ускорение/новый
device результат не объявляются готовыми заранее.


## Полученный пример и вопросы перед реализацией

База проверки пользователя — build 94 / d7b7d55. Материал из ad349b7:
`doc/Spice and Wolf v01.kepub.epub/`. Сохранённый XHTML: `div` с center и
inline `img` с dimensions 1160×1680; PNG тех же размеров. CSS сбрасывает body
и @page margin/padding. Изучать inline baseline/line-height вместе с effective
viewport, а не объявлять причиной ненулевые поля. OPF всё ещё содержит ссылки
на удалённые ресурсы: сделать отдельный test package, сохранить original bytes.

Согласованные решения поведения:

1. Сохранять mode отдельно для книги и восстанавливать при открытии, не
   включать автоматически в других книгах (да; default off).
2. Aspect fit без искажения/обрезки, даже если при несовпадении пропорций
   остаются естественные полосы (да; не вводить скрытый crop/stretch).
3. Пока mode активен, controls базовых полей/chrome доступны. Изменения
   сохраняются, но не применяются до выключения mode. Rotation доступен.
4. Одна исходная страница на экран в обеих ориентациях, без автоматического
   двухстраничного разворота в первой реализации (да; typography columns
   временно не применяются, их durable preference не меняется).

Chapter ticks: согласовано без дополнительного switch, только вместе с
существующей progress bar; unique valid TOC boundaries и pixel dedup.
Название и отсутствие длинного description уже согласованы; не спрашивать
их заново. Технический способ устранения strip определяется regression,
а не голосованием по исходному коду.


## Согласованный контракт режима комиксов

Пользователь утвердил поведение перед реализацией:
- Режим сохраняется отдельно для каждой книги; default off.
- Изображение вписывается целиком с сохранением пропорций, без crop/stretch.
- Поля и колонтитулы НЕ блокируются: пользователь может менять их обычным
  способом, значения сохраняются, но при активном режиме не применяются.
  После выключения применяются последние сохранённые значения, а не snapshot
  на момент включения. Apply/cancel сохраняет обычную семантику настроек.
- Одна исходная страница на экран в обеих ориентациях; без auto-spread.
- Отметки глав без отдельной настройки, только на существующей progress bar.
- Название EN/RU принято; новых descriptions не требуется.

Это утверждённое поведение, не отчёт о готовой реализации. Проверить edit→apply
при активном режиме, close/reopen и выключение с восстановлением обновлённых
значений; отдельно edit→cancel не должен менять сохранённую базу.


## Comics implementation (pending CI/device verification)

Added the approved per-book `comics_reading_mode` toggle in Layout. Stored
margins and global chrome remain editable and retain their values; the engine
applies a zero-margin, single-column graphics profile only while enabled.
Disabling uses the latest base values. Image CSS uses block layout and
aspect-preserving viewport caps, removing the inline baseline/leading-padding
reserve without cropping source pixels. The profile participates in appearance
and parsed-cache identity; no saved-data version bump or cleanup is needed.

Chapter boundaries now share the footer fill scale, filter invalid pages and
deduplicate page/pixel positions. The footer snapshot includes their positions,
so reflow invalidates ticks; paint does not parse the book. No new tick setting.
FB2 returns zero document fonts before any document-font enumeration; shared
font initialization and generic-family fallbacks remain intact. No measured
opening-time improvement is claimed.

New UI pair (approved): `reader.comics_reading_mode` — “Comics reading mode” /
«Режим чтения комиксов». No description. New engine regression covers portrait
and landscape fitting at 96/300 PPI, effective geometry, edits while enabled,
checkpoint/reopen and restoration. Manual checks on build d7b7d55 remain the
baseline, not proof of this implementation. Test Apply/Cancel, sleep/reopen,
chapter tick reflow and the supplied graphic on Kobo Qt5 after CI passes.
