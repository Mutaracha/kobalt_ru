# Beta7 — полный список RU-предложений на согласование

**Статус: пользователь согласовал весь список; применён в assets при слиянии beta7.**
Это запись согласования, не свидетельство новой проверки на устройстве. База upstream beta6 `7ff437e`,
цель beta7 `e86533c80e8aa41bfc7d79c9e445cbd4a6405039`.
Инвентаризация сравнением распарсенных словарей: **57 новых, 3 изменённых,
2 удалённых ключа**. Включены и Android-only строки; это не только release notes.
Все placeholders в предложениях сверены с beta7. Длина — число символов, не
измеренная ширина на Kobo. Дополнительно применены согласованные уточнения существующих строк в конце документа.

## Термины и важные решения

- «Синхронизация» везде сохранена. x-auth/HTTP/DNS/KOSync — имена протоколов,
  не переводятся; в сообщениях об ошибках сохранены различия между сетью,
  авторизацией, сервером и локально сохранённой позицией.
- Standby — **режим ожидания**, а не ручной сон и не режим заставки.
  По умолчанию новая опция выключена; предупреждение о расходе батареи сохранено.
- «Сворачиваемые главы»: раскрытие групп оглавления, не скрытие текста книги.
- Компактные размеры словаря: предлагается оставить **S / M / L / XL**, как в
  upstream, только там, где полные подписи не помещаются. Полные RU подписи
  «Маленький / Обычный / Большой / Максимальный» сохраняются без изменений.
  Замена на длинные проценты может снова нарушить narrow-layout fallback.
- Окно About теперь общее для библиотеки/читалки. «О Kobalt» — новый пункт
  библиотеки, не переименование существующих нижних вкладок.

## Новые строки

| Ключ | EN beta7 | Предлагаемый RU | EN/RU |
|---|---|---|---|
| `kosync.no_connection` | No network connection is available for sync. Connect to Wi-Fi and try again. Your position is saved locally. | Нет сети для синхронизации. Подключитесь к Wi-Fi и повторите попытку. Позиция сохранена на устройстве. | 108/102 |
| `kosync.connection_timeout` | No network connection became available before sync timed out. Connect to Wi-Fi and try again. Your position is saved locally. | Сеть не появилась за время ожидания синхронизации. Подключитесь к Wi-Fi и повторите попытку. Позиция сохранена на устройстве. | 125/125 |
| `kosync.dns_error` | Could not resolve the sync server name. Check the server URL and your network DNS connection. | Не удалось определить адрес сервера синхронизации. Проверьте адрес сервера и работу DNS в вашей сети. | 93/101 |
| `kosync.connection_refused` | The sync server refused the connection. Check the server URL, port and whether the service is running. | Сервер синхронизации отклонил подключение. Проверьте адрес, порт и работу сервера. | 102/82 |
| `kosync.connection_closed` | The connection closed before sync finished. Check the server and network, then try again. | Соединение закрылось до завершения синхронизации. Проверьте сервер и сеть, затем повторите попытку. | 89/99 |
| `kosync.tls_error` | Could not establish a trusted secure connection. Check the server certificate, hostname and device date. Keep certificate verification enabled. | Не удалось установить доверенное защищённое соединение. Проверьте сертификат, имя сервера и дату на устройстве. Не отключайте проверку сертификата. | 143/147 |
| `kosync.timeout_error` | Sync timed out. Check the server and network connection, then try again. Your reading position was kept. | Время ожидания синхронизации истекло. Проверьте сервер и сеть, затем повторите попытку. Позиция чтения сохранена. | 104/113 |
| `kosync.interrupted` | The sync connection was interrupted. Try again. Your reading position was kept. | Соединение синхронизации прервано. Повторите попытку. Позиция чтения сохранена. | 79/79 |
| `kosync.forbidden` | The server denied access to sync. Check account permissions and whether this endpoint supports KOSync x-auth authentication. | Сервер запретил синхронизацию. Проверьте права учётной записи и поддержку авторизации KOSync x-auth по этому адресу. | 124/116 |
| `kosync.redirect_error` | The sync server redirected the request. Credentials were not forwarded. Check the configured KOSync URL and path prefix. | Сервер перенаправил запрос синхронизации. Данные входа не передавались. Проверьте адрес KOSync и префикс пути. | 120/110 |
| `kosync.route_error` | The server does not provide this KOSync route. Check the URL and path prefix, and whether the endpoint supports KOSync x-auth authentication. | По этому пути нет сервиса KOSync. Проверьте адрес, префикс пути и поддержку авторизации KOSync x-auth. | 141/102 |
| `kosync.rate_limited` | The sync server is limiting requests. Wait before trying again. | Сервер ограничивает частоту запросов. Повторите синхронизацию позже. | 63/68 |
| `kosync.server_error` | The sync server reported an error. Try again later or ask the server administrator to check its logs. | Сервер синхронизации сообщил об ошибке. Повторите попытку позже или попросите администратора проверить журнал сервера. | 101/118 |
| `kosync.http_error` | The sync server returned an unexpected HTTP status. Check the URL and whether the endpoint is a compatible KOSync service. | Сервер синхронизации вернул неожиданный HTTP-статус. Проверьте адрес и совместимость сервера с KOSync. | 122/102 |
| `kosync.response_too_large` | The server response exceeds the sync size limit. Check the URL and whether the endpoint is a compatible KOSync service. | Ответ сервера превышает допустимый размер. Проверьте адрес и совместимость сервера с KOSync. | 119/92 |
| `kosync.reader_timeout` | Sync could not finish while the reader was busy. Try again after the current operation finishes. Your reading position was kept. | Синхронизация не завершилась, пока читалка была занята. Повторите попытку после завершения текущей операции. Позиция чтения сохранена. | 128/134 |
| `library.about` | About Kobalt | О Kobalt | 12/8 |
| `chapter_contents.collapse_group` | Collapse %1 | Свернуть %1 | 11/11 |
| `chapter_contents.expand_group` | Expand %1 | Развернуть %1 | 9/13 |
| `chapter_contents.expanded` | Expanded | Развёрнуто | 8/10 |
| `chapter_contents.collapsed` | Collapsed | Свёрнуто | 9/8 |
| `reader.appearance_preview` | Preview | Предпросмотр | 7/12 |
| `settings.section_links` | Links | Ссылки | 5/6 |
| `settings.section_chapter_list` | Chapter List | Список глав | 12/11 |
| `settings.chapter_groups` | Collapsible chapters | Сворачиваемые главы | 20/19 |
| `settings.chapter_groups_hint` | Choose whether chapter groups can collapse and how they open. | Выберите, можно ли сворачивать группы глав и как они раскрываются. | 61/66 |
| `settings.chapter_groups_current` | Current section | Текущий раздел | 15/14 |
| `settings.chapter_groups_current_description` | Expand the current chapter's parent groups. Default. | Раскрывать группы, содержащие текущую главу. По умолчанию. | 52/58 |
| `settings.chapter_groups_all` | All chapters | Все главы | 12/9 |
| `settings.chapter_groups_all_description` | Expand every group to show the full chapter list. | Раскрывать все группы и показывать весь список глав. | 49/52 |
| `settings.chapter_groups_off` | Off | Выключено | 3/9 |
| `settings.chapter_groups_off_description` | Show every chapter without expand or collapse buttons. | Показывать все главы без кнопок сворачивания и раскрытия. | 54/57 |
| `settings.section_advanced` | Advanced | Дополнительно | 8/13 |
| `settings.disable_reader_standby` | Disable standby | Отключить ожидание | 15/18 |
| `settings.disable_reader_standby_description` | Keeps the device awake while reading. Uses more battery power. | Не переходить в режим ожидания при чтении. Расходует больше заряда. | 62/67 |
| `settings.disable_reader_standby_title` | Disable standby while reading? | Отключить режим ожидания при чтении? | 30/36 |
| `settings.disable_reader_standby_warning` | Disabling standby can significantly reduce battery life. It may help with intermittent freezes or missed page turns on some devices, but is generally not recommended. You can still put the device to sleep. Are you sure you want to disable standby? | Отключение режима ожидания может значительно сократить время работы. На некоторых устройствах это может помочь при периодических зависаниях или пропусках перелистывания. Устройство по-прежнему можно вручную перевести в сон. Вы уверены, что хотите отключить режим ожидания? | 247/272 |
| `settings.dictionary_font_small_short` | S | S | 1/1 |
| `settings.dictionary_font_standard_short` | M | M | 1/1 |
| `settings.dictionary_font_large_short` | L | L | 1/1 |
| `settings.dictionary_font_extra_large_short` | XL | XL | 2/2 |
| `buttons.title` | Button controls | Управление кнопками | 15/19 |
| `buttons.upper` | Upper button | Верхняя кнопка | 12/14 |
| `buttons.lower` | Lower button | Нижняя кнопка | 12/13 |
| `buttons.volume_up` | Volume up (+) | Громкость (+) | 13/13 |
| `buttons.volume_down` | Volume down (−) | Громкость (−) | 15/13 |
| `buttons.description` | Choose what each page button does while reading. | Выберите действие каждой кнопки перелистывания при чтении. | 48/58 |
| `buttons.volume_description` | Choose what the volume buttons do while reading. | Выберите действия кнопок громкости при чтении. | 48/46 |
| `buttons.navigation_hint` | Menus, lists and page previews keep their usual button directions. | В меню, списках и предпросмотре страниц кнопки сохраняют обычные направления. | 66/77 |
| `buttons.previous` | Previous page | Предыдущая страница | 13/19 |
| `buttons.next` | Next page | Следующая страница | 9/18 |
| `buttons.none` | Do nothing | Ничего не делать | 10/16 |
| `buttons.none_description` | Ignore this button while reading. | Игнорировать эту кнопку при чтении. | 33/35 |
| `buttons.volume_title` | Volume button controls | Кнопки громкости | 22/16 |
| `buttons.volume_note` | On Android, these mappings use the physical volume buttons. | На Android эти действия назначаются физическим кнопкам громкости. | 59/65 |
| `buttons.while_reading` | Decide what this hardware button should do when you're reading. | Выберите действие этой физической кнопки при чтении. | 63/52 |
| `buttons.hardware_title` | Hardware buttons | Физические кнопки | 16/17 |

## Изменённые строки

| Ключ | EN beta7 | Предлагаемый RU | EN/RU |
|---|---|---|---|
| `kosync.auth_error` | The server rejected the sync credentials. Check your username, password and that the account exists on this server. It must support KOSync x-auth authentication. | Сервер отклонил данные входа. Проверьте имя, пароль и наличие учётной записи на этом сервере. Сервер должен поддерживать авторизацию KOSync x-auth. | 161/147 |
| `kosync.network_error` | Could not complete the sync request. Check the server URL and network connection, then try again. | Не удалось выполнить запрос синхронизации. Проверьте адрес сервера и подключение к сети, затем повторите попытку. | 97/113 |
| `kosync.invalid_response` | The server response is not compatible with KOSync. Check the server URL and path prefix. Your reading position was kept. | Ответ сервера несовместим с KOSync. Проверьте адрес сервера и префикс пути. Позиция чтения сохранена. | 120/101 |

## Прежние формулировки изменённых сообщений

| Ключ | Текущий RU |
|---|---|
| `kosync.auth_error` | Не удалось войти. Проверьте имя пользователя и пароль. |
| `kosync.network_error` | Сервер недоступен. Проверьте Wi-Fi, подключение к интернету и адрес сервера. |
| `kosync.invalid_response` | Сервер вернул неверные данные прогресса. Ваша позиция сохранена. |

## Удалённые upstream строки

| Ключ | Прежний RU | Причина |
|---|---|---|
| `reader.show_link_indicator` | Показывать индикатор ссылок при чтении | Удалён отдельный индикатор ссылок; tap/long press и подсветка остаются. |
| `reader.link_indicator_description` | Показывает индикатор на страницах со ссылками. Нажмите на него, чтобы показать ссылки. | Удалён отдельный индикатор ссылок; tap/long press и подсветка остаются. |

## Приоритетная проверка длины

- `buttons.next`: 9/18 — «Следующая страница».

Даже строки ниже этих порогов требуют UI-проверки, особенно заголовки и кнопки.

## Уточнение пользователя: предупреждение standby

В строку `settings.disable_reader_standby_warning` выше внесена **дословная
пользовательская формулировка**, включая финальный вопрос. Затем пользователь согласовал весь beta7 список и дополнительные уточнения;
строка применена в assets при слиянии. Длина — 272 символов; placeholder отсутствует.

## Дополнительные уточнения после просмотра ab6d9a0 — согласованы и применены

Просмотрена вся серия `screen_001` … `screen_044` из пользовательского коммита
093dd4d; карта контекстов и ограничения — в `FB2_FORK_PLAN.md`, материалы —
[`tests/manual`](../tests/manual/README.md). Это отдельные предложения к уже
существующим строкам, **не часть upstream-дельты 57/3/2**. Пользователь
согласовал весь перевод и разрешил продолжить слияние; уточнения применены.

| Ключ | EN сейчас | RU сейчас → предложение | EN/RU нового | Контекст / причина |
|---|---|---|---|---|
| `settings.navigation_buttons_only` | Hardware page turn buttons only | Только аппаратные кнопки перелистывания → **Только физические кнопки** | 31/24 | 017: подпись обрезана; «физические» согласуется с новым beta7 Hardware buttons. |
| `settings.navigation_needs_swipes` | Choose another tap-zone preset to turn off swipes. | Выберите другой пресет зон касаний, чтобы отключить свайпы. → **Выберите другую схему зон касаний, чтобы отключить свайпы.** | 50/58 | 017–018: объяснить «пресет» через видимую схему зон; смысл ограничения прежний. |
| `settings.justification_tracking_subtle` | Subtle | Лёгкий → **Лёгкая** | 6/6 | 027: согласовать род с «подгонка» и вариантом «Обычная». |
| `settings.justification_tracking_more` | More | Сильный → **Сильная** | 4/7 | 027: то же; не менять степень подгонки. |
| `reader.publisher_spacing` | Publisher | Издатель → **Из книги** | 9/8 | 025/029: это сохранение оформления, а не имя издателя; компактно для slider markers и summary. |
| `settings.widows_orphans_publisher` | Publisher | Издательский → **Из книги** | 9/8 | 030: тот же источник правил вместо неясного «Издательский». |
| `settings.vertical_spacing_mode` | Page spacing | Интервалы страницы → **Вертикальные интервалы** | 12/22 | 025/031: одинаковое название строки и открываемого окна; ключ settings.vertical_spacing уже имеет этот RU. |
| `settings.vertical_spacing_description` | Adjust blank space on nearly full pages. | Заполнение почти полных страниц. → **Распределение свободной высоты страницы.** | 40/40 | 025/031: уточнить, что настройка касается вертикального свободного пространства, не полей. |
| `settings.beautifier_spacing` | Remove paragraph gaps | Убрать интервалы абзацев → **Убрать интервалы между абзацами** | 21/31 | 032: явно назвать межабзацное расстояние, не путать с межстрочным. |
| `settings.beautifier_spacing_description` | Keep recognized scene breaks and special passages. | Убирает пустое место между абзацами. Разрывы сцен сохраняются. → **Убирать пустое место между обычными абзацами. Разрывы сцен и особые блоки сохраняются.** | 50/86 | 032: указать защищённые исключения; соответствует marked prose policy и EN. |
| `settings.beautifier_indent` | Standardize first-line indents | Единые отступы абзацев → **Единый отступ первой строки** | 30/27 | 032: «отступы абзацев» неоднозначны; речь о красной строке. |
| `settings.beautifier_indent_description` | Your paragraph adjustments take priority. | Единая красная строка; ваши отступы важнее. → **Обычные абзацы: красная строка 120%. Явный отступ в окне «Настройка абзацев» имеет приоритет.** | 41/93 | 032: проверено engine_document.cpp — 1.2em, затем явный paragraphIndent override; не обещать одинаковый отступ у заголовков/особых блоков. |
| `common.size_small_short` | S | М → **S** | 1/1 | 011: единая S/M/L/XL шкала вместо М/С/Б/XL; только короткие общие markers. |
| `common.size_medium_short` | M | С → **M** | 1/1 | 011: то же; полные RU размеры и отдельные dictionary compact keys не переименовываются. |
| `common.size_large_short` | L | Б → **L** | 1/1 | 011: то же; XL уже совпадает. |

Длина не равна ширине. Особое внимание после согласования: длинные пояснения
beautifier должны переноситься в общей row factory beta7 с правильной высотой
и пагинацией; заголовок «Вертикальные интервалы» проверить в узком layout.
Текущий экран 032 показывает старый текст — он не доказывает, что новый поместился.
Короткую подпись физических кнопок проверить в том же radio dialog, включая
disabled состояние; не менять input behavior ради исправления текста.

Нижние вкладки, «Стиль меню», «Замок», «Постоянно», результат синхронизации,
«Просмотреть / Закрыть / Вписать», согласованные sleep/сноски и About rendering
не меняются. Варианты текущего «Поворот» и пресеты колонтитулов автоматически
не переименовываются. Английские литературные образцы 026–031 в старом билде —
не пропуски локализации пунктов меню; beta7 style_sample проверяется отдельно.
