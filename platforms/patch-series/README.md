# Одна упорядоченная серия патчей форка

**Все патчи применяются вместе, строго по порядку.** Независимость между ними,
варианты сборки и выборочные пользовательские профили не являются целью.
Последующий патч вправе изменять те же файлы и использовать код предыдущего.
Деление нужно для review, адаптации и удаления заменённых upstream изменений.

## Базы

- Целевой upstream beta10: `1796a32559dbb2912bd565990d1c7a0ca7ac4e52`.
- Источник оставшейся дельты форка: `ca50dd358386e9fccc5122b464f74003caa51e75`
  (runtime до переносов; следующие upload/docs/tooling не добавляют функции).
- Старый upstream для выделения нашей дельты: `be47258850ffdc3295257475afdf8b8f6cb53176`.
- Первый патч: RU-overlay из `8bd576f` соседней ветки, с выравниванием строк
  из `e9ca3783` текущей ветки. Оригинальный коммит соседней ветки не меняется.

## Порядок

| № | Содержание | Состояние |
|---|---|---|
| 010 | RU, уже включённые шрифты/локальное отображение/тест RU/EN | Патч существует, чистое применение проверено |
| 020 | FB2 publication: распознавание, inspection, fingerprint | Native + Qt5 CI PASS |
| 021 | FB2 raw resources: stream/lease/cancellation | Native + Qt5 CI PASS |
| 022 | FB2 engine: raw/resource input, CSS/семантика, preview, cache | Native + Qt5/package CI PASS; устройство ещё не принято |
| 023 | Kobo raw-FB2 routing Qt5/Qt6, миграция настройки примечаний | Native + Qt5/package CI PASS; устройство ещё не принято |
| 030 | Комиксы: effective geometry, OPF center metadata, ColumnView spread geometry | Native + Qt5/package/qemu refresh CI PASS |
| 031 | Комиксы: EngineDocument/source/render/cache/preview и synthetic fixtures | Native + Qt5 CI PASS |
| 032 | Комиксы: appearance/session/UI и graphics refresh | Native/UI + Qt5/package/qemu refresh CI PASS |
| 040 | Согласованные reader toolbar/навигация/presentation; только отсутствующее upstream | План |
| 050 | Оставшиеся согласованные About/brand/UI/annotations/locale hooks | План; не дублировать 010 |
| 060 | Итоговые CI/package/provenance для полной серии | План |

Внутри каждого функционального патча находятся его build/source/test registration
и тесты; нельзя откладывать необходимые CMake-регистрации до 060.
На стадии 020 не требуется превращать общий код в независимую библиотеку.
Если общая вспомогательная функция впервые нужна FB2, она вводится здесь,
а 030/040 используют или расширяют её. Границы могут уточняться по hunks,
но порядок и причина уточнения фиксируются в review-коммите.

## Что не переносить

- Исправление полярности Libra2 и связанный native-ui hook: пользователь проверил
  upstream `invert_landscape`, не суммировать обе коррекции.
- Собственные Hybrid/freeze fixes, старые ускорения открытия и sample filtering.
- Удалённые dark/i.MX/BOOX policy overrides и старый progress/ticks renderer.
  При этом **оригинальные** beta10 progress/ticks не вырезаются только из-за того,
  что похожая прежняя доработка форка была отменена.
- Повторно fitting/сон/80%-цифры/кириллические ink-box изменения, уже находящиеся в 010.

Старый F04 Debug language chooser/CLDR и прочие дополнительные locale hooks —
предмет отдельного hunk-review в 050, а не автоматическое дополнение перевода.
Upstream invert-swipes сначала сравнить с форком: сохранить пользовательское
поведение, но не добавлять вторую настройку для уже имеющейся функции.

## Применение

Источник истины о порядке/готовности — `series.json`, в том числе SHA-256 патчей.

```sh
python3 platforms/patch-series/apply.py --output build/full-beta10-stack
```

Сейчас команда **намеренно отказывает**: 010–032 проверены в CI, но 040–060 ещё не адаптированы. Это защита
от выдачи незавершённого export за готовый полный форк. После готовности всей серии
будет создаваться новый export из git archive закреплённого upstream. Перед каждым
патчем выполняется apply --check, промежуточный каталог временный; существующий
output не перезаписывается. Основной checkout/ветки/HEAD не изменяются.

Статус `prepared` означает: патч существует и применяется, но проверки сборки/поведения ещё не приняты. `ready` — принятый этап. Для технической проверки подготовленного начала серии:

```sh
python3 platforms/patch-series/apply.py --verify-ready
python3 -m unittest discover -s platforms/patch-series -p 'test_*.py'
```

Проверка начала ничего не оставляет для сборки и не означает полную приёмку.
Это не отдельный поддерживаемый пользовательский профиль.

## Инвентаризация и следующий шаг

[Полная исходная дельта](../../doc/reports/patch-series/inventory.tsv),
[счётчики](../../doc/reports/patch-series/summary.json),
[план FB2](../../doc/reports/patch-series/020-fb2-plan.md).

```sh
python3 platforms/patch-series/inventory.py --output build/patch-inventory
```

Таблица воспроизводится из закреплённых git trees: 349 различных путей между
старым upstream и runtime-источником форка, из них 164 reference и 185 кандидатов
runtime/build/assets/tests. Два отдельных пути помечены DROP: устаревший Libra2 mapping и лишний
`assets/hyph/Russian_EnUS.pattern`; остальные 183 требуют hunk-review (native-ui hook Libra2 расположен в смешанном файле и
удаляется по hunks, а не удалением всего файла).

`stage_hints_NOT_approval` — подсказки по путям, **не завершённая классификация
hunks**. `also_touched_by_010_ru` предупреждает о пересечении с первым патчем.
`upstream_changed_since_beta9` показывает изменение target-файла; отсутствие
изменений не доказывает совместимость его callers. Новые файлы самого beta10
не являются доработками форка и этим отчётом не отбрасываются.

020/021 уже адаптированы поверх **результата 010**. В частности, сохранён новый
beta10 ZIP64 directory reader вместо восстановления старого 32-bit ZIP parser.
022 добавляет raw/resource engine path и форматные стили, но не полные platform inputs.
023 добавляет Kobo raw-FB2 routing и миграцию настройки сносок.
FB2.ZIP не входит в перенос: ранее формат не поддерживался и не отображается в системной библиотеке.
Пункт 024 удалён как ошибочное расширение объёма работ. 030–032 также перенесены и проверены в CI; следующие патчи — 040–060. Разделение FB2 на
020–023 отражает порядок переноса слоёв, а не независимые пользовательские модули.
См. [текущий отчёт](../../doc/reports/patch-series/fb2-publication-resources.md).
Проверки включают обычный EPUB. Финальная приёмка — полная серия + native/Qt5
package + устройство; Qt6/Android, если не проверены, отдельно помечаются.

[Адаптация движка 022 и её границы](../../doc/reports/patch-series/fb2-engine.md).

[Платформенная адаптация 023 и оставшиеся проверки](../../doc/reports/patch-series/fb2-platform-inputs.md).

## Дополнительное исключение: смешанный словарь переносов

По решению пользователя `assets/hyph/Russian_EnUS.pattern` не переносится в полный
beta10-форк. В 050/060 не добавлять его в asset manifest, список поставляемых
patterns и fork-only license notice. Обычный `Russian.pattern` сохраняется.
Переводной словарь `ru-en` из `assets/dictionaries.json` — другой ресурс, не удалять.
Upstream таблица известных языков может упоминать не поставляемые dictionaries:
само такое упоминание не означает включение файла в пакет и не требует отката upstream.
Префикс 010–023 уже не содержит смешанного файла; regression проверяет его отсутствие.
Исторический legacy checkout и исходные отчёты дельты не переписываются этим решением.

## Пакет для аппаратной проверки префикса

Workflow `Ordered patch series` теперь сохраняет проверенный Qt5 package как
`Beta10-series-prefix-qt5`, с applied-prefix.json и SHA256SUMS. Это тестовый
префикс, не самостоятельный поддерживаемый профиль и не полный форк.
Артефакт `Kobalt-qt5-device` другого workflow остаётся legacy-сборкой.
[Инструкция и сценарии проверки](DEVICE-TEST.md). У прогона 8c26105
тестовый prefix package ещё не публиковался; не подменять его legacy-пакетом.

[030 — выполненный первый слой комиксов и границы 031/032](../../doc/reports/patch-series/comics-layout.md).

[031/032: реализованный объём, тесты и CI](../../doc/reports/patch-series/comics-engine-integration.md).

Проверенный пакет с FB2 и comics Layout setting: [CI 45278312375](https://github.com/Mutaracha/kobalt_ru/actions/runs/45278312375),
артефакт `Beta10-series-prefix-qt5`, исходный коммит `08c66e4d`. Это prefix 010–032,
не legacy package и пока не полный форк. Toolbar из 040 ещё отсутствует.
