> АРХИВ: исторический контекст, не текущий план и не инструкция установки.
> Старые pending/next не возобновлять. Текущий источник — FB2_FORK_PLAN.md в корне.

# beta9 — строки Activity и настройки экрана сна: утверждено пользователем

**Утверждено пользователем перед внесением в assets.** Английский текст — pinned upstream beta9. Русский текст согласован до подключения интерфейса. Старые принятые названия (в том числе «Замок», «Капсула»), текст «Письма» и нижние вкладки меню чтения не изменяются. Новые вкладки ниже принадлежат только экрану Activity.

Безопасность данных: удаление истории в Activity не означает удаление книги/закладок/аннотаций. «Серия» — дни чтения подряд. Нейтральные конструкции «Книг: %1», «Дней: %1» обходят неверные русские окончания в upstream-парах singular/plural; окончательная длина и переносы проверяются по снимкам устройства.

## Activity

| Ключ | EN (upstream) | Утверждённый RU |
|---|---|---|
| `activity.title` | Activity | Активность |
| `activity.book_title` | Book Activity | Чтение книги |
| `activity.hidden_title` | Hidden Books | Скрытые книги |
| `activity.hidden_books` | Hidden books | Скрытые книги |
| `activity.hidden` | Hidden | Скрыта |
| `activity.hide_book` | Hide from Books | Скрыть из списка книг |
| `activity.show_book` | Show in Books | Показать в списке книг |
| `activity.delete_book` | Delete activity | Удалить историю чтения |
| `activity.delete_title` | Delete Reading Activity? | Удалить историю чтения? |
| `activity.delete_detail` | All reading sessions of “%1” will be deleted, and their time removed from your daily totals and streaks. This cannot be undone. | Все сеансы чтения книги «%1» будут удалены, а их время исключено из дневных итогов и серий. Это действие нельзя отменить. |
| `activity.delete_confirm` | Delete | Удалить |
| `activity.close` | Close Activity | Закрыть активность |
| `activity.welcome_title` | Welcome to Activity | Ваша активность |
| `activity.welcome_description` | Activity shows your reading time and reading stats on this device. It doesn't sync, so if you also read on another device, your streaks and totals here won't include that reading. | Здесь показаны время и статистика чтения на этом устройстве. Эти данные не синхронизируются: чтение на других устройствах не входит в местные серии и итоги. |
| `activity.loading` | Loading reading history… | Загрузка истории чтения… |
| `activity.unavailable` | Reading history is unavailable right now. | История чтения сейчас недоступна. |
| `activity.change_not_saved` | Could not save the change to your reading history. | Не удалось сохранить изменение истории чтения. |
| `activity.empty` | Your reading time appears here after your first reading session. | Время чтения появится здесь после первого сеанса. |
| `activity.no_books` | Books you read appear here. | Здесь появятся книги, которые вы читаете. |
| `activity.tab_overview` | OVERVIEW | ОБЗОР |
| `activity.tab_calendar` | CALENDAR | КАЛЕНДАРЬ |
| `activity.tab_books` | BOOKS | КНИГИ |
| `activity.tab_records` | RECORDS | РЕКОРДЫ |
| `activity.row_started` | Just started reading | Чтение только началось |
| `activity.row_minutes` | %1 min today | Сегодня %1 мин |
| `activity.row_hours` | %1h %2m today | Сегодня %1 ч %2 мин |
| `activity.row_streak` | %1 · %2-day streak | %1 · серия %2 дн. |
| `activity.duration_short` | %1 h %2 | %1 ч %2 |
| `activity.hours_unit` | h | ч |
| `activity.duration_compact_minutes` | %1m | %1м |
| `activity.duration_compact_hours` | %1h%2 | %1ч%2 |
| `activity.duration_hours` | %1 h | %1 ч |
| `activity.hours_minutes_unit` | h %1 min | ч %1 мин |
| `activity.hours` | hours | часов |
| `activity.today` | Today | Сегодня |
| `activity.streak` | Streak | Серия |
| `activity.day` | day | день |
| `activity.days` | days | дн. |
| `activity.best_streak` | Best %1 days | Рекорд: %1 дн. |
| `activity.best_streak_one` | Best %1 day | Рекорд: %1 день |
| `activity.best_yet` | Your best yet | Ваш лучший результат |
| `activity.no_reading_today` | No reading yet today | Сегодня ещё не читали |
| `activity.session_count` | %1 session | %1 сеанс |
| `activity.sessions_count` | %1 sessions | Сеансов: %1 |
| `activity.book_count` | %1 book | %1 книга |
| `activity.books_count` | %1 books | Книг: %1 |
| `activity.this_week` | This week | За неделю |
| `activity.all_time` | All time | За всё время |
| `activity.since` | Since %1 | С %1 |
| `activity.reading` | Reading | Чтение |
| `activity.per_day` | ≈ %1 a day | ≈ %1 в день |
| `activity.finished` | Finished | Прочитано |
| `activity.book` | book | книга |
| `activity.books_unit` | books | книг |
| `activity.in_progress` | %1 in progress | В процессе: %1 |
| `activity.sessions` | Sessions | Сеансы |
| `activity.average` | %1 average | В среднем %1 |
| `activity.days_read` | Days read | Дней чтения |
| `activity.of_days` | of %1 days | из %1 дн. |
| `activity.of_days_one` | of %1 day | из %1 дня |
| `activity.saved_words` | Saved words | Сохранённые слова |
| `activity.this_week_count` | %1 this week | За неделю: %1 |
| `activity.books` | Books | Книги |
| `activity.this_month_count` | %1 this month | За месяц: %1 |
| `activity.longest` | Longest | Самый долгий |
| `activity.this_month` | This month | За месяц |
| `activity.days_of` | %1 of %2 days | Дней: %1 из %2 |
| `activity.days_of_one` | %1 of %2 day | %1 из %2 дня |
| `activity.less` | Less | Меньше |
| `activity.more` | More | Больше |
| `activity.day_read` | %1 day read | %1 день чтения |
| `activity.days_read_count` | %1 days read | Дней чтения: %1 |
| `activity.finished_mark` | Finished | Прочитана |
| `activity.time_read` | Time read | Время чтения |
| `activity.to_finish` | To finish | До конца |
| `activity.last_read` | Last read | Последнее чтение |
| `activity.around` | ≈ by %1 | ≈ к %1 |
| `activity.estimate` | ≈ at your pace | ≈ в вашем темпе |
| `activity.progress` | Progress | Прогресс |
| `activity.started` | Started %1 | Начало: %1 |
| `activity.time_per_day` | Time per day | Время в день |
| `activity.at_recent_pace` | at your recent pace | в вашем недавнем темпе |
| `activity.range` | %1 → %2 | %1 → %2 |
| `activity.today_at` | Today, %1 | Сегодня, %1 |
| `activity.yesterday_at` | Yesterday, %1 | Вчера, %1 |
| `activity.when_you_read` | When you read | Когда вы читаете |
| `activity.mostly` | Mostly %1 – %2 | Чаще всего %1–%2 |
| `activity.records` | Records | Рекорды |
| `activity.longest_session` | Longest session | Самый долгий сеанс |
| `activity.most_in_day` | Most in one day | Больше всего за день |
| `activity.longest_streak` | Longest streak | Самая длинная серия |
| `activity.milestones` | Milestones | Достижения |
| `activity.progress_of` | %1 / %2 | %1 / %2 |
| `activity.milestone_first_book` | First book | Первая книга |
| `activity.milestone_books` | Books | Книги |
| `activity.milestone_hours` | Hours | Часы |
| `activity.milestone_streak` | Day streak | Дней подряд |
| `activity.milestone_days_read` | Days read | Дни чтения |
| `activity.all_milestones` | All milestones | Все достижения |
| `activity.milestones_title` | Milestones | Достижения |
| `activity.reached_of` | %1 of %2 | %1 из %2 |
| `activity.milestones_books` | Books finished | Прочитанные книги |
| `activity.milestones_hours` | Hours read | Часы чтения |
| `activity.milestones_streak` | Streaks | Серии |
| `activity.milestones_days` | Reading days | Дни чтения |
| `activity.phrase_books_none` | You haven't finished a book here yet. | Вы ещё не дочитали здесь ни одной книги. |
| `activity.phrase_books_one` | You've finished one book. | Вы прочитали одну книгу. |
| `activity.phrase_books` | You've finished %1 books. | Прочитано книг: %1. |
| `activity.phrase_hours_none` | Your reading hours start here. | Здесь начинается отсчёт часов чтения. |
| `activity.phrase_hours_one` | You've read for one hour. | Вы читали один час. |
| `activity.phrase_hours` | You've read for %1 hours. | Часов чтения: %1. |
| `activity.phrase_streak_none` | Read today to start a streak. | Почитайте сегодня, чтобы начать серию. |
| `activity.phrase_streak_one` | Your longest streak is one day. | Ваша самая длинная серия — один день. |
| `activity.phrase_streak` | Your longest streak is %1 days. | Ваша самая длинная серия: %1 дн. |
| `activity.phrase_streak_ongoing` | Your current ongoing streak is at %1 days. | Ваша текущая серия: %1 дн. |
| `activity.phrase_streak_ongoing_one` | Your current ongoing streak is at one day. | Ваша текущая серия — один день. |
| `activity.phrase_streak_ended` | Your longest streak of %1 days ended on %2. | Ваша самая длинная серия (%1 дн.) завершилась %2. |
| `activity.phrase_streak_ended_one` | Your longest streak of one day ended on %1. | Ваша самая длинная серия (один день) завершилась %1. |
| `activity.phrase_current_none` | You have no streak going now. | Сейчас у вас нет серии чтения. |
| `activity.phrase_current_one` | Your current streak is one day. | Ваша текущая серия — один день. |
| `activity.phrase_current` | Your current streak is %1 days. | Ваша текущая серия: %1 дн. |
| `activity.phrase_days_none` | Each day you read counts here. | Здесь учитывается каждый день чтения. |
| `activity.phrase_days_one` | You've read on one day. | Вы читали один день. |
| `activity.phrase_days` | You've read on %1 days. | Дней чтения: %1. |
| `activity.phrase_next` | %1 more to reach %2. | До цели %2 осталось: %1. |
| `activity.phrase_all` | You've reached every milestone. | Вы достигли всех целей. |

## Экран сна

| Ключ | EN (upstream) | Утверждённый RU |
|---|---|---|
| `sleep.design_custom` | Custom | Свой вариант |
| `sleep.custom_title` | Customize Lock & Sleep Screen | Экран блокировки и сна |
| `sleep.custom_editing` | Screen | Экран |
| `sleep.custom_background` | Background | Фон |
| `sleep.custom_status` | Status | Статус |
| `sleep.custom_same` | Same screen for sleep and power off | Один экран для сна и выключения |
| `sleep.custom_same_description` | Turn this off to choose a different screen for when the device is switched off. | Выключите, чтобы выбрать отдельный экран для выключенного устройства. |
| `sleep.backdrop_cover` | Book cover | Обложка книги |
| `sleep.backdrop_page` | Reading page | Страница книги |
| `sleep.backdrop_overlay` | PNG over page | PNG поверх страницы |
| `sleep.backdrop_wallpaper` | Wallpaper | Обои |
| `sleep.backdrop_plain` | Blank | Пустой фон |
| `sleep.status_none` | None | Нет |
| `sleep.wallpaper_random` | Random wallpaper | Случайные обои |
| `sleep.wallpaper_help_title` | Wallpapers | Обои |
| `sleep.wallpaper_help` | Did you know you can use your own wallpapers? Copy PNG or JPEG images to .adds/kobalt/wallpapers on your Kobo. Each image becomes a step in the Background row, and Random wallpaper picks a different one every time your Kobo sleeps. | Можно использовать свои обои. Скопируйте PNG или JPEG в .adds/kobalt/wallpapers на Kobo. Каждое изображение появится в строке «Фон», а «Случайные обои» будут выбирать новое при каждом переходе Kobo в сон. |
| `sleep.step_count` | %1 of %2 | %1 из %2 |
| `sleep.arrange_title` | %1 position | Положение: %1 |
| `sleep.arrange_hint` | Tap a dashed area to move the %1. | Нажмите на пунктирную область, чтобы переместить элемент «%1». |
| `sleep.arrange_book_only` | Only for this book | Только для этой книги |
| `sleep.arrange_book_only_description` | Other books keep their own position. | Другие книги сохранят своё положение. |
| `sleep.place_top_left` | Top left | Сверху слева |
| `sleep.place_top_right` | Top right | Сверху справа |
| `sleep.place_bottom_left` | Bottom left | Снизу слева |
| `sleep.place_bottom_right` | Bottom right | Снизу справа |
| `sleep.place_left` | Left | Слева |
| `sleep.place_right` | Right | Справа |
| `sleep.place_top` | Top | Сверху |
| `sleep.place_bottom` | Bottom | Снизу |
| `sleep.follow_orientation` | Follow reading orientation | Как при чтении |
| `sleep.rotation_enabled` | Your sleep screen will now also use landscape. | Экран сна теперь поддерживает и альбомную ориентацию. |
| `sleep.rotation_disabled` | Your sleep screen will only use portrait. | Экран сна будет только в книжной ориентации. |

## Подписи числа слов в общей статистике

| Ключ | EN (upstream) | Утверждённый RU |
|---|---|---|
| `stats.chapter_words` | Chapter %1 · %2 | Глава %1 · %2 |
| `stats.this_chapter_words` | This chapter · %1 | Эта глава · %1 |
| `stats.book_words` | Book · %1 | Книга · %1 |
