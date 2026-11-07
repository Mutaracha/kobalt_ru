# Нижние сноски EPUB / FB2

Текущая реализация следует стандартным CSS-профилям KOReader на том же
закреплённом CREngine f335f7a97e24. Полная карта исходников, отличий и проверок:
[сверка с KOReader](koreader-footnotes-review.md).

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
