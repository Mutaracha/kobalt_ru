# 020 — FB2 поверх beta10 + 010-ru

Статус: **подготовка переноса, не готовый патч**. В этом этапе реализации FB2
ещё не перенесены в beta10. Не выдавать саму инвентаризацию за проверку формата.

## Порядок работы с кодом

1. Publication: `src/publication/fb2.h`, `publication_fb2.cpp`,
   `fb2_resources.cpp/.h`, наши hunks `publication.cpp/.h`, `local_resources`,
   `resource_cache`. Сохранить raw-byte распознавание legacy encoding, проверку
   FictionBook root и форматные границы; FB2.ZIP не должен означать любой ZIP.
2. Engine source/document: `reader_fb2.cpp/.h`, форматные hunks `reader_source`,
   `engine_document`. В `reader_fb2.cpp` уже вместе находятся язык документа,
   alignment/beautifier и примечания: **не разделять искусственно ради автономности**.
   Не переносить рядом находящиеся comics hooks раньше 030 без явной записи.
3. Форматные стили и сноски: `reader_fork_styles`, `reader_styles.h.in`,
   `reader_links`, `reader_alignment`, `reader_beautifier`, `reader_page_layout`.
   В beta10 часть поведения уже общая: переносить только FB2-семантику и принятые
   отсутствующие стили, не заменять весь upstream note pipeline.
4. Открытие/платформы: `opening_job`, Kobo book_menu/coordinator,
   Qt5/Qt6 native_publication/controller, desktop/Android входы. Каждый общий файл
   сравнить с beta10; здесь уже есть hooks RU из 010, их не перезаписывать.
   Не возвращать старые opening accelerations, flash suppression, FB2 Hybrid.
5. Добавить source/test/fixture registration вместе с кодом. Только после этого
   регенерировать 020 как diff **от дерева после 010**, записать SHA-256 и ready.

Это рабочий порядок review, не утверждение, что перечисленные файлы целиком нужны.
Точки интеграции могут быть переименованы в beta10; проверять callers и тесты.

## Проверки перед ready

- Чистая последовательность upstream → 010 → 020, отсутствие случайного изменения
  исходного EN/RU-контракта процентов и локали; generation/source references.
- FB2 и FB2.ZIP: title/author/cover, ресурсы и изображения, кодировки UTF-8/1251,
  имя файла/тип документа, обычное/повторное открытие, сохранённая позиция.
- Note links туда/назад, примечания on/off, notes/comments и длинные примечания.
- Обычный EPUB (в том числе upstream opening/Hybrid), protected/unsupported input
  не становятся разрешёнными побочно.
- Перенести и выполнить релевантные проверки `fb2.cpp`, `fb2_session.cpp`,
  `local_resources.cpp`, `resource_cache.cpp`, `nickel_book_menu.cpp`,
  links/publisher_markup/fallback_styles/beautifier и desktop интеграцию.
- Native + Qt5 compile/package; затем полный стек и пользовательское устройство.
  Успешное apply не заменяет ни сборку, ни проверку платформ.
