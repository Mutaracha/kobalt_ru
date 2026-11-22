# Оставшиеся строки beta.9 — на согласование

Статус: **62 строки согласованы пользователем целиком; применены в assets**. EN берётся дословно из pinned
upstream `be472588`. Ниже RU, необходимый для оставшихся status/debug,
percentage, rotation-lock и close-timeout consumers. Это не включение beta.10.

Не возвращаются удалённые page-count/time-to-end menu strip, quiet/frontlight
диагностика. Новые `annotations.discard/look_up/note` не предлагаются: действующие
согласованные Cancel/Dictionary/NoteEditor тексты сохраняются.
`search_panel.chapter_detail` не требует новой формулировки: `%1: %2` уже имеется
в обеих локалях и используется через существующий formatting key.

## Колонтитулы и формат процентов

| Ключ | RU |
|---|---|
| header_footer.milliseconds | %1 мс |
| header_footer.field.page_turn_time | Время перелистывания |
| header_footer.field.chapter_load_time | Время загрузки главы |
| header_footer.field.book_status | Состояние книги |
| header_footer.field_description.book_status | Время последнего перелистывания и состояние вёрстки: быстрая — пока свёрстана одна глава, полная — когда свёрстана вся книга. Обновляется каждые 5 секунд. |
| header_footer.status.fast | быстрая |
| header_footer.status.full | полная |
| header_footer.status.failed | ошибка |
| header_footer.status.seconds | %1 с |
| header_footer.status.with_detail | %1 (%2) |
| header_footer.status.layout | вёрстка: %1 |
| header_footer.status.turn | перелистывание: %1 мс |
| header_footer.status.parts | %1 \| %2 |
| settings.percentage_format | Проценты прогресса |
| settings.percentage_format_description | Выберите формат прогресса книги и главы в колонтитулах. |
| settings.percentage_whole | Целые проценты |
| settings.percentage_decimal | Один знак после запятой |
| settings.percentage_whole_description | Например, 8% |
| settings.percentage_decimal_description | Например, 8,1% |

## Системное управление и закрытие

| Ключ | RU |
|---|---|
| light_panel.system_brightness | Системная яркость |
| light_panel.system_brightness_description | Использовать настройку яркости Android. Ручные изменения запоминаются для чтения. |
| reader.lock_current_rotation | Зафиксировать текущую ориентацию |
| closing.timeout | Сохранение занимает больше времени, чем обычно |
| closing.timeout_detail | Сохранение ещё может завершиться. Вы можете продолжить чтение или снова попробовать закрыть книгу. |
| closing.close_now | Закрыть без ожидания |
| closing.close_now_detail | Место в книге сохранено. Если закрыть её сейчас, время чтения и прогресс Kobo могут не обновиться. |

## Settings / Debug

| Ключ | RU |
|---|---|
| settings.section_engine | Движок |
| settings.section_developer | Для разработчиков |
| settings.debug_mode | Режим отладки |
| settings.debug_mode_description | Показать вкладку отладки, отладочные поля колонтитулов и движок Faster. |
| settings.section_annotations | Аннотации |
| settings.section_feature_visibility | Видимость функций |
| settings.section_system_integration | Интеграция с системой |
| settings.section_nitpicks | Мелкие настройки |
| settings.debug | Отладка |
| debug.section_caches | Кэши |
| debug.evict_cache | Удалить кэш книги |
| debug.evict_cache_description | Удалить разобранную копию и страницы этой книги, затем закрыть её. Место чтения, закладки и аннотации сохранятся. |
| debug.evict_cache_title | Удалить кэш книги? |
| debug.evict_cache_confirm_description | Книга закроется. При следующем открытии она будет заново разобрана и свёрстана. |
| debug.evict | Удалить |
| debug.clear_caches | Очистить все кэши |
| debug.clear_caches_description | Удалить разобранные копии всех книг и плитки предпросмотра, затем закрыть книгу. Данные чтения сохранятся. |
| debug.clear_caches_title | Очистить все кэши? |
| debug.clear_caches_confirm_description | Книга закроется. При открытии каждая книга будет заново разобрана и свёрстана. |
| debug.clear | Очистить |
| debug.section_diagnostics | Диагностика |
| debug.detailed_diagnostics | Подробная диагностика |
| debug.detailed_diagnostics_description | Записывать время операций и фоновую работу в диагностический журнал. Применяется сразу и после перезапуска. |
| debug.detailed_diagnostics_fixed | В этой сборке время операций и фоновая работа всегда записываются в диагностический журнал. |
| debug.clear_log | Очистить диагностический журнал |
| debug.section_device | Устройство |
| debug.version | Версия |
| debug.qt | Qt |
| debug.firmware | Прошивка |
| debug.cores | Ядра процессора |
| debug.cores_value | Доступно %1 из %2 |
| debug.storage | Свободное место |
| debug.storage_value | Свободно %1, кэши %2 |
| debug.unknown | Неизвестно |
| features.highlight_links | Подсветка ссылок |
| features.highlight_links_description | Скрыть кнопку подсветки ссылок и их количество в меню чтения. Нажатия и удержания ссылок продолжат работать. |

Upstream название Faster оставлено как имя движка, а не выдуманный перевод.
Новые действия очистки относятся только к восстанавливаемым кэшам, не к сбросу
профиля. Их перенос требует соответствующих upstream storage/API/test consumers.
Согласование текста не означает автоматическое включение debug mode или смену
режима рендеринга/Hybrid алгоритмов.
