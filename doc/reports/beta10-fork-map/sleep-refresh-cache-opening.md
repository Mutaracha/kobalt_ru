# Подробности F07–F09 и открытие книг

**Актуализация2b33d231/6a82161b:** dark/i.MX animation, BOOX и F09 возвращены к upstream
beta9, первый book-open flash восстановлен. Описания удалённых дельт ниже —
историческое объяснение, не инструкции сохранять их при beta10. F07 пока сохранён.

Сравнение: upstream beta9 `be47258850ffdc3295257475afdf8b8f6cb53176` →
fork `393c612d8991441ef5b1e812f9b9b6bbc2ed68b0` (runtime `cb705bdd`).
Это описание существующего кода, не новые изменения display/engine.

## F07. Сон — не только формирование русского текста

Файлы `src/app/reader/views/sleep_ribbon_text.cpp`, `sleep_screen.cpp/.h`,
`sleep_designs.cpp` содержат три retained изменения:

1. **Разделение подписи оставшегося времени на две строки.**
   `sleepRibbonRemainingLines()` использует текущий шаблон `sleep.time_left` и
   положение `%1`, а не список русских слов. В RU получается подпись → время,
   в EN время → подпись. Для старого snapshot на другом языке, который не
   совпадает с текущим шаблоном, функция сохраняет исходную строку целиком.
2. **Геометрия закладки-ribbon.** В высоте зарезервированы две detail-строки,
   отрисовка проходит по результату helper. Батарея размещается по стабильному
   нижнему anchor ribbon, независимо от того, одна или две строки получились.
   Это layout-доработка, не только перевод.
3. **Fitting текста.** В centered text ribbon и общем text helper sleep designs
   размер шрифта постепенно уменьшается, пока строка не помещается по ширине,
   с нижней границей примерно половина исходного pixelSize. Если этого
   недостаточно, применяется elision уже с подобранным шрифтом. Поведение
   относится к любой строке/языку, а не исключительно к RU.

Новых алгоритмов расчёта оставшегося времени, save/sleep/wake или собственного
sleep snapshot storage в этой группе нет. В beta10 переносить presentation только
если эквивалентное поведение ещё отсутствует. Проверки: RU/EN, старый snapshot
другого языка, длинные строки, батарея, разные screen sizes/orientations/designs.

## F08a. Libra2 — проверить заменяющую опцию beta10

Текущая дельта: `platforms/kobo/common/page_button_mapping.h` и
`platforms/kobo/nickel-qt5/native_ui.cpp`. Учитывается устройство и ориентация
при определении физической ротации кнопок; направление читается для native
F23/F24 с учётом инверсии, вместо общего трактования стрелок как page buttons.

**Новое указание пользователя:** разработчик upstream сообщил об отдельной опции
beta10, которая должна заменить наше исправление. При обновлении сначала найти
её реальную реализацию/default/сохранение и проверить Libra2 во всех ориентациях,
с инверсией on/off. Если покрывает тот же сценарий — убрать fork mapping и его
native-ui hook в пользу upstream. Не переносить локальное исправление автоматически
и не применять обе коррекции одновременно. Название/API опции здесь не выдумываются:
beta10 в рамках этого исследования ещё не сверялась.

## F08b. Kobo display/refresh — что удалено и что осталось

### Анимация в dark mode на старом i.MX — локальное отличие удалено

В upstream beta9 `pageAnimationsEnabled(support, dark)` запрещает sweep в тёмном
режиме при `PageAnimationSupport::Uncertain` (старый i.MX backend), сохраняя
пользовательское предпочтение. В форке этот дополнительный запрет убран:

- `src/platform/api/page_animation_support.h`: удалён helper с dark/Uncertain gate.
- `platforms/kobo/display/page_transition.h`: используется availability, а не
  указанный дополнительный dark gate.
- `platforms/kobo/display/page_animation.cpp`: `PageAnimation::arm()` сохраняет
  переданное direction без повторного зануления по dark/Uncertain.

Это снятие upstream-ограничения доступности анимации в конкретном сочетании
backend/theme; не новый общий алгоритм waveform и не гарантия качества на всех
панелях. В beta10 сравнить новую policy и проверить dark/light на физическом Kobo.

### Обновление графических/комиксных страниц

В `platforms/kobo/nickel-qt5/refresh.cpp` property `kobalt_graphics_refresh`
заставляет `pageRefresh()` выбрать `repairPolicy(dark)` и без отдельного clean
request. Для обычного текста остаётся обычная policy. Обновляется переданная
область damage — это **не** безусловный whole-screen full-flash на каждом ходе.
Дополнительно пишется `comic-refresh-submit`: bounds, число rectangles/policies
и policy hex, чтобы проверить, что реально отправлено backend.

Проверки beta10: text/comics, dark/light, partial/repair, анимации, ghosting,
смена orientation и возврат к обычному чтению. Не подменять device acceptance
зелёными model-тестами. Перестановки include/dragArea в diff не являются отдельными
пользовательскими доработками.

## F08c. BOOX — delayed cleaning удалён, ниже история

Файл: `platforms/android/app/src/org/kobalt/android/BooxRefresh.java`.

В upstream beta9 GU/REGAL/GC передаются как metadata через `Surface.addEpdc`
до Qt flush. В форке разделены два пути:

- Partial GU/REGAL по-прежнему передаются через `addEpdc` до flush; координаты
  пересчитываются относительно surface и обрезаются её bounds.
- Clean request идёт отдельным `View.refreshScreen` после задержки **250 ms**
  через `HandlerThread`, с waveform flags `32 | 64 | 2` (98), а не UI_GC_MODE из
  ViewUpdateHelper. Замысел — очищать уже устоявшийся кадр, не предыдущий buffer.
- При доступности вызывается `setWaveformAndScheme(5,1,0)` для подавления системной
  update policy; без этого optional API явный refresh всё равно предпринимается.
- Серия clean requests объединяется; generation/проверка view/surface/geometry
  предотвращают поздний запрос к сменившейся цели. cancel очищает queued callbacks,
  close завершает worker. Есть дополнительные hidden-API exemptions и логи.
- Есть и поведенческая дельта поиска surface: две видимые Qt surfaces вызывают
  ошибку и отключение integration, тогда как upstream ожидает sole target без
  такого исключения. Это надо отдельно проверить при surface replacement, не
  считать автоматически улучшением.

**Статус:** это существующая специфическая реализация форка, не новая правка
в рамках документации. BOOX/Android hardware acceptance не заявляется. При beta10
сверить upstream API/cleaning semantics прежде переноса, проверить menu/page bursts,
resume/orientation/surface replacement, отсутствие зависаний и поздних refresh.

## F09. Кэш образцов оформления — фильтрация удалена, ниже история

Файл `src/reading/style_sample.cpp`, `styleSampleSettings()`.

До вычисления ключа кэша из engine state убираются поля, не влияющие на внешний
вид sample. Это позволяет переиспользовать образцы при перелистывании и изменении
служебного состояния. Большинство исключений — уже upstream beta9.

**Прежняя дельта относительно beta9 — добавить `whole_book_failed` в существующий
список исключений.** Это индикатор результата фоновой полной раскладки, а не выбор
шрифта, размера или интервала. Даже появление поля со значением false не должно
менять ключ у того же оформления. Иначе теряется cache hit и запускается лишнее
построение sample.

Это не новый cache backend, eviction policy или механизм сохранения страниц
книги. Настоящие параметры оформления остаются в ключе, чтобы не показать старый
образец после изменения typography. Регрессия в `tests/model/style_sample_cache.cpp`
добавляет transient frame fields, в том числе этот flag, и требует одинаковых
sample settings; исходная проверка сохранена. Исправление прошло native CI.

Beta10: если upstream уже фильтрует этот flag или перешёл к whitelist typography,
локальная строка не нужна. Иначе перенести исключение и исходную проверку. К
неисправленному estimated-counter Hybrid эта правка отношения не имеет.

## Открытие книг: что осталось от прежних ускорений

**Shared pipeline открытия выровнен с upstream beta9.** Отдельного параллельного
fork-only механизма ускоренного EPUB opener не сохраняем. При этом полезные
механизмы не выключены: upstream retained/preload, parsed document cache и
отложенная подготовка UI продолжают работать.

Точная граница:

- `src/app/reader/views/opening.cpp/.h`, `src/reading/opening_job.h`,
  `src/reading/reading_book.cpp/.h`,
  `platforms/kobo/common/reader_coordinator.h`,
  `src/engine/blaze/adapter/document_cache.h`, `src/ui/style/private_text.cpp`
  совпадают с pinned beta9.
- В `reader_coordinator.cpp` единственная дельта — resolve/apply fork locale
  между сессиями. Его preload/retained orchestration — upstream.
- В `nickel-qt5/reader_controller.cpp` и Qt6-аналоге единственная дельта — locale
  initialization. Startup/preload gates и helpers берутся из upstream.
- `opening_job.cpp` отличается только FB2 resource probe, legacy-notes migration
  и сохранением comics setting при merge saved state. Это форматная интеграция,
  не новая оптимизация стадии parsing/layout.
- `reader_session.cpp` отличается include и `image(page) → frameImage()` для
  comics; основная session/background machinery — upstream.
- **Первый показ теперь upstream:** `requestFlash("book-open")` восстановлен.
  Единственная оставшаяся дельта reader.cpp — диагностическое поле
  appearance_controls_built при сохранении state. В `reader_source.cpp` и
  `engine_document.cpp` также есть FB2/comics/cache-policy integration, поэтому
  целые эти файлы не объявляются идентичными.

Таким образом, корректно говорить «наши прежние отдельные оптимизации shared
открытия заменены текущей upstream-реализацией», но не «весь код открытия побайтово
upstream» и не «любой cache/preload теперь отсутствует». Скорость на конкретной
книге измеряется отдельно: retained/cache hit, cold parse, first frame и quiet UI
нельзя смешивать в одно число. FB2 partial-layout/Hybrid не включён; EPUB Hybrid
counter остаётся deferred до beta10 по решению пользователя.


## Где проверять F07 перед решением об удалении

- **Геометрия ribbon и её fitting:** дизайн «Закладка». Также «Страница книги»
  или «PNG поверх текста», если выбран индикатор «Закладка»; «Свой вариант» с
  таким status layer. Не «Карточка» и не «Только обложка» без индикатора.
- Выбрать книгу с известным оставшимся временем, проверить RU и EN. В ribbon
  проверить две строки времени, батарею и отсутствие наложений; затем случай без
  remaining, где батарея тоже должна оставаться на своём месте.
- **Общий fitting sleep designs:** удобнее проверять «Подпись», «Виджеты», «Ночь»,
  «Кольцо», «Рамка», «Линии», «Письмо» с длинным названием/автором/подписью и
  длительным remaining. Общий text helper вызывается также через baselineText и
  battery helper; не весь текст любого дизайна проходит через него (у многострочных
  блоков есть свои layout helpers). Это не изменение текста книги.
- Превью полезно для геометрии, но финально проверить настоящий сон в portrait и
  landscape. Само отсутствие clipping с активным fitting не доказывает, что он
  не нужен: для решения об удалении нужно сравнение с upstream rendering без него.
- Пользователь пока только запросил места проверки, удаления F07 не поручал.

## Повторный аудит старых ускорений после cleanup

Собственного opener/preload/quiet pipeline не осталось. Первый book-open flash
теперь upstream. Оставлены только FB2 resource probe и допустимое .fb2 cache имя,
FB2 CSS/semantic hooks и совместимость notes; comics setting/frame/layout/cache
identity и locale hooks — необходимые отдельные функции форка, не ускорители.
Их удаление сломало бы ранее принятые возможности.

В `engine_document.cpp` saveDocumentCache(reason) возвращает подробную причину
отказа и принимает правильное FB2 cache имя; проверка stale/dirty и updateCache
сохраняют upstream логику. Не удалять FB2 cache name acceptance как «оптимизацию»:
без неё FB2 не сможет сохранять кэш под своим реальным именем. Версии CSS/layout/
comics cache keys — защита совместимости, не альтернативный ускоренный cache.
Диагностика appearance_controls_built не управляет построением UI. Скорость
открытия после выравнивания пользователь оценил как значительно лучшую;
собственных speed claims/benchmark без измерений не добавляется.


CI промежуточного2b33d231/build292: focused native PASS, Qt5 package check FAIL;
в `tests/pewter/refresh_qt5.cpp` ещё оставались fork assertions, требовавшие sweep
в dark i.MX. В6a82161b весь этот тест возвращён к pinned upstream, включая проверку
обычного single update и damage bounds в dark. Это возврат теста вместе с policy,
не отключение Qt5 проверки. Итоговый CI указан в плане/основной карте.
