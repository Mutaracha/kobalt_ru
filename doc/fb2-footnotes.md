# Нижние сноски EPUB / FB2

> Документ описывает текущую fork/beta7 реализацию. Новое решение пользователя:
> при интеграции beta.8 заменить её полным upstream механизмом, включая
> `inpage_footnotes` / default false, DOM markers и note interaction tests.
> Прежний default-on/custom-CSS контракт ниже не является условием миграции.
> Полный beta8 перенос пока не выполнен; актуальный статус — в FB2_FORK_PLAN.md.


Текущая реализация следует стандартным CSS-профилям KOReader на том же
закреплённом CREngine f335f7a97e24. Полная карта исходников, отличий и проверок:
[архив сверки с KOReader](../FB2_FORK_PLAN.md#archive-koreader).
Технические источники и действующие ограничения приведены ниже.

## Владение механизмом

- `reader_notes.h`: общие semantic popup helpers и EPUB CSS type/role. Нет
  собственного EPUB marker/DOM walk. Добавлен узкий publisher-profile
  `._idFootnotes > ._idFootnote` для EPUB 2 экспорта InDesign. Позднее правило задаёт EPUB 80% как в
  KOReader `inpage_footnote_font-size_smaller`.
- `reader_fb2.h`: отдельные FB2 popup hints, условный нижний вывод для всех
  section внутри notes/comments; размер только на верхних section, 75%/85%.
- `reader_fb2.cpp`: согласованная нормализация только числовых заголовков
  примечаний и preview. `getUnboxedParent()` учитывает engine run-in boxing.
  IDs и DOM nodes не удаляются, номера в основном тексте не меняются.
- `engine_document.cpp`: одна настройка `fb2_inpage_footnotes`, её reflow,
  CSS и PROP_FOOTNOTES. Off не отключает popup.
- `reader_source.cpp`: выбор формата до configure/parse, mode-aware checksum.
- CREngine lvrend/lvpagesplitter/lvdocview: внутренние IDs, reservation,
  separator, длинное continuation, вложенные hints, повторные ссылки, отрисовка.
  Нового footer-widget и патча зависимости нет.

## Совместимость

Одна default-on настройка сохранена. Notes и comments FB2 включены; уменьшенные
надстрочные номера без скобок — согласованное отличие от KOReader. Для EPUB
popup hints/classnames не являются стандартным in-page профилем, как и в
KOReader. Optional Wikipedia/classic/extension/flatten tweaks не включены
автоматически. Несвязанные обычные цифровые ссылки не угадываются.

EPUB policy `:epub-notes-v3:inpage-on/off:`, FB2 `:fb2-v6:inpage-on/off:`
перестраивают только разобранный rendering cache. Initial load и live toggle
используют одинаковые salts. Storage schema/позиции XPointer/аннотации прежние.
Пересчёт страниц ожидаем и не означает потерю позиции.

## Проверка

`tests/model/links.cpp::inPageNotes`, `tests/model/fb2.cpp`, appearance-settings
и document-cache входят в focused native job device workflow. Точные результаты
новой версии см. в FB2_FORK_PLAN.md; добавленный тест не равен выполненному.
Проверка на Kobo: короткие/длинные сноски, несколько/повторные ссылки, номер и
разделитель, off/on с popup, размер текста/столбцы, закрытие и повторное открытие.
Для сравнения с KOReader нужны одна книга, одинаковые шрифт/поля и профили;
само наличие общего backend не гарантирует pixel-identical layout.

## Закреплённые источники и границы профиля

Сверка относится к фиксированному snapshot, не к изменяемому master:

- [KOReader 9040116701c8: css_tweaks.lua](https://github.com/koreader/koreader/blob/9040116701c888889bd35233b23b83283be66745/frontend/ui/data/css_tweaks.lua): defaults, `footnote-inpage_epub`, `footnote-inpage_fb2`, `footnote-inpage_fb2_comments`, `inpage_footnote_font-size_smaller`.
- [ReaderStyleTweak](https://github.com/koreader/koreader/blob/9040116701c888889bd35233b23b83283be66745/frontend/apps/reader/modules/readerstyletweak.lua): применение CSS, отдельные профили, конфликты и default tweaks.
- [ReaderLink::showAsFootnotePopup](https://github.com/koreader/koreader/blob/9040116701c888889bd35233b23b83283be66745/frontend/apps/reader/modules/readerlink.lua): popup — отдельный detector и UI, не алгоритм нижнего вывода.
- Gitlink `base` этого snapshot — `1c9682506757d5fd30cfa01c5698929dcc35340f`, его `thirdparty/kpvcrlib/crengine` — **f335f7a97e240f505bb8a0dfb721fcb7912a7824**. Это тот же CREngine, который закреплён в dependencies/manifest.json нашего форка.
- [lvrend.cpp](https://github.com/koreader/crengine/blob/f335f7a97e240f505bb8a0dfb721fcb7912a7824/crengine/src/lvrend.cpp): `renderBlockElementEnhanced`, FOOTNOTE_INPAGE, сбор всех внутренних IDs, NOTEREF_IGNORE на ссылке.
- [lvpagesplitter.cpp](https://github.com/koreader/crengine/blob/f335f7a97e240f505bb8a0dfb721fcb7912a7824/crengine/src/lvpagesplitter.cpp): резервирование места, повторные ссылки, continuation/страницы только со сноской.
- [lvdocview.cpp](https://github.com/koreader/crengine/blob/f335f7a97e240f505bb8a0dfb721fcb7912a7824/crengine/src/lvdocview.cpp): PROP_FOOTNOTES / DOC_FLAG_ENABLE_FOOTNOTES, отрисовка separator и нижних fragments.

CSS defaults, поздний размер и popup detection — отдельные механизмы.
Inline `style="-cr-hint: footnote-inpage"` применяется после stylesheet: без
стандартных type/role позднее условие может не назначить 80%, как и в эталонном
профиле. Для него regression сравнивает отдельный reference engine; стандартные
type/role сохраняют строгую проверку 80% против publisher `!important`.
`FOOTNOTE_IGNORE` относится к popup; `NOTEREF_IGNORE` проверяется на source link.

InDesign `._idFootnotes > ._idFootnote` — намеренное расширение профиля форка:
в указанном KOReader snapshot оно не входит в defaults/classic list. Не
помечать весь контейнер, source/backlink или standalone класс вне этой структуры.
Реальная пользовательская книга и исходные снимки находятся в
[`tests/manual/`](../tests/manual/README.md); opt-in режим `--publisher-notes PATH`
не заменяет portable synthetic regression. Полная история, первоначальные
ограничения и последующие результаты — в плане, не в отдельном журнале `doc/`.
