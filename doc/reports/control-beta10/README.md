# Контрольная original beta10 + RU

База: `1796a32559dbb2912bd565990d1c7a0ca7ac4e52` (upstream).
Источник перевода: `ca50dd358386e9fccc5122b464f74003caa51e75` (наша ветка).

## Как исключено смешивание с форком

Сборка выполняется **не из корня форка**: `bash platforms/control-beta10/prepare.sh`
экспортирует `git archive` точного upstream в игнорируемый `build/control-beta10/`
и применяет только `platforms/control-beta10/ru.patch`. Это не новая ветка/worktree.
Корневые `src/`, `assets/` и старые fork workflows не являются входами этой сборки.
Повторный запуск не перезаписывает существующий экспорт.

## Точный runtime/source diff

Полный unified diff: `platforms/control-beta10/ru.patch`.
Контрольные SHA-256 исходных и результирующих файлов: `platforms/control-beta10/manifest.json`.
Всего **14 файлов, +1942 / −37 строк**:

| Файлы | Единственное назначение |
|---|---|
| `assets/ru.strings` | Таблица 1808 ключей, соответствующих EN beta10 |
| `assets/CMakeLists.txt` | Генерация RU, проверка паритета и зависимости reconfigure |
| `platforms/strings.cmake` | Общий parser, проверка ключей/аргументов, имя входа в generated header |
| `platforms/strings-test.cmake` | Ожидание actual input name в header |
| `src/support/string_table.h` | Общий тип Entry, вынесенный из strings.h |
| `src/support/string_locale.h` | Выбор таблицы и QLocale; Russian по умолчанию |
| `src/support/strings.h` | Поиск в выбранной таблице; каноническая diagnostics entries остаётся EN |
| `src/support/percentage_text.h` | QLocale выбранного интерфейса вместо принудительного EN |
| `src/app/reader/panels/activity_view.cpp` | QLocale выбранного интерфейса вместо принудительного EN для дат/месяцев |
| `platforms/desktop/main.cpp`, `platforms/android/main.cpp` | Установка Russian при запуске |
| `platforms/kobo/nickel-qt5/reader_controller.cpp`, `platforms/kobo/nickel-qt6/reader_controller.cpp` | Установка Russian при initialize |
| `platforms/kobo/common/reader_coordinator.cpp` | Russian перед читательской сессией |

Русский фиксирован для контрольной сборки: host locale/env/старые language markers
не меняют его. UI переключателя, включая вкладку «Отладка», не добавлен.

Никаких FB2/comics, sleep/refresh/Libra2, About/fork row, геометрии/иконок,
новых шрифтов, Russian_EnUS.pattern, изменений engine/storage не перенесено.
Все остальные upstream файлы сохраняются побайтно.

## Неполнота перевода — не скрывать

Повторно использованы **1604** существующих RU значения без изменения текста.
Для **194** новых ключей и **10** строк с несовместимыми `%N` сохранён upstream EN,
а не внесён несогласованный новый перевод. Точный список и исходные значения:
`translation-gaps.json`. EN assets не менялись. Набор ключей и аргументов совпадает.
Это RU-default контроль, **не полностью русифицированная beta10**.

CLDR plurals из форка пока не портированы: это потребовало бы дополнительных
ключей/изменения вызывающих мест и согласования строк. Singular/plural logic
остаётся upstream, поэтому некоторые русские склонения будут неверными.
Не путать этот минимальный контроль с языковой приёмкой полного форка.

## Шрифты/отрисовка — отдельно, не применено

- Штатные IBM Plex Sans/Mono содержат кириллицу; замена не предлагается.
- Instrument Serif не содержит кириллицу. Нужна device-проверка fallback;
  потенциальная замена/добавление шрифта требует отдельного согласования.
- Fork stacked-icon Button ink-box correction **не перенесён**.
  Возможное обрезание русских подписей проверяется отдельно; коррекция
  геометрии требует согласования. До этого ни шрифты, ни drawing не меняются.

## Проверки и сборка

Локально PASS: CMake strings generation/rebuild self-tests; паритет 1808 EN/RU
ключей и всех `%N`; валидация production string references; bash syntax.
CMake установлен в изолированное временное окружение, не является изменением проекта.
Локально нет Docker/Qt SDK: компиляция, линковка, native Qt tests и device проверка
не выполнялись. Не считать наличие patch/extracted sources готовым бинарником.

Отдельный workflow `.github/workflows/control-beta10.yml` собирает **только экспорт**
с оригинальным pinned Qt5 SDK, без fork name. Задаёт provenance `1796a32` + modified ON;
проверяет оригинальный `check-package` и публикует
`Kobalt-beta10-original-ru-qt5-control` вместе с patch/manifest/gaps.
Это orchestration в нашей ветке, не дополнительный runtime diff.
CI **PASS**: [run 45212880701](https://github.com/Mutaracha/kobalt_ru/actions/runs/45212880701),
source/orchestration `8b1111a35c76d1b42384f659c3abb926e7063caa`.
Успешны Qt5 compile/link, оригинальный `check-package` и artifact upload.
[Artifact 14838120646](https://github.com/Mutaracha/kobalt_ru/actions/runs/45212880701/artifacts/14838120646):
`Kobalt-beta10-original-ru-qt5-control`, 44 188 806 байт (Actions archive).
Внутри — установочный `Kobalt-qt5.zip`, patch, manifest и translation gaps.
Скачивание artifact в Arena заблокировано доступом к blob host (EOF);
локальное распаковывание/проверка содержимого архива не заявляются.
Native Qt test suite и физическое устройство не запускались.

Локальная повторная проверка после подготовки:

```sh
cmake -DROOT="$PWD/build/control-beta10" -P platforms/control-beta10/validate.cmake
cmake -DTEST_TMP_ROOT="$PWD/build/control-string-tests" -P build/control-beta10/platforms/strings-test.cmake
```

Установка использует оригинальный upstream профиль/пути. Отдельный профиль не
добавлен, чтобы не менять контроль. Перед устройством нужен backup профиля;
не считать downgrade между версиями безопасным без проверки совместимости.
