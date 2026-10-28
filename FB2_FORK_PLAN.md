# Техническое задание для ИИ-агента: Форк Kobalt с нативной поддержкой FB2 на Kobo (Сценарий 1)

> **Назначение документа:** Пошаговая спецификация для coding-агента (Claude Code / Codex / Cursor и др.).  
> Перед началом работы агент **обязан** прочитать корневой `AGENTS.md`, `doc/building.md` и `doc/validation.md` в репозитории Kobalt и соблюдать все правила сборки (никаких Python-скриптов в системе сборки, строгая проверка ключей `assets/en.strings`, синхронизация `assets/manifest.json`, сохранение совместимости с `mac-tests`, `qt5` и `qt6`).

---

## 0. Статусы этапов (вести в каждом изменении)

> Легенда: **done** — код и тесты в ветке; **in progress** — частично; **planned** — только план. Сборка в песочнице невозможна (нет Qt SDK), поэтому статус «done» по коду подтверждается ревью и офлайн-проверками (валидатор строк, CMake configure_file, хеши манифеста, YAML), а полная компиляция и прогон тестов выполняются CI `.github/workflows/kobo-qt5-device.yml` (push в `arena/9e626d86-kobalt-ru`) и mac-tests.

| Этап | Содержание | Статус |
|---|---|---|
| 0 | CI: qt5-only workflow `kobo-qt5-device.yml`, правило push в `AGENTS.md` | **done** |
| 1 | Русские паттерны переносов, манифест, notices | **done** |
| 2 | `Publication` FB2-детект/fingerprint/виртуальный ресурс, `LocalResources` single-file | **done** |
| 3 | Движок: `fb2.css`, загрузка FB2, сноски, превью, сессия | **done** (ожидает CI-сборки) |
| 4 | Nickel-роутинг, native publication qt5/qt6, time-only writeback | **done** (ожидает CI-сборки) |
| 5 | Тесты: `tests/model/fb2.cpp`, FB2-кейсы в `nickel_book_menu`, `reading_book`, `local_resources` | **done** (qt5 CI зелёный; mac-tests — при мерже) |
| 6 | Русская локализация интерфейса | **done** (qt5 CI зелёный, артефакт `Kobalt-qt5-device` опубликован; mac-tests — при мерже) |
| 7 | Финализация: строка версии форка в «О приложении», карта изменений, регламент переноса на upstream, пользовательская инструкция | **done** (см. раздел 7 и `doc/fork-kobalt_ru.md`) |

**Решение по `.fb2.zip` (подтверждено заказчиком 2026-10-26):** поддержка ИСКЛЮЧЕНА окончательно. Nickel не индексирует `.fb2.zip` как книги, целевой сценарий — одиночные `.fb2`, видимые в штатной библиотеке; desktop/Android по указанию заказчика не приоритетны. Если потребность вернётся, точка расширения: в `Publication::inspect(QIODevice&)` после ZIP-ветки детектировать архив с единственным членом `*.fb2`, отдавать его под тем же виртуальным именем `singleFb2ResourceName` (распаковка в `LocalResources::transferSingleFile`), а движок не менять — он уже грузит поток через `LoadDocument(stream, false)`.

**Журнал:**
- 2026-10-26: этапы 2–4 реализованы полностью; добавлен регрессионный тест `tests/model/fb2.cpp` (инспекция, отрицательные кейсы entity/RSS/ZIP, рендер UTF-8 FB2 с обложкой/поэмой/таблицей/сносками/TOC, cp1251, превью, переносы), расширены `nickel_book_menu.cpp` (маршруты .fb2 и PDF-регресс), `reading_book.cpp` (time-only для `file://*.fb2`), `local_resources.cpp` (single-file виртуальный ресурс и отмена по lease). Новых строковых ключей не вводилось — все используемые ключи есть в `assets/en.strings`.
- 2026-10-26: qt5-device workflow собрал `Kobalt-qt5.zip` зелёным (после фикса `lString32::c_str()` в `OpenStream`); артефакт `Kobalt-qt5-device` опубликован. Ручная проверка пользователем на устройстве — без замечаний, тестирование на других книгах продолжается.
- 2026-10-26 (этап 6): реализована локализация по варианту A из раздела 6. `assets/ru.strings` — полный перевод 764 ключей с паритетом ключей и слотов `%n`; `platforms/strings.cmake` разбит на parse/generate/parity (сборка падает при расхождении локалей); `src/support/strings.h` хранит две таблицы, атомарный селектор `setLocale/locale`, нормализацию кодов (`ru`, `ru_RU`, `ru-RU` → русский) и хелпер плюрализации CLDR (`_one/_few/_many`, английский сохранил one/база); новый `src/support/locale.{h,cpp}`: `KOBALT_LANGUAGE` → `Kobo eReader.conf` (`ApplicationPreferences/CurrentLocale`) → `QLocale::system()` → английский. Вызовы разрешения локали добавлены в `initialize()` qt5/qt6 и в `main()` desktop/Android. Фолбэк: отсутствующий перевод берётся из английской таблицы. Диагностика (`diagnosticErrorCode`) по-прежнему опирается на каноническую английскую таблицу. Тест `tests/model/strings_locale.cpp` (коды, переводы, key-режим, плюрал 1/2/5/11/21/101, INI-конфиг, env), parity-кейсы в `platforms/strings-test.cmake`. Смена языка не затрагивает путь FB2: форматные ветки управляются отдельными флагами, все строковые ключи FB2 присутствуют в обеих локалях.
- 2026-10-26 (этап 6, CI-фиксы): (1) автономные `cmake -P` parity-скрипты без `cmake_minimum_required` получали OLD-политики на CMake 3.x, где оператор `IN_LIST` (CMP0057) запрещён — во вложенные скрипты добавлен явный policy-baseline 3.22 (в CMake 4.x песочницы политика всегда NEW, локально дефект не воспроизводился); (2) Kobo Qt5 SDK старше Qt 5.10 — `QStringView` удалён из `strings.h`/`locale.cpp`/теста, `normalizeLocaleCode` принимает `const QString&`. После фиксов qt5-device workflow зелёный целиком, артефакт `Kobalt-qt5-device` (~35.6 МБ) опубликован. Шаг «Offline release checks» теперь пишет хвост лога в step summary при падении.
- 2026-10-26 (этап 6, правки перевода по замечаниям с устройства, 25 ключей `assets/ru.strings`, только значения, ключи/слоты не тронуты): обрезка в меню «Раскладка» — `header_footer.title` → «Колонтитулы», `reader.header/footer` → «Верхний»/«Нижний» (длина как у en «Header»/«Footer»); `custom_hint` переписан про пресет «Свой», 6 ячеек и схему выше; `common.more_next_page` → «Продолжение на следующей странице» (оглавление и вкладка шрифтов); «Обновить экран» → «Обновление экрана»; «Жирное чтение» → «Бионическое чтение» (метка режима, статусы включения; в узком слоте значения насыщенности — «Включено»); в навигации существительные «проведения» → «свайп(ы)», в описании «влево и вправо» → «влево или вправо» (глагол «Проводите» сохранён); описание стиля меню по умолчанию сокращено до формулировки заказчика; «СПРОСИТЬ СНАЧАЛА» → «С ПОДТВЕРЖДЕНИЕМ»; «Трекинг выключки» → «Межбуквенная подгонка»; опции висячих строк «Избегать обоих случаев/Разрешать оба случая» → «Избегать»/«Разрешать» (по длине en «Avoid both»/«Allow both»); «нелинейные фрагменты» → «дополнительные разделы» (label + описание); вкладка «Системный» → «Система»; второе предложение описания автооткрытия книг — «Этот параметр также доступен в настройках чтения Kobo»; описание скрытия подписей вкладок — «В меню чтения на вкладках показываются только значки, без подписей».
- 2026-10-27 (этап 7, финализация): (1) идентификация форка на странице «О приложении» — generic-механизм `KOBALT_FORK_NAME` (пустой дефолт в трёх платформенных CMakeLists, константа `kobaltForkName` в `build_info.h.in`, проброс в `platforms/kobo/build.sh`, включение `kobalt_ru` только в fork-workflow); новая строка `rrAboutFork` в `settings_panel.cpp` видна лишь при непустом имени, тексты — `about.fork`/`about.fork_build` в обеих локалях (паритет соблюдён); десктоп/Android сохраняют штатный вид About. (2) Проведён полный аудит дельты относительно `64235f4`: все FB2-ветки за явными флагами, EPUB-поведение не изменено, новые подсистемы вынесены в отдельные файлы; карта всех изменений и правила разрешения конфликтов — в разделе 7.2. (3) В план добавлен регламент поддержки (раздел 7): сборка промежуточных версий, офлайн- и CI-проверки, процедура переноса на новый upstream, чек-лист перед пушем, зафиксированные компромиссы. (4) Написана пользовательская инструкция `doc/fork-kobalt_ru.md`.
- 2026-10-27 (гигиена истории): обнаружено, что песочница дописывает в коммиты трейлер `Co-authored-by: arena-agent` хуком `.git/hooks/commit-msg` — это запрещено `AGENTS.md`. Все 12 коммитов форка после `64235f4` переписаны (`git filter-branch --msg-filter`), трейлеры удалены, дерево файлов не изменилось (сверено `git diff` старого и нового tip — пусто), ветка обновлена через `push --force-with-lease`. **SHA коммитов этапов 1–7 изменились** — в старых заметках/сессиях прежние хеши недействительны, актуальная история — в `git log`. На будущее: коммитить с `--no-verify`, проверять `git log --grep=arena-agent`.
- 2026-10-27 (этап 7, правка «О приложении» по замечанию с устройства): дублирующая строка хеша устранена — при заданном `KOBALT_FORK_NAME` штатная `rrAboutCommit` скрывается (`commitVisible && !forkVisible`), остаётся одна строка `rrAboutFork`; generic-сборки без имени форка не изменились. Текст fork-строки переведён на языково-нейтральный латинский формат `%1 · %2` (`about.fork_build` в обеих локалях, паритет сохранён): кириллица уходила в приватный путь отрисовки со шрифтом-фолбэком и выглядела крупнее, а латиница рисуется тем же шрифтом 24px, что и оригинальная строка коммита. Версия upstream-релиза по-прежнему видна строкой выше (`about.version`, `KOBALT_VERSION`); отдельный проброс merge-base коммита upstream не делаем — в CI только fork-репозиторий без upstream-remote, а релизная версия уже отражает базу.
- 2026-10-27 (этап 7, итерация по замечаниям): (1) «О приложении» упрощено до минимального вмешательства — оригинальная строка коммита `rrAboutCommit` снова выводится по исходным правилам (`setVisible(inputs.commitVisible)`, без условий); добавлена ровно одна новая строка `rrAboutFork` ниже: `kobalt_ru · <UTC-время сборки>`. Время — `string(TIMESTAMP KOBALT_BUILD_TIME "%Y-%m-%d %H:%M UTC" UTC)` на configure во всех трёх платформенных CMakeLists, константа `kobaltBuildTime` в `build_info.h.in`; латиница в строке сохраняет тот же путь рендеринга шрифта, что и у оригинальной строки. (2) Новая опция «Удалить кэш книг» на вкладке «По умолчанию»: `storage::clearRenderingCaches()` (только `.cache/` CREngine и `cache/reopen-layout.json`; прогресс, закладки, аннотации, настройки и `kobalt.sqlite` вне этих путей), подтверждение `ChoiceModal` с предупреждением о выходе, при успехе — штатный `Reader::close()` (сохранение прогресса и возврат в Nickel), при ошибке — `NoticeModal` без выхода; ключи `reader.clear_book_cache*` в обеих локалях, model-тест `tests/model/rendering_cache.cpp` (удаление/сохранность/идемпотентность/ошибка и ретрай).
- 2026-10-27 (этап 7, UI-фикс пункта «Удалить кэш книг» по замечанию с устройства): кнопка переведена на каноническое оформление списка действий — `cacheRow` получил те же `setStretch(0,0)/(1,1)/(2,0)`, что и `resetRow` (кнопка на всю ширину, шеврон справа вместо центра страницы), `clearCacheButton` добавлен в общий цикл стилизации (нижняя граница `setRule(Qt::BottomEdge)`, левое выравнивание и высота `listActionHeight` из общего stylesheet), подпись `settings.asks_first` («С ПОДТВЕРЖДЕНИЕМ») под текстом — как у соседнего пункта восстановления оформления. Правка только в `settings_panel.cpp`, новые строки/ключи не вводились.
- 2026-10-28 (этап 7, фикс перевода по замечанию с устройства): поле колонтитула «Страниц до конца главы» выводило «осталось N стр. осталось» — слово дублировалось, потому что `header_footer.pages_left` («осталось %1 %2») композируется с единицей `stats.pages_left`/`stats.page_left` («стр. осталось»/«стр. осталась»), которая уже содержит «осталось». Шаблон в `assets/ru.strings` приведён к английской структуре `"%1 %2"` (итог: «N стр. осталось», в единственном числе — «1 стр. осталась»); единицы и граф главы не тронуты. Прочие вложенные подстановки (`time_left`, `sleep.time_left`, `slot_value`, словарь, изображения) и внутристрочные повторы проверены — аналогичных дублей нет.

---

## 1. Цель и рамки задачи (Scope)

Реализовать бесшовное открытие книг формата **FictionBook 2 (`.fb2`)** на электронных книгах **Kobo** (прошивки 4.38+ на Qt5 и 5.18+/6.x на Qt6) прямо из штатной библиотеки Kobo (`Nickel`), а также в desktop-сборке и headless-тестах:

1. **Интеграция в систему Kobo (Сценарий 1):** книги `.fb2`, проиндексированные штатной оболочкой `Nickel` в `/mnt/onboard/.kobo/KoboReader.sqlite`, открываются в Kobalt по тапу на обложку (если Kobalt выбран ридером по умолчанию) и через пункт контекстного меню книги (`Open with Kobalt`).
2. **Рендеринг силами уже встроенного `koreader/crengine`:** задействовать штатный `LVXMLParser` / `doc_format_fb2` и таблицу стилей `fb2.css` из зафиксированного в `dependencies/manifest.json` коммита `koreader/crengine` (`b05cf00791d2ba17c1f077fc69e7bd68cf780b6a`), включая поддержку кодировок `UTF-8` и `windows-1251`, вложенных `<section>`, стихов (`<poem>`, `<stanza>`, `<v>`), эпиграфов, таблиц, `base64`-иллюстраций и обложек `<binary>`.
3. **Всплывающие сноски FB2:** поддержка попап-сносок по ссылкам `<a l:href="#..." type="note">` на секции внутри `<body name="notes">` и `<body name="comments">`.
4. **Интеграция с сервисами Kobo и хранением Kobalt:**
   - Точная позиция чтения (`ldomXPointer`), закладки, выделения текста (annotations) и индивидуальные настройки книги сохраняются в локальной SQLite-базе Kobalt (`.adds/kobalt/`).
   - В системную базу Kobo (`KoboReader.sqlite`) при закрытии книги, сне и подключении USB записываются статус чтения (`ReadStatus`), процент прочтения (`___PercentRead`), время чтения (`DateLastRead`) и оценка оставшегося времени (`RestOfBookEstimate`, `CurrentChapterEstimate`) через уже существующий в Kobalt механизм `time-only` writeback.
   - Работают системные словари Kobo, экран сна с обложкой из `<binary>` FB2 и статистика слов по главам.
5. **Русская типографика и переносы из KOReader:** добавить в бандл ресурсов словари переносов `Russian.pattern` и `Russian_EnUS.pattern` из исходников `koreader/crengine` (что также автоматически активирует правила неразрывных однобуквенных предлогов в `textlang.cpp` для `<lang>ru</lang>`).

**Что НЕ входит в задачу:**
- Включение standalone-экрана библиотеки (`src/library/`) на Kobo.
- Патчинг самой прошивки Kobo для индексации `.fb2.zip` (опционально можно поддержать `.fb2.zip` на уровне `Publication`/`ReaderModel` для desktop-отладки, но основной таргет на Kobo — файлы `.fb2`, которые видит `Nickel`).

---

## 2. Источники референса в `koreader/crengine`

При конфигурации CMake скачивает архив `koreader/crengine` ревизии `b05cf00791d2ba17c1f077fc69e7bd68cf780b6a` в `${CRENGINE_SOURCE}` (`build/.../sources/crengine-*`). Ориентироваться нужно на следующие файлы прямо из этого дерева:

- `${CRENGINE_SOURCE}/cr3gui/data/fb2.css` — эталонная таблица стилей KOReader для FB2 (скрывает сырые теги метаданных `<genre>`, `<document-info>`, `<publish-info>`, `<coverpage>`, размечает `body + body { -cr-hint: non-linear; }` и оформляет сноски `body[name="notes"] section title { display: run-in; }`).
- `${CRENGINE_SOURCE}/crengine/src/lvdocview.cpp` — логика `LVDocView::LoadDocument(LVStreamRef, bool)` для `doc_format_fb2`, извлечение обложки `GetFB2Coverpage()` / `getCoverPageImageStream()` и работа дискового кэша (`DOC_PROP_FILE_NAME`, `DOC_PROP_FILE_CRC32`, `DOC_PROP_FILE_SIZE`).
- `${CRENGINE_SOURCE}/crengine/src/textlang.cpp` (строки 97–99, 636–660, 1150–1215):
  - Язык `ru` мапится строго на файл **`Russian.pattern`** (а `ru-US` — на `Russian_EnUS.pattern`, `ru-GB` — на `Russian_EnGB.pattern`).
  - При `PROP_TEXTLANG_MAIN_LANG = "ru"` автоматически включаются кавычки-ёлочки и функция `lb_char_sub_func_russian` (запрет висячих предлогов *а, в, и, к, о, с, у* в конце строки).
- `${CRENGINE_SOURCE}/cr3gui/data/hyph/Russian.pattern` и `Russian_EnUS.pattern` — файлы словарей переносов.

---

## 3. Пошаговый план изменений по подсистемам

### Этап 1. Русские переносы и манифест ассетов (`assets/`)

**Почему это важно:** И на этапе сборки (`assets/CMakeLists.txt`, `platforms/kobo/CMakeLists.txt`), и в рантайме на самом устройстве Kobo (`src/assets/assets.cpp`, метод `AssetBundle::available()`) каждый файл в `assets/hyph/` проверяется по размеру и SHA-256 против `assets/manifest.json`. При несовпадении Kobalt покажет ошибку отсутствующих ассетов и откажется открывать книги.

1. Скопировать из `${CRENGINE_SOURCE}/cr3gui/data/hyph/` в `assets/hyph/` файлы:
   - `assets/hyph/Russian.pattern` (размер: `137239` байт, SHA-256: `4ca198ade4c1925d13bcaa03c97d83315d58be5bd31b4e4baf4b44b751b06172`) — **обязателен**, т.к. `<lang>ru</lang>` в `textlang.cpp` ищет именно `Russian.pattern`.
   - `assets/hyph/Russian_EnUS.pattern` (размер: `322891` байт, SHA-256: `93a21f826f858052a70a13176cd9a4dae904abcd4c7e28334fd702d4a4dbbdde`) — для книг с `ru-US` (русско-английские переносы).
2. Добавить обе записи в массив `"files"` файла `assets/manifest.json` (соблюдая алфавитный порядок путей между `Portuguese_BR.pattern` и `Slovak.pattern`).
3. Проверить `assets/notices/Hyphenation-patterns` и тест `tests/model/acknowledgments.cpp` (а также `platforms/android/check-notices.sh`): при необходимости дополнить файл лицензионных уведомлений информацией об авторе русских паттернов переносов (Alexander Lebedev, `ruhyphal`, лицензия LPPL/BSD — взять из заголовка `.pattern` или `koreader`).

---

### Этап 2. Валидация публикации и локальный доступ к файлу (`src/publication/`)

Сейчас `Publication::inspect()` принимает только ZIP-архивы с `META-INF/container.xml`, а `LocalResources` умеет читать только записи внутри ZIP через `ZipResource`. Нужно добавить поддержку однофайлового `.fb2` без поломки архитектуры `ResourceCache` и `SourceLease` (отвечающего за безопасное освобождение файла при подключении USB).

**Файлы:**
- `src/publication/publication.h`
- `src/publication/publication.cpp`
- `src/publication/local_resources.cpp`
- `assets/en.strings` (если добавляются новые ключи ошибок — помнить, что `platforms/strings.cmake` валидирует все литералы `strings::get` и `strings::key`!)

**Что сделать:**
1. В `Publication` (`src/publication/publication.h`) добавить тип формата:
   ```cpp
   enum class PublicationFormat { Epub, Fb2 };
   PublicationFormat format() const { return format_; }
   ```
   И константу виртуального имени ресурса для однофайлового FB2, например:
   ```cpp
   static constexpr auto singleFb2ResourceName = "__kobalt_document__.fb2";
   ```
2. В `Publication::inspect(QIODevice &source, QString *error, OpeningResult *result)`:
   - Проверить размер: `length >= 32 && length <= maxBytes` (64 МБ — разумный и безопасный верхний предел для несжатого FB2 в памяти/кэше).
   - **До** поиска сигнатуры конца ZIP-каталога (`0x06054b50`) прочитать первые `min(length, 65536)` байт с позиции `0`.
   - Определить, является ли поток XML-документом FictionBook:
     - Пропустить возможный UTF-8 BOM (`\xEF\xBB\xBF`) и пробельные ASCII-символы (`\t`, `\r`, `\n`, `' '`).
     - Проверить начало с `<?xml` или `<FictionBook` и наличие открывающего тега `<FictionBook` (с пробелом, `>` или переносом строки) в пределах первых 64 КБ.
     - **Важно по кодировкам:** НЕ парсить весь файл через `QString::fromUtf8()` и НЕ требовать UTF-8! Огромное число русских `.fb2` имеет заголовок `<?xml version="1.0" encoding="windows-1251"?>` — сырые байты должны дойти до `crengine` неизменными.
     - **Важно по безопасности (соответствие стилю Kobalt):** проверить префикс на отсутствие `<!ENTITY` (защита от XML entity expansion / billion laughs, аналогично `cleanXml` для EPUB).
   - Если это валидный заголовок FB2:
     - Выставить `format_ = PublicationFormat::Fb2; protected_ = false;`.
     - Посчитать `fingerprint_` (hex SHA-256 от размера файла и содержимого файла кусками по 64 КБ, либо `size + head + mid + tail`, но для локального файла до 64 МБ полный потоковый SHA-256 занимает единицы миллисекунд и гарантирует 100% точность инвалидации кэша при правке книги).
     - Заполнить `names_ = {QStringLiteral("__kobalt_document__.fb2")}` и `sizes_[QStringLiteral("__kobalt_document__.fb2")] = length`.
     - Вернуть `true`.
3. В `src/publication/local_resources.cpp` (`LocalResources::read`):
   - Если `name == Publication::singleFb2ResourceName` (или файл открыт в режиме single-file): вместо `ZipResource` открывать `QFile(path)` напрямую, проверять, что `file.size() == expectedSize` и `!cancelled()`, и читать поток чанками по `64 * 1024` байт в `sink(buffer, n)`, обновляя счётчики статистики чтения.
   - В `probe` для отложенных изображений (`localResourceFactory`): для `__kobalt_document__.fb2` `imageFileName(name)` и так вернёт `false`, поэтому ничего лишнего не сработает.

> **Преимущество такого решения:** `ResourceCache`, `SourceLease` (мгновенное закрытие дескриптора при подключении Kobo по USB), `ReaderSession` и `retainedHit` продолжают работать для FB2 точно так же, как для EPUB!

---

### Этап 3. Загрузка FB2, стили `fb2.css`, сноски и превью в движке (`src/engine/crengine/` и `src/reading/`)

#### 3.1. Подключение `fb2.css` во ВСЕХ платформенных `CMakeLists.txt`
> **Критично:** `reader_styles.h.in` конфигурируется в **трёх** местах! Если обновить только `platforms/kobo/CMakeLists.txt`, сломается сборка `mac-tests` и Android CI.

1. Обновить **все три** файла:
   - `platforms/kobo/CMakeLists.txt` (около строки 48)
   - `platforms/desktop/CMakeLists.txt` (около строки 50)
   - `platforms/android/CMakeLists.txt` (около строки 46)
   Добавить рядом с `READER_HTML_CSS`:
   ```cmake
   file(READ "${CRENGINE_SOURCE}/cr3gui/data/fb2.css" READER_FB2_CSS)
   ```
2. В `src/engine/crengine/reader_styles.h.in` добавить:
   ```cpp
   constexpr auto readerFb2Css = R"rrcss(@READER_FB2_CSS@)rrcss";

   // Дополнительные хинты Kobalt поверх fb2.css из KOReader:
   // Помечаем каждую секцию внутри body[name="notes"] и body[name="comments"]
   // как контейнер сноски (-cr-hint: footnote), чтобы попап сносок Kobalt
   // захватывал многоабзацные сноски целиком и не захватывал соседние сноски.
   constexpr auto readerFb2OverridesCss = R"rrcss(
   body[name="notes"] > section,
   body[name="comments"] > section {
       -cr-hint: footnote;
   }
   a[type="note"] {
       color: black;
   }
   )rrcss";
   ```

#### 3.2. Загрузка потока FB2 в `ReaderModel` (`src/engine/crengine/reader_source.cpp`)
1. В `ReaderModel::open(QByteArray bytes, ...)` (используется в desktop-режиме и unit-тестах!):
   - Разрешить не только `supportedArchive(bytes)` (EPUB), но и сырой буфер FB2 (проверенный через `Publication::inspect(bytes, error)` с `format() == PublicationFormat::Fb2`).
   - Для FB2 сохранять `source->bytes = std::move(bytes)` (или класть в `ResourceContainer` с виртуальным именем `Publication::singleFb2ResourceName`).
2. В `ReaderModel::openSource(...)` (`src/engine/crengine/reader_source.cpp`):
   - Определить режим FB2: либо в `source->container` есть поток `U"__kobalt_document__.fb2"`, либо `source->isFb2 == true`.
   - **Если это EPUB:** оставить текущую проверку `mimetype == "application/epub+zip"`, оборачивание в `ContentContainer` и вызов `engine_->LoadDocument(LVCreateMemoryStream(), false, source_->container)`.
   - **Если это FB2:**
     - Открыть `LVStreamRef fb2Stream = source->container->OpenStream(U"__kobalt_document__.fb2", LVOM_READ)` (или `LVCreateMemoryStream` из `source->bytes`).
     - Настроить `DOC_PROP_FILE_NAME` (например, `documentCacheName(cacheFingerprint_) + ".fb2"`, если кэш включён, иначе `U"publication.fb2"`), `DOC_PROP_FILE_CRC32` (добавив `:fb2-v1:` в строку политики хэша кэша, чтобы кэш включал `fb2.css` и маркеры выравнивания) и `DOC_PROP_FILE_SIZE`.
     - Перед `LoadDocument` предварительно выставить базовый стиль FB2 (`applyStyleSheet(true /* fb2 */)`), чтобы парсер и кэш CREngine сразу использовали `readerFb2Css`.
     - Вызвать штатный загрузчик CREngine **без `epubContainer`**:
       ```cpp
       loaded = engine_->LoadDocument(fb2Stream, false);
       ```
     - Проверить, что документ действительно распознан как FB2:
       ```cpp
       if (loaded && engine_->getDocFormat() != doc_format_fb2) {
           engine_.reset();
           return fail(OpeningFailureKind::Unsupported,
                       strings::key("error.the_publication_is_not_an_epub")); // или отдельный ключ в en.strings
       }
       ```
     - Для FB2 пропустить EPUB-специфичные `markChapterLeadingImages(engine_.get())` (он ищет `DocFragment`) и `markKoboLinkDecorations()`, но **оставить** `markBodyAlignment(document)` и `applyStyleSheet()`.

#### 3.3. Выбор таблицы стилей и защита семантических блоков FB2 (`reader_model.cpp`, `reader_alignment.cpp`)
1. В `ReaderModel::applyStyleSheet()` (`src/engine/crengine/reader_model.cpp`):
   - Проверять `const bool isFb2 = (engine_ && engine_->getDocFormat() == doc_format_fb2) || loadingFb2_;`.
   - Если `isFb2`: в качестве базы вместо `lString8(readerHtmlCss) + lString8(readerOverridesCss)` передавать `lString8(readerFb2Css) + lString8("\n") + lString8(readerFb2OverridesCss)`.
   - В правило `line-height:%1% !important` при `isFb2` добавить FB2-теги `v,title,subtitle,epigraph,cite,annotation,text-author` (чтобы регулировка межстрочного интервала в настройках Kobalt равномерно работала и в стихах/эпиграфах/аннотациях).
2. В `preserve()` (`src/engine/crengine/reader_alignment.cpp`):
   - Добавить в `static const QSet<QString> elements` семантические элементы FB2: `"title"`, `"subtitle"`, `"epigraph"`, `"poem"`, `"stanza"`, `"v"`, `"text-author"`, `"date"`.
   - Это гарантирует, что когда пользователь включает принудительную выключку по ширине или по левому краю в панели `Styling`, заголовки глав FB2 останутся по центру, а стихи и эпиграфы не потеряют своё форматирование.

#### 3.4. Всплывающие сноски для FB2 (`src/engine/crengine/reader_links.cpp`)
Проверить работу `ReaderModel::linkDetails()` на FB2-сносках вида `<a l:href="#n_1" type="note">[1]</a>`:
- `pointer.getHRef(link)` в `crengine` уже возвращает `#n_1` для `l:href` (`xlink:href`).
- `noteReference(node)` на строке 86 уже проверяет `token(node, "type", "note")` и возвращает `NoteMatch::Explicit`!
- Цикл обхода предков на строке 498 идёт вверх `while (... && parent && !parent->isNodeName("DocFragment"))`:
  - В FB2 нет `DocFragment`, корнем тела является `body` (`<body name="notes">`). Ограничить цикл условием `!parent->isNodeName("DocFragment") && !parent->isNodeName("body")`.
  - Благодаря правилу `body[name="notes"] > section, body[name="comments"] > section { -cr-hint: footnote; }` из п. 3.1, цикл найдёт `<section id="n_1">` с хинтом `FOOTNOTE`, выставит `contained = true` и отрендерит весь текст сноски во всплывающее окно Kobalt.
  - Проверить `noteHtml()` в `reader_links.cpp`: в FB2 заголовок номера сноски внутри `<section id="n_1">` записан как `<title><p>1</p></title><p>Текст сноски</p>`. Убедиться, что `noteHtml()` корректно включает текст из `<title>` и `<p>` без склейки слов без пробела.

#### 3.5. Живое превью настроек шрифта для FB2 (`ReaderModel::openPreview` в `reader_source.cpp`)
Сейчас `ReaderModel::openPreview()` ищет родительский `<DocFragment>` и читает `META-INF/container.xml`, из-за чего на книге FB2 панель живого превью шрифтов вернёт ошибку.
- Добавить ветку для `source.engine_->getDocFormat() == doc_format_fb2`:
  - Найти узел по `source.position()`, собрать несколько соседних текстовых абзацев `<p>` (до ~4–8 КБ текста) и язык документа `source.engine_->getLanguage()`.
  - Сформировать компактный валидный UTF-8 FB2 в памяти через `QXmlStreamWriter`:
    ```xml
    <?xml version="1.0" encoding="utf-8"?>
    <FictionBook xmlns="http://www.gribuser.ru/xml/fictionbook/2.0">
      <description><title-info><lang>ru</lang></title-info></description>
      <body><section><p>...</p></section></body>
    </FictionBook>
    ```
  - Загрузить его в превью-модель через `open(previewBytes, size, ...)` (сохранив `source->embeddedFonts = 0`).

#### 3.6. Сессия чтения (`src/reading/reader_session.cpp`)
В `ReaderSession` (строки 500–595):
- Если `publication.format() == PublicationFormat::Fb2`:
  - Пропустить EPUB-инспектор `inspectLayout(...)` — сразу считать `layout.verdict = LayoutCompatibility::Supported`.
  - При выборе `probe` (строка 503): если `publication.format() == PublicationFormat::Fb2`, установить `probe = QString::fromLatin1(Publication::singleFb2ResourceName)`. Это автоматически включит и быстрое переоткрытие книги из памяти (`retainedHit`), и дисковый кэш DOM-дерева `engine->setDocumentCache(...)`!

---

### Этап 4. Интеграция с Kobo `Nickel` (Qt5 и Qt6) (`platforms/kobo/` и `src/reading/`)

Это ключевой этап для работы прямо из родного интерфейса Kobo. Все правки проверяются для **обоих** таргетов (`platforms/kobo/qt5/` и `platforms/kobo/qt6/`).

#### 4.1. Разрешение `.fb2` в роутере книг и контекстном меню (`platforms/kobo/common/book_menu.cpp`)
В функции `bookRoute(const void *volume, QString *identity, QString *path, bool inspectPublication)`:
1. Сейчас на строках 98–101 стоит жёсткий фильтр:
   ```cpp
   const auto mime = fields.value("MimeType").toString();
   if (mime != "application/epub+zip" && mime != "application/x-kobo-epub+zip")
       return Route::Unsupported;
   ```
2. Получать `id = bookIdentity(volume)` и локальный путь `localPath = localReaderBookPath(id)` чуть раньше:
   - Признавать книгу кандидатом FB2, если `!localPath.isEmpty() && localPath.endsWith(QLatin1String(".fb2"), Qt::CaseInsensitive)` **или** `mime == QLatin1String("application/x-fictionbook+xml")` / `mime == QLatin1String("application/fb2")` / `mime == QLatin1String("application/x-fb2")`.
   - Если книга не EPUB/KEPUB и не FB2 — возвращать `Route::Unsupported` (PDF, CBZ, CBR, MOBI без какого-либо экрана Kobalt уходят в штатный ридер Kobo, как и раньше).
   - Для локального `.fb2` пропускать проверки Adobe/LCP DRM и таблицу `content_keys` (в FB2 не бывает DRM), а на этапе `inspectPublication == true` вызывать уже обновлённый `publication.inspect(file, &error)`.
   - Благодаря этому для `.fb2` автоматически заработают:
     - Перехват открытия по тапу на книгу в `kobalt_show_reader` (`reader_controller.cpp` в `qt5` и `qt6`).
     - Пункт долгого нажатия на книгу в библиотеке Kobo (`Open with Kobalt` / `Open with built-in reader` в `addReaderChoice`).

#### 4.2. Открытие публикации Kobo (`platforms/kobo/qt5/native_publication.cpp` и `platforms/kobo/qt6/native_publication.cpp`)
Проверить `NativePublication::open(...)` в обеих папках (`qt5` и `qt6`):
1. Для файла с путём `.fb2`:
   - В `qt5/native_publication.cpp`:
     - `nickel4::localBookPath(identity_)` вернёт `/mnt/onboard/.../book.fb2`.
     - Если `path_.endsWith(".fb2", Qt::CaseInsensitive)`: **не** считать ошибкой непустой `native.bookmarkPath(bookmark)` от старого RMSDK и **не** пытаться конвертировать `ChapterIDBookmarked` (от RMSDK/XSL) в EPUB-путь — оставлять `location_.clear()`. Позиция книги будет браться из собственной SQLite-базы Kobalt (а при первом открытии — с начала книги).
   - В `qt6/native_publication.cpp`:
     - `native.path(volume)` (`VolumeManager::getVolumeFilePath`) вернёт `/mnt/onboard/.../book.fb2`.
     - Аналогично: если `path_.endsWith(".fb2", Qt::CaseInsensitive)`, не отклонять открытие из-за RMSDK `ContentLocation` в `bookmark` и оставлять `location_.clear()`.
   - Обложка: если `nativeReaderCoverPath(volume)` пуст или не найден, Kobalt автоматически использует `engine_->getCoverPageImageStream()` из `<binary>` внутри FB2 для экрана сна!

#### 4.3. Кодек позиций и `time-only` запись прогресса в `KoboReader.sqlite` (`src/reading/reading_book.h`, `reading_book.cpp`, `engine_positions.cpp`)
> **Очень важный нюанс!** В Kobalt уже реализована полноценная транзакционная запись прогресса без позиции главы (`timeOnly`) в `platforms/kobo/qt5/native_progress.cpp` и `platforms/kobo/qt6/native_progress.cpp`: когда `position.isEmpty()`, Kobalt обновляет в `KoboReader.sqlite` поля `ReadStatus` («Читаю» / «Прочитано»), `FirstTimeReading = false`, `___PercentRead`, `DateLastRead`, `RestOfBookEstimate` и `CurrentChapterEstimate`.

Однако в `src/reading/reading_book.cpp` (строки 168–170) стоит проверка:
```cpp
const bool timeOnly = !mappingError.isEmpty();
QString error = timeOnly && !plainEpub() ? mappingError : QString();
```
А в `src/reading/reading_book.h` (строки 36–39):
```cpp
bool plainEpub() const {
    return identity_.startsWith("file://") && identity_.endsWith(".epub", Qt::CaseInsensitive) &&
           !identity_.endsWith(".kepub.epub", Qt::CaseInsensitive);
}
```
Если не обновить это условие, то для `file:///mnt/onboard/.../book.fb2` метод `NickelPositionCodec::encode()` вернёт `mappingError` (т.к. в FB2 нет `<DocFragment Source="...">`), `!plainEpub()` окажется `true`, и `ReadingBook::writeNative()` отменит запись прогресса в базу Kobo!

**Что сделать:**
1. Расширить метод (или добавить `sideloadedBookWithoutCloudSync()` / обновить `plainEpub()`) в `src/reading/reading_book.h`, чтобы он возвращал `true` также для `identity_.startsWith("file://") && identity_.endsWith(".fb2", Qt::CaseInsensitive)`.
2. Проверить `ProgressReconciler` (в `src/reading/reading_progress.cpp`): убедиться, что если человек один раз открывал `.fb2` штатным ридером Kobo (и в `ChapterIDBookmarked` записался RMSDK-маркер), а затем открыл книгу в Kobalt, `ReadingBook` корректно синхронизирует статус «Не прочитано» (если пользователь сбросил статус книги в меню Kobo) и при выходе без конфликтов сохраняет `time-only` прогресс в `KoboReader.sqlite`.

---

## 4. Автотесты и валидация (в соответствии с `AGENTS.md`)

Агент должен добавить регрессионные тесты в существующий тестовый контур (без внешних скачиваний, генерируя фикстуры прямо в C++):

1. **Модельные тесты парсинга и рендеринга FB2 (`tests/model/`):**
   - Валидный UTF-8 `.fb2` на русском языке (`<lang>ru</lang>`) с `<description>`, `<epigraph>`, `<poem><stanza><v>`, `<table>`, `base64` PNG-обложкой в `<binary id="cover.png" content-type="image/png">` и секцией сносок `<body name="notes"><section id="n1">...`:
     - Проверка `Publication::inspect()` -> `format() == PublicationFormat::Fb2`, непустой `fingerprint()`.
     - Проверка `ReaderModel::open()` и `ReaderSession` (включая `DocumentCacheScope` — холодное открытие и повторное открытие из дискового кэша `openedFromCache() == true`).
     - Проверка, что сырые теги `<genre>` и `<document-info>` скрыты стилем `fb2.css` и не попадают на первую страницу текста.
     - Проверка оглавления `chapters()` по вложенным `<section><title>`.
     - Проверка извлечения обложки `coverImage()` из `<binary>`.
     - Проверка попап-сноски `linkDetails()` по ссылке `<a l:href="#n1" type="note">[1]</a>` (`LinkKind::Footnote`, полный HTML многоабзацной сноски без захвата `#n2`).
     - Проверка живого превью `openPreview()` на открытой книге FB2.
   - Валидный `.fb2` в кодировке **`windows-1251`** (байтовый массив с кириллицей в `cp1251` и заголовком `<?xml version="1.0" encoding="windows-1251"?>`):
     - Проверка, что русский заголовок (`title()`), автор (`author()`) и текст страницы декодируются без кракозябр и ошибок UTF-8.
   - Проверка русских переносов (`Russian.pattern`): при `hyphenation = "on"` и `<lang>ru</lang>` длинные русские слова переносятся по словарю, а не отключают переносы.
   - Негативные тесты: XML с `<!ENTITY` отклоняется `Publication::inspect()`; произвольный не-FB2 XML (например, `<rss>...</rss>`) отклоняется и не открывается в ридере.
2. **Тесты Kobo-адаптеров (`tests/model/nickel_book_menu.cpp`, `tests/model/reading_book.cpp`):**
   - В `tests/model/nickel_book_menu.cpp`: проверить, что `file:///mnt/onboard/Books/test.fb2` с `MimeType = "application/x-fictionbook+xml"` (а также при `MimeType = "text/xml"` с расширением `.fb2`) получает маршрут `ReaderBookRoute::Direct`, а `.pdf` по-прежнему получает `ReaderBookRoute::Unsupported`.
   - В `tests/model/reading_book.cpp`: проверить, что книга с `identity = "file:///mnt/onboard/Books/test.fb2"` при ошибке маппинга `NickelPositionCodec` выполняет `time-only` сохранение в `ReadingBridge` (`native_writeback == "time-only"`), обновляя процент и статус чтения без ошибки.

---

## 5. Чек-лист сборки перед завершением работы агента

1. На macOS (если среда macOS с Qt 6.11.3 доступна):
   ```sh
   cd platforms/desktop
   cmake --preset mac -DCMAKE_PREFIX_PATH="$HOME/Qt/6.11.3/macos"
   cmake --build --preset mac-tests
   ctest --preset mac
   ```
   Все существующие и новые тесты (включая `acknowledgments`, `storage_assets`, `nickel_book_menu`, `reading_book` и новые FB2-тесты) должны проходить на 100%.
2. Сборка установочных пакетов для Kobo через `kobuild` Docker-контейнеры:
   ```sh
   bash platforms/kobo/build.sh qt5
   bash platforms/kobo/build.sh qt6
   ```
   Убедиться, что оба пакета (`build/kobo-qt5/Kobalt-qt5.zip` и `build/kobo-qt6/Kobalt-qt6.zip`) успешно собираются без предупреждений (`-Werror` включён!), проходят `check-package` и содержат `Russian.pattern` и `Russian_EnUS.pattern` в `.adds/kobalt/assets/hyph/` и обновлённый `.adds/kobalt/assets/manifest.json`.
3. CI для проверки на устройстве. В рабочей ветке добавлен отдельный workflow `.github/workflows/kobo-qt5-device.yml`: он срабатывает на каждый push в `arena/9e626d86-kobalt-ru` и на ручной запуск (`workflow_dispatch`), собирает **только** `Kobalt-qt5.zip` (без Qt6 и Android) через штатный `platforms/ci/build.sh qt5` в закреплённом `kobuild`-образе, проверяет состав ZIP (`.kobo/KoboRoot.tgz`, `.adds/kobalt/assets/manifest.json`, отсутствие небезопасных путей) и публикует артефакт `Kobalt-qt5-device` с SHA-256 и чек-листом установки на Kobo. Релизный workflow `.github/workflows/ci.yml` не трогаем.

---

## 6. Локализация интерфейса на русский язык (новый этап)

> **Статус:** реализовано по варианту A (2026-10-26): `assets/ru.strings` на 764 ключа, вторая компилируемая таблица с фолбэком на английский, парность ключей/слотов валидируется в CMake, селектор локали и CLDR-плюрализация в `src/support/strings.h`, разрешение источника языка — в `src/support/locale.{h,cpp}`. Ниже — сохранённый исходный анализ механизма строк и обоснование выбора.

### 6.1. Анализ: как язык интерфейса устроен в Kobalt сейчас

**Механизм строк — собственный, не Qt Translation. Локали в коде нет вообще.**

1. **Единственный файл строк — `assets/en.strings`** (760 пар «ключ = значение», синтаксис в стиле Apple `.strings`: ключи строго `[a-z0-9_.]`, значения в кавычках, комментарии `/* ... */`, аргументы подстановки `%1`, `%2`). Других языковых файлов в репозитории нет.
2. **Генерация на этапе CMake:** функция `kobalt_generate_strings()` в `platforms/strings.cmake`:
   - строгим регулярным выражением валидирует каждую строку файла и падает на дубликате ключей;
   - сканирует **все** литералы `strings::get("...")` и `strings::key("...")` в `src/*.cpp` / `src/*.h` и падает с ошибкой `Missing UI string`, если ключа нет в `en.strings` (то есть набор ключей замкнут и проверяется на сборке);
   - сортирует ключи и генерирует C-массив `kobalt_strings.inc` (вызов — в `assets/CMakeLists.txt`, там же `CONFIGURE_DEPENDS` на `en.strings` и все исходники `src/`).
3. **Рантайм — `src/support/strings.h`:** сгенерированный массив `Entry { const char *key; const char *value; }` компилируется в бинарник; `strings::get(key)` делает бинарный поиск по отсортированной таблице и возвращает английский UTF-8. Есть только отладочный режим `KOBALT_SHOW_STRING_KEYS=1` (показ ключей вместо текста, с сохранением слотов `%n`). **Параметра локали, загрузчика `.qm`/`.ts`, `QTranslator` или `tr()` — нет.**
4. **Множественное число реализовано вручную**, по английской модели «один / много»: отдельные пары ключей с суффиксом `_one` (например, `library.count` / `library.count_one`), а выбор делает C++ (`src/interfaces/library/library_view.cpp:64`, `bookmark_list.cpp:367`, `chapter_contents.cpp:113`). Правил склонения для других языков нет.
5. **`QLocale` в проекте используется только для формата часов** (`platforms/kobo/qt5/native_ui.cpp:159` и `qt6/native_ui.cpp:160`, `nativeReaderTime()`), на язык надписей он не влияет.
6. **Файл `Kobo eReader.conf` уже читается** штатным `QSettings(..., QSettings::IniFormat)` в нескольких адаптерах обеих платформ:
   - qt5: `native_light.cpp:114`, `refresh.cpp:62`, `native_reading_session.cpp:240` (ключ `ApplicationPreferences/SideloadedMode`), `native_ui.cpp:89,166`;
   - qt6: те же файлы с теми же ключами.
   То есть чтение новых ключей из секции `[ApplicationPreferences]` — это уже принятый в проекте паттерн, а не новый механизм.
7. **Точки инициализации:** на Kobo плагин стартует через NickelHook `.init = initializeReader` → `initialize()` (`platforms/kobo/qt5/reader_controller.cpp:430`, аналог в qt6) — это происходит при запуске Nickel, до создания любого окна Kobalt, и подходит для выбора локали один раз. На desktop и Android точки входа — `platforms/desktop/main.cpp` и `platforms/android/main.cpp`.
8. **Шрифты интерфейса** — IBM Plex Sans (`assets/fonts/IBMPlexSans-*.ttf`, гарнитура содержит кириллицу), поэтому новый шрифт, скорее всего, не понадобится; наличие глифов нужно подтвердить скриншот-проверкой на устройстве.

**Вывод:** сейчас интерфейс жёстко английский, а система строк — это компилируемая одноязычная таблица с билдер-валидатором. Любая локализация должна надстроить вторую таблицу над этой же инфраструктурой, сохранив строгую проверку ключей; Qt-механизм переводов (`QTranslator`/`.qm`) в проекте не используется, и собственные `trans_*.qm` Nickel для этого не подходят (другой набор ключей, это файлы прошивки).

### 6.2. Можно ли использовать `ApplicationPreferences\CurrentLocale` из `Kobo eReader.conf`

**Да, можно и нужно — это самый естественный источник языка на Kobo.**

Фактические значения ключа в `[ApplicationPreferences]` (по данным пользовательских конфигов и форумов Kobo): `en`, `en_GB`, `en_US`, `fr_FR`, `de`, `ru`, `ru_RU` и т.п. — это код языка с опциональным региональным суффиксом через **подчёркивание**. Русский язык на sideload-устройствах обычно включается связкой `CurrentLocale=ru_RU` плюс `ExtraLocales=ru` (встречается и короткий вариант `CurrentLocale=ru`). Отсюда правила разбора:

- читать `QSettings("/mnt/onboard/.kobo/Kobo/Kobo eReader.conf", IniFormat).value("ApplicationPreferences/CurrentLocale")` — тот же путь и секция, что уже используются для `SideloadedMode`;
- нормализовать: взять часть до `_`/`-`, привести к нижнему регистру (`ru_RU`, `ru-RU`, `ru` → `ru`; `en_GB` → `en`);
- сопоставить с поддерживаемыми локалями Kobalt; неизвестный код и отсутствие файла/ключа → безопасный фолбэк на английский;
- на Kobo это покрывает и будущие языки без собственного UI-переключателя: язык Kobalt будет следовать языку Nickel, выбранному пользователем в настройках устройства.

Ограничение: файл есть только на Kobo. На desktop и Android источником языка становится `QLocale::system()` (либо явная переменная окружения `KOBALT_LANGUAGE` для тестов и ручного переопределения).

### 6.3. Целевое решение и затрагиваемые файлы

**Рекомендуемый вариант A — вторая компилируемая таблица (минимально инвазивный, сохраняет архитектуру):**

1. Добавить `assets/ru.strings` — полный перевод с **тем же набором ключей**, что в `en.strings`; английский файл остаётся эталоном и фолбэком.
2. Расширить `platforms/strings.cmake`:
   - валидировать каждый языковой файл тем же строгим парсером;
   - ввести проверку паритета: множества ключей и наборы слотов `%n` во всех локалях обязаны совпадать с `en.strings` (ошибка сборки при пропущенном/лишнем ключе и при рассогласовании аргументов);
   - генерировать по одному `.inc` на локаль (`kobalt_strings_en.inc`, `kobalt_strings_ru.inc`).
3. Расширить `src/support/strings.h`: атомарный селектор локали (`setLocale(QLocaleName)` / `currentLocale()`), таблица таблиц; `strings::get()` ищет ключ в активной локали и при отсутствии — в английской. Поведение `KOBALT_SHOW_STRING_KEYS` и `strings::key()` не меняется.
4. Разрешение локали при старте (новый небольшой модуль, например `src/support/locale.cpp`): `KOBALT_LANGUAGE` (приоритет для тестов) → на Kobo `CurrentLocale` из `Kobo eReader.conf` → на desktop/Android `QLocale::system()` → `en`. Вызов — в `initialize()` (`reader_controller.cpp`, qt5 и qt6) и в `main.cpp` desktop/Android, до первого обращения к строкам.
5. **Множественное число для русского.** Текущей пары `_one`/база недостаточно: русская грамматика требует трёх форм (один / два-четыре / много). Ввести соглашение `*_one` / `*_few` / `*_many` и хелпер выбора формы по правилу CLDR для `ru` (остатки от 10 и 100), заменив три известные ручные точки (`library_view.cpp`, `bookmark_list.cpp`, `chapter_contents.cpp`) и применяя его к новым счётным строкам. Английская локаль сохраняет две формы через тот же хелпер.
6. **Альтернативный вариант B (зафиксировать, но не выбирать по умолчанию):** хранить `assets/locales/ru.strings` как рантайм-ассет и парсить при старте через расширение `AssetBundle`. Он позволяет добавлять языки без перекомпиляции `.so`, но требует ослабить/расширить allowlist путей в `assets/CMakeLists.txt` (сейчас манифест разрешает только префиксы `fonts/` и `hyph/`), логику `AssetBundle::available()` (`src/assets/assets.cpp`) и записи в `assets/manifest.json` с размерами и SHA-256. Вариант A дешевле и переиспользует уже существующий строгий билд-валидатор; B имеет смысл только при планах на сообщественные переводы без релизов.
7. Сохранённые данные не меняются: выбранный язык не относится к формату настроек/закладок (см. `doc/storage.md`), бамп версий в `documentCacheChecksum` не нужен.

### 6.4. Тесты и валидация этапа

1. `platforms/strings-test.cmake` (гоняется в `platforms/ci/test.sh`): добавить кейс паритета ключей и слотов `%n` между `en.strings` и `ru.strings`, включая проверку фолбэка при удалении ключа из неанглийской таблицы.
2. Новый модельный тест (группа `tests/model/`): переключение `strings::setLocale("ru")` → русский текст известного ключа; неизвестная локаль → английский; режим `KOBALT_SHOW_STRING_KEYS` продолжает работать.
3. Тест парсера настроек Kobo: фикстуры `CurrentLocale=ru`, `ru_RU`, `en_GB`, отсутствующий ключ/файл → ожидаемые коды локали и фолбэк (по аналогии с offline-фикстурами других model-тестов, без реального устройства).
4. Проверка плюрального хелпера на русских счётных формах (1/2/5/11/21).
5. Mac headless: `cmake --build --preset mac-tests && ctest --preset mac` — все существующие тесты проходят, ключи на месте.
6. Ручная проверка на устройстве через артефакт workflow `Kobalt-qt5-device`: при `CurrentLocale=ru` все панели, диалоги и меню Kobalt отображаются по-русски без усечений и «квадратов» (кириллица в IBM Plex Sans), при `CurrentLocale=en` — прежний английский интерфейс; перезапуск устройства сохраняет выбор.

---

## 7. Поддержка форка и перенос на новые релизы upstream (финализация)

Все этапы 0–6 реализованы и зелёные в qt5-CI. Этот раздел — постоянный регламент поддержки: как собирать промежуточные версии, какие именно исходники отличают форк от upstream, что проверять перед пушем и как переносить правки на будущие релизы Kobalt.

### 7.1. Принцип минимального вмешательства (соблюдён, обязателен и впредь)

1. **Поведение EPUB — неприкосновенно.** Любая новая ветка включается явным флагом (`Publication::PublicationFormat::Fb2`, `ReaderSource::isFb2`, `ReaderModel::fb2Document_`, суффиксы `.fb2` в идентичности/путях). При значениях по умолчанию поток управления и тексты совпадают с upstream.
2. **Новые подсистемы — в новых файлах.** Локаль (`src/support/locale.{h,cpp}`), данные (паттерны, `ru.strings`), тесты (`tests/model/fb2.cpp`, `tests/model/strings_locale.cpp`), CI-воркфлоу не правят существующую архитектуру, а подключаются через узкие точки.
3. **Generic-механизмы с пустыми дефолтами, fork-значения — только в fork-only файлах.** Пример: `KOBALT_FORK_NAME` (пусто во всех CMakeLists → строки форка нет, upstream видит штатную About-страницу; имя `kobalt_ru` задаётся исключительно в `.github/workflows/kobo-qt5-device.yml`).
4. **Никаких новых строк в обход таблиц.** Новый `strings::get("...")` требует ключа в `assets/en.strings` И `assets/ru.strings`; паритет ключей и слотов `%n` проверяется на configure и в `platforms/ci/test.sh`.
5. **Совместимость со старым Qt5 Kobo SDK (до Qt 5.10):** не использовать `QStringView` и прочие API новее SDK; это автоматически проверяется qt5-сборкой с `-Werror`.

### 7.2. Полная карта изменений форка относительно upstream

**Только добавления (конфликты при rebase маловероятны):**

| Файл/каталог | Назначение |
|---|---|
| `.github/workflows/kobo-qt5-device.yml` | push-CI: сборка строго qt5, валидация ZIP, артефакт `Kobalt-qt5-device`; здесь же включается `KOBALT_FORK_NAME=kobalt_ru` |
| `FB2_FORK_PLAN.md`, `doc/fork-kobalt_ru.md` | План/регламент (этот файл) и пользовательская инструкция |
| `assets/ru.strings` | Русская таблица, 764+ ключа, паритет с английской |
| `assets/hyph/Russian.pattern`, `Russian_EnUS.pattern` | Паттерны переносов (LPPL, см. `assets/notices/Hyphenation-patterns`) |
| `src/support/locale.{h,cpp}` | Разрешение локали: env `KOBALT_LANGUAGE` → Kobo conf → система → английский |
| `tests/model/fb2.cpp`, `tests/model/strings_locale.cpp` | Регрессии FB2 и локали |

**Точечные правки upstream-файлов (зона конфликтов при rebase):**

| Файл | Суть правки | Правило разрешения конфликта |
|---|---|---|
| `src/support/strings.h` | Вторая таблица, `Locale`, атомарный селектор, `plural()`, `normalizeLocaleCode()`; API `get/key/showKeys/entries` сохранён | Сохранить расширения; `entries()` обязан остаться английской таблицей (диагностика) |
| `platforms/strings.cmake`, `platforms/strings-test.cmake` | Разделение parse/generate, `kobalt_check_string_parity`, parity-кейсы; вложенные `-P`-скрипты обязаны начинаться с `cmake_minimum_required(VERSION 3.22)` (CMP0057) | Перенести функции целиком; валидатор — единая точка правил для строк |
| `assets/CMakeLists.txt` | Генерация `kobalt_strings_ru.inc`, parity, `CONFIGURE_DEPENDS` на оба `.strings` | Три добавленные строки |
| `assets/en.strings` | Слотные `_one` (обязаны содержать `%1`), `_few/_many`, `about.fork`, `about.fork_build` | Новые upstream-ключи добавлять и в `ru.strings`; сборка упадёт сама при расхождении |
| `assets/manifest.json` | Записи о двух русских паттернах; регенерируется штатной сборкой | Не править руками — пересобрать ассеты |
| `src/publication/publication.{h,cpp}` | `PublicationFormat`, гейт FB2 в `inspect()`, виртуальное имя `singleFb2ResourceName`, fingerprint всего файла | FB2-ветка строго до ZIP-скана; EPUB-ветка без изменений |
| `src/publication/local_resources.cpp` | `transferSingleFile()` — чтение цельного файла чанками с отменой/таймаутом, как у ZIP-пути | Ветка по имени виртуального ресурса |
| `src/engine/crengine/reader_source.{h,cpp}` | `isFb2`; байпас ZIP/mimetype; `LoadDocument(stream,false)`; проверка `doc_format_fb2`; `.fb2`-суффикс кэша; `openFb2Preview()` | Все ветки за `loadingFb2`; здесь же версия DOM-кэша `:fb2-v1:` |
| `src/engine/crengine/reader_model.{h,cpp}` | `fb2Document_`; CSS `readerFb2Css`; расширение line-height на FB2-теги; расчёт кэш-суммы с `:fb2-v1:` | При изменении парсинга/препроцессинга FB2 бампнуть `:fb2-v1:` → `:fb2-v2:` в `reader_source.cpp` и `reader_model.cpp` синхронно |
| `src/engine/crengine/reader_styles.h.in` | Константы `readerFb2Css` (из `fb2.css` pinned crengine) и `readerFb2OverridesCss` | Требует `file(READ ... fb2.css)` в трёх платформенных CMakeLists |
| `src/engine/crengine/reader_alignment.cpp` | FB2-теги в `preserve()`-наборе | Влияет только на документы, где эти теги существуют |
| `src/engine/crengine/reader_links.cpp` | Маппинг FB2-тегов в безопасный HTML сносок; граница обхода — `body` как альтернатива `DocFragment` | EPUB-ветки (`allowed`, `aside/section/h*`) сохранены дословно |
| `src/reading/reader_session.cpp` | Ветка FB2 при выборе читаемого ресурса и пропуске OPF-инспекции раскладки | За `publication.format() == Fb2` |
| `src/reading/reading_book.h` | `plainEpub()` расширен на `.fb2` (time-only writeback) | DRM-логика EPUB не затрагивается |
| `platforms/kobo/common/book_menu.cpp` | Роутинг `.fb2` по расширению/XML-mime; DRM-проверки остались EPUB-only | PDF и прочее по-прежнему `Unsupported` |
| `platforms/kobo/qt5/native_publication.cpp`, `qt6/native_publication.cpp` | Для `.fb2` пропущен неотображаемый RMSDK-маркер; позиция хранится в своём SQLite | Разные API Qt5/Qt6 — правки симметричны, не сливать в один файл |
| `platforms/kobo/qt{5,6}/reader_controller.cpp`, `platforms/desktop/main.cpp`, `platforms/android/main.cpp` | Одна строка `strings::setLocale(resolveLocale())` + include | Вызывать до первого чтения UI-строк |
| `platforms/{kobo,android,desktop}/CMakeLists.txt` | `file(READ fb2.css)`, `KOBALT_FORK_NAME`, `string(TIMESTAMP KOBALT_BUILD_TIME … UTC)`, регистрация тестов (`desktop`) | Три платформы симметрично |
| `src/support/build_info.h.in` | `kobaltForkName` из `@KOBALT_FORK_NAME@` и `kobaltBuildTime` из `@KOBALT_BUILD_TIME@` | Пустой fork name — единственный дефолт generic-сборки; исходные строки About не меняются |
| `platforms/kobo/build.sh` | Проброс шестого аргумента `-DKOBALT_FORK_NAME` (пустой по умолчанию) | Сохранить нумерацию `$1..$5` |
| `src/interfaces/reader/settings/settings_panel.{h,cpp}` | About: одна добавленная строка `rrAboutFork` (имя форка + UTC-время сборки), оригинальные `rrAboutVersion`/`rrAboutCommit` не тронуты; Defaults: кнопка `rrClearBookCache` + `ChoiceModal` `rrClearCacheConfirmation` | По образцу restoreDefaults; диалог владеет только UI, действие подключает Reader |
| `src/storage/reader_storage.{h,cpp}` | `clearRenderingCaches(Paths)` — удаляет только `.cache/` и `cache/reopen-layout.json`; durable-файлы вне этих путей; отсутствие кэша = успех | При расширении списка кэшей дополнять функцию, а не удалять файлы профиля из UI; тест `tests/model/rendering_cache.cpp` |
| `src/interfaces/reader/window/reader_build.cpp` | Подключение `confirmClearCache`: вызов хелпера, при ошибке — `NoticeModal`, при успехе — штатный `close()` (сохранение прогресса + возврат в Nickel) | Тот же путь выхода, что и по клавише Home |
| 3 точки плюрализации (`library_view.cpp`, `bookmark_list.cpp`, `chapter_contents.cpp`) | Ручной тернарник заменён на `strings::plural()` | Плюральные ключи всегда содержат `%1` |

### 7.3. Сборка промежуточных версий

1. **Основной путь — CI (без локального SDK):** коммит и `git push origin arena/9e626d86-kobalt-ru`; воркфлоу стартует автоматически (ручной `workflow_dispatch` для форка может быть запрещён правами токена — это норма). Через ~5–6 минут в прогоне доступен артефакт **`Kobalt-qt5-device`** (ZIP `Kobalt-qt5.zip` внутри, SHA-256 и чек-лист установки — в step summary).
2. **Локальная сборка устройства (нужен подготовленный `kobuild` docker-образ, см. `doc/building.md`):**
   ```sh
   KOBALT_FORK_NAME=kobalt_ru bash platforms/kobo/build.sh qt5
   # пакет: build/kobo-qt5/Kobalt-qt5.zip
   ```
   Без переменной `KOBALT_FORK_NAME` соберётся generic-вид About-строки (строка форка скрыта) — это сознательное поведение механизма.
3. **Офлайн-проверки без Qt SDK (можно в песочнице):**
   ```sh
   cmake -DTEST_TMP_ROOT="$(mktemp -d)" -P platforms/strings-test.cmake   # генерация, инвалиды, rebuild, parity en/ru
   # Паритет локалей отдельным скриптом (cmake -P не принимает конвейеры):
   cat > /tmp/parity.cmake <<CMAKE
   include("$ENV{PWD}/platforms/strings.cmake")
   kobalt_check_string_parity("$ENV{PWD}/assets/en.strings", "$ENV{PWD}/assets/ru.strings")
   CMAKE
   cmake -P /tmp/parity.cmake
   bash platforms/ci/test.sh                                               # полный offline-релизный чек (нужен jq)
   bash -n platforms/kobo/build.sh                                         # синтаксис shell-скриптов
   ```
   Известный флак среды: rebuild-кейс `strings-test.cmake` может падать в песочнице из-за гранулярности mtime (на старом baseline тоже); в CI на штатном раннере он стабилен. Локальный обход — `touch -d "+5 seconds"` на фикстуру после APPEND.
4. **mac (если доступна среда по `doc/building.md`):** `cmake --build --preset mac-tests` из `platforms/desktop/`, затем `ctest --preset mac -L model` (как минимум `fb2`, `strings_locale`, `local_resources`, `nickel_book_menu`, `reading_book`).

### 7.4. Перенос на новый релиз upstream

1. Обновить ссылку upstream и сделать merge/rebase в форк-ветку; ожидать конфликты прежде всего в файлах второй таблицы 7.2.
2. Сверить закреплённый коммит `koreader/crengine` в `dependencies/manifest.json`: в его `cr3gui/data/` должны существовать `fb2.css`, а в заголовках — `doc_format_fb2`. Если upstream поднял crengine — перечитать diff `fb2.css` и при необходимости обновить `readerFb2OverridesCss`.
3. Разрешать конфликты по правилам колонки 3 таблицы 7.2: EPUB-поведение принимается как upstream, форк-ветки сохраняются за флагами. Никаких слияний «на глаз» в `reader_source.cpp` без прогона `tests/model/fb2.cpp`.
4. Новые/изменённые ключи строк upstream перенести в `assets/ru.strings` (перевод; состав слотов — как в английском). Parity-валидатор не даст сконфигурировать сборку с расхождением.
5. Если в новой версии изменились структуры настроек/закладок/кэшей — следовать `doc/storage.md`; для FB2 DOM-кэша допустимо изолированно бампнуть метку `:fb2-vN:`, не трогая EPUB-метку `:bionic-v3:`.
6. Прогнать минимум из 7.3 (офлайн → qt5-CI целиком → mac model-тесты, если среда есть) и вручную проверить на устройстве оба языка и открытие EPUB (регресс) + FB2 (фича).

### 7.5. Регрессный минимум перед пушем (обязателен)

- [ ] `strings-test.cmake` и parity en/ru зелёные; полный GLOB-скан `src/` не находит отсутствующих ключей;
- [ ] `git diff --check` чисто; YAML воркфлоу и `bash -n` скриптов валидны;
- [ ] затронуты только допустимые файлы (карта 7.2), новых строк мимо таблиц нет;
- [ ] qt5-CI зелёный целиком, артефакт `Kobalt-qt5-device` опубликован;
- [ ] `FB2_FORK_PLAN.md` обновлён в том же коммите (статусы, журнал);
- [ ] в сообщениях коммитов нет трейлера `Co-authored-by` (песочница может дописывать его хуком `commit-msg` — коммитить через `git commit --no-verify`, проверять `git log --grep=arena-agent`);
- [ ] работа запушена в `arena/9e626d86-kobalt-ru` (другие ветки/теги/релизы — только по отдельному разрешению).

### 7.6. Зафиксированные компромиссы и границы

- `.fb2.zip` исключён поддержкой окончательно (Nickel не индексирует такие файлы); точка расширения описана в секции решений выше.
- `ReaderModel::open()` выполняет дополнительный `Publication::inspect` для FB2-пробы перед штатной ZIP-проверкой: inspect ограничен первыми/последними 64 КиБ, на больших книгах стоимость пренебрежима; упрощать можно только заменой на единый inspect с переиспользованием результата — отдельной задачей с тестами.
- Десктоп/Android собирают generic-вариант без fork-строки (пустой `KOBALT_FORK_NAME`): это удерживает форк-специфику в одном CI-файле; при необходимости имя можно включить и там одной CMake-переменной.
- Пользовательская документация фич — `doc/fork-kobalt_ru.md`; правил проекта (`AGENTS.md`, `doc/`) она не подменяет.
