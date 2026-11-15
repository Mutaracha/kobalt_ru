# Beta.8 — контекстный аудит переводов и пересечений

## Принятое решение — текущий этап интеграции

Все предложенные переводы согласованы пользователем, включая уточнённую группу.
258 согласованных строк EN/RU внесены в assets. Runtime merge выполнен;
проверки и пакет для устройства фиксируются в текущем результате ниже.

Для кнопок использовать **позиционную модель upstream beta.8**: held(),
назначения одинаковых действий, none, частичные назначения и отображение
настроек — из базовой версии. От форка оставить только Libra 2 / Qt5 landscape
correction в определении поворота. Старый actionRotation убрать при интеграции,
не применять его поверх held(). Это новое решение заменяет прежнее требование
сохранять physical-action семантику для обоих-next/none.

Если механизмы дублируют друг друга (standby при открытии, notes и т.п.),
брать цельную реализацию upstream. Если изменены отдельные пункты общей функции,
делать контекстное слияние с сохранением согласованных fork-отличий.
Слияние upstream оформлено коммитом 190031dc с последующими исправлениями
API/инициализации и регрессионными проверками. Результаты CI и пакет
для физической проверки — в FB2_FORK_PLAN.md.

Целевой upstream: `679175046d8c806ea3b404525180fbc29a26fcd2`.
Проверены места вызова, ветвления и фактические аргументы. Runtime код в этой итерации не изменён; переводы ниже согласованы и внесены
в EN/RU assets. Ранее описанные варианты — основание принятого решения.

## 1. Принятые решения пользователя

- `opening.protected`: **Книга защищена**, без точки.
- `settings.inpage_footnotes`: **Сноски внизу страницы**.
- Сохранить прежние русские формулировки колонтитулов и открытия, кроме удаления точки выше; HTTP, экран выключения и описание новых сносок обновить по смыслу.
- `settings.section_dictionary_ui`: **Словарь** — раздел beta.8 содержит не только оформление, но и автоматическое сохранение найденных слов (`settings_panel.cpp:997–1008`).
- Термин **ридер**, не «читалка», во всех новых/обновлённых русских строках.
- Пункт скрытия переключателя тёмной темы и описание согласованы ранее.
- Остальные строки согласованы, кроме уточняемой группы в этом отчёте.

## 2. Письмо на экране сна: одна связная речь ридера

Источник: `src/app/reader/views/sleep_designs.cpp`, `letterParagraphs()`, строки1139–1189; `paintLetter()`, строки1249–1254.

**`sleep.letter_finished*` — не подпись процента.** Ветка выбирается при
`content.shot.percent >= 100`, но **`%1` в этих двух шаблонах — название книги**
(либо privacy-подстановка `sleep.private_book`). Это условие процента snapshot,
а не отдельный флаг завершения книги в БД. Формулировка «Чтение завершено»
описывает именно эту ветку и не требует склонять название книги.

В незавершённой ветке `%1` — уже форматированный процент (включая `%`),
`%2` — название, `%3` — необязательная фраза о главе. В `letter_remaining`
`%1` — время до конца книги из `sleepDuration(...)`; в `letter_battery`
`%1` — уже форматированный заряд с `%`. Добавлять второй знак процента нельзя.
После `letter_closing` отдельно рисуется подпись **Kobalt**.

Предложение: ридер говорит от первого лица, но вместо «уснул/выключился»
используется состояние. Названия не склоняются, приватная подстановка
«ваша книга» остаётся грамматически допустимой. Кавычки вокруг всего названия
не добавляются: само название выделяется курсивом и может уже содержать кавычки.

| Ключ | Предыдущий proposal | Уточнённый proposal |
|---|---|---|
| `sleep.letter_heading` | Читательский стол | Письмо читателю |
| `sleep.letter_greeting` | Дорогой читатель! | Дорогой читатель! |
| `sleep.letter_progress` | Я уснул. Вы прочли %1 книги «%2»%3. | Я перешёл в спящий режим. Прочитано %1. Книга: %2%3. |
| `sleep.letter_progress_off` | Я выключился. Вы прочли %1 книги «%2»%3. | Я выключен. Прочитано %1. Книга: %2%3. |
| `sleep.letter_chapter` | , в главе «%1» | , глава «%1» |
| `sleep.letter_finished` | Я уснул. Вы дочитали «%1». | Я перешёл в спящий режим. Чтение завершено: %1. |
| `sleep.letter_finished_off` | Я выключился. Вы дочитали «%1». | Я выключен. Чтение завершено: %1. |
| `sleep.letter_remaining` | Осталось примерно %1 чтения. | До конца книги — примерно %1. |
| `sleep.letter_charging` | Пока я заряжаюсь. | Тем временем я заряжаюсь. |
| `sleep.letter_battery` | Заряд батареи — %1. | Заряд батареи: %1. |
| `sleep.letter_wake` | Нажмите кнопку питания, и продолжим читать. | Нажмите кнопку питания, чтобы продолжить чтение. |
| `sleep.letter_restart` | Нажмите кнопку питания, чтобы включить меня. | Нажмите кнопку питания, чтобы включить ридер. |
| `sleep.letter_closing` | Ваш, | С уважением, |

Пример целиком (название и глава — данные книги):

> Письмо читателю
>
> Дорогой читатель!
>
> Я перешёл в спящий режим. Прочитано 37%. Книга: Кокаиновые короли, глава «Майами».
>
> До конца книги — примерно 2 ч 15 мин.
>
> Тем временем я заряжаюсь.
> Заряд батареи: 89%.
>
> Нажмите кнопку питания, чтобы продолжить чтение.
>
> С уважением,
> Kobalt

Вариант завершения: «Я выключен. Чтение завершено: Кокаиновые короли.»

## 3. Другие уточняемые строки

| Ключ | Было в proposal | Предлагается | Контекст |
|---|---|---|---|
| `sleep.design_pill` | Капсула | Плашка | paintPill() рисует горизонтальную плашку с полностью скруглёнными торцами. «Капсула» — буквальный перевод формы, «Плашка» естественнее как название UI-стиля. |
| `navigation.annotate` | АННОТАЦИИ | ВЫДЕЛИТЬ | Действие: вход в режим выделения/аннотирования; не список сохранённых записей. BottomNavigation и Reader::activateNavigation(). |
| `navigation.annotations` | АННОТАЦИИ | ПОМЕТКИ | Список пометок вместо закладок, когда Bookmarks выключены (BottomNavigation::setZen(), annotationsOnly). Не та же команда, что Annotate. Старые вкладки не переименовываются. |
| `stats.page_turn` | %1 перелистывание | Перелистываний: %1 | Счётчик book->sessionPageTurns(), не номер страницы, не число страниц в книге и не pages remaining. Формат «Перелистываний: N» не требует склонять существительное после числа. |
| `stats.page_turns` | %1 перелистываний | Перелистываний: %1 | Тот же счётчик; одинаковый шаблон на русском устраняет ошибку one/other для 2/3/4/21. Альтернатива — три формы через RU plural helper. |
| `kosync.from` | Откуда | Сейчас | Подпись текущей позиции: session->chapterTitle(), jump.here и fromPercent. Не устройство-отправитель. |
| `kosync.to` | Куда | После перехода | Подпись целевой синхронизированной позиции: info.chapter, jump.synced и toPercent. Показывается до подтверждения перехода, поэтому «После перехода», а не утверждение, что позиция уже применена. |

Для KOSync обе подписи в `SyncChoiceView::rows()` сопровождаются главой и
процентом. Данные устройства и времени — отдельные строки ниже. Более длинную
русскую подпись «После перехода» проверить по ширине, поскольку ширина колонки
вычисляется по подписям и уменьшает доступное место для главы.

## 4. Физические кнопки: beta.8 действительно меняет ту же область

### Что сделано upstream

- `page_buttons.h`: IDs теперь означают позиции кнопок при текущем удержании устройства, а не неизменные физические клавиши.
- `pageButtons::held(buttons, rotated)` меняет F23/F24 местами перед поиском сохранённого назначения; значения next/previous/none не инвертирует.
- Qt5 `nativePageButtonsRotated()` возвращает true для InvertedPortrait/InvertedLandscape. Отдельного условия `Device::isIo` для Libra 2 здесь нет.
- Qt6 определяет флаг из Nickel direction, исключив системную инверсию; его mapping остаётся Nickel-owned.
- `nativeInvertedPageButtons()` кэширует системную настройку; `forgetNativeInvertedPageButtons()` сбрасывает кэш перед созданием services для очередного открытия книги (reader_coordinator.cpp:1103–1105).
- `Reader::buttonControls()` использует held() для отображения настроек. `Reader::pageButton()` — для сохранённых действий; при пустом объекте назначений оставляет native direction.
- Меню/списки/preview по-прежнему обрабатываются до применения custom reading actions.

### Почему это не полная замена нашей поправки

| Область | Upstream beta.8 | Подтверждённый fork Libra 2 |
|---|---|---|
| Флаг перестановки в Qt5: Portrait / Landscape / InvertedPortrait / InvertedLandscape | `false / false / true / true` | `false / true / true / false` (эквивалент знака actionRotation) |
| Сохранённые действия | Переставляет ключи между позиционными IDs | Преобразует направление действия у физического ID |
| Разные next/previous у пары | Во многих случаях результат совпадает | Именно этот сценарий проверен пользователем |
| Оба назначения next | Оба остаются next после перестановки | В обращённом положении оба направления становятся previous |
| Одно назначение none | none перемещается к другой физической клавише вместе с позиционным ID | none остаётся у той же физической клавиши |

**Нельзя накладывать `held()` и старый `actionRotation` последовательно.**
Для обычной пары next/previous это может дать двойное преобразование. Но и
замена умножения на held() не эквивалентна для одинаковых действий, none и
частично заполненных предпочтений. Поэтому переход на позиционную модель
сохранённых настроек нельзя объявлять чистым рефакторингом.

Решение пользователя: взять из beta.8 held(), позиционные назначения,
поведение both-next/none/partial и отображение настроек. Сохранить только
нашу Libra 2 landscape correction. Прежний actionRotation при переносе убрать;
двойной transform недопустим. Это явное согласованное изменение предыдущего
контракта физически привязанных действий, а не незаметный рефакторинг.

## 5. Пересечения кроме нижних сносок

| Область | Что найдено в beta.8 | Пересечение/решение при merge |
|---|---|---|
| Физические кнопки | held(), pageButtonsRotated, инверсия с cache invalidation | Сохранить проверенный Libra 2 landscape и контракт назначений; исключить двойной transform. |
| Standby при открытии | openingWorkPending = busy && !run.fallbackAlert в reader_coordinator.cpp; gate в native_lifecycle.cpp | Наше correctness-исправление уже совпадает по назначению. Одна реализация upstream + существующий regression, не два veto подряд. |
| Сон и выключение | Новый каталог sleep_designs, privacy book, page snapshot/home sleep integration | Пересекается с fork оформлением сна и меню темы; сохранить approved shared styling и privacy. Новый letter-контекст переводить целиком. |
| Пресеты текста и Zen | text_presets.*, features.h, menu/navigation, appearance_settings.* | Не потерять comics overlay, durable/effective margins/chrome и Top-progress над header. Zen не должен перезаписать обычные значения. |
| Скрытие функций и тёмная тема | features.h + новые настройки; upstream удаляет light_dark_mode_shortcut из preferences и делает shortcut доступным через lightAvailable() | Наш новый hide-toggle пункт должен управлять именно reading-menu кнопкой, не сбрасывать тему и не конфликтовать с upstream light popup. |
| Drawing / metadata / refresh | Новый render/page_content.*, typed access вместо прежнего разбора kobalt.image_regions; reader_refresh.cpp откладывает sync скрытых controls до открытия | Сохранить centered/rotated comic hit regions и complete-pair publication; адаптировать producer и consumer metadata вместе. Не менять успешную waveform. Не считать upstream deferred-sync заменой нашего lazy Appearance. |
| Изображения / ResourceCache | Учитывает фактическую capacity и уплотняет последний блок ресурса | Сохранить наши begin/end image request trace, priority pending resources, frameImage и cancellation. Clean textual merge не доказывает atomic-pair behavior. |
| Appearance / lifecycle UI | Новые presets, words, sheets и stale-controls paths меняют build/refresh/readiness | Наша lazy Appearance остаётся: refresh не должен создавать скрытые controls; Settings пока eager. Общую opening-оптимизацию не возобновлять. |
| Native сохранение позиции | engine_positions.cpp учитывает duplicate IDs и границы текстовых узлов; native reading save использует saves::settled() | Перенести upstream исправления, сохранить fork FB2 percentage/time-only behavior и cancellation/save veto. |
| Синхронизация | sync_choice + новый coordinator и expanded position comparison; HTTP/metadata | Сохранить FB2 position capability и RU semantics; не переносить старые fork sync paths поверх нового coordinator. |
| Выделение/аннотации/словарь | My Words и scopes поиска используют общие selection/link/bookmark paths | Сохранить FB2 определения и проверки popup/notes; новый список слов и annotations-only Zen destination не путать с Annotate action. |
| Storage | Profile schema3 →4, saved_words и новые preference keys | Backup до первого запуска; rollback только с совместимым backup; canonical legacy migrations вместо двух notes switches. |
| Общие platform hooks и UI | native_symbol.h, CMake targets, fonts/styles/privacy/large-TOC changes | Сохранить RU locale/plurals, fork About/UTC, approved labels, Qt5 compatibility. Не принимать файл целиком ours/theirs. |

Это аудит исходников, **не результат запущенного объединённого runtime**.
Полная карта31 текстового конфликта: [beta8-migration.md](beta8-migration.md).
Автоматически объединённые файлы перечислены отдельно: они также требуют
семантической проверки, особенно page_buttons.h, ResourceCache и metadata.

## Источники

- [`src/app/reader/views/sleep_designs.cpp`](https://github.com/nicoverbruggen/kobalt/blob/679175046d8c806ea3b404525180fbc29a26fcd2/src/app/reader/views/sleep_designs.cpp)
- [`src/app/reader/views/navigation.cpp`](https://github.com/nicoverbruggen/kobalt/blob/679175046d8c806ea3b404525180fbc29a26fcd2/src/app/reader/views/navigation.cpp)
- [`src/app/reader/views/sync_choice.cpp`](https://github.com/nicoverbruggen/kobalt/blob/679175046d8c806ea3b404525180fbc29a26fcd2/src/app/reader/views/sync_choice.cpp)
- [`src/app/reader/panels/chapter_graph.cpp`](https://github.com/nicoverbruggen/kobalt/blob/679175046d8c806ea3b404525180fbc29a26fcd2/src/app/reader/panels/chapter_graph.cpp)
- [`src/app/reader/window/reader_kosync.cpp`](https://github.com/nicoverbruggen/kobalt/blob/679175046d8c806ea3b404525180fbc29a26fcd2/src/app/reader/window/reader_kosync.cpp)
- [`src/platform/api/page_buttons.h`](https://github.com/nicoverbruggen/kobalt/blob/679175046d8c806ea3b404525180fbc29a26fcd2/src/platform/api/page_buttons.h)
- [`platforms/kobo/nickel-qt5/native_ui.cpp`](https://github.com/nicoverbruggen/kobalt/blob/679175046d8c806ea3b404525180fbc29a26fcd2/platforms/kobo/nickel-qt5/native_ui.cpp)
- [`platforms/kobo/nickel-qt6/native_ui.cpp`](https://github.com/nicoverbruggen/kobalt/blob/679175046d8c806ea3b404525180fbc29a26fcd2/platforms/kobo/nickel-qt6/native_ui.cpp)
- [`platforms/kobo/common/reader_host_services.cpp`](https://github.com/nicoverbruggen/kobalt/blob/679175046d8c806ea3b404525180fbc29a26fcd2/platforms/kobo/common/reader_host_services.cpp)
- [`src/app/reader/window/reader.cpp`](https://github.com/nicoverbruggen/kobalt/blob/679175046d8c806ea3b404525180fbc29a26fcd2/src/app/reader/window/reader.cpp)
- [`src/app/reader/window/reader_settings.cpp`](https://github.com/nicoverbruggen/kobalt/blob/679175046d8c806ea3b404525180fbc29a26fcd2/src/app/reader/window/reader_settings.cpp)
- [`src/app/reader/window/reader_refresh.cpp`](https://github.com/nicoverbruggen/kobalt/blob/679175046d8c806ea3b404525180fbc29a26fcd2/src/app/reader/window/reader_refresh.cpp)
- [`src/render/page_content.cpp`](https://github.com/nicoverbruggen/kobalt/blob/679175046d8c806ea3b404525180fbc29a26fcd2/src/render/page_content.cpp)
- [`src/publication/resource_cache.cpp`](https://github.com/nicoverbruggen/kobalt/blob/679175046d8c806ea3b404525180fbc29a26fcd2/src/publication/resource_cache.cpp)
- [`src/storage/profile_database.cpp`](https://github.com/nicoverbruggen/kobalt/blob/679175046d8c806ea3b404525180fbc29a26fcd2/src/storage/profile_database.cpp)
