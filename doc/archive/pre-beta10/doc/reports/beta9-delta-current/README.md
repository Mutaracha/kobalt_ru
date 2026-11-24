> АРХИВ: исторический контекст, не текущий план и не инструкция установки.
> Старые pending/next не возобновлять. Текущий источник — FB2_FORK_PLAN.md в корне.

# beta9 delta — финальные shared consumers и удаление fork progress

Pinned upstream `be47258850ffdc3295257475afdf8b8f6cb53176` → runtime `80ee8d791c32d51c80cff194fd21418c3a571c66`.
[Исторический вход beta9](../beta9-delta/README.md) не изменён.

[Все tracked пути](files.tsv), [счётчики](summary.json), [stat](changes.stat).
**344 nonidentical**: 180 different,
164 fork-only, 0 upstream-only;
1203 identical. 149 записей — документация/evidence;
195 — runtime/build/assets/tests. Числа не являются количеством багов.
Проверено точное равенство множества TSV путей `git diff --no-renames --name-only -z`.
`REVIEW` — намеренно консервативная подсказка для следующего обновления,
а не утверждение, что каждый отличный файл должен быть заменён автоматически.

## Закрытые реализации последней группы

- Reader `start/activate/releaseQuiet`, отложенные host observers/clock/preferences,
  sleep-image preparation и lazy Settings/Appearance/FieldChooser/Acknowledgments —
  pinned upstream. Coordinator целиком upstream с единственным RU-locale hook;
  Qt5/Qt6 controllers возвращают upstream preload и asset availability gates.
  Ранний format gate дополнен существующими FB2 форматами, не вторым opener.
  Прогрев последней книги остаётся upstream EPUB-only: FB2 partial-layout не включён.
- Header/footer model/panel — byte-identical pinned upstream: debug fields,
  Book status timer/turn/layout timings, decimal percentages, chooser filtering.
  Settings/Debug wiring, lazy confirmations, cache actions и percentage draft/apply
  восстановлены вместе с API и consumers, не только строками.
- Fork progress renderer **удалён**, как и chapter fractions/ticks, `progressTop`,
  `progress_top`, position property, Top/Bottom captions и mirrored-tick test.
  Geometry/paintFooter и model/tests — штатные beta9. Остаётся upstream on/off
  нижней полосы. Comics скрывает chrome без изменения сохранённых значений;
  draft preview использует актуальную геометрию, не старый paint cache.
- Shared RadioList hidden rows и icon fallback, font coverage/optional-book-face
  registration, logo-only splash/outlined wordmark, library preference merge,
  low-density text-preset matching, native unmapped-progress policy, sanitizer/
  source archive helpers возвращены из pinned upstream. Никакой второй KOSync
  policy, font loader или save pipeline не добавлено.
- Chapter contents сохраняет только CLDR summary; размещение номера/страницы
  upstream. Dictionary My Words использует upstream CheckCellButton. Search
  chapter caption переиспользует утверждённый `%1: %2` key.

## Сохранённые границы — не недостающие shared consumers

- FB2 source/resources/notes/metadata и per-book comics pairing/viewport/frameImage.
- RU/CLDR, locale entry points, утверждённые captions и существующий string reuse.
- About fork/UTC, IBM Plex Sans UI/Instrument Serif brand, toolbar dark/comics,
  общая checkbox presentation, annotation palette labels/section headers,
  sleep ribbon/newline/text fitting и независимый invert-page-swipes.
- Центрированные session minutes сохранены; отменённая page-count/time-to-end
  menu strip **не возвращена**. Quiet/frontlight diagnostics не возвращены.
- Принятые Libra2 mapping и Kobo/BOOX waveform остаются исключениями; не
  переносятся новые dark-mode/cleaning waveform изменения под видом alignment.
- EPUB Hybrid algorithms (slices/background layout/adoption/fallback) не менялись;
  FB2 Hybrid не включён. Freeze deferred до нового воспроизводимого примера.

## Проверки

**Финальная [CI45060819419 / build289](https://github.com/Mutaracha/kobalt_ru/actions/runs/45060819419): PASS.**
Runtime `80ee8d79`: focused native regression suite PASS и Qt5 package build,
contents validation и offline release checks PASS. Qt6 пропущен по политике workflow;
Android и physical-device acceptance не выполнены.
Отчёт фиксирует runtime до следующего documentation-only checkpoint: сами эти
обновлённые отчёты не входят в сравниваемый Git tree.

Восстановлены upstream quiet-window, background panel preparation, deferred tiles,
choice-resize/teardown, BookStatus, count-without-turn, field-sheet hide,
Debug mode, percentage format, logo splash, ReadingBook и library-merge regressions.
Fork appearanceChromeDraft/comicsQuickToggle/darkThemeToggle явно зарегистрированы
в CTest/area mapping и focused workflow; удалённый progressStripMirrors не запускается.

Промежуточный79851d78 / CI45057846487 FAIL: не перенесены зависимости
`RadioList::setLeftOut` и `paintBrandSplash`; они восстановлены целиком до этого
checkpoint. Ошибка не маскируется снятием checks или возвратом fork renderer.
bb86ce8b / CI45058361740: Qt5 PASS, native compile FAIL из-за отсутствующего
`KOBALT_TEST_FIXTURES` для целого upstream ReadingBook test; восстановлены fixture
definitions в41c2c934, тесты не урезаны.622e6563 повторяет этот compiler failure
до исправления.443c05ae дополнительно возвращает whole upstream settingsRendering,
settingsStandby и savedWords сценарии под их восстановленные implementations.
443c05ae / CI45059361797 / build288: Qt5 PASS; native 514/515 PASS,
единственный FAIL — model.style-sample-cache: transient `whole_book_failed`
влиял на ключ. В80ee8d79 он исключён из sample settings вместе с остальными
frame diagnostics; исходный тест сохранён без ослабления.
Последующие superseded queued runs могут быть CANCELLED, не SUCCESS.
Native workflow — focused regression suite, не весь upstream desktop/Android matrix.

Воспроизведение:
```sh
python platforms/ci/upstream_delta.py --upstream be47258850ffdc3295257475afdf8b8f6cb53176 \
  --fork 80ee8d791c32d51c80cff194fd21418c3a571c66 --output /tmp/beta9-delta
```
