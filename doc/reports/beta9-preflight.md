# Beta.9: предварительная проверка переноса

## Статус и границы

**Аудит исходников, не выполненная миграция.** Runtime форка не менялся;
merge/rebase/cherry-pick не выполнялись. Native/Qt5/device проверки beta.9 с нашими
правками ещё не запускались. Последний проверенный runtime форка: `23e2d023e1ed732878a21cd2148baaddc24d50e6`,
[CI 44574307848](https://github.com/Mutaracha/kobalt_ru/actions/runs/44574307848).
Иконка с заливкой и перенос presentation полосы ещё не имеют отдельной device-приёмки.

Точные деревья сравнения:

- upstream beta.8: `679175046d8c806ea3b404525180fbc29a26fcd2`;
- upstream `v1.0-beta.9`: `be47258850ffdc3295257475afdf8b8f6cb53176`;
- fork baseline: `caf9b177cb4fe09d718b304880658f7fe595dbd5` (docs поверх указанного runtime).
- [Release](https://github.com/nicoverbruggen/kobalt/releases/tag/v1.0-beta.9),
  [upstream comparison](https://github.com/nicoverbruggen/kobalt/compare/v1.0-beta.8...v1.0-beta.9).

Проверены полные деревья, а не только GitHub compare API: API вернул первые 300
files, поэтому его список не использован как полный. 189 upstream commits.
С учётом Git rename detection: 676 entries — 346 modified, 270 added, 37 deleted,
23 renamed. При сравнении по буквальным путям: 699 changed paths (293 added,
60 deleted, 346 modified). Большой объём включает импорт библиотеки движка.

193 пути отличаются у форка от beta.8. Пересечение по буквальным путям — 74;
это не число конфликтов и не учитывает смысловые пересечения через новые файлы.
[Полная карта](beta9-port-map.tsv) сопоставляет все 193 пути форка. Для трёх
сильно переписанных файлов engine_document.h, reader_source.cpp/.h путь adapter
сопоставлен явно; остальные обнаруженные переименования взяты из git diff -M.
Статическая проверка `git merge-file -p fork base beta9` в отдельных файлах:

| Категория | Путей |
|---|---:|
| Fork-only additions, отсутствующие в upstream | 100 |
| Изменённые в обеих ветках, текстовое слияние без маркеров | 44 |
| Текстовые конфликты | 29 |
| Содержимое upstream не изменилось относительно beta.8 | 20 |

Это не пробный merge репозитория. «Text-clean» не означает совместимость API,
поведения, include paths или сборки. Fork-only файлы тоже требуют адаптации.

## Главные риски и решение для переноса

| Область | Что установлено в beta.9 | Как переносить / что не делать |
|---|---|---|
| Движок / сборка | CREngine импортирован в `src/engine/blaze/crengine`, adapter — в `src/engine/blaze/adapter`; extensions выделены отдельно. Прежний внешний crengine pin удалён из manifest; сборка через dependencies/local-engine.cmake. База Blaze — тот же KOReader commit 4ce1aed, но библиотека уже изменена upstream. | Принять upstream Blaze и его сборку целиком. Не возвращать прежний manifest/patch set или старый каталог crengine поверх нового. Наши format-specific модули адаптировать к adapter/extensions по слоям. |
| Hybrid / позиции | Новый ReaderSource ведёт chapter layouts, фоновые whole-book builds, estimates, лимит документов и idle cleanup. ReaderSession различает bookPage/bookPages/estimated и локальные страницы. | Перепроверить каждый fork-вызов page/pages/bookmark. Не считать локальную страницу номером всей книги. Не дублировать новый scheduler и не выполнять полный layout на GUI thread. |
| FB2 | Наличие библиотечного fb2.css/FB2 parser не равно поддержке публикации. Adapter open() требует архив, openSource проверяет EPUB mimetype. | Сохранить нашу отдельную публикацию/ресурсы и FB2 dispatch. Не проталкивать FB2 через EPUB-only spine/container. Решение о применимости Hybrid к FB2 ещё требует отдельного изучения; не обещать ускорение FB2. Не отключать Hybrid глобально ради FB2. |
| FB2 preview / CSS / notes | Наши semantic preview, offset/language binding, note titles и selectors зависят от прежнего DOM/lifecycle. Upstream layout/shaping уже перенесены и изменены. | Перенести root-cause fixes как format-specific hooks; снова проверить title/inline/poem/cite/subtitle, Cyrillic/non-BMP offsets и source-state immutability. Сохранить принятые сноски/зазоры. EPUB оставить новой upstream реализации. |
| Comics | Наши ColumnView/EngineDocument extensions затрагивают код, существенно изменённый для chapter layouts и async whole-book adoption. | Отдельный этап: эффективные margins/chrome, artwork readiness, landscape spreads, wide и center image, rotation/hits. Сохранить настройки и preview без восстановления старых upstream функций целиком. |
| Lazy UI / полоса | Settings теперь может быть null. Upstream Navigation::update уже вычисляет `settingsOpen = state.settings && ...`. Наш updateForkPageStrip разыменовывает state.settings без guard; appearance также надо проверить на null. | Использовать новую upstream lifecycle модель. Адаптировать короткие hooks, сохранить factory/formatting/visibility в fork_navigation. Не возвращать eager Settings. Добавить тесты первого меню до построения панелей, скрытия, reopen/rotation/theme. |
| Полоса / estimates | Наш formatter уже берёт Values/BookTimeLeft, но номер страницы пока идёт через session->page/pages. В Hybrid upstream имеет отдельные whole-book API. | Передавать book-wide поля и согласованные upstream estimates. Проверить Calculating/estimated/exact transitions без ложных точных чисел и без периодического лишнего repaint. Новые строки согласовать, не сочинять UI молча. |
| Sleep / standby | Новый custom lock screen, wallpaper, отдельные sleep/power-off настройки и shared checkbox; изменения editor/caches/privacy/lifecycle. | Взять новый upstream механизм, затем контекстно вернуть только наши действующие отличия. Не переносить старые sleep_designs/настройки экрана целиком. Повторно проверить RU fitting/ribbon, master light и standby. |
| Kobo lifecycle / refresh | Изменены подготовка книги/окна, startup/teardown, асинхронные записи/close-without-waiting, первый full refresh, standby clock, NickelFix hooks и Wi-Fi. | Не накрывать старым ReaderController/coordinator. Libra2 landscape correction сверить с актуальной positional/held моделью. Первый refresh beta.9 — отдельное upstream решение, не повод добавлять постоянные flashes. |
| Fonts / RU | В beta.9 новые lazy fonts/hyphenation, cache и shaping исправления; в assets/en.strings много новых UI ключей. | Сохранить новую upstream реализацию и наши locale/plural/font-family policies. Сверить все EN/RU ключи, не копировать старый EN словарь. Новые/изменённые переводы вынести на согласование. |
| Activity / annotations / storage | Новая Activity, highlight notes/quick annotations, progress precision; обновлены записи через profile writer и сохранение по границам суток. | Принимать целиком upstream; не портировать наши изменения как копии старых общих экранов. Перед устройством backup .adds/kobalt; проверить сохранение unknown fork keys и восстановление позиций/заметок. Не считать старые cache совместимыми. |
| Tests / CI | Новый CI разделяет native build/tests, добавляет opt-in sanitizers и Kobo adapter tests. | Наш Qt5-first workflow адаптировать к новым targets/paths/Blaze, а не заменить upstream CI старым файлом. Держать обе группы регрессий и реальные fixtures. |

Prepared SDK digests Qt5/Qt6/Android/Linux совпадают с текущим форком
(`platforms/sdk-image.sh`). В manifest остальных внешних библиотек нет изменений
относительно beta.8; это не означает неизменность dependency build/cache policy.

## Предварительная классификация наших изменений

- **Сохранить, адаптируя:** native FB2/resources/recognition/preview/notes,
  comics и aspect/landscape поведение, top/bottom progress, RU/plurals/ExtraLocales,
  скрываемые dark/comics кнопки и filled-state glyph, Libra2 landscape correction,
  fork About + UTC (stock About не менять), пользовательские названия и общие стили,
  полосу книги/страниц и её lifecycle.
- **Upstream теперь владелец:** Hybrid/background pagination, новый lazy UI,
  Activity/annotation notes, custom sleep editor, opening/close/profile writer fixes.
  Наши прошлые остановленные opening/lazy Settings задачи не возобновляются как
  собственные оптимизации; используется реализация beta.9.
- **Не объявлять автоматически исправленным:** старое зависание после завершённого
  frame load. Release notes упоминают другой startup freeze и close hang. Пока нет
  воспроизведения/stack, это не доказательство исправления нашей старой ошибки.
- **Не переносить механически:** старые EngineDocument/ReaderSource/ReaderController,
  snapshot cache policy, общие Settings/sleep screens, dependency build и CI.

## Рекомендуемая последовательность (ещё не выполнена)

1. Зафиксировать эту карту, владельцев и контракты. Сохранить текущий проверенный
   runtime как точку сравнения; не создавать другие branches/tags без разрешения.
2. Перенести базу beta.9 и сборку Blaze, затем узкие публикационные/FB2 границы.
   Сначала compile/API correctness и upstream EPUB tests, не UI-полировка.
3. Перенести FB2 rendering/preview/notes и comics по слоям, сохраняя новые upstream
   scheduler/source/position contracts. Tests на исходной книге обязательны.
4. Интегрировать UI/RU/progress/sleep/Kobo hooks контекстно; исключить nullable-panel
   обращения и локальные/whole-book page-count смешения.
5. Native regression matrix, строки/parity/source checks, Qt5 package + содержимое.
   Sanitizers — отдельная native проверка. Desktop/Android не новые product blockers,
   но общие API нельзя оставлять заведомо сломанными.
6. Устройство после backup: FB2/EPUB reopen/cache/positions; menu до lazy UI;
   типографика/заголовки; сноски; comics portrait/landscape/spreads; progress/estimates;
   sleep/wake/clock/light; закрытие в Nickel и сохранение annotations/bookmarks.

## Что ещё не доказано

Нет сборки объединённого дерева, sanitizer результата, runtime/device validation,
полного аудита всех 189 коммитов по строкам и решения по Hybrid для FB2.
Синтаксически чистые пересечения требуют ручной проверки. Тестовые 29 конфликтов
не являются исчерпывающим списком будущих правок. Это первый preflight, не готовность
к установке и не разрешение на автоматическое разрешение конфликтов.
