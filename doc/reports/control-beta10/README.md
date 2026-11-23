# Контрольная original beta10 + RU — пакет для скриншотов

База: `1796a32559dbb2912bd565990d1c7a0ca7ac4e52` (upstream).
Источник прежнего перевода: `ca50dd358386e9fccc5122b464f74003caa51e75`.
Все 204 предложения перевода приняты пользователем и применены.
Шрифты получены из `9d20cd4729c4fa5fbd5370ef7c1ec39519d74a04`;
другой код соседней ветки не переносился.

## Изоляция и точный diff

`bash platforms/control-beta10/prepare.sh` экспортирует точный upstream через
`git archive` в игнорируемый `build/control-beta10/` и применяет только
`platforms/control-beta10/ru.patch` (включая binary font assets).
Повторный запуск не перезаписывает экспорт. Это не branch/worktree.
Корневые src/assets форка и старые fork workflows не входят в сборку.

Полный точный unified/binary diff: `platforms/control-beta10/ru.patch`.
Исчерпывающий список **27** отличающихся файлов с исходными/новыми SHA-256:
`platforms/control-beta10/manifest.json`. Остальные upstream файлы побайтно прежние.

| Файлы | Назначение |
|---|---|
| `assets/ru.strings` | 1808 ключей: существующий RU + 204 согласованных beta10 значения; common.percent = `%1` |
| `assets/CMakeLists.txt`, `platforms/strings.cmake`, `platforms/strings-test.cmake` | RU generation, паритет, input-derived generated header |
| `src/support/string_table.h`, `string_locale.h`, `strings.h` | Locale/table API, Russian default; diagnostics entries остаётся EN |
| `src/support/percentage_text.h`, `src/app/reader/panels/activity_view.cpp` | QLocale выбранного интерфейса вместо forced EN |
| `platforms/desktop/main.cpp`, `platforms/android/main.cpp`, `platforms/kobo/nickel-qt5/reader_controller.cpp`, `platforms/kobo/nickel-qt6/reader_controller.cpp`, `platforms/kobo/common/reader_coordinator.cpp` | RU startup/session wiring |
| `src/ui/controls/buttons.cpp` | Прежний Cyrillic ink-box stacked label из ca50dd3, отдельно разрешён пользователем |
| `assets/fonts/MTCORSVA.TTF`, `Caveat-Regular.ttf`, `Caveat-Bold.ttf` | Точные пользовательские font bytes |
| `assets/manifest.json` | Размеры и SHA-256 трёх новых шрифтов |
| `assets/notices/Caveat-OFL`, `Monotype-Corsiva`, `assets/acknowledgments.json` | Лицензии/атрибуция шрифтов |
| `src/ui/style/fonts.cpp`, `private_text.cpp` | RU serif = Monotype Corsiva в Qt и private renderer; загрузка/pinning и warm-up согласованы |
| `src/app/reader/settings/interface_style.h` | RU playful preset: Caveat вместо Kalam |
| `src/assets/bundled_fonts.cpp`, `font_catalog.cpp` | Caveat Regular/Bold доступен в RU интерфейсе и UI-разделе font picker |

Никаких FB2/comics, sleep/refresh/Libra2, fork About row, иных icon/geometry,
engine/storage или Russian_EnUS.pattern не перенесено. Переключателя языка нет,
host locale/env/markers не меняют фиксированный RU этого контрольного пакета.

## Шрифты и ограничения

Только RU: MTCORSVA вместо Instrument Serif; Caveat вместо Kalam в штатной
предустановке «Игровая». Явный пользовательский выбор шрифта не перезаписывается.
Чтобы увидеть Caveat, выберите «Игровая»; классический UI не превращается в Caveat.
English сохраняет Instrument Serif/Kalam. Переключение языка на лету не добавлено.

В файлах проверены family/style имена и полный русский алфавит с Ё/Й.
MTCORSVA содержит семейство **Monotype Corsiva**, Regular; отдельного Italic/Bold
нет, используется штатный путь синтеза при запросе. В private renderer точная
семья проверяется **до** общего contains("Mono"), иначе выбрался бы IBM Plex Mono.
Caveat Regular/Bold имеют одно семейство Caveat; Medium/SemiBold не требуются
и оставлены только как исходные пользовательские вложения, не runtime assets.

Caveat — OFL 1.1, полный notice приложен. Monotype Corsiva — proprietary:
встроенный notice сохранён, не является OFL/AGPL. Пользователь предоставил файл
для этого теста; это не подтверждает права на дальнейшее публичное распространение.
Перед самостоятельным релизом нужно отдельно подтвердить подходящую лицензию.

Все ключи переведены, но upstream singular/plural logic сохранена: полного CLDR
переноса нет, некоторые склонения требуют отдельной работы. UI-текст принят,
визуальная приёмка и размещение длинных подписей — по будущим скриншотам.
[Принятые EN → RU значения](translation-proposals.md), машинный список — JSON рядом.
`translation-gaps.json` — исторический список пропусков первого пакета, не текущий статус.

## Проверки и пакет

Локально PASS: strings generation/rebuild (Ninja), 1808 EN/RU key/slot parity,
production references, common.percent contract, все runtime manifest SHA-256,
применимость patch к pristine beta10. Отдельно проверены font names/glyph coverage.
Прежний отрицательный `%1%` guard подтверждён. Компиляции локально нет (нет Docker/Qt).
C/C++ форматировался clang-format без переноса его посторонних изменений `{}`.

Workflow `.github/workflows/control-beta10.yml` собирает только экспорт в оригинальном
pinned Qt5 SDK, проверяет `check-package`, публикует
`Kobalt-beta10-original-ru-qt5-control`. Результат текущего CI фиксируется отдельно.
Provenance пакета: upstream `1796a32` + modified ON; точный overlay — commit workflow
и patch/manifest в artifact. Native полная suite и устройство не запускались.

Предыдущие пакеты: CI45212880701 PASS (первый частичный RU), CI45221686704 PASS
(ink-box/percent fix). Они **не проверяют** новые fonts/translations текущего пакета.

Установка использует оригинальные upstream paths/profile. Перед установкой
сохранить backup профиля; не считать downgrade безопасным без проверки совместимости.
Для скриншотов нужны: «Экран»/батарея/«Отладка», заголовки/загрузка (Corsiva),
предустановка «Игровая» (Caveat), длинные новые русские строки.
