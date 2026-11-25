# Одна упорядоченная серия патчей форка

**Все семь патчей применяются вместе, строго по series.json.** Они не являются
самостоятельными профилями сборки. Промежуточные префиксы не обязаны компилироваться:
итоговый файл с межобластной интеграцией принадлежит одному функциональному слою,
а используемые им типы/сборочные зависимости могут появляться позже в полной серии.

База неизменна: upstream beta10 `1796a32559dbb2912bd565990d1c7a0ca7ac4e52`.
Источник исторической дельты: `ca50dd358386e9fccc5122b464f74003caa51e75`.

| Патч | Итоговая область |
|---|---|
| 010-ru-localization | Итоговые RU-строки, шрифты/роли/лицензии, locale/font UI, language chooser и проверки |
| 020-publication-resources | FB2 inspection/resources и платформенное распознавание входа |
| 030-engine-integration | FB2/comics/rendering, font manager и stable system-font cache identity; engine-тесты |
| 034-session-hybrid | Session/opening/cache/Hybrid orchestration и replay/session-тесты |
| 040-reader-controls-settings | Общие reader window/host/settings/toolbar/status-toggle wiring и проверки |
| 050-presentation | Общие UI/brand/sleep/annotation presentation и тесты |
| 060-build-integration | CMake, packaging, сборочная интеграция и регистрация тестов |

Это консолидация **без изменения кода runtime**: каждый изменённый файл имеет одного
владельца и уже содержит проверенный конечный результат. Общие окна/asset manifest
могут содержать интеграцию нескольких функций; это зафиксировано в file-map, а не
выдано за независимость патчей или новый архитектурный рефакторинг.

## Проверка эквивалентности

Эталон — полная прежняя серия runtime `5b45f2c74857180f0689f80b5e48a9134f58bec7`.
Её воспроизводимый manifest сохранён как `equivalence/previous-series.json`; исходные
старые patch bytes доступны в Git на `b45add66fb50f6d66a02a9dcd55c895b1916e3b9`.
`equivalence/reference-tree.json` фиксирует все runtime paths, SHA256, sizes и git modes;
`equivalence/file-map.json` — соответствие каждого итогового файла старым/новому патчам.

```sh
python3 -m unittest discover -s platforms/patch-series -p 'test_*.py' -v
python3 platforms/patch-series/apply.py --output build/verified-full-series
python3 platforms/patch-series/verify_equivalence.py build/verified-full-series
```

Сравнивать до package.prepare: source-info/series-provenance генерируются с новым
commit/hash серии и намеренно отличаются. Двоичная идентичность ELF/ZIP не заявляется:
идентична runtime-исходная база и ресурсы; provenance нового пакета должен быть новым.
До изменения runtime в будущем reference обновляют только с отдельным объяснением и
проверенным эталоном, не для маскировки ошибки консолидации.

Исторический standalone RU control overlay в `platforms/control-beta10/` не изменён
и больше не является первым патчем полной серии. Не применять его дополнительно.
Исторические отчёты/старые patch IDs относятся к своим SHA и не переписываются.
[Таблица консолидации](../../doc/reports/patch-series/056-patch-consolidation.md),
[действующий план](../../FB2_FORK_PLAN.md).

Сборка: `package.py prepare` экспортирует весь стек, затем Qt5 CI компилирует и
`package.py collect` проверяет установленную provenance/ELF32 ARM/архив. Prefix-only
пакеты и выборочное отключение функциональных патчей не используются.
