# FB2: готовые сноски внизу страницы

Проверено 2026-11-06. Это подключение существующего механизма KOReader/CREngine,
не собственная реализация раскладки. Закреплённая зависимость не обновляется.

## Источники

- [KOReader css_tweaks.lua](https://github.com/koreader/koreader/blob/41027902e1984d99d4ae0ec02b5ccd9c1194249d/frontend/ui/data/css_tweaks.lua):
  `footnote-inpage_fb2` включён по умолчанию; `footnote-inpage_fb2_comments`
  использует тот же механизм для endnotes. Основная подсказка —
  `-cr-hint: footnote-inpage`. Размеры и margin/padding также задаются CSS.
- [Закреплённый lvrend.cpp](https://github.com/koreader/crengine/blob/f335f7a97e240f505bb8a0dfb721fcb7912a7824/crengine/src/lvrend.cpp):
  `renderBlockElementEnhanced` собирает IDs блока с `FOOTNOTE_INPAGE`,
  регистрирует ссылки и передаёт их готовому page splitter.
- [Закреплённый lvdocview.cpp](https://github.com/koreader/crengine/blob/f335f7a97e240f505bb8a0dfb721fcb7912a7824/crengine/src/lvdocview.cpp):
  `PROP_FOOTNOTES` по умолчанию true; `Draw` резервирует место, рисует
  сноски у нижнего края страницы и разделитель. Длинные сноски разбивает движок.
- [Закреплённый lvstsheet.cpp](https://github.com/koreader/crengine/blob/f335f7a97e240f505bb8a0dfb721fcb7912a7824/crengine/src/lvstsheet.cpp):
  CSS parser поддерживает несколько hints одновременно. `footnote` и
  `footnote-inpage` — разные, совместимые биты, а не альтернативные режимы.

## Интеграция и сопровождение

Оставляем FB2-only CSS в `fb2::overridesCss` (`reader_fb2.h`), подключённом
существующей FB2-веткой `ReaderModel::applyStyleSheet`. Блоки `section` внутри
стандартных `body[name="notes"]` / `body[name="comments"]` уже распознавались
для popup; добавляем им `footnote-inpage`, не заменяя `footnote`. Примечания
уменьшены до 80% основного шрифта. В отличие от defaults KOReader, сразу
включены оба стандартных body: это сохраняет одинаковую обработку тех типов
сносок, которые форк уже открывает по тапу. Нового переключателя нет.

Общий generated stylesheet больше не содержит наш FB2 override. Своего
нижнего виджета, поиска маркеров на странице, изменения page splitter или
патча зависимости нет. EPUB CSS/props не меняются. Штатное нелинейное тело
сносок не удаляется из документа: переходы и popup продолжают работать.

Policy `:fb2-v2:` инвалидирует только старый разобранный FB2-кэш, поскольку
изменяются сохранённые стили/разбиение. Позиции чтения остаются XPointer,
schema и форматы хранилища прежние. Пересчёт номера страницы после reflow
ожидаем; он не означает сброс прогресса.

Проверки `tests/model/fb2.cpp` читают engine page list через test-only probe:
нижний блок действительно имеет footnote fragments, ссылка продолжает
давать multi-paragraph popup, комментарии также поддерживаются, несвязанная
сноска не добавляется. Это не заменяет визуальный smoke test. Native Qt SDK
в sandbox отсутствует; эти тесты добавлены, но здесь не выполнены.

Для проверки на Kobo: открыть FB2 с короткой и многоабзацной сносками;
проверить нижний блок, тап по той же ссылке, перелистывание назад/вперёд,
смену размера текста и длинную сноску. Позиция после повторного открытия
должна сохраняться. Дополнительно полезны два столбца и скрытие нелинейных
фрагментов. Нестандартные body/неверные ссылки специально не угадываем.

Синхронная очистка кэша из UI — отдельный открытый IO/lifecycle-долг первого
аудита; этот change не объявляет его исправленным.
