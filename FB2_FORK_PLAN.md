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
| 5 | Тесты: `tests/model/fb2.cpp`, FB2-кейсы в `nickel_book_menu`, `reading_book`, `local_resources` | **done** (ожидает прогона в CI/mac-tests) |
| 6 | Русская локализация интерфейса | **planned** (см. раздел 6, не реализовывать) |

**Решение по `.fb2.zip` (вопрос закрыт 2026-10-26):** в текущий объём НЕ входит. Nickel не индексирует `.fb2.zip` как книги, а целевой сценарий — одиночные `.fb2`, видимые в штатной библиотеке; desktop/Android по указанию заказчика не приоритетны. Если потребуется позже, точка расширения: в `Publication::inspect(QIODevice&)` после ZIP-ветки детектировать архив с единственным членом `*.fb2`, отдавать его под тем же виртуальным именем `singleFb2ResourceName` (распаковка в `LocalResources::transferSingleFile`), а движок не менять — он уже грузит поток через `LoadDocument(stream, false)`.

**Журнал:**
- 2026-10-26: этапы 2–4 реализованы полностью; добавлен регрессионный тест `tests/model/fb2.cpp` (инспекция, отрицательные кейсы entity/RSS/ZIP, рендер UTF-8 FB2 с обложкой/поэмой/таблицей/сносками/TOC, cp1251, превью, переносы), расширены `nickel_book_menu.cpp` (маршруты .fb2 и PDF-регресс), `reading_book.cpp` (time-only для `file://*.fb2`), `local_resources.cpp` (single-file виртуальный ресурс и отмена по lease). Новых строковых ключей не вводилось — все используемые ключи есть в `assets/en.strings`.

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

> **Статус:** планирование. Ниже — сначала анализ текущего механизма выбора языка в Kobalt (обязательная часть задачи), затем ответ про `ApplicationPreferences\CurrentLocale` и целевое решение.

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
