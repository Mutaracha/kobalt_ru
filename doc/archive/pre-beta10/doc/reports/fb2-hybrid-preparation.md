> АРХИВ: исторический контекст, не текущий план и не инструкция установки.
> Старые pending/next не возобновлять. Текущий источник — FB2_FORK_PLAN.md в корне.

# FB2: подготовка границы Hybrid без изменения EPUB

Статус: source analysis; partial FB2 layout не реализован и не включён.
Baseline: beta.9 adapter форка `9633d3fd`; EPUB Hybrid остаётся upstream-owned.

## Что уже обеспечивает безопасную границу

- `publication/fb2.h::matchesHead`: raw-byte probe до 64 KiB, без перекодирования
  windows-1251; это recognition, не полная XML validation.
- `publication/fb2_resources.cpp::transferFile`: whole-source resource transfer
  с SourceLease/revocation, проверкой размера, block bounds и deadline.
- `reader_source.cpp::open`: raw FB2 до archive validation; source хранит isFb2.
  При configure FB2 обнуляет inherited spine bounds и slicePages. XML остаётся
  исходным потоком, искусственного EPUB/spine нет.
- `reader_fb2.*`: форматные семантика, язык и numeric note titles; политики
  сносок принадлежат canonical inpage_footnotes, а не второму FB2 механизму.
- `tests/model/fb2.cpp`: engine boundary проверяет exact book/local pages,
  отсутствие partial estimates/full background build, reflow/source anchor и
  lifetime semantic preview после удаления source. Применяет реальный body passage.
- `tests/model/fb2_session.cpp`: Accurate/Hybrid selection для FB2 по-прежнему
  даёт whole-book frames, turns/reflow/exact mapping; не доказывает partial FB2.

Эти invariants надо сохранить при cleanup. Выбор пользователем Hybrid сам по
себе не означает, что FB2 уже рендерится секциями.

## Не переносить EPUB-модель на FB2 механически

FB2 — единый XML с вложенными sections, несколькими body, inline binaries и
ссылками по ID. Номер section не равен EPUB spine item. Разрезание XML или
выдуманный DocFragment могут изменить XPointer, scope IDs, title hierarchy,
стили/язык и доступность notes/comments. Не добавлять универсальный hook framework
или EPUB algorithm exceptions, пока форматный контракт не доказан.

## Следующий исследовательский результат (до production implementation)

| Контракт | Что установить | Приёмочный пример |
|---|---|---|
| Reading units | Детерминированная граница section и вложенности; основной body против notes/comments | Nested sections, несколько main body, пустая section, длинная section |
| Source anchors | Стабильный исходный XPointer независимо от partial/full представления; отдельные spaces для local/book pages | round-trip bookmarks/highlights после reflow, повторного открытия и переключения режима |
| Dependencies | ID index и cross-section notes/images без удаления исходного source | forward/backward note refs, shared note, cyclic/missing ID, binary после body |
| Layout context | Что необходимо наследовать от предков section | язык, alignment, poem/epigraph, title, числовой note title и inline vocabulary |
| Progress | Когда exact и когда estimated; завершение только в конце книги | Последняя local page не-final unit не помечает книгу finished |
| Resources/lifetime | Shared source lease/revocation и независимый preview lifetime | close/USB/cancellation во время чтения/preview/background work |
| Cache | Source/layout/format identity и восстановление без layout-transient state | stale source, смена шрифта/viewport, повторный open; пользовательские данные не удаляются |
| Safe fallback | Консервативные признаки отказа от partial layout до публикации неполного кадра | cross-unit bottom notes, неразрешённые зависимости, unsupported structure |

Сначала составить маленький переносимый corpus с expected anchors/dependencies;
оригинальные пользовательские книги не копировать в fixtures автоматически.
Текущие whole-book tests сохранять отдельной группой, не переписывать их assertions
так, чтобы ошибочно объявить partial поддержку. Для будущего режима потребуется
отдельная группа. Любая необходимость менять общие EPUB slicing/adoption/fallback
останавливает реализацию до отдельного согласования.

## Параллельность

Corpus/контрактный анализ независимы от UI/presets/build cleanup. Production
интеграция зависит от стабилизации общих Engine/ReaderSession и актуальной карты
дельты. Сейчас никаких новых runtime API, настроек или cache versions не вводится.
