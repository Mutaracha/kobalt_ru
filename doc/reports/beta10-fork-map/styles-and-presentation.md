# Подробная дельта beta9: сноски, форматные стили и UI presentation

Дополнение к [карте обновления](README.md), разделы F02/F05. Сверка исходников:
upstream `be47258850ffdc3295257475afdf8b8f6cb53176` → fork
`cb705bdd4fd72b8c1f48535204f54c4eb3264ede`.
Ниже описана текущая дельта, а не все когда-либо сделанные изменения.
Пути с короткими именами в разделе движка относятся к `src/engine/blaze/adapter/`.

## 1. Нижние сноски и форматные стили

### 1.1. Что уже делает upstream, а что добавлено нами

Нижние EPUB-примечания, их разбиение по страницам и размещение выполняет общий
CREngine/upstream pipeline. Это не наш новый алгоритм раскладки. Форк добавляет
семантику FictionBook 2, совместимость старого параметра и оформление номера
нижнего примечания. Поэтому при обновлении нельзя переносить весь старый
`engine_document.cpp` вместо beta10 ради нескольких форматных hooks.

| Обязанность | Наша дельта | Точки кода |
|---|---|---|
| Совместимость настройки | `inpage_footnotes` — canonical Boolean, default false; legacy `fb2_inpage_footnotes` используется только при отсутствии canonical Boolean | `src/reading/appearance_settings.h`, `.cpp`; вызовы миграции в `EngineDocument::configure()` |
| FB2 engine switch | Для FB2 `PROP_FOOTNOTES` зависит от `inpageFootnotes_`; для EPUB исходный engine switch не выключается этим форматным условием | `engine_document.cpp`: настройка props при configure/reflow |
| Какие FB2 узлы считать примечаниями | К общему набору in-page selectors добавлены секции `body[name="notes"]` и `body[name="comments"]` | `reader_fork_styles.cpp`: `fb2NoteSelectors()`; подключение в `engine_document.cpp` |
| Какие FB2 заголовки — только номер | Отдельный DOM marker, не эвристика для любого числа на странице | `reader_fb2.cpp`: `markNumericNoteTitles()`; вызов в `reader_source.cpp` |
| Как оформить номер | Верхний индекс и run-in, только внутри нижнего примечания | `reader_fork_styles.cpp`: `noteLabelsCss()` |
| Базовая семантика FB2 | Отдельная FictionBook stylesheet, а не применение HTML defaults к XML-тегам | `reader_styles.h.in`, `engine_document.cpp`, build inputs |

**Настройки и UI:** `appearance_panel.cpp/.h`, `settings_panel.cpp`,
`fork_settings.cpp/.h` находятся в `src/app/reader/settings/`; они обеспечивают
видимость/подписи и общий путь изменения appearance. Это не отдельный FB2 settings
save pipeline. Сохранять перенос из legacy, не возвращать два независимых ключа.

### 1.2. Точное ограничение оформления номера

`markNumericNoteTitles()` обходит FB2 DOM. Marker ставится на `title`, чей родитель
— `section` внутри notes/comments body. Текст очищается от пробелов; допускаются
обёртки `[123]` и `(123)`. После удаления обёртки строка должна состоять из цифр,
быть непустой и не длиннее 16 символов. Словесный заголовок не превращается в номер.

CSS в `noteLabelsCss()`:

- Для `a.label` и `a._idFootnoteAnchor` внутри in-page footnote: `vertical-align:
  super`, размер `0.75em`. Это не все ссылки и не цифры основного текста.
- Для помеченного FB2 `title`: `display: run-in`, обычный вес, наследуемый размер.
- Для `title p`: `display: inline`, верхний индекс, `0.75em`, обычный вес.
- После такого `p` добавляется неразрывный пробел `\00a0`, чтобы номер не слипался
  с текстом. Размещение номера и текста выполняется движком, не отдельным paint
  offset после расчёта страницы.
- Правила ограничены `-cr-only-if: inside-inpage-footnote`; они не должны менять
  тот же title в обычном потоке или в окне иного типа.

Из этого не следует, что текст всей сноски уменьшен до 75%: это правило номера.
Изменять размер основного текста примечания, нижнее поле, общий pagination или
selection geometry ради визуального зазора пользователь не поручал.

### 1.3. Базовый FB2 CSS и межстрочный интервал

`readerFb2Css` в `reader_styles.h.in` генерируется из закреплённой reference
FictionBook stylesheet CREngine. Она скрывает metadata, задаёт семантику
вспомогательных bodies, стихов, эпиграфов и таблиц. Это подключение существующей
форматной stylesheet, не ручная замена всех HTML-правил.

`fb2LineHeightSelectors()` добавляет к shared настройке интервала теги
`v,title,subtitle,epigraph,cite,annotation,text-author`. Без дополнения HTML-набор
не покрывает FB2-элементы. В `engine_document.cpp` базовый CSS выбирается по формату,
а пользовательский CSS и общие настройки остаются в общем pipeline.

При beta10 нужно проверить upstream stylesheet и её source/build fingerprint.
Если upstream уже поддерживает эквивалентную FB2 семантику, не наслаивать вторую
копию. CSS/cached-layout policy должны обновляться совместно; не переиспользовать
несовместимый parsed layout только потому, что документ открылся.

### 1.4. Сохранение авторской структуры при нормализации

- `reader_alignment.cpp` перед обычными HTML-исключениями вызывает
  `fb2::preserveAlignment()`. Сохраняются notes/comments bodies и элементы
  `title`, `subtitle`, `epigraph`, `poem`, `stanza`, `v`, `text-author`, `date`.
  Смысл — не выровнять стихотворение/подпись как обычный абзац.
- `reader_beautifier.cpp` добавляет `fb2::preserveBeautifier()` для тех же
  семантических блоков, а также `cite` и `annotation`. FB2 `emphasis` и
  `strikethrough` признаются inline при проверке пригодности абзаца для beautifier.
  Общий beautifier не переписан.
- `reader_page_layout.cpp`: распознавание ведущей иллюстрации дополнено XML-тегом
  `image` наряду с `img`/`svg`. Это форматная адаптация, не новый алгоритм глав.
- `reader_source.cpp` вызывает маркировку числовых note titles перед раскладкой
  свежеразобранного FB2. Для cached documents важно не инвалидировать страницы
  повторной произвольной правкой DOM.
- В `reader_links.cpp` есть FB2-адаптация ссылок; переносить вместе с parsing/IDs,
  не заменять общий EPUB link/navigation pipeline. В note preview теги FB2
  переводятся в существующее безопасное HTML-подмножество: `emphasis→i`,
  `strikethrough→s`, `a→span`, `empty-line→br`, title/subtitle/verse/stanza/poem/
  epigraph/cite/text-author/annotation→div. Так блоки не сливаются в одну строку.
  Поиск enclosing note останавливается не только у EPUB `DocFragment`, но и у FB2
  `body`, чтобы не захватить несвязанные секции.

### 1.5. Связанные, но не собственно note-CSS изменения

В том же `reader_fb2.cpp` есть `bindLanguage()`: язык привязывается к `FictionBook`,
если у него нет явного `lang`, чтобы layout/paint/selection использовали один
языковой узел. Явные языковые атрибуты сохраняют приоритет. Там же расположен
`openFb2Preview()` — форматный preview с семантическими ancestors и mapping позиции,
а не второй механизм рисования сносок.

Эти обязанности относятся также к F01/F04. Не удалять их как «лишний код для
сносок», и не записывать comics drawing/cache extraction из того же shared файла
как самостоятельную доработку note layout.

### 1.6. Как переносить и проверять

1. Взять engine/styles beta10; сравнить новый note pipeline и поддержку FB2.
2. Перенести canonical-key migration, FB2 selectors/markers и необходимые
   семантические exclusions. Удалить дублирующий hook, если beta10 уже покрывает его.
3. Восстановить consumers/build CSS inputs/cache policy вместе с helpers.
4. Проверить EPUB и FB2: notes on/off, обычные и числовые заголовки, notes/comments,
   соседние и многостраничные примечания, forward/back links, позиции после reopen,
   preview, selection/hit-testing, poems/epigraphs/author attribution.

Основные тесты: `tests/model/fb2.cpp`, `links.cpp`, `publisher_markup.cpp`,
`fallback_styles.cpp`, `beautifier.cpp`, `appearance_settings.cpp`,
`location_round_trip.cpp`; соответствующие desktop/reader scenarios.
[Принятая граница и device-история сносок](../../fb2-footnotes.md).
Не возвращать отменённый эксперимент adjacent-footnotes.

## 2. About, brand/UI presentation и иконки

### 2.1. Страница «О приложении»: одна добавленная строка

В `src/app/about/about_page.cpp` относительно beta9 добавлены include
`fork_about.h` и вызов `appendForkBuildRow()` после upstream commit label, до нижней
линии блока сборки. Логотип, wordmark, version/Qt, описание и acknowledgments
остаются компонентами общей AboutPage. Скриншот не означает, что весь экран — наш.

`fork_about.cpp/.h` добавляют:

- Только при непустом `kobaltForkName` — строку с именем форка.
- При наличии build timestamp — имя и UTC-время сборки через существующий
  `about.fork_build`; иначе `about.fork`.
- `InterfaceLabel`, object name `rrAboutFork`, выравнивание по центру, общий
  interface font с метрикой `metrics::about::commitText`.
- Значения приходят через generated `src/support/build_info.h.in` и build wiring.

Скрытого жеста выбора языка на этой строке больше нет. После текущего рефакторинга
About не включает language chooser header. Код окна расположен в
`src/app/reader/settings/debug_language_dialog.cpp/.h`; подключён первым пунктом
Debug из `settings_panel.cpp`, название «Язык интерфейса». Persistence/resolution
остаются в `src/support/debug_language.*` и `locale.*`, а не в UI About/Debug.

**Beta10:** сохранить небольшой insertion hook и build identity, а не копию старой
AboutPage. Проверить Library About и Reader About, пустой fork-name, пустой timestamp,
RU/EN и отсутствие возвращённого языкового жеста.

### 2.2. Какие шрифтовые изменения действительно наши

Политика форка: IBM Plex Sans для интерфейсного текста и Instrument Serif для
брендового слова. Но наличие этой политики не означает, что каждый файл fonts/
private text отличается от beta9. Shared font loader, coverage/cache/private-text
helpers и logo-only splash были выровнены с upstream; не переносить их как
«собственный загрузчик шрифтов».

Конкретная retained дельта `src/ui/controls/return_title_text.h`:

- Находит первое вхождение `Kobalt` без учёта регистра, выделяет его в brand-font run.
- Окружающие слова рисует интерфейсным Sans, а не Serif для всей фразы.
- Измерение width/height и painting используют один и тот же набор runs.
- Выравнивает runs по общей baseline, учитывает ink bounds и contents margins.
- Сохраняет private-text fallback и tint для glyphs, которым он нужен.

`reader_build.cpp` подключает helper к ReturnTitle, переопределяет sizeHint и
minimumSizeHint и выбирает обычный UI font для остальной строки. Это устранение
расхождения измерения/рисования смешанной брендовой подписи, а не изменение текста книги.

**Beta10:** не сохранять старые глобальные font APIs ради этого маленького helper;
адаптировать run measurement к новым APIs. Проверить длинную RU/EN подпись, маленький
экран, смену density и отсутствие clipping на serif glyphs.

### 2.3. Checkbox/radio: геометрия общих controls

`src/ui/controls/options.cpp/.h`:

- `stylePanelCheckbox()` задаёт общий Square indicator, label/description metrics,
  leading indicator и row height для панельных checkbox-потребителей.
- Галочка масштабируется **по фактическим width/height квадрата**, а не через старую
  фиксированную 48px-сетку с независимым scale. Толщина зависит от меньшего масштаба.
- `RadioList::setBottomDivider()` позволяет не рисовать повторную нижнюю линию там,
  где её уже рисует modal action. Это используется и в неизменённом окне языка.

`settings_panel.cpp` применяет общий Settings checkbox presentation к своим options.
Первый action языка ранее не попал в список `styleListAction()` — потому получил
необщую высоту/типографику. Текущее исправление добавляет его в тот же список, что
cache/version/другие action rows: общая высота `chrome::listActionHeight`, label font,
padding и bottom rule. Не введены отдельный top-offset или особая высота для языка.

**Beta10:** сохранять нужный внешний вид через новые shared APIs, не две копии
checkbox implementation. Проверки: разные indicator sizes, label/description fit,
light/dark, RU/EN, first-row height и разделители modal.

### 2.4. Toolbar icons и state без инвертированной плашки

`src/ui/controls/fork_icon_geometry.cpp/.h` содержит отдельную геометрию:

- **Comics:** вытянутый speech bubble с изогнутым хвостом по принятому reference;
  canvas 256 и stroke 16 интегрированы в общий icon renderer.
- **DarkTheme:** Fluent `ic_fluent_dark_theme_24_regular`, MIT, перенесён с canvas 24
  на 256. Встречное направление subpaths оставляет половину круга полой; это не
  случайная незакрашенная область.

`icons.cpp/.h` добавляют enum/configuration/rendering hooks; Comics в checked-state
может заполнять path. `assets/icons/phosphor/dark-theme.svg`, manifest и
`assets/notices/Fluent-MIT` обеспечивают asset/provenance. Fluent не следует ошибочно
приписывать лицензии Phosphor только из-за места хранения asset.

`buttons.cpp/.h` отделяют logical checked от инвертированной заливки: можно не
показывать State_On/background highlight, но передавать checked в иконку.
`icons.cpp` при `stateInkOnly` выбирает disabledInk для off и ink для on без обычной
инверсии фона. Disabled control и off-state — не одно и то же.

Consumers находятся в `reader_build.cpp`/`fork_reader_actions.cpp`; это F06,
а не новый общий dark-mode engine. Состояние кнопки не должно меняться на «busy»
только из-за reflow. **Проверки:** dark/comics on/off, light/dark palettes, disabled,
нажатие/отпускание, repaint после reflow; `darkThemeToggle`, `comicsQuickToggle`,
`tests/model/ui.cpp`.

### 2.5. Подписи аннотаций и разделители

`src/ui/controls/highlight_palette.h` и `reader_annotations.cpp` сохраняют
согласованные подписи, заголовки секций и их fitting в palette/editor. Это UI
presentation поверх общей annotation storage/session логики, а не второй highlight
store. Переводы и plural rules находятся в строковых ресурсах/locale helpers,
не следует встраивать RU literals в новый renderer beta10.

### 2.6. Ресурсы, лицензии и граница переноса

`assets/CMakeLists.txt`, `manifest.json`, `acknowledgments.json`, notices и
`build_info.h.in` — часть обновления вместе с UI-кодом. Перед переносом проверить
конкретную дельту каждого файла: общий manifest/acknowledgments не принадлежит
форку целиком. Сохранить notices для используемых материалов; незакрытые вопросы
атрибуции не считать решёнными только из-за присутствия файла лицензии.

Нельзя переносить из history отменённую menu page-count/time-to-end strip,
собственную полосу прогресса/Top/метки глав или прежний About five-tap input.
Логотип/шрифт, присутствующие и в beta9, брать из beta10 без fork merge, если их
mode/OID совпадают с нашей базой; необходимые hooks затем проверять на совместимость.

## Проверяемость

Для каждого пункта есть конкретные символы и пути; полный реестр файлов и OID —
[all-files.tsv](all-files.tsv). Воспроизводимая команда для отдельного shared файла:

```sh
git diff --no-renames be47258850ffdc3295257475afdf8b8f6cb53176 \
  cb705bdd4fd72b8c1f48535204f54c4eb3264ede -- src/ui/controls/options.cpp
```

Сверка кода не заменяет native/device acceptance; Hybrid-алгоритмы этим дополнением
не изменены. Новые runtime изменения относятся только к action row языка и
перемещению его UI-кода; расширение этой карты — документация.
