# Ревью переводов интеграции upstream — 2026-11-01

Это полный список значений, добавленных или изменённых в `f3686f9` относительно
`b68fe9c`: **218 новых ключей и 10 изменённых**. Русские значения приведены
точно как в сборке, без исправлений после замечаний пользователя. Переносы
строк показаны как `<br>`. Совпадение ключей и подстановок не доказывает
смысловую точность перевода или отсутствие обрезки на Kobo.

## Замечания, требующие согласования и проверки

- **«Какие страницы»** (`settings.vertical_spacing_apply_to`, `settings.vertical_spacing_scope`): это не выбор номеров страниц, а область применения коррекции свободного места по высоте. Предлагается «Область применения». Вариант `protection` означает только пробелы, образовавшиеся из-за защиты от висячих строк, а не сами висячие строки. «Только висячие строки» неточно; предложение: «После защиты от висячих строк». Описания «обычные пробелы пагинации» также требуют замены на понятное «свободное место внизу страницы».
- **«Оформление экрана сна»** (`settings.sleep_tile`): функция меняет только информационную плашку поверх экрана сна, не весь экран/обложку. Предложение: «Плашка на экране сна». Карточка, нижняя полоса и лента-закладка — три формы этой плашки.
- **«Очень большой»** в выборе размера словаря обрезается на screen_002. В строке справа резервируется ширина превью, растущая вместе со шрифтом. Возможные решения: «Максимальный» и более короткий текст образца либо изменение размещения подписи/превью; один перевод не гарантирует исправление всех шрифтов.
- Нижние **«НАВИГАЦИЯ»/«РАСКЛАДКА»** на screen_001: тексты не менялись при интеграции. Это замечание к геометрии/отрисовке, не основание сокращать принятый перевод. Upstream изменил вычисление высоты надписи и добавил масштаб интерфейса. Без воспроизведения на устройстве причина обрезки пока не подтверждена.
- Вкладка **«Синхронизация»** на screen_002 также тесно помещается в боковой панели. Кандидат для узкой вкладки: «KOsync»; заголовки и описания могут оставаться развёрнутыми.
- «По содержимому (по умолчанию)» (`kosync.match_binary`) означает совпадение одного и того же файла, не распознавание одинакового текста в разных изданиях. Описание это уточняет; компактный кандидат: «По файлу».
- `settings.justification_tracking_off`: «Обычная» соответствует Normal в контексте подгонки, а не явному включению дополнительного межбуквенного интервала.
- Recovery-строки описывают реальное upstream-поведение: оригиналы переносятся в backups/, приложение может начать с пустыми данными. Не смягчать их до «всё восстановлено».
- `dictionary_panel.update_lookup` — обновляет поисковый запрос, а не словарные файлы; «Обновить поиск» следует проверить на понятность в самом диалоге.
- `reader.book_beautifier` и настройки нормализации: исходные файлы не меняются. В нашей интеграции EPUB-разметка нормализации не применяется к семантическим FB2-блокам; обещание «во всех книгах» требует отдельной проверки и уточнения для FB2.

Ни один из предложенных вариантов в этом документе ещё не внесён в таблицу строк.

## Новые значения

### reader

| Ключ | English | Русский в сборке |
|---|---|---|
| `reader.book_beautifier` | Book normalization | Нормализация книги |
| `reader.publisher_spacing` | Publisher | Издатель |

### kosync

| Ключ | English | Русский в сборке |
|---|---|---|
| `kosync.title` | Sync | Синхронизация |
| `kosync.description` | Sync progress with KOReader and Kobalt. | Синхронизация прогресса с KOReader и Kobalt. |
| `kosync.not_set` | Tap to enter | Нажмите для ввода |
| `kosync.server` | Server URL | Адрес сервера |
| `kosync.username` | Username | Имя пользователя |
| `kosync.password` | Password | Пароль |
| `kosync.login` | Sign in | Войти |
| `kosync.logout` | Sign out | Выйти |
| `kosync.automatic` | Automatic sync | Автосинхронизация |
| `kosync.automatic_description` | Sync on open, close, sleep and wake, and while reading. Ask before moving to a synced position. | При открытии, закрытии, сне, пробуждении и чтении. Переход к полученной позиции — с подтверждением. |
| `kosync.sync_now` | Sync now | Синхронизировать |
| `kosync.manual_description` | With automatic sync disabled, a manual sync button is added to the top toolbar. | Без автосинхронизации на верхней панели появляется кнопка ручной синхронизации. |
| `kosync.manual_prompt` | Would you like to sync progress with KOsync? | Синхронизировать прогресс с KOsync? |
| `kosync.manual_prompt_description` | This button syncs progress when you choose. Automatic sync is disabled, so Kobalt will not sync on open, close, sleep or wake. Later taps sync immediately. | Эта кнопка запускает синхронизацию вручную. Автосинхронизация выключена: при открытии, закрытии, сне и пробуждении обмена нет. Следующие нажатия запускают обмен сразу. |
| `kosync.show_password` | Show | Показать |
| `kosync.hide_password` | Hide | Скрыть |
| `kosync.show_password_accessible` | Show password | Показать пароль |
| `kosync.hide_password_accessible` | Hide password | Скрыть пароль |
| `kosync.signed_in` | Signed in. | Вход выполнен. |
| `kosync.account_changed` | Sign in to apply the server or username change. | Войдите заново, чтобы применить новый сервер или имя пользователя. |
| `kosync.signed_out` | Not signed in. | Вход не выполнен. |
| `kosync.signing_in` | Signing in… | Вход… |
| `kosync.opening` | Syncing… | Синхронизация… |
| `kosync.completed` | Sync complete | Синхронизация завершена |
| `kosync.cancel_sync` | Cancel sync | Отменить обмен |
| `kosync.opening_description` | Kobalt is syncing your progress. Please wait... | Kobalt синхронизирует прогресс. Подождите... |
| `kosync.skipped` | Sync skipped. Your position is saved locally. | Обмен пропущен. Позиция сохранена на устройстве. |
| `kosync.saved_locally` | Could not sync. Your position is saved locally. | Обмен не удался. Позиция сохранена на устройстве. |
| `kosync.syncing` | Syncing progress… | Синхронизация прогресса… |
| `kosync.up_to_date` | Reading progress is up to date. | Прогресс чтения актуален. |
| `kosync.pending` | Return to reading to choose which position to keep. | Вернитесь к чтению, чтобы выбрать позицию. |
| `kosync.retry_later` | The reader is busy. Sync will retry later. | Ридер занят. Повторим обмен позже. |
| `kosync.busy` | A sync request is already running. | Синхронизация уже выполняется. |
| `kosync.storage_error` | Could not read or save the sync account. Existing account data was kept. | Не удалось прочитать или сохранить учётную запись. Прежние данные сохранены. |
| `kosync.auth_error` | Sign-in failed. Check your username and password. | Не удалось войти. Проверьте имя пользователя и пароль. |
| `kosync.network_error` | Could not reach the sync server. Check Wi-Fi or your internet connection and the server URL. | Сервер недоступен. Проверьте Wi-Fi, подключение к интернету и адрес сервера. |
| `kosync.connecting` | Connecting to Wi-Fi… | Подключение к Wi-Fi… |
| `kosync.invalid_response` | The sync server returned invalid progress. Your reading position was kept. | Сервер вернул неверные данные прогресса. Ваша позиция сохранена. |
| `kosync.invalid_account` | Enter an HTTPS server URL, username and password. The URL cannot contain credentials, a query or a fragment. | Введите HTTPS-адрес сервера, имя и пароль. Адрес не должен содержать данные входа, параметры запроса или фрагмент. |
| `kosync.native_sync` | This book uses Kobo’s built-in sync system. KOsync is unavailable for this copy. | Эта книга использует встроенную синхронизацию Kobo. KOsync для этой копии недоступен. |
| `kosync.unavailable` | Sync is unavailable for this book or account. | Синхронизация недоступна для этой книги или учётной записи. |
| `kosync.position_error` | This synced position could not be opened. Your position was kept and automatic sync was disabled. | Не удалось открыть полученную позицию. Ваша позиция сохранена, автосинхронизация выключена. |
| `kosync.resume_title` | Resume synced reading? | Продолжить с полученной позиции? |
| `kosync.resume_description` | Another device saved a position at %1% (%2). Choose which position to keep. | Другое устройство сохранило позицию %1% (%2). Выберите, какую позицию оставить. |
| `kosync.keep_local` | Keep this position | Оставить текущую |
| `kosync.use_remote` | Use synced position | Принять полученную |
| `kosync.kept_local` | Your position was kept. It will be sent on the next sync. | Ваша позиция сохранена. Отправим её при следующем обмене. |
| `kosync.resumed` | Resumed at the synced position. | Чтение продолжено с полученной позиции. |
| `kosync.last_synced` | Last synced: %1 | Последний обмен: %1 |
| `kosync.never` | Never | Никогда |
| `kosync.sideload_copy` | To use KOsync, download a DRM-free copy of the book and add it to your device separately. | Для KOsync скачайте копию без DRM и добавьте её на устройство отдельно. |
| `kosync.learn_more` | Learn More | Подробнее |
| `kosync.learn_more_title` | Kobo sync and KOsync | Синхронизация Kobo и KOsync |
| `kosync.learn_more_description` | Kobo-managed books use Kobo’s built-in sync, even when the file has no DRM. Kobalt leaves their progress syncing to Kobo.<br><br>KOsync is a separate service for KOReader and Kobalt. To use it for this title, add a DRM-free copy to your device as a separate sideloaded book and open that copy.<br><br>Use the same book file on every device. Only reading progress is synced; books, bookmarks and annotations are not transferred. | Книги из магазина Kobo используют встроенную синхронизацию Kobo, даже без DRM. Kobalt оставляет обмен прогрессом Kobo.<br><br>KOsync — отдельный сервис для KOReader и Kobalt. Для этой книги добавьте копию без DRM как отдельную загруженную вручную книгу и откройте её.<br><br>На всех устройствах используйте один файл книги. Передаётся только прогресс чтения, без книг, закладок и аннотаций. |
| `kosync.device_name` | Device name | Имя устройства |
| `kosync.device_name_saved` | Device name saved. | Имя устройства сохранено. |
| `kosync.matching` | Document matching | Сопоставление книг |
| `kosync.match_binary` | Binary (default) | По содержимому (по умолчанию) |
| `kosync.match_filename` | Filename | По имени файла |
| `kosync.match_binary_description` | Match identical book files, even if renamed. Use the same matching method on every device. | Сопоставляет одинаковые файлы книг, даже переименованные. Используйте один способ на всех устройствах. |
| `kosync.match_filename_description` | Match the exact filename, including its extension. Different files with the same name share progress. | Точное имя файла, включая расширение. Разные файлы с одинаковым именем имеют общий прогресс. |
| `kosync.matching_changed` | Document matching changed. | Способ сопоставления изменён. |
| `kosync.filename_unavailable` | The original filename is unavailable. Reimport this book on Android, or choose Binary matching. | Исходное имя файла недоступно. Импортируйте книгу на Android заново или выберите сопоставление по содержимому. |
| `kosync.comparing_positions` | Comparing reading positions… | Сравнение позиций чтения… |
| `kosync.position_comparison` | On this device · %1%<br>%2<br><br>Synced position · %3%<br>%4<br>Device: %5<br>%6<br><br>%7 | На этом устройстве · %1%<br>%2<br><br>Полученная позиция · %3%<br>%4<br>Устройство: %5<br>%6<br><br>%7 |
| `kosync.direction_forward` | Accepting the synced position would move you forward in the book. | Полученная позиция находится дальше по книге. |
| `kosync.direction_backward` | This would move you backward in the book. Check both positions before replacing your current position. | Это вернёт вас назад по книге. Проверьте обе позиции, прежде чем заменять текущую. |
| `kosync.direction_same` | Both positions are at the same place in the book. | Обе позиции указывают на одно место в книге. |
| `kosync.direction_unknown` | The synced position could not be compared with your current position. | Не удалось сравнить полученную позицию с текущей. |
| `kosync.resume_backward_title` | Move backward to synced position? | Вернуться к полученной позиции? |
| `kosync.chapter_unavailable` | Chapter unavailable | Глава недоступна |
| `kosync.device_unknown` | Unknown device | Неизвестное устройство |
| `kosync.time_unavailable` | Sync time unavailable | Время обмена неизвестно |
| `kosync.synced_at` | Synced: %1 | Обмен: %1 |

### library

| Ключ | English | Русский в сборке |
|---|---|---|
| `library.exit_to_library` | Library | Библиотека |

### common

| Ключ | English | Русский в сборке |
|---|---|---|
| `common.size_extra_large_short` | XL | XL |
| `common.done` | Done | Готово |

### components

| Ключ | English | Русский в сборке |
|---|---|---|
| `components.wifi_off` | Wi-Fi off | Wi-Fi выключен |
| `components.wifi_disconnected` | Wi-Fi on, not connected | Wi-Fi включён, соединения нет |
| `components.wifi_connected` | Wi-Fi connected | Wi-Fi подключён |
| `components.wifi_signal` | Wi-Fi signal: %1 of 4 | Сигнал Wi-Fi: %1 из 4 |

### dictionary_panel

| Ключ | English | Русский в сборке |
|---|---|---|
| `dictionary_panel.edit_lookup` | Edit lookup | Изменить запрос |
| `dictionary_panel.lookup_word` | Word or phrase to look up | Слово или фраза для поиска |
| `dictionary_panel.update_lookup` | Update lookup | Обновить поиск |
| `dictionary_panel.cancel_edit` | Cancel lookup edit | Отменить правку запроса |
| `dictionary_panel.edit_hint` | Update the word or phrase to look up in the dictionary. | Измените слово или фразу для поиска в словаре. |
| `dictionary_panel.dictionary_sources` | Dictionary sources: %1 | Источники словарей: %1 |

### error

| Ключ | English | Русский в сборке |
|---|---|---|
| `error.profile_already_in_use` | This profile is open in another Kobalt app. Close it there and try again. | Этот профиль открыт в другом Kobalt. Закройте его там и повторите попытку. |
| `error.profile_storage_failed` | Kobalt couldn't read its saved data or move it to a backup. The storage may be failing. Repair the file system or reset the device. | Не удалось прочитать данные или перенести их в резервную копию. Возможна неисправность накопителя. Исправьте файловую систему или сбросьте устройство. |

### header_footer

| Ключ | English | Русский в сборке |
|---|---|---|
| `header_footer.progress_bar_detail` | % of book | % книги |
| `header_footer.field.battery_percent_only` | Battery percentage only | Только процент заряда |

### closing

| Ключ | English | Русский в сборке |
|---|---|---|
| `closing.saving` | Saving your progress… | Сохранение прогресса… |
| `closing.may_take_a_while` | This may take a few moments. | Это может занять некоторое время. |
| `closing.failed` | Couldn't save your progress | Не удалось сохранить прогресс |
| `closing.failed_detail` | Your progress couldn’t be saved, so the book is still open. Please try again. If this keeps happening, back up your data. | Прогресс не сохранён, поэтому книга остаётся открытой. Повторите попытку. Если ошибка повторяется, сделайте резервную копию данных. |
| `closing.retry` | Retry save and close | Сохранить и закрыть |
| `closing.quit` | Quit without saving | Выйти без сохранения |
| `closing.quit_warning` | Quitting now may lose unsaved progress or annotations. | При выходе могут потеряться несохранённый прогресс и аннотации. |
| `closing.return` | Keep reading | Продолжить чтение |

### settings

| Ключ | English | Русский в сборке |
|---|---|---|
| `settings.section_paragraphs` | Paragraphs | Абзацы |
| `settings.paragraph_spacing` | Paragraph adjustments | Настройка абзацев |
| `settings.paragraph_spacing_summary` | Adjust spacing and first-line indents. | Интервалы и отступы первой строки. |
| `settings.paragraph_spacing_description` | Values are percentages of the text size: with 12 pt text, a 100% indent is 12 pt. Applies to all books. Headings and special blocks keep their formatting. | Значения — в процентах от размера текста: при 12 пт отступ 100% равен 12 пт. Для всех книг. Заголовки и особые блоки сохраняют оформление. |
| `settings.paragraph_choices_description` | Each Publisher choice keeps that part of the book’s formatting. Diagonal lines mark extra space between paragraphs. | «Издатель» сохраняет соответствующее оформление книги. Штриховка обозначает дополнительный интервал между абзацами. |
| `settings.paragraph_gap` | Spacing | Интервал |
| `settings.paragraph_indent` | First-line indent | Отступ первой строки |
| `settings.paragraph_adjustments_value` | Gap %1 · Indent %2 | Интервал %1 · Отступ %2 |
| `settings.session_timeout` | New session after sleep | Новая сессия после сна |
| `settings.session_timeout_description` | Waking your device after this amount of time starts a new reading session. You do not need to close and reopen your book, this is automatic. | Пробуждение после этого времени начинает новую сессию чтения автоматически. Закрывать и открывать книгу не нужно. |
| `settings.session_timeout_15` | 15 minutes | 15 минут |
| `settings.session_timeout_60` | 1 hour | 1 час |
| `settings.session_timeout_240` | 4 hours | 4 часа |
| `settings.session_timeout_360` | 6 hours | 6 часов |
| `settings.session_timeout_720` | 12 hours | 12 часов |
| `settings.session_timeout_0` | Never | Никогда |
| `settings.session_timeout_never_description` | A session always continues when the device wakes, even after a longer period of inactivity. | После пробуждения продолжается прежняя сессия, даже после долгого простоя. |
| `settings.section_words_letters` | Words and letters | Слова и буквы |
| `settings.section_page_breaks` | Page breaks | Разрывы страниц |
| `settings.section_smart_styling` | Smart styling | Умные стили |
| `settings.styling_preview_same` | These sample pages look the same with every option. | При всех вариантах эти образцы выглядят одинаково. |
| `settings.sleep_tile` | Sleep screen style | Оформление экрана сна |
| `settings.sleep_tile_card` | Card | Карточка |
| `settings.sleep_tile_card_description` | Default. A card in the lower-left corner. | По умолчанию. Карточка в левом нижнем углу. |
| `settings.sleep_tile_band` | Footer band | Нижняя полоса |
| `settings.sleep_tile_band_description` | A strip along the bottom edge. Its top line shows how much you have read. | Полоса вдоль нижнего края. Верхняя линия показывает прогресс чтения. |
| `settings.sleep_tile_bookmark` | Bookmark | Закладка |
| `settings.sleep_tile_bookmark_description` | A ribbon that hangs from the top edge and shows the percentage read. | Лента от верхнего края с процентом прочитанного. |
| `settings.lookup` | Lookup | Поиск |
| `settings.avoid_wasteful_styling_changes` | What this changes: | Что меняется: |
| `settings.avoid_wasteful_styling_margins` | Ignores the book's own page margins and padding, so your Layout margins apply. | Поля и внутренние отступы книги игнорируются; применяются поля из «Раскладки». |
| `settings.avoid_wasteful_styling_breaks` | Removes the space a book adds above text that starts on a new page, such as a chapter opening. | Убирает отступ над текстом, который начинается с новой страницы, например в начале главы. |
| `settings.avoid_wasteful_styling_illustrations` | Preserves the book's space above illustrations at chapter starts. | Сохраняет отступ над иллюстрациями в начале глав. |
| `settings.section_dictionary_ui` | Dictionary UI | Интерфейс словаря |
| `settings.dictionary_font` | Dictionary font | Шрифт словаря |
| `settings.dictionary_font_size` | Dictionary font size | Размер шрифта словаря |
| `settings.dictionary_font_small` | Small | Маленький |
| `settings.dictionary_font_standard` | Standard | Обычный |
| `settings.dictionary_font_large` | Large | Большой |
| `settings.dictionary_font_extra_large` | Extra large | Очень большой |
| `settings.dictionary_font_sample` | A short definition. | Краткое определение. |
| `settings.tappable_links` | Tappable links | Ссылки по касанию |
| `settings.tappable_links_description` | Choose how to open footnotes and links while reading. | Как открывать сноски и ссылки при чтении. |
| `settings.tappable_links_off` | Always require long press | Только долгое нажатие |
| `settings.tappable_links_footnotes` | Footnotes only | Только сноски |
| `settings.tappable_links_all` | Footnotes and links | Сноски и ссылки |
| `settings.tappable_links_off_description` | Long press and choose Follow link to avoid accidental opening. | Для защиты от случайного открытия удерживайте ссылку и выберите «Перейти по ссылке». |
| `settings.tappable_links_footnotes_description` | Tap footnotes to open them. Long press other links, then choose Follow link. | Касайтесь сносок для открытия. Другие ссылки удерживайте, затем выберите «Перейти по ссылке». |
| `settings.tappable_links_all_description` | Tap footnotes and links to open them. | Касайтесь сносок и ссылок для открытия. |
| `settings.vertical_spacing` | Vertical page spacing | Вертикальные интервалы |
| `settings.vertical_spacing_mode` | Page spacing | Интервалы страницы |
| `settings.vertical_spacing_apply_to` | Pages to adjust | Какие страницы |
| `settings.vertical_spacing_description` | Adjust blank space on nearly full pages. | Заполнение почти полных страниц. |
| `settings.vertical_spacing_off` | Normal | Обычные |
| `settings.vertical_spacing_off_description` | Keep the usual line spacing and top alignment. | Обычный межстрочный интервал, выравнивание по верху. |
| `settings.vertical_spacing_expand` | Expand line spacing | Растянуть строки |
| `settings.vertical_spacing_expand_description` | Spread spare height between lines. Keep the first line in place. | Распределяет свободную высоту между строками. Первая строка остаётся на месте. |
| `settings.vertical_spacing_center` | Center nearly full pages | Центрировать текст |
| `settings.vertical_spacing_center_description` | Center prose that occupies at least 90% of the reading height. Keep line spacing unchanged. | Центрирует прозу, занимающую не менее 90% высоты чтения. Межстрочный интервал не меняется. |
| `settings.vertical_spacing_scope` | Pages to adjust | Какие страницы |
| `settings.vertical_spacing_scope_description` | Choose which page gaps to adjust. | Какие пробелы по высоте корректировать. |
| `settings.vertical_spacing_scope_all` | All suitable pages | Все подходящие |
| `settings.vertical_spacing_scope_all_description` | Also adjust ordinary pagination gaps on nearly full prose pages. | Также корректирует обычные пробелы пагинации на почти полных страницах прозы. |
| `settings.vertical_spacing_scope_protection` | Only widow/orphan gaps | Только висячие строки |
| `settings.vertical_spacing_scope_protection_description` | Adjust only when widow/orphan protection moves lines that would otherwise fit. | Корректирует лишь когда защита от висячих строк переносит строки, которые иначе поместились бы. |
| `settings.beautifier_summary` | Choose how text initials and ordinary paragraphs are normalized. | Нормализация буквиц и обычных абзацев. |
| `settings.beautifier_description` | Experimental. Checked rules apply to all books. Uncheck every rule to turn off. Your book files are never changed. | Экспериментально. Отмеченные правила действуют во всех книгах. Снимите все галочки для отключения. Файлы книг не меняются. |
| `settings.beautifier_initials` | Normalize opening letters | Нормализовать буквицы |
| `settings.beautifier_initials_description` | Turn recognized text drop caps into ordinary letters, keeping every character. | Превращает распознанные текстовые буквицы в обычные буквы без потери символов. |
| `settings.beautifier_spacing` | Remove paragraph gaps | Убрать интервалы абзацев |
| `settings.beautifier_spacing_description` | Remove extra space below ordinary prose. Keep recognized scene breaks and special passages. | Убирает лишний отступ под обычной прозой. Сохраняет разрывы сцен и особые фрагменты. |
| `settings.beautifier_indent` | Standardize first-line indents | Единые отступы абзацев |
| `settings.beautifier_indent_description` | Indent ordinary prose consistently. Your explicit paragraph adjustments take priority. | Единый отступ первой строки прозы. Ваши явные настройки абзацев имеют приоритет. |
| `settings.beautifier_active` | On · %1 of 3 rules | Вкл. · %1 из 3 правил |

### sleep

| Ключ | English | Русский в сборке |
|---|---|---|
| `sleep.powered_off` | Powered off | Выключено |

### dictionaries

| Ключ | English | Русский в сборке |
|---|---|---|
| `dictionaries.download` | Download | Скачать |
| `dictionaries.download_title` | Download %1? | Скачать %1? |
| `dictionaries.download_description` | This is a %1 download.<br>This dictionary is licensed under %2. | Размер загрузки: %1.<br>Лицензия словаря: %2. |

### opening

| Ключ | English | Русский в сборке |
|---|---|---|
| `opening.storage_failed` | Kobalt can't use its saved data | Сохранённые данные недоступны |
| `opening.storage_failed_detail` | Kobalt couldn't read its saved data or move it to a backup. The storage may be failing. Repair the file system or reset the device. Kobalt will close now. | Не удалось прочитать данные или перенести их в резервную копию. Возможна неисправность накопителя. Исправьте файловую систему или сбросьте устройство. Kobalt закроется. |

### recovery

| Ключ | English | Русский в сборке |
|---|---|---|
| `recovery.title` | Saved data reset | Сохранённые данные сброшены |
| `recovery.settings` | Kobalt's settings could not be used, so the defaults are back in use. | Настройки Kobalt недоступны. Восстановлены значения по умолчанию. |
| `recovery.sync_account` | The sync account could not be used. Sign in again to keep syncing. | Учётная запись синхронизации недоступна. Войдите заново для продолжения обмена. |
| `recovery.database` | Kobalt's reading positions, bookmarks and annotations could not be used. Kobalt started again with empty saved data. | Позиции чтения, закладки и аннотации Kobalt недоступны. Работа начата с пустыми данными. |
| `recovery.backups` | The original files are in %1. | Исходные файлы находятся в %1. |

### benchmark

| Ключ | English | Русский в сборке |
|---|---|---|
| `benchmark.finished` | Benchmark finished | Тест завершён |
| `benchmark.failed` | Benchmark failed | Ошибка теста |
| `benchmark.counts` | Completed: %1. Skipped: %2. Failed: %3. | Готово: %1. Пропущено: %2. Ошибок: %3. |
| `benchmark.first_failure` | The first failure was %1. | Первая ошибка: %1. |
| `benchmark.first_failure_reason` | The first failure was %1: %2. | Первая ошибка: %1: %2. |
| `benchmark.not_saved` | The results could not be saved. | Не удалось сохранить результаты. |
| `benchmark.no_scenarios` | No scenarios ran. Check the scenario file and its filter. | Сценарии не запущены. Проверьте файл сценариев и фильтр. |
| `benchmark.connect` | Connect the Kobo to a computer and open %1 to see the results. | Подключите Kobo к компьютеру и откройте %1 для просмотра результатов. |

### annotations

| Ключ | English | Русский в сборке |
|---|---|---|
| `annotations.export` | Export all annotations | Экспорт всех аннотаций |
| `annotations.more` | More annotation actions | Другие действия с аннотациями |
| `annotations.export_markdown` | Export as Markdown… | Экспорт в Markdown… |
| `annotations.export_html` | Export as HTML… | Экспорт в HTML… |
| `annotations.export_compression` | Exports are compressed ZIP files. Unarchive the ZIP to open the Markdown or HTML file. | Экспорт сохраняется в ZIP. Распакуйте архив, чтобы открыть Markdown или HTML. |
| `annotations.export_format_title` | Export annotations as %1 | Экспорт аннотаций в %1 |
| `annotations.export_action` | Export ZIP | Экспорт ZIP |
| `annotations.export_title` | Export annotations? | Экспортировать аннотации? |
| `annotations.export_untitled` | Untitled book | Книга без названия |
| `annotations.export_replace_title` | Replace annotation export? | Заменить экспорт аннотаций? |
| `annotations.export_replace_action` | Replace ZIP | Заменить ZIP |
| `annotations.export_replace_description` | A %2 ZIP export already exists for “%1”. Replace it with all current annotations? Unarchive the ZIP to open the %2 file. | Для «%1» уже есть ZIP с %2. Заменить его всеми текущими аннотациями? Распакуйте ZIP, чтобы открыть %2. |
| `annotations.export_destination` | Save to %1. | Сохранить в %1. |
| `annotations.export_complete` | Annotations exported. | Аннотации экспортированы. |
| `annotations.export_description` | Export all annotations from “%1” as a compressed %2 file? Unarchive the ZIP to open the %2 file. | Экспортировать все аннотации из «%1» в сжатый файл %2? Распакуйте ZIP, чтобы открыть %2. |
| `annotations.export_failed` | Could not export annotations. Check available storage and try again. | Не удалось экспортировать аннотации. Проверьте свободное место и повторите попытку. |
| `annotations.export_filter` | ZIP archives (*.zip) | Архивы ZIP (*.zip) |
| `annotations.export_saved` | Annotations exported to %1 | Аннотации сохранены в %1 |
| `annotations.change_not_saved` | Could not save the last highlight change. The highlights are back as they were saved. | Не удалось сохранить последнее изменение выделения. Восстановлено сохранённое состояние. |

### keyboard

| Ключ | English | Русский в сборке |
|---|---|---|
| `keyboard.shift` | Shift | Регистр |
| `keyboard.symbols` | Symbols | Символы |
| `keyboard.space` | Space | Пробел |
| `keyboard.backspace` | Backspace | Удалить |
| `keyboard.done` | Done | Готово |

## Изменённые существующие значения

### `reader.default_font_instructions`

- EN ранее: Long press another font to make it the default override for new books. The chosen font will show Default font below its name. Publisher Fonts keeps its automatic fallbacks.
- EN теперь: Long press a font, including Publisher Fonts, to make it the default for new books. The chosen font will show Default font below its name. Publisher Fonts keeps the book’s fonts and automatic fallbacks.
- RU ранее: Нажмите и удерживайте другой шрифт, чтобы сделать его переопределением по умолчанию для новых книг. У выбранного шрифта под названием появится пометка «Шрифт по умолчанию». Издательские шрифты сохраняют свои автоматические запасные варианты.
- RU в сборке: Удерживайте шрифт, включая «Шрифты издателя», чтобы выбрать его для новых книг. Под ним появится подпись «Шрифт по умолчанию». «Шрифты издателя» сохраняет шрифты книги и автоматические замены.

### `reader.make_default_font_description`

- EN ранее: It will show Default font below its name and become the initial font override for new books. Publisher Fonts keeps its automatic fallbacks.
- EN теперь: It will show Default font below its name and become the initial font choice for new books. Publisher Fonts keeps the book’s fonts and automatic fallbacks.
- RU ранее: Под его названием появится пометка «Шрифт по умолчанию», и он станет исходным переопределением шрифта для новых книг. Издательские шрифты сохраняют свои автоматические запасные варианты.
- RU в сборке: Под ним появится подпись «Шрифт по умолчанию», и он будет выбираться для новых книг. «Шрифты издателя» сохраняет шрифты книги и автоматические замены.

### `settings.justification_tracking_off`

- EN ранее: Off
- EN теперь: Normal
- RU ранее: Выкл.
- RU в сборке: Обычная

### `dictionary_panel.try_another_dictionary`

- EN ранее: Choose another dictionary above, or hold a different word.
- EN теперь: Edit the lookup above, or choose another dictionary.
- RU ранее: Выберите другой словарь выше или выделите другое слово.
- RU в сборке: Измените запрос выше или выберите другой словарь.

### `dictionary_panel.try_another_word`

- EN ранее: Hold a different word to look it up.
- EN теперь: Edit the lookup above to try another word.
- RU ранее: Выделите другое слово для поиска.
- RU в сборке: Измените запрос выше для поиска другого слова.

### `settings.avoid_wasteful_styling`

- EN ранее: Smart Styling
- EN теперь: Reduce publisher spacing
- RU ранее: Умные стили
- RU в сборке: Уменьшить отступы издателя

### `settings.avoid_wasteful_styling_description`

- EN ранее: Kobalt will automatically improve book rendering appearance. If you prefer the original book CSS, disable this.
- EN теперь: Use Kobalt's page margins and reduce blank space at chapter starts.
- RU ранее: Kobalt автоматически улучшает внешний вид книги. Если предпочитаете исходный CSS книги — отключите.
- RU в сборке: Применяет поля Kobalt и уменьшает пустое место в начале глав.

### `settings.justification_tracking_summary`

- EN ранее: Reduce wide word gaps with letter spacing. Only affects justified text (Text > Alignment > Justify).
- EN теперь: Adjust letter spacing in justified text.
- RU ранее: Уменьшает большие пробелы между словами за счёт межбуквенного интервала. Влияет только на текст по ширине (Текст > Выравнивание > По ширине).
- RU в сборке: Подгонка межбуквенных интервалов в тексте по ширине.

### `settings.widows_orphans_summary`

- EN ранее: Keep single paragraph lines from appearing alone at the top or bottom of a page.
- EN теперь: Control single lines at page edges.
- RU ранее: Не даёт одиночным строкам абзаца оставаться одними вверху или внизу страницы.
- RU в сборке: Одиночные строки у краёв страницы.

### `settings.word_spacing_summary`

- EN ранее: Adjust the normal width of spaces between words.
- EN теперь: Adjust spacing between words.
- RU ранее: Настройка обычной ширины пробелов между словами.
- RU в сборке: Настройка интервалов между словами.

## Удалённые ключи

- `android.return`: Kobalt
- `settings.advanced_text`: Расширенная типографика
- `settings.justification_tracking_description`: Влияет только на текст по ширине. Выберите %1 в разделе «Текст > Выравнивание». Распределяет свободное место между буквами, уменьшая большие пробелы между словами.
- `settings.section_window`: Окно
- `settings.ui`: Интерфейс
- `settings.hide_status_bar`: Скрыть строку состояния
- `settings.hide_status_bar_description`: Скрывает строку состояния с часами, батареей и меню на этом Mac.

## Источники проверки поведения

- `reader_kosync.cpp::updateKosyncToolbar()` — условия показа ручной кнопки.
- `settings_panel.cpp::showDictionaryFontSizes()` — подпись и превью в одной строке.
- `navigation.cpp::Tab::paintEvent()` и `tabCapital()` — геометрия подписей вкладок.
- `settings_panel.cpp` и `reader_dialogs.cpp::openSleepTile()` — выбор формы плашки сна.
- `appearance_settings.cpp` и диалоги vertical spacing — область коррекции вертикального свободного места.

Снимки замечаний: [верхнее меню](screenshots/kobo-ru/upstream-review/screen_001.png),
[размер словаря](screenshots/kobo-ru/upstream-review/screen_002.png).
