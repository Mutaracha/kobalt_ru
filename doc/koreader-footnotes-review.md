# Сверка нижних сносок с KOReader

Дата: 2026-11-07. Эталон — конкретный snapshot, а не изменяемый master:

- [KOReader 9040116701c8: css_tweaks.lua](https://github.com/koreader/koreader/blob/9040116701c888889bd35233b23b83283be66745/frontend/ui/data/css_tweaks.lua): defaults, `footnote-inpage_epub`, `footnote-inpage_fb2`, `footnote-inpage_fb2_comments`, `inpage_footnote_font-size_smaller`.
- [ReaderStyleTweak](https://github.com/koreader/koreader/blob/9040116701c888889bd35233b23b83283be66745/frontend/apps/reader/modules/readerstyletweak.lua): применение CSS, отдельные профили, конфликты и default tweaks.
- [ReaderLink::showAsFootnotePopup](https://github.com/koreader/koreader/blob/9040116701c888889bd35233b23b83283be66745/frontend/apps/reader/modules/readerlink.lua): popup — отдельный detector и UI, не алгоритм нижнего вывода.
- Gitlink `base` этого snapshot — `1c9682506757d5fd30cfa01c5698929dcc35340f`, его `thirdparty/kpvcrlib/crengine` — **f335f7a97e240f505bb8a0dfb721fcb7912a7824**. Это тот же CREngine, который закреплён в dependencies/manifest.json нашего форка.
- [lvrend.cpp](https://github.com/koreader/crengine/blob/f335f7a97e240f505bb8a0dfb721fcb7912a7824/crengine/src/lvrend.cpp): `renderBlockElementEnhanced`, FOOTNOTE_INPAGE, сбор всех внутренних IDs, NOTEREF_IGNORE на ссылке.
- [lvpagesplitter.cpp](https://github.com/koreader/crengine/blob/f335f7a97e240f505bb8a0dfb721fcb7912a7824/crengine/src/lvpagesplitter.cpp): резервирование места, повторные ссылки, continuation/страницы только со сноской.
- [lvdocview.cpp](https://github.com/koreader/crengine/blob/f335f7a97e240f505bb8a0dfb721fcb7912a7824/crengine/src/lvdocview.cpp): PROP_FOOTNOTES / DOC_FLAG_ENABLE_FOOTNOTES, отрисовка separator и нижних fragments.

## Найденные расхождения и исправления

| Область | Было в форке | Подход после сверки |
|---|---|---|
| EPUB распознавание | Собственный проход DOM, приватный атрибут, блокировка вложенных candidates | CSS по стандартным token-атрибутам type/role; вложенность, IDs и сбор fragments целиком обрабатывает тот же CREngine |
| Popup vs in-page | FOOTNOTE автоматически превращался в FOOTNOTE_INPAGE, FOOTNOTE_IGNORE подавлял нижний вывод | Два независимых назначения: popup hint не включает нижний блок; FOOTNOTE_IGNORE сам по себе не запрещает in-page, NOTEREF_IGNORE на source link запрещает его участие |
| Классы EPUB | footnote/endnote автоматически считались нижними сносками | Как стандартный профиль KOReader — только type/role. Popup recognition по классам остаётся. Classic classnames/Wikipedia — отдельные необязательные профили в KOReader, не включаем их безусловно |
| Размер EPUB | Обычный font-size на своём marker, мог проигрывать publisher CSS | Позднее условное правило `late`/`inpage-footnote`, 80% user font с !important, включая autoBoxing, как default legacy-size tweak KOReader |
| FB2 sections | Только `body > section`, 80% notes/comments | `body section`, margin 0; размер только на первом уровне: notes 75%, comments 85%, как соответствующие tweaks KOReader |
| Выключение | Только удаление нашего CSS | Удаление CSS плюс штатный PROP_FOOTNOTES: подавляется и явный publisher `footnote-inpage`; popup и IDs сохраняются |
| Кэш | Сохранял частную EPUB классификацию | EPUB v2 / FB2 v6, mode-aware; перестраивается disposable rendering cache, без миграции прогресса/аннотаций |

Сам page splitter не переписан и dependency не обновлялась. Нет вычисления
текста сноски по номеру, копирования примечания в footer-widget, собственного
лимита высоты или вручную разбитого continuation. CSS декларации выражают те
же стандартизованные селекторы/engine hints; Lua UI KOReader не переносится.

Отдельно проверен порядок применения inline styles: в `lvrend.cpp` stylesheet
применяется до `style="..."`. Поэтому при одном лишь inline
`-cr-hint: footnote-inpage` позднее условие KOReader ещё не видит hint и размер
может остаться издательским. Это не исправляется новым собственным алгоритмом:
regression сравнивает такой случай с отдельным engine и эталонным CSS-профилем.
Для стандартных type/role дополнительно проверяется 80%, включая publisher
font-size:300% !important.

## Сохранённые согласованные особенности форка

- Одна default-on настройка для EPUB, FB2 notes **и comments**. В новых
  установках KOReader default-on EPUB/FB2 notes/legacy 80%; comments — отдельный
  optional tweak. Мы не выключаем ранее включённые comments у пользователя.
- Номера FB2 остаются уменьшенными надстрочными без скобок, как ранее заказано.
  В KOReader базовый fb2.css использует жирный run-in заголовок. Это намеренная
  разница оформления, не новый алгоритм раскладки.
- Qt popup и консервативный detector Kobalt не заменены Lua/MuPDF popup KOReader.
  Уже существующая popup classification matrix остаётся проверкой против
  регрессий. Полная идентичность popup UI/всех fallback-эвристик не заявляется.
- Автоматическое продление до следующего примечания, выравнивание/обнуление всех
  внутренних отступов, flatten сложной разметки, Wikipedia и широкий classic
  classnames не включаются: в KOReader это отдельные optional tweaks. Они могут
  захватывать лишний текст/ломать списки; смешивать их с обычными defaults нельзя.

## Проверка

Focused native cases покрывают стандартные EPUB атрибуты/alternate namespace,
раздельные hints, source ignore, publisher font-size, явный in-page hint,
поддельный прежний marker, вложенные блоки и длинное continuation; сохраняются
cross-XHTML, popup/off-on, source anchor, font/columns, on/off parsed cache.
FB2 regression проверяет вложенный section и отсутствие повторного уменьшения
шрифта на каждом уровне; прежние номер/popup/CP1251/normalization сохраняются.

Статус конкретного прогона фиксируется в FB2_FORK_PLAN.md. Это engine tests на
нашем закреплённом CREngine, не запуск самого приложения KOReader и не pixel
comparison двух readers. Внешний вид, перенос длинных сносок, разделитель и
повторные ссылки нужно сравнить на одном Kobo, с одной книгой, одинаковым
шрифтом/размером/полями и перечисленными tweaks KOReader. Сравнение разных
профилей KOReader не доказывает ошибку стандартного профиля.
