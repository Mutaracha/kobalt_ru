# Beta.9 — контракты движка и безопасная граница FB2

## Область и статус

Продолжение [preflight](beta9-preflight.md). Проверены исходники upstream
`be47258850ffdc3295257475afdf8b8f6cb53176` и форка
`caf9b177cb4fe09d718b304880658f7fe595dbd5`. Это технический план интеграции,
**не реализованный порт и не результат runtime-тестов**. Runtime остаётся
`23e2d023e1ed732878a21cd2148baaddc24d50e6`.

## Выбранная начальная граница переноса

Сохранить upstream Blaze и его EPUB Hybrid pipeline. FB2 сначала интегрировать
как **single-document whole-book path** в новом Blaze adapter. FB2 не превращать
в искусственный EPUB/spine и не реализовывать отдельный Hybrid scheduler.
Это сохраняет прежний контракт FB2: исходный DOM/XPointer, полная пагинация,
семантический preview и сноски. Ускорение FB2 не заявляется. Настройку rendering
mode не менять и не записывать за пользователя; ограничение применяется локально
к формату, а не через глобальный chapterModeOverride.

Это начальная архитектура порта с обязательной проверкой на Blaze. Даже если
предыдущий whole-book FB2 работал на CREngine, это ещё не доказывает совместимость
новой библиотеки. Если её контракт мешает прямому FB2 load, перенос останавливается
на этой границе — не маскируется ошибками EPUB/container.

## Найденные контракты и обязательные точки адаптации

### 1. Publication → ResourceCache → openSource

В beta.9 `adapter/reader_source.cpp`:

- `EngineDocument::open` сначала выполняет supportedArchive, затем открывает
  archive; XML не дойдёт до библиотечного format detection.
- `openResources` строит ResourceContainer; `openSource` обязательно читает EPUB
  mimetype и далее оборачивает container в ContentContainer.
- LoadDocument вызывается с пустым memory stream и container: это не raw-FB2 load.

Сохранить нашу publication recognition и виртуальный single-file ресурс,
bounded transfer/cancellation/error precedence и format dispatch. Для FB2:
проверенный stream с исходными XML bytes, без EPUB mimetype/ContentContainer.
EPUB ветка и её проверки остаются upstream. Формат выбрать **до configure/CSS**,
как в текущем фиксе FB2: иначе fresh DOM нормализуется с HTML stylesheet.
Не делать отдельных копий всей openSource: общие font initialization, slot check,
progress/cancellation, resource errors и завершение открытия остаются общими.

### 2. Whole-book FB2 — это больше, чем отключение первого spine window

В upstream openSource при chapterMode() выбираются `koblaze_first_spine_item`,
`koblaze_last_spine_item` и slicePages; при LoadDocument применяется глобальный
koblazeSetSpineWindow с последующим сбросом. Для FB2 нужно принудительно использовать
whole-book runtime state (first/last = -1, slicing off), включая повторное открытие,
а не доверять сохранённым EPUB/runtime ключам.

**Дополнительный найденный риск:** ReaderSession::editAppearance/dispatch может
снова запросить chapter mode после изменения настроек/размера в Hybrid.
`EngineDocument::setChapterMode` сейчас пересоздаёт source через open/openResources
и назначает slicePages по `fast`. Поэтому одного guard в первом open недостаточно.

Для FB2 setChapterMode должен применять appearance/size в whole-book режиме с
сохранением source bookmark и корректно возвращать результат. Нельзя просто
return false: dispatch трактует это как `could_not_update_text`. Нельзя return true
без применения patch/size: внешний session получит ложное успешное изменение.
Нужен тест изменения размера, шрифта, rotation и режима после первого открытия.

### 3. Background work и документные лимиты

В upstream:

- prepareFullDocument возвращает None при windowFirst < 0 или slicePages <= 0;
- refinePageEstimate возвращает false при windowFirst < 0;
- roughPages не выдаёт оценку при whole-book layout;
- applySlice при отсутствии slices сбрасывает layout slice;
- bookPages для whole-book использует pagesOf + 1.

Эти guard подходят whole-book FB2 при условии правильного состояния на **каждом**
open/reflow. FB2 не должен запускать лишний full build и удерживать второй whole DOM.
Сохраняются библиотечный slot guard и единая worker/idle дисциплина upstream.
Проверить также возврат с FB2 на EPUB в переиспользуемых документах: формат,
stylesheet, slice и source flags не должны протечь в следующий load.

### 4. Preview

Beta.9 `EngineDocument::openPreview(Engine &other, ...)` принимает новый общий
Engine interface, сверяет имя и затем ищет DocFragment и EPUB container.
Raw FB2 не имеет этого контекста. Наш dispatch openFb2Preview должен стоять после
проверки доступного источника/типа, но **до** поиска DocFragment/container.

Перенести semantic fragment writer из reader_fb2.cpp в adapter-слой с сохранением
bounds, ancestor title/poem/cite/section, inline attrs, codepoint offset, language,
исключения generated boxes/pseudo content и неизменности source DOM. Путь открытия
synthetic FB2 также обязан обходить EPUB-only проверки и chapter slicing.
Контракт preview новый: не возвращать прежнюю concrete-only сигнатуру вместо
upstream virtual API. Проверить lifetime parent/source при закрытии основной книги.

### 5. Позиции и видимые номера страниц

Новый ReaderSession:

- page/pages — локальный layout; используются для навигации и геометрии;
- bookPage/bookPages — whole-book display; bookPage zero-based;
- bookPagesEstimated — признак оценки;
- Engine::bookPages(positions) возвращает one-based страницы книги, 0 для ещё
  неизвестной оценки, -1 для неразрешимого source position.

Полоса не должна самостоятельно выбирать page/pages. Upstream chromeValues
уже учитывает rough_pages/rough_item_pages, pagesEstimated, pending counts,
percent fallback и decimalPercentages. Именно его Values передаются formatter.

**Уточнение предыдущего preflight:** наша последняя полоса уже форматирует Values;
опасность не в прямом session->page вызове её formatter, а в механическом переносе
старой реализации chromeValues из reader_settings.cpp или в сохранении условия
`session->pages()>0` как признака известного whole-book count. В beta.9 локальные
pages могут быть известны, когда общий номер ещё рассчитывается.

Для полосы нужны состояния: pending / estimated / exact. Использовать существующие
upstream поля и форматирование, не показывать локальные `10 из 15` как книгу из
230 страниц. Если нужен новый текст/маркер оценки вне существующих строк — сначала
согласовать EN/RU. Не заставлять worker синхронно считать книгу ради подписи.
Для whole-book FB2 Values должны сразу содержать обычные точные page/pages.

### 6. Comics и новая выдача кадров

Comics не отдельный engine: сохранить модульность ColumnView/EngineDocument и
общий frame lifecycle. В beta.9 previewTurn умеет использовать заранее собранный
full document (`preparedSpreadTurn`) и меняет документ при adoption. Наши artwork
request tracking/paint caches/readiness/hit rects нельзя привязывать только к
прежнему engine_ или к номеру страницы без source/layout identity.

Проверить physical-column ↔ display-page mapping на каждом документе. Не отключать
Hybrid глобально ради comics; необходимость format/content-specific fallback
определять по воспроизведению, не добавлять заранее новую настройку.

### 7. Nullable UI

Upstream Navigation::update уже использует `state.settings && ...`.
Наш fork_navigation::updateForkPageStrip должен учитывать отсутствие settings,
appearance и других ещё не построенных панелей; не вызывать их построение ради
проверки видимости. Hide observer toolbar и initial hide полосы остаются, repaint
не заменяется full flash. Factory/format/placement — в fork_navigation, общий
Navigation оставляет dispatch. Прежнее eager создание UI не возвращается.

## Карта размещения модулей

| Текущее место | После переноса | Владелец |
|---|---|---|
| engine/crengine/reader_fb2.* | engine/blaze/adapter/reader_fb2.* | XML/DOM preview и FB2 predicates |
| engine/crengine/reader_fork_styles.* | engine/blaze/adapter/reader_fork_styles.* | Format selectors/stylesheet integration |
| engine/crengine/reader_comics.* | engine/blaze/adapter/reader_comics.* | Adapter metadata/geometry policy |
| column_view_comics.cpp, engine_document_comics.cpp | рядом с новым adapter ColumnView/EngineDocument | Render/frame mapping, не ReaderSession scheduler |
| publication/fb2*.h/cpp | publication, как сейчас | Recognition/resources, без Blaze/UI зависимости |
| views/fork_navigation.*, window/fork_reader_actions.cpp | те же слои | Presentation и тонкое соединение с session |
| fork_icon_geometry.*, icons.cpp hook | те же слои | Glyph geometry + shared cache/state painter |

Пути выше — план, файлы ещё не перемещены. Библиотечные Blaze source/extensions
изменять только если конкретный контракт невозможно выполнить adapter hook;
не внедрять новую библиотечную поддержку FB2 slicing без отдельного основания.
Старые standalone tests include paths, CMake shared inputs/CSS configure и source
scan нуждаются в явном обновлении; не возвращать удалённый внешний engine build.

## Приёмочные gates до устройства

| Gate | Проверка | Ожидаемый результат |
|---|---|---|
| G1 — baseline | Upstream beta.9 EPUB/native + Qt5 build с новой сборкой | Blaze/source manifests/targets подключены, EPUB не регрессировал |
| G2 — FB2 open | UTF-8/Windows-1251, prolog/decoys, errors/cancel, missing/truncated resources | Recognition и сообщения корректны, без EPUB-only rejection |
| G3 — layout modes | FB2 при Hybrid/Accurate, смена appearance/viewport/rendering mode | Whole-book остаётся whole-book; patch применяется, позиция сохранена |
| G4 — source/preview | Исходная книга: «На другое утро», Cyrillic/non-BMP/inline/verse/cite/subtitle | Style/anchor/language сохранены, source image/state неизменны |
| G5 — notes | FB2 notes/comments/run-in/superscript и upstream EPUB notes | Принятый вид и зазоры, корректная пагинация и возврат по ссылке |
| G6 — pages | EPUB pending→estimated→exact + adoption/rotation; FB2 exact | Нет смешения local/book pages, нет ненужного reflow/repaint |
| G7 — UI lifetime | Первое меню до lazy panels, reopen, preview, cover, theme/rotation | Нет null deref, полосы при открытии книги/после закрытия меню нет |
| G8 — comics | Portrait/wide/center, landscape пары, off/on, reflow/adoption, images/hits | Геометрия/позиция/readiness сохранены, кнопка не мерцает при busy |
| G9 — storage/platform | Reopen/cache, bookmarks/annotations, native position, sleep/close | Source positions и записи сохранены, новый close/profile writer принят |

Все gates **pending**: это спецификация проверок, не PASS. Native/Qt5/device для
порта ещё не запускались. Старое post-frame-load зависание не объявляется исправленным.

## Порядок дальнейшей реализации

1. Новая upstream база/Blaze build и адаптация include/target boundaries.
2. Publication + FB2 whole-book open/reflow/preview с G1–G4.
3. Notes/comics/frame mapping и G5/G8; не копировать старый source scheduler.
4. UI/RU/полоса/progress/sleep/Kobo hooks и G6/G7/G9.
5. Полный focused native набор, Qt5 package/contents, затем отдельная device-приёмка.

При переносе не применять `text-clean` файлы вслепую: например, null-panel ошибка
и whole-book formatter compatibility не выявляются отсутствием merge markers.
