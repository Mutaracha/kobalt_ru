# Перенос beta7 — план и предварительный аудит

**Статус: подготовка и согласование RU; слияние не выполнено.**
Проверенная база форка: код `ab6d9a0`, документация `2409912`.
Пользователь подтвердил работу сносок после проверочного билда InDesign;
это новая device-confirmed база, не обещание полного визуального A/B с KOReader.

Цель — [upstream v1.0-beta.7](https://github.com/nicoverbruggen/kobalt/releases/tag/v1.0-beta.7),
commit `e86533c80e8aa41bfc7d79c9e445cbd4a6405039`, предыдущий upstream beta6
`7ff437e8d20196c1082a763cc80584d5ca5db893` уже является предком форка.
Исходники получены напрямую с GitHub, работа остаётся только в
`arena/9e626d86-kobalt-ru`. Локальные tags/другие branches не создавались.

## Порядок — как при beta6

1. **Подготовка / инвентаризация:** прочитать release notes и полный source diff;
   записать дельту строк, конфликтные и автоматически объединяемые fork hooks.
   Сохранить рабочую исходную точку; не менять поведение до согласования RU.
2. **Согласование строк:** полный EN/RU proposal и длины в
   [translation-proposal-beta7.md](translation-proposal-beta7.md). Не применять
   перевод в assets до ответа пользователя. Старые согласованные формулировки
   вне upstream delta не переписывать; Android строки включить в паритет.
3. **Перенос:** merge upstream в ту же ветку с адаптацией fork helpers к новой
   структуре. Не восстанавливать старые копии settings/About/preview screens.
   Общие progress/save, настройки, новые previews и hardware controls — upstream;
   FB2 использует ту же развивающуюся formatting pipeline, что EPUB.
4. **Сопровождение:** source audit всей fork delta, не только конфликтов;
   обновить AGENTS, storage contract, пользовательскую инструкцию и этот план.
5. **Проверки:** EN/RU key/placeholder/source refs и lengths; production offline;
   focused native engine/locale/settings/About/preview/button regressions;
   отдельный оригинальный InDesign EPUB; Kobo Qt5 cross-build/package/contents.
   Полную native/UI/lifecycle suite отделить от focused coverage. Qt6/Android
   не являются блокером Qt5, но общий код и host contracts не ломать.
6. **Handoff:** дать конкретный проверенный SHA и artifact, перечислить
   device-only проверки. Не объявлять визуальную проверку выполненной по CI.

## Что изменилось в upstream и где риск

| Область | Проверенные изменения | Правило переноса |
|---|---|---|
| Text/Layout | Live preview в reading size, отдельные preview widgets, anchor-based image; margins drag/step; отмена stale previews | Использовать общий upstream pipeline для EPUB/FB2. Проверить возврат позиции и неизменность основного page cache после preview; отдельно страницы с нижними сносками |
| Оглавление | Новая chapter_groups/current/all/off, сворачивание вложенных групп | Учитывать настоящее вложенное оглавление FB2, не делать отдельную урезанную реализацию |
| Кнопки | Новые pageButtons/actions, hardware mapping и Android volume buttons | Чтение previous/next/none; меню/списки/preview сохраняют направления upstream. Не менять существующие gestures |
| Standby | Optional disable_reader_standby, default false, подтверждение, host support guard | Взять upstream без самодельной power policy. Различать standby и ручной sleep. Не подключать ранее отложенный Wi-Fi saver |
| About | Новый общий app/about/AboutPage используется библиотекой и Settings | Перенести только fork-label/debug attachment в общую точку. Stock version/commit rendering неизменен; не хранить две fork-копии About |
| KOSync | Подробные safe error categories, structured not-found bootstrap, server diagnostics | Сохранить upstream save/conflict/auth/security semantics; перевести все сообщения, не логировать credentials/server body |
| Ссылки | Удалён отдельный reading-page indicator, есть секция Links; tap/long press сохраняются | Не возвращать удалённый индикатор. Не потерять нижние сноски, popup и их настройку |
| UI | Новая геометрия dialogs/descriptions/options, header-footer miniature, compact dictionary labels | Обновить fork row factory с учётом новых общих правил, не замораживать старые margins. IBM Plex/approved fonts, baseline вкладок, theme icon/order сохранить |
| Подсветка | Финальный процент удерживается минимум 1500 ms, PreciseTimer | Сохранить upstream debounce/master switch. RU «после изменения», а не «после паузы», остаётся верным |
| Covers/Tolino | Native Library cover reuse, Tolino sleep/cover support | Общие opening/lifecycle пути upstream; сохранить FB2 publication guards |
| Assets/dependencies | EN strings и version manifest изменились; dependencies delta отсутствует | Сохранить RU/hyphenation/fork metadata. CREngine pin не менять ради переноса; EPUB v3/FB2 v6 не повышать без реального изменения CSS/DOM policy |
| Хранилище | Optional chapter_groups, button_actions, disable_reader_standby; новая data/database schema не вводится | Сохранить unknown keys/control IDs и текущие версии схем. Не подключать cache cleanup и не удалять данные |

## Предварительное пробное объединение

Выполнен `git merge-tree --write-tree HEAD <beta7-sha>` — **без изменения index,
рабочих файлов и без начатого merge**. Обнаружены content conflicts:

- `assets/en.strings` (upstream добавления рядом с fork strings);
- `platforms/desktop/CMakeLists.txt` (новые sources/тесты и fork регистрации);
- `src/app/reader/settings/settings_panel.cpp`;
- `src/app/reader/settings/settings_panel.h`;
- `src/app/reader/window/reader_build.cpp`;
- `src/app/reader/window/reader_p.h`;
- `src/app/reader/window/reader_settings.cpp`.

Auto-merge не равен проверке: вручную сверить engine_document/source helpers,
manifest, appearance_settings, chapter_contents, reader_refresh, Library/About,
platform CMake и tests. Например, About переносится в новый общий класс, но
старый helper зависит от SettingsPanelInputs: нужен связный перенос ответственности,
а не удержание старых inputs только ради fork label.

В upstream engine delta добавлен imageFromAnchor, ReaderSession — anchoredPage;
parser/footnote splitter в этом diff не менялись. Тем не менее preview временно
меняет страницу для рисования и восстанавливает cache/damage — нужны regressions
для нижних сносок и source anchor, а не только успешная компиляция.

## Результат подготовки

- Release/source inventory и merge-tree review выполнены.
- Новые/изменённые строки полностью сопоставлены; placeholder/length review в proposal.
- Actual merge, применение переводов, beta7 compilation/native/device validation
  **не выполнялись**. Рабочий бинарный билд остаётся beta6 + проверенные сноски.
- Следующий шаг: согласовать RU proposal, затем выполнять перенос и проверки.
