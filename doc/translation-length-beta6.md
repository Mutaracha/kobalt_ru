# Проверка длины RU перед переносом beta6

Сравнение по числу Unicode-символов, не по байтам и не по ширине глифов. Это не проверка размещения в UI.

## Согласованные уточнения

| Ключ | EN | RU | RU/EN | Русский текст |
|---|---:|---:|---:|---|
| `reader.edge_light` | 38 | 30 | 0.79 | Регулировка подсветки свайпами |
| `settings.edge_light_description` | 89 | 75 | 0.84 | Свайпы у самой границы экрана: слева — яркость, справа — теплота подсветки. |
| `settings.light_gesture_feedback` | 30 | 31 | 1.03 | Индикация регулировки подсветки |
| `settings.light_gesture_feedback_description` | 110 | 71 | 0.65 | Показывать всплывающее окно с уровнем в процентах после паузы в свайпе. |
| `settings.wifi_power_saver` | 24 | 22 | 0.92 | Энергосбережение Wi-Fi |
| `settings.wifi_power_saver_description` | 118 | 126 | 1.07 | Выключать Wi-Fi при открытии книги и переходе в сон, кроме случаев, когда включён Bluetooth или Wi-Fi нужен для синхронизации. |
| `settings.normalization_chapters_description` | 47 | 49 | 1.04 | Сохранять отступ над иллюстрациями в начале глав. |
| `settings.hanging_punctuation_summary` | 52 | 52 | 1.00 | Выносить пунктуацию и знаки переноса в боковые поля. |
| `settings.sleep_overlay` | 25 | 27 | 1.08 | Страница с PNG-изображением |
| `settings.sleep_overlay_description` | 130 | 152 | 1.17 | Приглушать страницу и накладывать PNG-изображение под индикатором сна. Добавьте PNG с прозрачным фоном в .adds/kobalt/screensaver или .adds/screensaver. |
| `settings.time_remaining_format` | 21 | 23 | 1.10 | Вид оставшегося времени |
| `settings.time_remaining_detailed` | 8 | 9 | 1.12 | В минутах |
| `settings.time_remaining_approximate` | 11 | 13 | 1.18 | С округлением |
| `settings.time_remaining_detailed_description` | 47 | 28 | 0.60 | Например: «Осталось 83 мин». |
| `settings.time_remaining_approximate_description` | 52 | 31 | 0.60 | Например: «Осталось около 2 ч». |
| `kosync.automatic_off_caption` | 31 | 34 | 1.10 | Только вручную — кнопкой на панели |

## Существующий перевод: кандидаты на проверку в UI

Порог: RU длиннее EN более чем на 25% и минимум на 8 символов. Это эвристика, а не ошибка перевода. Ранее согласованные тексты автоматически не меняются. Новые строки beta6, отсутствующие в текущем RU, этим разделом не проверены.

| Ключ | EN | RU | RU/EN | Русский текст |
|---|---:|---:|---:|---|
| `settings.session_timeout_0` | 5 | 25 | 5.00 | Продолжать прежнюю сессию |
| `annotations.style_underline` | 5 | 22 | 4.40 | Сплошное подчёркивание |
| `annotations.style_dotted` | 6 | 24 | 4.00 | Пунктирное подчёркивание |
| `settings.session_timeout_60` | 6 | 16 | 2.67 | После 1 часа сна |
| `settings.session_timeout_360` | 7 | 17 | 2.43 | После 6 часов сна |
| `settings.session_timeout_240` | 7 | 17 | 2.43 | После 4 часов сна |
| `settings.animations_description` | 45 | 102 | 2.27 | Анимировать перелистывание на поддерживаемых устройствах. Изображения обновляются после пролистывания. |
| `settings.session_timeout_720` | 8 | 18 | 2.25 | После 12 часов сна |
| `reader.lock_portrait` | 13 | 28 | 2.15 | Закрепить книжную ориентацию |
| `reader.lock_landscape` | 14 | 30 | 2.14 | Закрепить альбомную ориентацию |
| `reader.restore_defaults` | 16 | 34 | 2.12 | Восстановить значения по умолчанию |
| `settings.navigation_bar_pages` | 10 | 21 | 2.10 | Переходы по страницам |
| `kosync.username` | 8 | 16 | 2.00 | Имя пользователя |
| `kosync.sync_now` | 8 | 16 | 2.00 | Синхронизировать |
| `kosync.cancel_sync` | 11 | 22 | 2.00 | Отменить синхронизацию |
| `common.next_page` | 9 | 18 | 2.00 | Следующая страница |
| `settings.beautifier_indent_description` | 41 | 80 | 1.95 | Единый отступ первой строки прозы. Ваши явные настройки абзацев имеют приоритет. |
| `settings.section_user_interface` | 14 | 26 | 1.86 | Пользовательский интерфейс |
| `settings.beautifier_initials_description` | 42 | 78 | 1.86 | Превращает распознанные текстовые буквицы в обычные буквы без потери символов. |
| `reader.older_synced_position` | 21 | 39 | 1.86 | Более ранняя синхронизированная позиция |
| `settings.sleep_tile_card_description` | 32 | 58 | 1.81 | Область с информацией о чтении в левом нижнем углу экрана. |
| `settings.session_timeout_15` | 10 | 18 | 1.80 | После 15 минут сна |
| `marks.sort_position` | 10 | 18 | 1.80 | По порядку в книге |
| `kosync.last_synced` | 15 | 27 | 1.80 | Последняя синхронизация: %1 |
| `kosync.completed` | 13 | 23 | 1.77 | Синхронизация завершена |
| `reader.tap_elsewhere_to_exit` | 21 | 37 | 1.76 | Коснитесь в другом месте, чтобы выйти |
| `reader.fail_next_native_save` | 21 | 37 | 1.76 | Сбой следующего системного сохранения |
| `reader.formatting_warning` | 18 | 31 | 1.72 | Предупреждение о форматировании |
| `settings.justification_tracking_more_description` | 32 | 55 | 1.72 | Разрешает больший дополнительный межбуквенный интервал. |
| `reader.go_to_synced_position` | 21 | 36 | 1.71 | Перейти к синхронизированной позиции |
| `reader.defaults_selected` | 17 | 29 | 1.71 | Выбраны значения по умолчанию |
| `recovery.title` | 16 | 27 | 1.69 | Сохранённые данные сброшены |
| `reader.new_synced_position` | 19 | 32 | 1.68 | Новая синхронизированная позиция |
| `settings.beautifier_spacing_description` | 50 | 84 | 1.68 | Убирает лишний отступ под обычной прозой. Сохраняет разрывы сцен и особые фрагменты. |
| `reader.make_default` | 12 | 20 | 1.67 | Сделать по умолчанию |
| `settings.landscape_columns_description` | 47 | 75 | 1.60 | В книжной ориентации используется одна колонка. Применяется ко всем книгам. |
| `settings.justification_tracking_subtle_description` | 36 | 57 | 1.58 | Разрешает небольшой дополнительный межбуквенный интервал. |
| `settings.navigation_taps_only_description` | 38 | 60 | 1.58 | Касайтесь зон перелистывания, чтобы переворачивать страницы. |
| `error.invalid_source_text_offset` | 26 | 41 | 1.58 | Некорректное текстовое смещение источника |
| `settings.vertical_spacing_scope_description` | 33 | 52 | 1.58 | Когда корректировать свободное место внизу страницы. |
| `reader.line_spacing` | 14 | 22 | 1.57 | Межстрочный интервал:  |
| `reader.column_spacing` | 14 | 22 | 1.57 | Межколоночный интервал |
| `library.mark_unread` | 14 | 22 | 1.57 | Отметить непрочитанной |
| `kosync.device_unknown` | 14 | 22 | 1.57 | Неизвестное устройство |
| `common.more_next_page` | 21 | 33 | 1.57 | Продолжение на следующей странице |
| `reader.set_default_font` | 16 | 25 | 1.56 | Задать шрифт по умолчанию |
| `reader.font_weight` | 16 | 25 | 1.56 | Вариативная насыщенность: |
| `header_footer.field.battery_warning` | 15 | 23 | 1.53 | Предупреждение о заряде |
| `annotations.tap_boundary` | 36 | 55 | 1.53 | Коснитесь текста или потяните маркер для корректировки. |
| `settings.section_default_global_font` | 19 | 29 | 1.53 | Глобальный шрифт по умолчанию |
| `reader.make_default_font` | 21 | 32 | 1.52 | Сделать %1 шрифтом по умолчанию? |
| `opening.may_take_a_while` | 21 | 32 | 1.52 | Это может занять некоторое время |
| `native_ui.missing_assets_title` | 18 | 27 | 1.50 | Kobalt не может запуститься |
| `error.unsupported_native_text_offset` | 30 | 45 | 1.50 | Неподдерживаемое системное текстовое смещение |
| `reader.tap_revealed_link` | 35 | 52 | 1.49 | Нажмите на подсвеченную ссылку, чтобы перейти по ней |
| `settings.justification_tracking_off_description` | 27 | 40 | 1.48 | Сохраняет обычный межбуквенный интервал. |
| `opening.try_native` | 19 | 28 | 1.47 | Попробовать встроенный ридер |
| `kosync.resume_title` | 22 | 32 | 1.45 | Продолжить с полученной позиции? |
| `settings.session_timeout_never_description` | 91 | 132 | 1.45 | После сна любой длительности время текущей сессии продолжает отсчитываться с прежнего значения. Время сна не учитывается как чтение. |
| `settings.vertical_spacing_scope_protection_description` | 78 | 113 | 1.45 | Корректирует свободное место, только если защита от висячих строк перенесла строки, которые иначе поместились бы. |
| `light_panel.automatic_warmth` | 27 | 39 | 1.44 | Следовать расписанию тёплого света Kobo |
| `light_panel.automatic_warmth_description` | 79 | 114 | 1.44 | Использует время отхода ко сну, заданное в настройках Kobo. Ручная регулировка тёплого света отключает этот режим. |
| `annotations.ready` | 55 | 79 | 1.44 | Потяните маркеры для корректировки. Коснитесь выделения, чтобы выбрать пометку. |
| `sync.title` | 23 | 33 | 1.43 | Синхронизированная позиция чтения |
| `reader.long_press_link` | 37 | 53 | 1.43 | Нажмите и удерживайте ссылку, чтобы увидеть её опции. |
| `error.native_text_offset_exceeds_its_anchor` | 37 | 53 | 1.43 | Системное текстовое смещение выходит за пределы якоря |
| `kosync.time_unavailable` | 21 | 30 | 1.43 | Время синхронизации неизвестно |
| `error.no_supported_native_reading_position` | 36 | 51 | 1.42 | Поддерживаемая системная позиция чтения отсутствует |
| `kosync.account_changed` | 47 | 66 | 1.40 | Войдите заново, чтобы применить новый сервер или имя пользователя. |
| `settings.navigation_buttons_only_description` | 35 | 49 | 1.40 | Используйте кнопки устройства для перелистывания. |
| `annotations.tap_start` | 39 | 54 | 1.38 | Коснитесь слова или нажмите и протяните для выделения. |
| `settings.navigation_bar_pages_description` | 34 | 47 | 1.38 | Предпросмотр предыдущей или следующей страницы. |
| `error.invalid_source_element_offset` | 29 | 40 | 1.38 | Некорректное смещение элемента источника |
| `annotations.help_text` | 69 | 95 | 1.38 | Нажмите и удерживайте слово, чтобы начать выделение. Коснитесь выделения, чтобы пометить текст. |
| `reader.duplicate_current_update` | 24 | 33 | 1.38 | Продублировать текущее обновление |
| `annotations.color_failed` | 32 | 44 | 1.38 | Не удалось сохранить цвет. Попробуйте снова. |
| `error.this_note_is_too_long_to_preview` | 33 | 45 | 1.36 | Эта сноска слишком длинная для предпросмотра. |
| `settings.tappable_links_off_description` | 62 | 84 | 1.35 | Для защиты от случайного открытия удерживайте ссылку и выберите «Перейти по ссылке». |
| `error.encrypted_or_unsupported_epub` | 29 | 39 | 1.34 | Зашифрованный или неподдерживаемый EPUB |
| `reader.already_default_font` | 32 | 43 | 1.34 | %1 уже является вашим шрифтом по умолчанию. |
| `sync.finished` | 53 | 71 | 1.34 | Синхронизированное состояние чтения отмечает эту книгу как прочитанную. |
| `reader.restore_appearance_defaults` | 27 | 36 | 1.33 | Восстановить оформление по умолчанию |
| `reader.enabling_bold_reading` | 24 | 32 | 1.33 | Включение бионического чтения... |
| `header_footer.replace_custom_prompt` | 27 | 36 | 1.33 | Заменить вашу собственную раскладку? |
| `reader.restore_appearance_defaults_prompt` | 28 | 37 | 1.32 | Восстановить оформление по умолчанию? |
| `reader.disabling_bold_reading` | 25 | 33 | 1.32 | Выключение бионического чтения... |
| `bookmarks.empty_hint` | 50 | 66 | 1.32 | Коснитесь правого верхнего угла страницы, чтобы добавить закладку. |
| `error.native_saved_position_cannot_be_converted` | 41 | 54 | 1.32 | Не удалось преобразовать сохранённую системную позицию |
| `settings.navigation_swipes_only_description` | 35 | 46 | 1.31 | Проводите влево или вправо для перелистывания. |
| `kosync.unavailable` | 45 | 59 | 1.31 | Синхронизация недоступна для этой книги или учётной записи. |
| `reader.could_not_change_dark_mode` | 26 | 34 | 1.31 | Не удалось переключить тёмную тему |
| `error.native_saved_position_is_unsupported` | 36 | 47 | 1.31 | Сохранённая системная позиция не поддерживается |
| `recovery.sync_account` | 66 | 86 | 1.30 | Учётная запись синхронизации недоступна. Войдите заново для продолжения синхронизации. |
| `error.this_note_has_no_text_to_preview` | 33 | 43 | 1.30 | У этой сноски нет текста для предпросмотра. |
| `search_panel.no_matches_hint` | 43 | 56 | 1.30 | Проверьте написание или попробуйте более короткую фразу. |
| `settings.justification_tracking_summary` | 40 | 52 | 1.30 | Подгонка межбуквенных интервалов в тексте по ширине. |
| `library.no_matches` | 27 | 35 | 1.30 | По вашему запросу книги не найдены. |
| `settings.edge_brightness_description` | 52 | 67 | 1.29 | Проводите вверх или вниз вдоль левого края для регулировки яркости. |
| `light_panel.brightness_swipe_hint` | 52 | 67 | 1.29 | Проводите вверх или вниз вдоль левого края для регулировки яркости. |
| `reader.formatting_warning_description` | 111 | 143 | 1.29 | В этой книге возможны проблемы с форматированием. Можно продолжить чтение, но часть текста или вёрстки может отображаться не так, как задумано. |
| `settings.minimal_annotation_mode_description` | 88 | 113 | 1.28 | Скрывает панель аннотаций. Коснитесь текста или пустого места, затем выберите «Завершить выделение», чтобы выйти. |
| `annotations.tap_end` | 39 | 50 | 1.28 | Коснитесь другого слова или потяните любой маркер. |
| `settings.beautifier_description` | 96 | 122 | 1.27 | Экспериментально. Отмеченные правила действуют во всех книгах. Снимите все галочки для отключения. Файлы книг не меняются. |
| `library.folder_unavailable` | 63 | 80 | 1.27 | Эту папку не удаётся прочитать. Добавьте её заново, чтобы вернуть доступ Kobalt. |
| `kosync.skipped` | 45 | 57 | 1.27 | Синхронизация пропущена. Позиция сохранена на устройстве. |
| `error.this_note_has_no_separate_preview` | 34 | 43 | 1.26 | У этой сноски нет отдельного предпросмотра. |
| `settings.widows_orphans_allow_description` | 38 | 48 | 1.26 | Разрешать одиночные строки на границах страницы. |
| `search_panel.search_hint` | 57 | 72 | 1.26 | Поиск не учитывает регистр. Нажмите «Найти» на клавиатуре, чтобы начать. |
| `settings.navigation_bar_chapters_description` | 46 | 58 | 1.26 | Предпросмотр предыдущей или следующей главы. По умолчанию. |
| `settings.navigation_buttons_only` | 31 | 39 | 1.26 | Только аппаратные кнопки перелистывания |
| `kosync.resumed` | 31 | 39 | 1.26 | Чтение продолжено с полученной позиции. |
| `annotations.empty_hint` | 51 | 64 | 1.25 | Используйте «Аннотация» в меню чтения, чтобы сохранить фрагмент. |
