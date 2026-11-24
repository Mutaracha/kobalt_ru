# 050 — последний функциональный патч: реализация и проверка

Статус: **done / ready по CI; аппаратная проверка не выполнена**.
Вход: pristine beta10 `1796a32559dbb2912bd565990d1c7a0ca7ac4e52`
плюс ready 010–040. Retained reference:
`ca50dd358386e9fccc5122b464f74003caa51e75`.
Результат проверен на `489ee40878ed27491255e246f0802f95705cb178`.
Сравнивались Git trees, не грязный корень checkout.

050 завершает функциональный объём текущего плана. 060 остаётся отдельным
этапом итоговой упаковки/provenance/CI, а не местом для отложенных функций.
Патчи применяются одной упорядоченной серией; автономность не требуется.

## Выполненный объём

| Группа | Перенос / сохранение |
| --- | --- |
| Язык | English/Русский/Automatic первой стандартной action row Debug; settings-owned modal, async marker writes, отмена без записи, без live retranslation и без входа в About |
| Locale resolution | Hidden markers, legacy fallback, ENV, Kobo config, system locale; desktop/Android startup и Kobo session boundary; English fallback вместо forced-Russian control policy |
| Strings / CLDR | Canonical English diagnostics table, RU fallback/plurals, формы library/chapter counts и Activity book/hour units; новые captions согласованы EN/RU; уже перенесённые dates/grouped digits из 010 сохранены |
| Activity | Standalone month captions и units добавлены выборочно; новая beta10 Activity и её поведение не заменены старым файлом |
| About | Fork distribution/build row, UTC metadata на desktop/Kobo/Android; generic empty name скрывает строку; desktop rebuild-time header сохраняет fork name |
| Аннотации | Color/Style/Actions section captions, видимые подписи swatches/styles, Cancel/Dictionary copy; новый beta10 draft/actions pipeline сохранён |
| Сон | Две строки remaining в ribbon в языковом порядке; fallback для старой/неразбираемой строки; фиксированное место battery; bounded fitting/elision обоих языков поверх RU height fitting из 010 |
| Пресеты | Локализация известных shipped names только при отображении; пользовательские имена, неизвестные IDs, сохранённые значения/типографика не переписываются |
| Return title | Согласованные measurement/painting runs: Sans вокруг serif brand word; sizeHint использует те же runs; upstream arrow placement сохранён |

Runtime sources, translations, CMake registrations и тесты включены в сам 050.
Новые support sources desktop регистрируются явно; Kobo/Android используют свои
source globs. Не требуется функциональный follow-up в 060.

## Что не дублировалось и не возвращалось

- RU tables, шрифты и locale ink/fitting из 010 сохранены, а не заменены wholesale.
- Dark/comics actions, vector geometry и Fluent MIT acknowledgment уже в 040.
  Не добавлялся неиспользуемый SVG asset второй раз.
- Checkbox helper/tick из 032 не дублировались. Language modal использует
  штатные beta10 RadioList dividers; старый RadioList с потерей новых
  two-column/layout rules не переносился ради одного footer rule.
- `Russian_EnUS.pattern`, его fork-only manifest/list/notice entries и старый
  Libra2/NickelFix mapping не возвращались. `Russian.pattern` и переводной
  ru-en dictionary сохранены; новые upstream assets/notice не откатывались.
- FB2.ZIP, собственный Hybrid, opening acceleration и chapter-progress
  labels/ticks/setting/top variant не добавлялись.
- Upstream reverse_page_swipes, физические кнопки, Activity, chapter-start
  spread, dark/i.MX animation и BOOX behavior не менялись этим патчем.
- Старые fork diff удаляли некоторые upstream tests: эти удаления не перенесены.
  В частности, PPI/rounding loop в text_presets.cpp сохранён полностью.
- Root README пользователя не редактировался. Legacy device-workflow остаётся
  отключённым; successful CI не объявляется аппаратной приёмкой.

Подробная классификация исходного path-level остатка:
[presentation-residual.tsv](presentation-residual.tsv). Это карта provenance,
не требование добиться побайтового совпадения нового beta10 со старым форком.

## Проверки

[CI 45375027889](https://github.com/Mutaracha/kobalt_ru/actions/runs/45375027889)
на `489ee40878ed27491255e246f0802f95705cb178`: **все три jobs PASS**.

- Clean ordered application, SHA validation, 10 Python series tests и inventory.
- EN/RU key/placeholder parity, production string references и asset hashes.
- Native `model.strings-locale`, `component.debug-language-ui`,
  `component.fork-presentation`, `model.text-presets`, `component.ru-control-ui`,
  предыдущие FB2/cache/comics/reader-controls/columns/chapter-parity suites.
- Presentation test проходит EN/RU, 212/300 PPI и light/dark: наличие/границы
  swatch label, ribbon rendering/line order/legacy fallback, display-only
  preset names, About fork-row presence и отсутствие language entry в About.
- Qt5 build/check-package и qemu refresh PASS. Metadata включена через
  `KOBALT_FORK_NAME=kobalt_ru`; это не переименование приложения/системного hook.

Первые попытки выявили missing area labels новых тестов и отсутствующую пару
`reader.link_dictionary` в EN/RU catalog. Исправлены регистрация и строки,
не удалены tests/validation. Первый языковой срез отдельно прошёл
CI45372368929 на `b9713968`; итоговый прогон выше включает весь 050.

Артефакт **Beta10-series-prefix-qt5**, ID `14910054747`, 44,578,677 bytes,
содержит 010–050 и provenance applied-prefix.json / source-commit.txt / SHA256SUMS.
Это не старый `Kobalt-qt5-device` и ещё не финальная упаковка 060.
Документационное принятие статуса ready не создаёт нового проверенного binary.

## Невыполненные проверки

На физическом устройстве остаются: запуск пакета, Debug language
cancel/save/reopen/restart, normal/Zen/hidden-feature toolbar, длинные EN/RU
captions/annotations, сон/пробуждение с ribbon/battery, пользовательские presets.
Предыдущее однократное touch-зависание не имеет установленной причины и не
объявляется исправленным. Qt6 firmware/Android build/runtime не проверены этим
Qt5/native workflow. Дальше — 060 packaging/provenance/full-stack CI.
