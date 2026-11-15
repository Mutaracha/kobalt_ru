# Beta.8 — согласованные переводы EN/RU

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

Upstream `679175046d8c806ea3b404525180fbc29a26fcd2`; fork baseline `130091fb93e455727d00acb77c312710ad8a9290`.

**Переводы согласованы и внесены в `assets/*.strings`.** Новые функции
появятся после завершения runtime-интеграции beta.8. Существующие согласованные fork-подписи и неизменные
нижние вкладки сохраняются; пересечения сверять по смыслу, не заменять массово.
EN/RU length — число Unicode-символов (без учёта переноса строки как двух символов);
это предварительная оценка, не доказательство отсутствия clipping.

## Добавление форка: скрытие переключателя тёмной темы — согласовано

| Key | EN | RU | EN/RU |
|---|---|---|---|
| `features.dark_mode` | Dark theme toggle | Переключатель тёмной темы | 17/25 |
| `features.dark_mode_description` | Hides the dark theme toggle in the reading menu. Your current theme is kept. | Скрывает переключатель тёмной темы в меню чтения. Текущая тема сохраняется. | 76/75 |

Пункт и описание согласованы пользователем. Реализация ещё не выполнена.

Согласованное поведение: скрывается только кнопка в меню чтения. Это не
принудительный переход к светлой теме, не сброс выбора и не запрет тёмного режима
во всей программе. Переключатель скрытия выключен по умолчанию.

Сравнение со старым русским переводом и языковые замечания:
[сводная таблица](beta8-strings-comparison.md).

## Уточнение после проверки контекста

Пользователь согласовал прежние рекомендации с исключениями: opening.protected
без точки; settings.inpage_footnotes — «Сноски внизу страницы»; термин «ридер»
вместо «читалка». Остальные строки приняты за исключением повторно обсуждаемых
sleep.letter_*, sleep.design_pill, navigation.annotate/annotations,
stats.page_turn/page_turns и kosync.from/to. Их обновлённые формулировки ниже —
**согласованы пользователем**, внесены в assets.
Полный разбор: [beta8-context-review.md](beta8-context-review.md).

## Upstream beta.8: новые и изменённые строки

256 новых/изменённых EN значений относительно beta.7. Строки с числами требуют
проверки fork RU pluralization; не подставлять английскую схему one/other вместо неё.

| Key | EN | RU proposal | EN/RU |
|---|---|---|---|
| `bookmarks.custom_missing` | Missing icon: %1 | Нет значка: %1 | 16/14 |
| `bookmarks.custom_help_action` | Help | Справка | 4/7 |
| `bookmarks.custom_help_title` | Custom bookmark icons | Свои значки закладок | 21/20 |
| `bookmarks.custom_help_kobo` | Copy your SVG files to .adds/kobalt/custom-pins/ over USB. | Скопируйте SVG-файлы в .adds/kobalt/custom-pins/ по USB. | 58/56 |
| `bookmarks.custom_help_android` | In Library, open More > Resource folders and choose your custom icon folder. | В библиотеке откройте «Ещё» > «Папки ресурсов» и выберите папку своих значков. | 76/78 |
| `bookmarks.custom_help_desktop` | Copy your SVG files to:<br>%1 | Скопируйте SVG-файлы в:<br>%1 | 26/26 |
| `bookmarks.custom_help_reload` | Close and reopen your book to load them. Your icons appear first. | Закройте и снова откройте книгу. Ваши значки появятся первыми. | 65/62 |
| `bookmarks.custom_help_limits` | You can add 120 icons. Depending on how they are formatted, icons may not load correctly. | Можно добавить 120 значков. В зависимости от формата они могут отображаться неверно. | 89/84 |
| `bookmarks.custom_errors_title` | Icons not loaded | Значки не загружены | 16/19 |
| `bookmarks.custom_errors_count` | Files that could not be loaded: %1 | Не удалось загрузить файлов: %1 | 34/31 |
| `bookmarks.custom_errors_more` | And %1 more. | И ещё %1. | 12/9 |
| `bookmarks.custom_errors_limit` | Some files were not checked. Kobalt loads up to 120 icons and checks up to 256 folder entries. | Часть файлов не проверена. Kobalt загружает до 120 значков и проверяет до 256 элементов папки. | 94/94 |
| `kosync.invalid_account` | Enter an HTTP or HTTPS server URL, username and password. The URL cannot contain credentials, a query or a fragment. | Введите HTTP- или HTTPS-адрес сервера, имя и пароль. Адрес не должен содержать данные входа, параметры запроса или фрагмент. | 116/124 |
| `chapter_graph.show_more_chapters` | Show more chapters | Больше глав | 18/11 |
| `chapter_graph.show_wider_bars` | Show wider chapter bars | Более широкие столбцы глав | 23/26 |
| `header_footer.customize_title` | Customize Header and Footer | Настроить колонтитулы | 27/21 |
| `opening.preparing` | Preparing your book | Подготовка книги | 19/16 |
| `opening.reopening` | Reopening your book | Повторное открытие книги | 19/24 |
| `reader.choose_typeface` | Choose a typeface | Выберите шрифт | 17/14 |
| `settings.section_opening` | Opening books | Открытие книг | 13/13 |
| `settings.opening_cover` | Do not show book cover while opening | Не показывать обложку при открытии | 36/34 |
| `settings.opening_cover_description` | Always show the Kobalt logo when opening a book. | При открытии книги всегда показывать логотип Kobalt. | 48/52 |
| `settings.automatic_book_opening_off_title` | Have Kobo open books by default? | Открывать книги в ридере Kobo? | 32/30 |
| `settings.automatic_book_opening_off_description` | Books from your library will open in Kobo's built-in reader. To open a book in Kobalt, long-press it and choose Read with Kobalt. | Книги из библиотеки будут открываться в штатном ридере Kobo. Для открытия в Kobalt удерживайте книгу и выберите «Читать в Kobalt». | 129/130 |
| `settings.automatic_book_opening_off` | Use built-in reader | Использовать ридер Kobo | 19/23 |
| `settings.hide_book_on_sleep_screen` | Hide this book while asleep | Скрывать эту книгу во сне | 27/25 |
| `settings.hide_book_on_sleep_screen_description` | Keep the style and progress, but hide the cover and identifying text on sleep and power-off screens. Saved for this book only. | Сохраняет стиль и прогресс, но скрывает обложку и сведения о книге на экранах сна и выключения. Только для этой книги. | 126/118 |
| `settings.sleep_screen` | Sleep and power-off screen | Экран сна и выключения | 26/22 |
| `settings.section_modes` | Modes | Режимы | 5/6 |
| `settings.navigation_choice` | Navigation Choice | Способ навигации | 17/16 |
| `settings.swipe_direction` | Swipe Direction | Направление свайпа | 15/18 |
| `settings.reverse_swipe_direction` | Reverse swipe direction | Обратное направление свайпа | 23/27 |
| `settings.reverse_swipe_direction_hint` | Applies when page swipes are enabled. Does not change taps or buttons. | Только для включённых свайпов перелистывания. Не меняет касания и кнопки. | 70/73 |
| `sleep.read` | read | прочитано | 4/9 |
| `sleep.state_with_battery` | %1 (%2%) | %1 (%2%) | 8/8 |
| `sleep.goodnight` | Goodnight | Спокойной ночи | 9/14 |
| `sleep.battery_charged` | %1% charged | Заряжено на %1% | 11/15 |
| `sleep.greeting_title` | Night greeting | Ночное пожелание | 14/16 |
| `sleep.greeting_hint` | Shown on the Night sleep screen instead of Goodnight. Leave it empty to use Goodnight. | Вместо «Спокойной ночи» на ночном экране сна. Оставьте пустым для стандартного текста. | 86/86 |
| `sleep.missing_cover` | ? | ? | 1/1 |
| `sleep.private_book` | your book | ваша книга | 9/10 |
| `sleep.percent_through` | %1% through %2 | %1% книги «%2» | 14/14 |
| `sleep.now_reading` | Now reading | Сейчас читаю | 11/12 |
| `sleep.read_label` | Read | Прочитано | 4/9 |
| `sleep.today` | Today | Сегодня | 5/7 |
| `sleep.battery` | Battery | Батарея | 7/7 |
| `sleep.of_reading` | of reading | чтения | 10/6 |
| `sleep.time_remaining_in_book` | Time remaining<br>in this book | Осталось времени<br>в этой книге | 27/29 |
| `sleep.today_minutes` | %1m | %1 мин | 3/6 |
| `sleep.today_hours` | %1h %2m | %1 ч %2 мин | 7/11 |
| `sleep.letter_heading` | Reading desk | Письмо читателю | 12/15 |
| `sleep.letter_greeting` | Dear reader, | Дорогой читатель! | 12/17 |
| `sleep.letter_progress` | I have gone to sleep. You were %1 of the way through %2%3. | Я перешёл в спящий режим. Прочитано %1. Книга: %2%3. | 58/52 |
| `sleep.letter_progress_off` | I have switched off. You were %1 of the way through %2%3. | Я выключен. Прочитано %1. Книга: %2%3. | 57/38 |
| `sleep.letter_chapter` | , in “%1” | , глава «%1» | 9/12 |
| `sleep.letter_finished` | I have gone to sleep. You finished %1. | Я перешёл в спящий режим. Чтение завершено: %1. | 38/47 |
| `sleep.letter_finished_off` | I have switched off. You finished %1. | Я выключен. Чтение завершено: %1. | 37/33 |
| `sleep.letter_remaining` | About %1 of reading remains. | До конца книги — примерно %1. | 28/29 |
| `sleep.letter_charging` | I am charging in the meantime. | Тем временем я заряжаюсь. | 30/25 |
| `sleep.letter_battery` | The battery is at %1. | Заряд батареи: %1. | 21/18 |
| `sleep.letter_wake` | Press the power button and we will carry on with this book. | Нажмите кнопку питания, чтобы продолжить чтение. | 59/48 |
| `sleep.letter_restart` | Press the power button to start me again. | Нажмите кнопку питания, чтобы включить ридер. | 41/45 |
| `sleep.letter_closing` | Yours, | С уважением, | 6/12 |
| `sleep.gallery_title` | Sleep and power-off screen | Экран сна и выключения | 26/22 |
| `sleep.design_stock` | Use Kobo’s screen | Экран Kobo | 17/10 |
| `sleep.design_bookmark` | Bookmark | Закладка | 8/8 |
| `sleep.design_card` | Card | Карточка | 4/8 |
| `sleep.design_cover` | Cover only | Только обложка | 10/14 |
| `sleep.design_seal` | Seal | Печать | 4/6 |
| `sleep.design_medallion` | Stamp | Марка | 5/5 |
| `sleep.design_pill` | Pill | Плашка | 4/6 |
| `sleep.design_caption` | Caption | Подпись | 7/7 |
| `sleep.design_widgets` | Widgets | Виджеты | 7/7 |
| `sleep.design_framed` | Framed | Рамка | 6/5 |
| `sleep.design_night` | Night | Ночь | 5/4 |
| `sleep.design_ring` | Ring | Кольцо | 4/6 |
| `sleep.design_lines` | Lines | Линии | 5/5 |
| `sleep.design_letter` | Letter | Письмо | 6/6 |
| `sleep.design_page` | Reading page | Страница книги | 12/14 |
| `sleep.design_overlay` | PNG over text | PNG поверх текста | 13/17 |
| `navigation.search` | SEARCH | ПОИСК | 6/5 |
| `navigation.annotate` | ANNOTATE | ВЫДЕЛИТЬ | 8/8 |
| `navigation.bookmarks` | BOOKMARKS | ЗАКЛАДКИ | 9/8 |
| `navigation.annotations` | ANNOTATIONS | ПОМЕТКИ | 11/7 |
| `navigation.zen` | ZEN | ДЗЕН | 3/4 |
| `zen.reading_stats` | Reading stats | Статистика чтения | 13/17 |
| `zen.this_session` | This session | Этот сеанс | 12/10 |
| `zen.pages_turned` | Pages turned | Перелистано страниц | 12/19 |
| `zen.chapter_left` | Chapter left | До конца главы | 12/14 |
| `zen.hours` | h | ч | 1/1 |
| `zen.minutes` | min | мин | 3/3 |
| `stats.page_turn` | %1 page turn | Перелистываний: %1 | 12/18 |
| `stats.page_turns` | %1 page turns | Перелистываний: %1 | 13/18 |
| `opening.native` | Letting Kobo handle this book | Открытие в ридере Kobo | 29/22 |
| `opening.protected` | This book is protected | Книга защищена | 22/14 |
| `opening.remembered` | This book uses Kobo's reader | Эта книга открывается во встроенном ридере. | 28/43 |
| `settings.inpage_footnotes` | Footnotes below the text | Сноски внизу страницы | 24/21 |
| `settings.inpage_footnotes_description` | Show recognized notes below the text. Long notes continue on the next page. References keep their usual popup. | Показывает распознанные сноски под текстом. Длинные продолжаются на следующей странице. Ссылки по-прежнему открывают всплывающее окно. | 110/134 |
| `settings.section_dictionary_ui` | Dictionary | Словарь | 10/7 |
| `settings.zen_mode` | Zen mode | Режим дзен | 8/10 |
| `settings.zen_mode_description` | A special mode for those who want to focus on reading. Simplifies the reading menu user interface. | Режим для сосредоточенного чтения. Упрощает меню ридера. | 98/56 |
| `settings.zen_leave` | Hold to leave Zen mode | Удерживайте для выхода из дзен | 22/30 |
| `settings.zen_leave_holding` | Keep holding | Продолжайте удерживать | 12/22 |
| `settings.zen_leave_note` | All settings return when Zen mode is off. | При выходе из дзен все настройки возвращаются. | 41/46 |
| `settings.zen_leave_release` | Let go to stay in Zen mode. | Отпустите, чтобы остаться в дзен. | 27/33 |
| `settings.zen_light` | Light | Подсветка | 5/9 |
| `settings.zen_light_caption` | Brightness and warmth | Яркость и теплота | 21/17 |
| `settings.zen_light_brightness` | Brightness | Яркость | 10/7 |
| `settings.zen_about` | About Kobalt | О Kobalt | 12/8 |
| `kosync.ahead_title` | You read further on another device | На другом устройстве вы прочли дальше | 34/37 |
| `kosync.behind_title` | The synced position is behind you | Позиция синхронизации позади текущей | 33/36 |
| `kosync.stay_here` | Stay here | Остаться здесь | 9/14 |
| `kosync.jump_ahead` | Jump ahead | Перейти вперёд | 10/14 |
| `kosync.go_back` | Go back | Вернуться назад | 7/15 |
| `kosync.pages_ahead` | %1 pages ahead | Вперёд на %1 стр. | 14/17 |
| `kosync.pages_ahead_one` | 1 page ahead | Вперёд на 1 стр. | 12/16 |
| `kosync.pages_back` | %1 pages back | Назад на %1 стр. | 13/16 |
| `kosync.pages_back_one` | 1 page back | Назад на 1 стр. | 11/15 |
| `kosync.chapters_crossed` | %1 chapters | Глав: %1 | 11/8 |
| `kosync.chapters_crossed_one` | 1 chapter | 1 глава | 9/7 |
| `kosync.jump_label` | %1 · %2 | %1 · %2 | 7/7 |
| `kosync.page_number` | p. %1 | стр. %1 | 5/7 |
| `kosync.here` | Here | Здесь | 4/5 |
| `kosync.synced_label` | Synced | Синхронизировано | 6/16 |
| `kosync.percent` | %1% | %1% | 3/3 |
| `kosync.from` | From | Сейчас | 4/6 |
| `kosync.to` | To | После перехода | 2/14 |
| `kosync.device` | Device | Устройство | 6/10 |
| `kosync.synced` | Synced | Синхронизировано | 6/16 |
| `kosync.same_chapter` | Same chapter | Та же глава | 12/11 |
| `kosync.today_at` | Today at %1 | Сегодня в %1 | 11/12 |
| `kosync.send_metadata` | Send document metadata | Отправлять сведения о книге | 22/27 |
| `kosync.send_metadata_description` | Adds the file name, title and authors to each sync. Only some servers use them. | Добавляет имя файла, название и авторов к синхронизации. Используется не всеми серверами. | 79/89 |
| `kosync.yesterday_at` | Yesterday at %1 | Вчера в %1 | 15/10 |
| `settings.normalization_text_colors` | Normalize text colors | Нормализация цвета текста | 21/25 |
| `settings.normalization_text_colors_description` | Use black text, or white in dark mode. Remove publisher background colors. | Чёрный текст или белый в тёмной теме. Убирает фон издателя. | 74/59 |
| `settings.hidden_features` | Hidden features | Скрытые функции | 15/15 |
| `settings.hidden_features_none` | None | Нет | 4/3 |
| `settings.disable_features` | Hide unneeded features | Скрыть ненужные функции | 22/23 |
| `settings.disable_features_description` | Maybe you find yourself not needing some of the features Kobalt offers. You can hide what you don't need. | Можно скрыть функции Kobalt, которыми вы не пользуетесь. | 105/56 |
| `features.navigate` | Navigation | Навигация | 10/9 |
| `features.navigate_description` | Hides the Navigate tab and its options. If it is your default menu view, the default returns to None. | Скрывает вкладку навигации и её параметры. Если она выбрана стартовой, выбор сбрасывается на «Нет». | 101/99 |
| `features.stats` | Stats | Статистика | 5/10 |
| `features.stats_description` | Hides the Stats tab and its options. Reading history and time estimates continue. | Скрывает вкладку статистики и её параметры. История чтения и расчёт времени сохраняются. | 81/88 |
| `features.annotations` | Annotations | Аннотации | 11/9 |
| `features.annotations_description` | Hides annotation tools and the Annotations list. Saved highlights stay visible in your books. | Скрывает инструменты и список аннотаций. Сохранённые выделения остаются видны в книгах. | 93/87 |
| `features.bookmarks` | Bookmarks | Закладки | 9/8 |
| `features.bookmarks_description` | Hides bookmark controls, ribbons and pinned shortcuts. Saved bookmarks are kept. | Скрывает управление закладками, ленты и закреплённые ярлыки. Сохранённые закладки остаются. | 80/91 |
| `features.kosync` | KOSync | KOSync | 6/6 |
| `features.kosync_description` | Stops KOSync and hides its controls. Your saved account is kept. Kobo’s own sync is unaffected. | Останавливает KOSync и скрывает его настройки. Аккаунт сохраняется. Не влияет на синхронизацию Kobo. | 95/100 |
| `text_presets.title` | Text presets | Стили текста | 12/12 |
| `text_presets.gallery_title` | Text Presets | Стили текста | 12/12 |
| `text_presets.example_kobalt` | Kobalt | Kobalt | 6/6 |
| `text_presets.example_bookish` | Bookish | Книжный | 7/7 |
| `text_presets.example_modern` | Modern | Современный | 6/11 |
| `text_presets.example_classic` | Classic | Классический | 7/12 |
| `text_presets.none` | None | Нет | 4/3 |
| `text_presets.all` | All | Все | 3/3 |
| `text_presets.show` | Show text presets | Показать стили текста | 17/21 |
| `text_presets.hide` | Hide text presets | Скрыть стили текста | 17/19 |
| `text_presets.modified` | %1* | %1* | 3/3 |
| `text_presets.modified_name` | %1, changed | %1, изменён | 11/11 |
| `text_presets.update` | Update selected preset | Обновить выбранный стиль | 22/24 |
| `text_presets.save_as_new` | Save as new… | Сохранить как новый… | 12/20 |
| `text_presets.save_title` | Save Text Preset | Сохранение стиля текста | 16/23 |
| `text_presets.rename_title` | Rename Text Preset | Переименование стиля | 18/20 |
| `text_presets.name` | Preset name | Название стиля | 11/14 |
| `text_presets.name_taken` | Another preset already has this name. | Стиль с таким названием уже есть. | 37/33 |
| `text_presets.limit` | Kobalt keeps up to %1 presets. Delete one in Settings to save another. | Kobalt хранит до %1 стилей. Для нового удалите один в настройках. | 70/65 |
| `text_presets.could_not_save` | Could not save the text presets. | Не удалось сохранить стили текста. | 32/34 |
| `text_presets.rename` | Rename | Переименовать | 6/13 |
| `text_presets.update_from_current` | Update from current text | Обновить из текущего текста | 24/27 |
| `text_presets.delete` | Delete preset | Удалить стиль | 13/13 |
| `text_presets.delete_prompt` | Delete %1? | Удалить %1? | 10/11 |
| `text_presets.delete_detail` | Books keep their text. Only the preset is removed. | Текст книг не изменится. Удаляется только стиль. | 50/48 |
| `text_presets.restore` | Restore default presets | Восстановить стандартные стили | 23/30 |
| `text_presets.restore_prompt` | Restore Default Presets? | Восстановить стандартные стили? | 24/31 |
| `text_presets.restore_detail` | The presets that come with Kobalt return as they shipped. Your own presets stay. The presets apply when you close Settings. | Встроенные стили вернутся к исходным. Ваши стили сохранятся. Изменения применятся после закрытия настроек. | 123/106 |
| `text_presets.restore_confirm` | Restore presets | Восстановить стили | 15/18 |
| `text_presets.restore_selected` | Default presets selected | Выбраны стандартные стили | 24/25 |
| `text_presets.gallery_hint` | Tap to apply · hold for options | Нажмите для выбора · удерживайте для действий | 31/45 |
| `text_presets.save_current` | Save current text | Сохранить текущий стиль | 17/23 |
| `settings.text_for_new_books` | Text for new books | Текст новых книг | 18/16 |
| `settings.text_for_new_books_title` | Text for New Books | Текст новых книг | 18/16 |
| `settings.new_book_text_heading` | A new book starts with | Новая книга начинается с | 22/24 |
| `settings.new_book_book_default` | Book default | Как в книге | 12/11 |
| `settings.new_book_last_used` | Same as last book | Как в прошлой книге | 17/19 |
| `text_presets.used_for_new_books` | Used for new books | Для новых книг | 18/14 |
| `search_panel.scope` | Search in | Искать в | 9/8 |
| `search_panel.current_chapter` | Current chapter | Текущей главе | 15/13 |
| `search_panel.up_to_here` | Up to this page | До этой страницы | 15/16 |
| `search_panel.after_here` | After this page | После этой страницы | 15/19 |
| `search_panel.whole_book` | Whole book | Всей книге | 10/10 |
| `search_panel.go_to_page` | Go to results page | Перейти к странице результатов | 18/30 |
| `search_panel.page_range` | Enter a page from 1 to %1 | Введите страницу от 1 до %1 | 25/27 |
| `search_panel.go` | Go | Перейти | 2/7 |
| `search_panel.clear_page` | Clear page number | Убрать номер страницы | 17/21 |
| `search_panel.erase_digit` | Erase digit | Стереть цифру | 11/13 |
| `search_panel.now_searching` | Now searching: %1 | Сейчас поиск: %1 | 17/16 |
| `search_panel.clear_number` | Clear | Очистить | 5/8 |
| `search_panel.whole_book_description` | Searches the whole book for your search term. | Ищет указанный текст во всей книге. | 45/35 |
| `search_panel.current_chapter_description` | Searches the current chapter only, including unread pages. | Ищет только в текущей главе, включая непрочитанные страницы. | 58/60 |
| `search_panel.up_to_here_description` | Searches all previous pages, but nothing after the page that's open. | Ищет на всех предыдущих страницах, но не после открытой. | 68/56 |
| `search_panel.after_here_description` | Searches all upcoming pages, but nothing from prior pages. | Ищет на всех следующих страницах, но не на предыдущих. | 58/54 |
| `words.tab` | Words | Слова | 5/5 |
| `words.title` | My Words | Мои слова | 8/9 |
| `words.added` | Added to My Words! | Добавлено в «Мои слова»! | 18/24 |
| `words.removed` | Removed from My Words | Удалено из «Моих слов» | 21/22 |
| `words.update_failed` | Could not update My Words | Не удалось обновить «Мои слова» | 25/31 |
| `words.count` | %1 words | Слов: %1 | 8/8 |
| `words.count_one` | 1 word | 1 слово | 6/7 |
| `words.empty` | No words yet | Пока нет слов | 12/13 |
| `words.empty_hint` | Words you look up in the dictionary are saved here. | Здесь сохраняются слова, которые вы ищете в словаре. | 51/52 |
| `words.loading` | Loading words… | Загрузка слов… | 14/14 |
| `words.remove` | Remove word | Удалить слово | 11/13 |
| `words.all_books` | All books | Все книги | 9/9 |
| `words.untitled` | Untitled book | Без названия | 13/12 |
| `words.filter` | Filter | Фильтр | 6/6 |
| `words.sort_alphabetical` | A to Z | От А до Я | 6/9 |
| `words.sort_alphabetical_description` | Words in alphabetical order. | Слова в алфавитном порядке. | 28/27 |
| `words.definition` | Definition | Значение | 10/8 |
| `words.in_context` | In context | В контексте | 10/11 |
| `words.go_to_page` | Go to page %1 | К странице %1 | 13/13 |
| `words.open_passage` | Open at this passage | Открыть этот отрывок | 20/20 |
| `words.book_missing` | Book not on this device | Книги нет на устройстве | 23/23 |
| `words.open_book_first` | Open this book to go to the passage. | Откройте книгу для перехода к отрывку. | 36/38 |
| `words.passage_missing` | This passage is no longer in the book. | Этого отрывка больше нет в книге. | 38/33 |
| `words.looked_up` | looked up %1× | поиск: %1 раз | 13/13 |
| `words.first_looked_up` | first on %1 | впервые: %1 | 11/11 |
| `dictionary_panel.save_to_words` | Save to My Words | Сохранить в «Мои слова» | 16/23 |
| `dictionary_panel.remove_from_words` | Remove from My Words | Удалить из «Моих слов» | 20/22 |
| `settings.save_words` | Automatically save looked-up words | Сохранять найденные слова | 34/25 |
| `settings.save_words_description` | Words that have a definition are added to My Words. | Слова с определением добавляются в «Мои слова». | 51/47 |
| `library.my_words` | My Words | Мои слова | 8/9 |
| `words.remove_prompt` | Remove this word? | Удалить это слово? | 17/18 |
| `words.remove_detail` | “%1” will be removed from My Words. | «%1» будет удалено из «Моих слов». | 35/34 |
| `words.remove_confirm` | Remove | Удалить | 6/7 |
| `dictionary_panel.actions` | Lookup actions | Действия со словом | 14/18 |
| `features.words` | My Words | Мои слова | 8/9 |
| `features.words_description` | Hides the Words tab, the dictionary's save button and My Words in the Library. Lookups are no longer saved; saved words are kept. | Скрывает вкладку слов, кнопку сохранения в словаре и «Мои слова» в библиотеке. Новые слова не сохраняются; старые остаются. | 129/123 |
| `words.no_entry` | No dictionary entry · Added by you | Нет в словаре · добавлено вами | 34/30 |
| `words.no_entry_detail` | The dictionary had no entry for this word. You added it to My Words yourself. | В словаре не было этого слова. Вы добавили его в «Мои слова» сами. | 77/66 |
| `library.resource_folders` | Resource folders | Папки ресурсов | 16/14 |
| `library.resource_folders_description` | Choose folders for dictionaries, fonts and custom icons. Changes load when you next open a book. | Выберите папки словарей, шрифтов и своих значков. Изменения загрузятся при следующем открытии книги. | 96/100 |
| `library.dictionary_folder` | Dictionaries | Словари | 12/7 |
| `library.font_folder` | Fonts | Шрифты | 5/6 |
| `library.icon_folder` | Custom icons | Свои значки | 12/11 |
| `library.folder_not_selected` | Choose a folder | Выберите папку | 15/14 |
| `library.clear_folder` | Clear folder choice | Сбросить выбор папки | 19/20 |
| `search_panel.match` | %1 match | Совпадений: %1 | 8/14 |
| `search_panel.matches_in_scope` | %1 in %2 | %1 — %2 | 8/7 |
| `search_panel.matches_so_far_in_scope` | %1 so far in %2 | Пока %1 — %2 | 15/12 |
| `search_panel.search_complete` | Search complete | Поиск завершён | 15/14 |
| `search_panel.search_stopped` | Search stopped | Поиск остановлен | 14/16 |
| `search_panel.result_pages` | Page %1 of %2 | Страница %1 из %2 | 13/17 |
| `search_panel.result_pages_so_far` | Page %1 of at least %2 | Страница %1 из как минимум %2 | 22/29 |
