# FB2 после приёмки рефакторинга: первичный разбор

Пользователь принял runtime `0e3e9f98` по согласованному checklist этапов 3+4.
Три сообщения ниже — отдельные ошибки. Их не считать исправленными результатом
предыдущего CI. Исходные скриншоты: `test & diagnostic/fb2-post-refactor/`.

## Начало главы и обновление экрана

В `Reader::paintTurn()` нет ограничения на EPUB: forward turn и
`chapterPage() == 1` передаются в `refreshOnTurn()`. Smart требует также минимум
`smartRefreshTurns` (10) перелистываний с последнего flash. Chapter этого порога
не имеет. Это существующее поведение, не новое объяснение причины FB2-дефекта.

Источник `chapter_page`: CREngine TOC → `EngineDocument::chapters()` /
`chapterSelection()` → `chapterProgress()` → `captureChapters()` → Reader frame.
FB2 использует TOC, построенный CREngine из section/title; при отсутствии
разрешимых native chapters общий код не должен закрывать fallback TOC.
Добавлен synthetic FB2 contract-test главы → chapter page 1 → eligible Smart.
Он не заменяет воспроизведение на книге пользователя и аппаратный flash.

Проверены зависимые пути по коду:

| Механизм | Структура / граница поддержки |
|---|---|
| Chapter header/footer, страницы главы, time estimates | Общий `chapterProgress`/`ChapterSelection`; зависит от тех же якорей и пагинации. |
| Progress ticks, выбор главы, график Stats | `session.chapters()`/`chapterGraph()`; те же разрешённые позиции. `reader_word_counts` считает DOM, не требует EPUB spine для fallback TOC. |
| Уменьшение отступов главы / leading image | `markChapterLeadingImages` обходит body и TOC anchors; image поддержан наряду с img/svg. Full-page EPUB spine wrapper имеет отдельную проверку DocFragment, это не универсальная разметка FB2. |
| Alignment / paragraph controls / normalization | Общие markers; `reader_fb2` сохраняет title/subtitle, epigraph, poem/stanza/v, cite/annotation/text-author и другие специальные блоки. |
| Нижние сноски / popup | notes/comments section и FB2 note reference имеют отдельные правила; это не главы основного текста. |

Статический обзор не доказывает отсутствие ошибок на нетипичной разметке.
Нужны FB2 и конкретный переход главы; порог/направление обновления не менялись.

## «Убирать интервалы между абзацами»

Скриншот показывает наложение текста у нижней границы. Причина пока не доказана.
Флаг `beautifier_spacing` в `applyStyleSheet()` обнуляет margin-bottom и
padding-bottom только у `data-kobalt-beautify` prose. Явный `paragraph_spacing`
применяется позже и имеет приоритет. Эта опция не меняет переносы/высоту строк
напрямую. Базовый pinned FB2 CSS уже задаёт обычным p нулевые top/bottom margins;
книжный CSS и текущая геометрия могут менять фактический результат.

Для воспроизведения нужны исходный «Виртуальный свет», место/глава, шрифт и
размер, межстрочный интервал, vertical justification и paragraph spacing.
Не делать draw-only offset, не добавлять fake margin/font-size и не объявлять
исправленными footnote layout/renderer только по этому снимку.

## Запись FB2 в Nickel

Подтверждённая граница beta.8: `nickel::allowsUnmappedPosition()` разрешала
activity-only write только plain sideloaded EPUB. `ReadingBook::writeNative()`
при mapping failure FB2 поэтому выбирал LocalOnly и не доходил до native write.
Исправление добавляет raw sideloaded `.fb2` (case-insensitive) в тот же механизм.
KEPUB, store identities, сетевые URL и `.fb2.zip` туда не добавляются.

Сохраняются native transaction, compare/read-back и lifecycle guards. Существующий
Qt5 time-only writer пишет DateLastRead/ReadStatus/percent, не создавая chapter
bookmark. Точная позиция FB2 по-прежнему хранится Kobalt; это не новая поддержка
точного перехода между ридерами и не облачная синхронизация.

Старый FB2 session test содержал несовместимый с NativeSaveResult bool assertion
и не задавал FB2 identity у fake native baseline. Обе ошибки теста исправлены;
`session.reading-book` теперь включён в focused workflow, вместе с screen-refresh.
Native SDK локально отсутствует; валидация идёт в pinned CI. Устройство pending.

### Проверка исправления Nickel FB2

Runtime `5eb8a600f71b5f46794a8c469783577f2ca01eed`.
[CI №175 / 44111989597](https://github.com/Mutaracha/kobalt_ru/actions/runs/44111989597)
**PASS**: focused native, включая session.reading-book с FB2 identity и activity-only
writeback, model.screen-refresh, engine.fb2 с новым chapter/eligible Smart contract;
offline checks и Kobo Qt5 build/package/contents. Qt6 пропущен, all-variant не выполнен.
[Qt5 artifact](https://github.com/Mutaracha/kobalt_ru/actions/runs/44111989597/artifacts/14340793511).
[Native results](https://github.com/Mutaracha/kobalt_ru/actions/runs/44111989597/artifacts/14342042111).
Device-проверка исправления Nickel pending. Исходный баг Smart/normalization не
воспроизведён на книге пользователя; синтетический тест не закрывает эти пункты.
Предыдущий CI №174 остановился на старом bool assertion теста NativeSaveResult;
он исправлен, №175 полностью прошёл. Новых production rendering изменений нет.
