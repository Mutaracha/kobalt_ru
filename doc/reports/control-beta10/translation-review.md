# Выравнивание русского перевода beta10

## Основание и приоритет

- Overlay: `8bd576f144069de7eac7a21fbbc79ce602486c93` → upstream `1796a32559dbb2912bd565990d1c7a0ca7ac4e52`.
- Приоритет: 11 снимков `test & diagnostic/control-beta10-round2/` из 8bd576f.
- Дополнение: 60 снимков `test & diagnostic/56843a0/` из 4b59de0.
- В этой проверке просмотрены все 71 снимок на обзорных листах. Это не попиксельная
  приёмка мелкого текста, не проверка других ориентаций/масштабов и не новый тест устройства.
- Снимки в этом наборе повторно не копируются: доступны по указанным commit/path.

Основная ветка остаётся прежним форком. Импортированы только отдельные инструменты
и overlay в `platforms/control-beta10/`. Новые исходники/шрифты из него не установлены
в основной `src/`/`assets/`. Runtime-состав overlay, кроме 12 текстовых значений и
комментария в RU, идентичен 8bd576f: все остальные 35 выходных SHA-256 сохранены.

## Найденные расхождения

- old010: незавершённое «Статистика книги для» → «Область статистики книги».
- old025–027: пояснение восстановления ссылалось на несуществующие варианты
  «Текст для новых книг»/«Как в последней книге». Использованы реальные
  подписи «Текст новых книг»/«Как в прошлой книге»/«Как в книге».
- new003–007: статистика использует и «Сессия», и «Сеанс». Выбран «Сеанс»,
  тот же термин применён к настройке продолжения после сна (old044/052).
- old048–051: группы выбора шрифтов названы «типографикой». Уточнены заголовки
  «Основные шрифты»/«Дополнительные шрифты», без изменения самих настроек.
- Отдельная новая brightness-строка выровнена по существующему названию индикатора
  подсветки; исправлены две формулировки ошибок словаря, проверенные по исходному
  EN, не по снимкам (этих ошибок на снимках нет).

Короткая подпись `buttons.invert_landscape` **сохранена побайтно из 8bd576f**:
«Инвертировать кнопки в альбомной ориентации.» Снимок new008 сделан до этого
сокращения (это отмечено в исходном README), поэтому длинный текст на нём не
возвращён. Точка также не удалялась: это ранее согласованная строка.

Новые снимки статистики/сна имеют приоритет над старыми: старые геометрические
дефекты и двойной знак процента не использованы как повод откатывать исправления
из 8bd576f. Перевод страниц и экранов различается намеренно. Английские имена
шрифтов/брендов, фрагменты Alice и `SAMPLE TEXT` — upstream-образцы, не пропущенные
переводы каталога. Падежи при произвольных числах не исправлены заменой строк:
полноценная RU plural-логика вне этой задачи; нулевой структурный аудит её не доказывает.

## Все внесённые изменения

| Ключ | English | Было в 8bd576f | Теперь | Причина |
|---|---|---|---|---|
| stats.count_words_in | Book stats for | Статистика книги для | Область статистики книги | Завершённая подпись вместо оборванного «для»; old010. |
| stats.count_words_in_title | Book Stats For | Статистика книги для | Область статистики книги | Одинаковый термин в заголовке и действии. |
| reader.restore_book_defaults_detail | The font, size, line spacing, alignment, weight and hyphenation of this book return to Text for new books, or to Book default when that is Same as last book. Other books and other settings do not change. The defaults apply when you close Settings. | Шрифт, размер, межстрочный интервал, выравнивание, насыщенность и переносы этой книги вернутся к настройкам «Текст для новых книг» либо к настройкам книги, если выбрано «Как в последней книге». Другие книги и настройки не изменятся. Настройки применятся после закрытия раздела «Настройки». | Шрифт, размер, межстрочный интервал, выравнивание, насыщенность и переносы этой книги вернутся к настройкам «Текст новых книг» либо к варианту «Как в книге», если выбрано «Как в прошлой книге». Другие книги и настройки не изменятся. Настройки применятся после закрытия раздела «Настройки». | Названия точно совпадают с вариантами в old025–027; смысл Same as last book → Book default сохранён. |
| stats.session | Session | Сессия | Сеанс | Единый термин с разделом «Активность»; new003–007 приоритетнее старых цифр/геометрии. |
| settings.session_timeout_0 | Never | Продолжать прежнюю сессию | Продолжать прежний сеанс | Тот же термин; old044/052. |
| settings.session_timeout_description | Waking your device after this amount of time starts a new reading session. You do not need to close and reopen your book, this is automatic. | Если устройство спало не меньше выбранного времени, после пробуждения время текущей сессии чтения начнётся с нуля. Позиция в книге и общая статистика сохраняются. | Если устройство спало не меньше выбранного времени, после пробуждения время текущего сеанса чтения начнётся с нуля. Позиция в книге и общая статистика сохраняются. | Тот же термин. |
| settings.session_timeout_never_description | A session always continues when the device wakes, even after a longer period of inactivity. | После сна любой длительности время текущей сессии продолжает отсчитываться с прежнего значения. Время сна не учитывается как чтение. | После сна любой длительности время текущего сеанса продолжает отсчитываться с прежнего значения. Время сна не учитывается как чтение. | Тот же термин; без изменения таймера. |
| settings.primary_typography | Primary typography | Основная типографика | Основные шрифты | Заголовок группы выбора шрифтов, не настройки типографики книги; old048–050. |
| settings.secondary_typography | Secondary typography | Дополнительная типографика | Дополнительные шрифты | Парный заголовок в оформлении; old048–051. |
| settings.brightness_gesture_feedback | Show brightness gesture pop-up | Показывать всплывающий индикатор жеста яркости | Индикация регулировки яркости | Единообразно с существующей строкой «Индикация регулировки подсветки»; не добавляет обещания изменения жеста. |
| dictionaries.cannot_save_the_download | Cannot save the download. | Не удалось сохранить загрузку. | Не удалось сохранить загруженный файл. | Уточнение объекта ошибки; не визуально проверено, только словарь/исходный смысл. |
| dictionaries.dictionary_index_points_outside_its_data | Dictionary index points outside its data. | Индекс словаря ссылается за пределы данных. | Индекс словаря ссылается на данные за пределами файла. | Грамматика технического сообщения; не визуально проверено. |

## Покрытие снимков

| Набор | Область / результат |
|---|---|
| new001–002 | Текст/раскладка: подписи и термины сохранены; старый crop не возвращён. |
| new003–007 | Статистика/активность/календарь/список/достижения: термин «сеанс» унифицирован; размер цифр не менялся. |
| new008 | Физические кнопки: приоритет имеет короткая итоговая строка из кода 8bd576f, а не её более ранний снимок. |
| new009–010 | Сон: варианты «Капсула», «Подпись», «Виджеты», «Ночь», «Кольцо», «Рамка», «Линии», «Письмо» сохранены; проценты не получают лишний знак. |
| new011 | Отладка: «ОТЛАДКА», кэш, диагностика сохранены. |
| old001–009 | Меню чтения, колонтитулы, стили, подсветка: дополнительное покрытие; геометрия из новых снимков приоритетнее. |
| old010–014 | Область статистики и активность; терминология исправлена, числа/алгоритмы не менялись. |
| old015–024 | Экран, навигация, колонки, обновление, кнопки, касания, ссылки: существующие варианты сохранены. |
| old025–038 | Новые книги, восстановление, типографика/нормализация: исправлены ссылки на названия опций. Английские образцы книги не заменялись. |
| old039–047 | Система/скрытые функции/сон/время/проценты/нумерация: «сеанс», без отката presentation fixes. |
| old048–057 | Оформление/шрифты/словарь: заголовки групп выровнены. |
| old058–060 | Рендеринг, отладка, устройство: без изменения поведения и выбранного движка. |

## Повторная проверка 204 добавленных/изменённых строк beta10

Это список из translation-proposals.json исходного контрольного коммита, сверенный
с фактическим RU внутри patch (не с устаревшим assets/ru.strings основного форка).
«Сохранено» — редакционное решение по каталогу/исходному смыслу, не утверждение,
что каждая строка присутствует на снимке. Дополнительные сопутствующие изменения
перечислены выше.

| Ключ | English | Итоговый RU | Решение |
|---|---|---|---|
| activity.chart_dates | %1 to %2 | %1 — %2 | Сохранено |
| activity.chart_value | %1: %2 | %1: %2 | Сохранено |
| activity.copies | Book copies | Копии книг | Сохранено |
| activity.copies_count | %1 copies | Копий: %1 | Сохранено |
| activity.projected_finish | Projected finish %1 | Ожидаемое завершение: %1 | Сохранено |
| activity.under_minute | Less than one minute | Менее минуты | Сохранено |
| annotations.add_note_title | Add Note | Добавить заметку | Сохранено |
| annotations.color | Color | Цвет | Сохранено |
| annotations.discard | Discard | Отменить изменения | Сохранено |
| annotations.edit_note_title | Edit Note | Изменить заметку | Сохранено |
| annotations.look_up | Look up | Найти в словаре | Сохранено |
| annotations.note | Note | Заметка | Сохранено |
| annotations.numbered_entry | Annotation %1 | Аннотация %1 | Сохранено |
| annotations.style | Style | Стиль | Сохранено |
| bookmarks.modify_title | Modify Pinned Bookmark | Изменить закреплённую закладку | Сохранено |
| bookmarks.numbered_entry | Bookmark %1 | Закладка %1 | Сохранено |
| bookmarks.pin_title | Pin Bookmark | Закрепить закладку | Сохранено |
| bookmarks.remove_screen_detail | Screen %1 · %2 | Экран %1 · %2 | Сохранено |
| buttons.invert_landscape | Invert page buttons in landscape | Инвертировать кнопки в альбомной ориентации. | Сохранено |
| buttons.invert_landscape_hint | Turn this on if the buttons are reversed when you hold your device sideways. | Включите, если при повороте устройства набок кнопки работают наоборот. | Сохранено |
| chapter_contents.additional_actions | Additional actions | Дополнительные действия | Сохранено |
| chapter_contents.both | Start and end | Начало и конец | Сохранено |
| chapter_contents.end | End | Конец | Сохранено |
| chapter_contents.enter_printed_page | Enter a page number from the print edition. | Введите номер страницы из печатного издания. | Сохранено |
| chapter_contents.go_to_printed_page | Go to printed page | Перейти к печатной странице | Сохранено |
| chapter_contents.illustrated_entry | Illustrated entry %1 | Пункт с иллюстрацией %1 | Сохранено |
| chapter_contents.landmark_count | Landmarks · %1 | Ориентиры · %1 | Сохранено |
| chapter_contents.landmarks | Landmarks | Ориентиры | Сохранено |
| chapter_contents.location | Location | Позиция | Сохранено |
| chapter_contents.mark_end | Mark as end | Отметить как конец | Сохранено |
| chapter_contents.mark_start | Mark as start | Отметить как начало | Сохранено |
| chapter_contents.more | More navigation options | Другие способы навигации | Сохранено |
| chapter_contents.no_printed_pages | This book doesn’t include printed page numbers. | В этой книге нет номеров печатных страниц. | Сохранено |
| chapter_contents.other_labels | Other page labels | Другие обозначения страниц | Сохранено |
| chapter_contents.preview_image | View illustration for %1 | Посмотреть иллюстрацию: %1 | Сохранено |
| chapter_contents.printed_count | Printed pages · %1 | Печатные страницы · %1 | Сохранено |
| chapter_contents.printed_pages | Printed pages | Печатные страницы | Сохранено |
| chapter_contents.reset_end | Restore default end | Восстановить исходный конец | Сохранено |
| chapter_contents.reset_start | Restore default start | Восстановить исходное начало | Сохранено |
| chapter_contents.start | Start | Начало | Сохранено |
| chapter_contents.view_landmarks | View book landmarks | Посмотреть ориентиры книги | Сохранено |
| closing.local_only | Exit without sending progress | Выйти без отправки прогресса | Сохранено |
| closing.sync_detail | Kobalt will close when sync finishes. If Kobo has a different reading position, you can choose which one to keep. Tap outside this message to keep reading. | Kobalt закроется после завершения синхронизации. Если на Kobo сохранена другая позиция чтения, вы сможете выбрать, какую оставить. Нажмите вне этого сообщения, чтобы продолжить чтение. | Сохранено |
| closing.sync_title | Your Kobo is syncing | Kobo выполняет синхронизацию | Сохранено |
| closing.wait_for_sync | Wait for sync to complete | Дождаться завершения синхронизации | Сохранено |
| closing.waiting_for_sync | Waiting for sync to complete… | Ожидание завершения синхронизации… | Сохранено |
| common.screen | screen | экран | Сохранено |
| common.screens | screens | экраны | Сохранено |
| dictionaries.cannot_activate_installed_dictionary | Cannot activate installed dictionary. | Не удалось активировать установленный словарь. | Сохранено |
| dictionaries.cannot_create_dictionary_storage | Cannot create dictionary storage. | Не удалось создать хранилище словарей. | Сохранено |
| dictionaries.cannot_install_this_dictionary | Cannot install this dictionary. | Не удалось установить этот словарь. | Сохранено |
| dictionaries.cannot_read_dictionary_index | Cannot read dictionary index. | Не удалось прочитать индекс словаря. | Сохранено |
| dictionaries.cannot_remove_this_dictionary | Cannot remove this dictionary. | Не удалось удалить этот словарь. | Сохранено |
| dictionaries.cannot_save_dictionary_files | Cannot save dictionary files. | Не удалось сохранить файлы словаря. | Сохранено |
| dictionaries.cannot_save_dictionary_index | Cannot save dictionary index. | Не удалось сохранить индекс словаря. | Сохранено |
| dictionaries.cannot_save_dictionary_metadata | Cannot save dictionary metadata. | Не удалось сохранить метаданные словаря. | Сохранено |
| dictionaries.cannot_save_the_download | Cannot save the download. | Не удалось сохранить загруженный файл. | Выровнено |
| dictionaries.dictionary_archive_is_damaged | Dictionary archive is damaged. | Архив словаря повреждён. | Сохранено |
| dictionaries.dictionary_archive_is_missing_required_files | Dictionary archive is missing required files. | В архиве словаря отсутствуют необходимые файлы. | Сохранено |
| dictionaries.dictionary_checksum_differs | Dictionary checksum differs. | Контрольная сумма словаря не совпадает. | Сохранено |
| dictionaries.dictionary_checksum_or_size_differs | Dictionary checksum or size differs. | Контрольная сумма или размер словаря не совпадает. | Сохранено |
| dictionaries.dictionary_definition_is_damaged | Dictionary definition is damaged. | Словарная статья повреждена. | Сохранено |
| dictionaries.dictionary_download_is_incomplete | Dictionary download is incomplete. | Словарь загружен не полностью. | Сохранено |
| dictionaries.dictionary_index_is_damaged | Dictionary index is damaged. | Индекс словаря повреждён. | Сохранено |
| dictionaries.dictionary_index_is_too_large | Dictionary index is too large. | Индекс словаря слишком велик. | Сохранено |
| dictionaries.dictionary_index_points_outside_its_data | Dictionary index points outside its data. | Индекс словаря ссылается на данные за пределами файла. | Выровнено |
| dictionaries.dictionary_installed | Dictionary installed. | Словарь установлен. | Сохранено |
| dictionaries.dictionary_removed | Dictionary removed. | Словарь удалён. | Сохранено |
| dictionaries.download_cancelled | Download cancelled. | Загрузка отменена. | Сохранено |
| dictionaries.download_size_or_storage_error | Download size or storage error. | Ошибка размера загрузки или хранилища. | Сохранено |
| dictionaries.download_timed_out | The dictionary download timed out. | Время ожидания загрузки словаря истекло. | Сохранено |
| dictionaries.downloading_name | Downloading %1 | Загрузка: %1 | Сохранено |
| dictionaries.downloading_progress | Downloading %1 · %2 | Загрузка: %1 · %2 | Сохранено |
| dictionaries.installation_cancelled | Installation cancelled. | Установка отменена. | Сохранено |
| dictionaries.installed_dictionary_is_damaged | Installed dictionary is damaged. | Установленный словарь повреждён. | Сохранено |
| dictionaries.installed_inflection_index_is_damaged | Installed inflection index is damaged. | Установленный индекс словоформ повреждён. | Сохранено |
| dictionaries.installing_name | Installing %1 | Установка: %1 | Сохранено |
| dictionaries.invalid_dictzip_data | Invalid dictzip data. | Некорректные данные dictzip. | Сохранено |
| dictionaries.invalid_stardict_index | Invalid StarDict index. | Некорректный индекс StarDict. | Сохранено |
| dictionaries.invalid_stardict_metadata | Invalid StarDict metadata. | Некорректные метаданные StarDict. | Сохранено |
| dictionaries.missing_stardict_inflection_index | Missing StarDict inflection index. | Отсутствует индекс словоформ StarDict. | Сохранено |
| dictionaries.missing_stardict_metadata | Missing StarDict metadata. | Отсутствуют метаданные StarDict. | Сохранено |
| dictionaries.no_dictionary_installed | No dictionary installed. | Нет установленных словарей. | Сохранено |
| dictionaries.not_enough_space_for_this_dictionary | Not enough space for this dictionary. | Недостаточно места для этого словаря. | Сохранено |
| dictionaries.stardict_index_size_differs | StarDict index size differs. | Размер индекса StarDict не совпадает. | Сохранено |
| dictionaries.stardict_word_count_differs_from_its_index | StarDict word count differs from its index. | Число слов StarDict не совпадает с индексом. | Сохранено |
| dictionaries.this_dictionary_is_not_available | This dictionary is not available. | Этот словарь недоступен. | Сохранено |
| dictionaries.this_dictionary_is_not_installed | This dictionary is not installed. | Этот словарь не установлен. | Сохранено |
| dictionaries.unsafe_download_redirect | Unsafe download redirect. | Небезопасное перенаправление загрузки. | Сохранено |
| dictionaries.unsupported_stardict_format | Unsupported StarDict format. | Неподдерживаемый формат StarDict. | Сохранено |
| dictionaries.wait_for_this_download_to_finish | Wait for this download to finish. | Дождитесь завершения этой загрузки. | Сохранено |
| features.activity | Activity | Активность | Сохранено |
| features.activity_description | Hides the Activity section in Stats. Reading history continues. | Скрывает раздел «Активность» в статистике. История чтения продолжает сохраняться. | Сохранено |
| header_footer.both_off_hint | Turn on Header or Footer below, then tap a slot to edit it. | Включите верхний или нижний колонтитул ниже, затем нажмите на поле, чтобы изменить его. | Сохранено |
| header_footer.field.chapter_and_screen | Chapter and screen in chapter | Глава и экран в главе | Сохранено |
| header_footer.field.chapter_screen | Screen in chapter | Экран в главе | Сохранено |
| header_footer.field.chapter_screens_left | Screens remaining in chapter | Осталось экранов в главе | Сохранено |
| header_footer.footer_off_hint | Tap a header slot to edit it. Turn on Footer to edit its slots. | Нажмите на поле верхнего колонтитула, чтобы изменить его. Включите нижний колонтитул, чтобы изменить его поля. | Сохранено |
| header_footer.header_off_hint | Tap a footer slot to edit it. Turn on Header to edit its slots. | Нажмите на поле нижнего колонтитула, чтобы изменить его. Включите верхний колонтитул, чтобы изменить его поля. | Сохранено |
| header_footer.progress_bar_bottom | Bottom | Снизу | Сохранено |
| header_footer.progress_bar_top | Top | Сверху | Сохранено |
| header_footer.progress_ticks | Chapter ticks | Метки глав | Сохранено |
| kosync.device_name_title | Device Name | Имя устройства | Сохранено |
| kosync.matching_title | Document Matching | Сопоставление документов | Сохранено |
| kosync.screens_ahead | %1 screens ahead | Вперёд на %1 экр. | Сохранено |
| kosync.screens_ahead_one | 1 screen ahead | Вперёд на 1 экр. | Сохранено |
| kosync.screens_back | %1 screens back | Назад на %1 экр. | Сохранено |
| kosync.screens_back_one | 1 screen back | Назад на 1 экр. | Сохранено |
| marks.location_range | %1 to %2 | %1 — %2 | Сохранено |
| marks.page_range | Pages %1 to %2 | Страницы %1–%2 | Сохранено |
| marks.row_action | %1: %2 | %1: %2 | Сохранено |
| marks.screen_range | Screens %1–%2 | Экраны %1–%2 | Сохранено |
| marks.screen_range_description | Screens %1 to %2 | Экраны с %1 по %2 | Сохранено |
| marks.sort_dialog_title | Sort By | Сортировать по | Сохранено |
| reader.applying_theme | Applying theme... | Применение темы… | Сохранено |
| reader.book_defaults_selected | Book defaults selected | Выбраны настройки книги по умолчанию | Сохранено |
| reader.counting_pages | Counting pages | Подсчёт страниц | Сохранено |
| reader.counting_screens | Counting screens | Подсчёт экранов | Сохранено |
| reader.edge_brightness | Swipe along the left edge to adjust brightness | Проведите вдоль левого края, чтобы изменить яркость | Сохранено |
| reader.international_supplemental_fonts | Supplemental Fonts (International) | Дополнительные шрифты (международные) | Сохранено |
| reader.menu_screen_of | SCREEN %1 OF %2 | ЭКРАН %1 ИЗ %2 | Сохранено |
| reader.restore_book_defaults | Restore book defaults | Восстановить настройки книги по умолчанию | Сохранено |
| reader.restore_book_defaults_detail | The font, size, line spacing, alignment, weight and hyphenation of this book return to Text for new books, or to Book default when that is Same as last book. Other books and other settings do not change. The defaults apply when you close Settings. | Шрифт, размер, межстрочный интервал, выравнивание, насыщенность и переносы этой книги вернутся к настройкам «Текст новых книг» либо к варианту «Как в книге», если выбрано «Как в прошлой книге». Другие книги и настройки не изменятся. Настройки применятся после закрытия раздела «Настройки». | Выровнено |
| reader.restore_book_defaults_prompt | Restore book defaults? | Восстановить настройки книги по умолчанию? | Сохранено |
| reader.return_screen | Return to screen %1 | Вернуться к экрану %1 | Сохранено |
| reader.screen_number | Screen %1 | Экран %1 | Сохранено |
| reader.strip_padding | Padding | Отступы | Сохранено |
| reader.strip_size_value | %1 px | %1 пикс. | Сохранено |
| reader.text_preview_lifted | The preview starts below the illustration so you can see how your text settings look. | Предпросмотр начинается под иллюстрацией, чтобы были видны выбранные настройки текста. | Сохранено |
| reader.text_preview_sample | This page has no text to preview. This sample shows how your text settings will look. | На этой странице нет текста для предпросмотра. Этот образец показывает, как будут выглядеть выбранные настройки текста. | Сохранено |
| reader.ui_fonts | UI Fonts | Шрифты интерфейса | Сохранено |
| reader.zoom_screen_range | Screens %1 to %2 of %3 | Экраны с %1 по %2 из %3 | Сохранено |
| search_panel.chapter_detail | %1: %2 | %1: %2 | Сохранено |
| search_panel.screen_chapter | Screen %1 · %2 | Экран %1 · %2 | Сохранено |
| search_panel.screen_detail | Screen %1: %2 | Экран %1: %2 | Сохранено |
| settings.brightness_gesture_feedback | Show brightness gesture pop-up | Индикация регулировки яркости | Выровнено |
| settings.chapter_spread | Start chapters on a new spread | Начинать главы с нового разворота | Сохранено |
| settings.chapter_spread_description | When a chapter starts on a new page, show it in the first column. | Если глава начинается с новой страницы, показывать её в первой колонке. | Сохранено |
| settings.columns_description | Column spacing is measured in multiples of the current text size. | Расстояние между колонками измеряется в долях текущего размера текста. | Сохранено |
| settings.dark_mode | Dark Mode | Тёмный режим | Сохранено |
| settings.font_too_large | Font disabled due to size limitations. | Шрифт отключён из-за ограничения размера. | Сохранено |
| settings.icon_color | Color only | Только цвет | Сохранено |
| settings.icon_monochrome | Monochrome | Монохромные | Сохранено |
| settings.icon_pastel | Pastel tiles | Пастельные плитки | Сохранено |
| settings.icon_rings | Color rings | Цветные кольца | Сохранено |
| settings.icon_theme | Icon theme | Тема значков | Сохранено |
| settings.landscape_column_layout | Multiple columns in landscape mode | Несколько колонок в альбомной ориентации | Сохранено |
| settings.night_mode_description | Also known as Night Mode. Displays light text on a dark background, and inverts some colors. | Также известен как ночной режим. Показывает светлый текст на тёмном фоне и инвертирует некоторые цвета. | Сохранено |
| settings.palette | Color theme | Цветовая тема | Сохранено |
| settings.palette_blue | Blue | Синяя | Сохранено |
| settings.palette_grayscale | Grayscale | Оттенки серого | Сохранено |
| settings.palette_sage | Sage | Шалфейная | Сохранено |
| settings.palette_sepia | Sepia | Сепия | Сохранено |
| settings.primary_typography | Primary typography | Основные шрифты | Выровнено |
| settings.publisher_page_numbers | Always display publisher provided page numbers | Всегда показывать номера страниц издателя | Сохранено |
| settings.publisher_page_numbers_description | Use the print edition’s page labels when available. Kobalt counts its own layout in screens. | Использовать обозначения страниц печатного издания, если они есть. Собственную раскладку Kobalt считает в экранах. | Сохранено |
| settings.secondary_arabic | Arabic numerals | Арабские цифры | Сохранено |
| settings.secondary_letters | Lowercase letters | Строчные буквы | Сохранено |
| settings.secondary_numbers | Secondary page numbers | Дополнительная нумерация страниц | Сохранено |
| settings.secondary_numbers_hint | Choose how pages before and after the main text are numbered in every book. | Выберите способ нумерации страниц до и после основного текста во всех книгах. | Сохранено |
| settings.secondary_prefixed | Section-prefixed numbers | Номера с префиксом раздела | Сохранено |
| settings.secondary_preview | Front  %1   ·   Back  %2 | До текста  %1   ·   После текста  %2 | Сохранено |
| settings.secondary_roman | Lowercase Roman | Строчные римские цифры | Сохранено |
| settings.secondary_typography | Secondary typography | Дополнительные шрифты | Выровнено |
| settings.secondary_upper_roman | Uppercase Roman | Прописные римские цифры | Сохранено |
| settings.sideloaded_fonts_first | Show sideloaded fonts first | Показывать пользовательские шрифты первыми | Сохранено |
| settings.sideloaded_fonts_first_description | List your own fonts before recommended fonts in every typeface picker. | Показывать ваши шрифты перед рекомендуемыми во всех списках выбора гарнитуры. | Сохранено |
| settings.single_column | Single column | Одна колонка | Сохранено |
| settings.theme_classic | Classic | Классическая | Сохранено |
| settings.theme_custom | Custom | Своя | Сохранено |
| settings.theme_details | Detailed configuration | Подробная настройка | Сохранено |
| settings.theme_notebook | Notebook | Блокнот | Сохранено |
| settings.theme_playful | Playful | Игровая | Сохранено |
| settings.theme_preset | Theme preset | Предустановка темы | Сохранено |
| settings.theme_selection | Preset | Предустановка | Сохранено |
| settings.theming | Theming | Оформление | Сохранено |
| settings.two_in_landscape | Two in landscape | Две в альбомной ориентации | Сохранено |
| shutdown.restarting | Restarting | Перезапуск | Сохранено |
| shutdown.saving | Saving your progress first... | Сначала сохраняем прогресс чтения… | Сохранено |
| shutdown.shutting_down | Shutting down | Выключение | Сохранено |
| sleep.status_random | Random status | Случайный статус | Сохранено |
| stats.count_words_in | Book stats for | Область статистики книги | Выровнено |
| stats.count_words_in_title | Book Stats For | Область статистики книги | Выровнено |
| stats.main_words | Main text · %1 | Основной текст · %1 | Сохранено |
| stats.scope_hint | Choose which pages, words and reading time the Book figures cover. This choice applies to all books. | Выберите, какие страницы, слова и время чтения учитываются в показателях книги. Выбор применяется ко всем книгам. | Сохранено |
| stats.scope_main | Main text | Основной текст | Сохранено |
| stats.scope_main_hint | From the start to the end marked in Chapters or supplied by the book. | От начала до конца, отмеченных в разделе «Главы» или заданных книгой. | Сохранено |
| stats.scope_volume | Whole volume | Весь том | Сохранено |
| stats.scope_volume_hint | Include the front and back matter and every book in the volume. | Включать материалы до и после основного текста и все книги тома. | Сохранено |
| stats.screen_left | screen remaining | экр. остался | Сохранено |
| stats.screen_of | Screen %1 of %2 | Экран %1 из %2 | Сохранено |
| stats.screens_left | screens remaining | экр. осталось | Сохранено |
| stats.volume_words | Volume · %1 | Том · %1 | Сохранено |
| words.go_to_screen | Go to screen %1 | Перейти к экрану %1 | Сохранено |
| annotations.summary_one | 1 annotation | 1 аннотация | Сохранено |
| chapter_contents.summary_one | Contents · 1 chapter | Содержание · 1 глава | Сохранено |
| library.count_one | 1 book | 1 книга | Сохранено |
| settings.paragraph_spacing_description | Values are percentages of the text size: with 12 pt text, a %1 indent is 12 pt. Applies to all books. Headings and special blocks keep their formatting. | Значения — в процентах от размера текста: при 12 пт отступ %1 равен 12 пт. Для всех книг. Заголовки и особые блоки сохраняют оформление. | Сохранено |
| settings.percentage_decimal_description | For example, %1 | Например, %1 | Сохранено |
| settings.percentage_whole_description | For example, %1 | Например, %1 | Сохранено |
| settings.vertical_spacing_center_description | Center prose that occupies at least %1 of the reading height. Keep line spacing unchanged. | Центрирует прозу, занимающую не менее %1 высоты чтения. Межстрочный интервал не меняется. | Сохранено |
| settings.word_spacing_narrow_description | Use %1 of the normal space width. | Использовать %1 обычной ширины пробела. | Сохранено |
| settings.word_spacing_normal_description | Keep the font’s normal space width (%1). | Сохранять обычную ширину пробела шрифта (%1). | Сохранено |
| settings.word_spacing_wide_description | Use %1 of the normal space width. | Использовать %1 обычной ширины пробела. | Сохранено |

## Проверки и ограничения

- Чистое применение binary patch к закреплённому upstream и все 36 выходных хешей: PASS.
- Остальные 35 файлов идентичны overlay 8bd576f: PASS.
- EN/RU: 1808/1808 ключей, нет пропущенных/лишних ключей, дрейфа EN, несовпадений
  аргументов или устаревших процентных суффиксов: PASS.
- Восемь unit-тестов аудитора (новые/изменённые/удалённые строки, аргументы,
  проценты, синтаксис, CLI-коды, защита входных файлов): PASS.
- CMake validate: generation, production references, percent guards и asset hashes: PASS.
- Qt5/Qt6 компиляция, новый установочный ZIP, новая проверка на устройстве: НЕ выполнялись.
  Старые CI-проходы 8bd576f не объявляются проверками новой редакции.

На следующих обновлениях сначала запускать `audit_strings.py` с новым EN. Даже
успешное `git apply --check` не доказывает неизменность смысла строк и контрактов C++.
См. инструкцию `platforms/control-beta10/README.md`.
