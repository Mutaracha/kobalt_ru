# kobalt_ru — действующий план beta10

Единственная активная очередь. История сборок не является очередью задач.
База: upstream `1796a32559dbb2912bd565990d1c7a0ca7ac4e52`, полная ordered серия
010–060 (включая 033/034), все патчи применяются вместе.
Последний проверенный бинарник: **eafb6161**, [CI45456569246](https://github.com/Mutaracha/kobalt_ru/actions/runs/45456569246),
[пакет Qt5](https://github.com/Mutaracha/kobalt_ru/actions/runs/45456569246/artifacts/14952120554).
CI: series/native/Qt5/package/qemu PASS; не полная аппаратная приёмка.
Docs-only обновления не меняют этот бинарник.

## Очередь

| ID | Статус | Следующее действие / критерий закрытия |
|---|---|---|
| B10-UI | implemented / CI verified, device pending | Проверить eafb6161: Instrument Serif только у Kobalt в строке закрытия, RU-префикс Monotype Corsiva; переключатель подробных логов включается/выключается и сохраняется после перезапуска. При выключении остаётся компактный журнал предупреждений/ошибок. |
| B10-CACHE | in progress: device validation | Сохранение первой FB2 исправлено; пользовательские последние циклы дают saved=true и disk hit. Проверить устойчивое время после перезагрузки для обеих книг и обоих режимов. Не смешивать retained и disk cache. |
| B10-CACHED-LAYOUT | planned: investigation | Разобрать отдельные медленные открытия при cache hit: совместимость layout и повторная пагинация. Последний model_load=6207 ms без завершённого session reading не считать измеренным временем до текста. Нужны прицельные логи, если дефект повторится. |
| B10-HYBRID-COVERAGE | planned: validation | Расширить corpus/проверки: вложенные и несколько body, notes/images/anchors, граница префикса и adoption, reflow, cancellation/close/USB/preview lifetime. Сначала существующие tests, затем недостающие регрессии. Две книги не дают общей production-приёмки. |
| B10-EPUB-RECHECK | deferred: device check | Повторить прежнее наблюдение оценочного счётчика на beta10; без собственного изменения EPUB slicing/fallback. Старые beta9 логи — только архивное evidence, не доказательство дефекта beta10. |
| B10-FREEZE | blocked: reproducible beta10 evidence | Старое зависание не объявлено исправленным. При повторе получить время, сценарий, source commit и полный диагностический архив. Не переносить непроверенные legacy fixes. |
| B10-FONTS | planned: integration | Полное семейство из monotype-corsiva.zip ещё не подключено. Проверить внутренние имена/начертания, подключить в RU-роли и добавить тесты; не менять отдельно заданный Instrument Serif слова Kobalt в строке закрытия. |

## Выполнено в текущей серии

- Полный стек beta10: FB2, comics, RU/CLDR/согласованные строки и presentation,
  toolbar/swipe, сборка/provenance. df8e910a: запуск и raw FB2 подтверждены пользователем.
- 034: начальная частичная вёрстка новой FB2, фоновый полный документ,
  сохранение native anchors и безопасный full fallback. Это стартовый префикс,
  не произвольная секционная модель FB2. Saved-position/cache открытия полные.
- 7f47164: исправлены автоматический swap при отключённом кэше и метаданные
  первых кадров. Пользователь подтвердил ускорение первой книги и правильный
  счётчик во время расчёта; логи показывают успешные saves и disk reuse.
- eafb6161: Instrument Serif у exit-wordmark и switchable diagnostics; CI PASS,
  устройство ещё не проверено. Подробности: [текущее evidence](doc/reports/patch-series/034-hybrid-summary-and-7f47164.md).

## Границы дальнейшей работы

Ускорение открытия с сохранённой середины и полноценная секционная FB2-модель
не реализованы и не являются автоматически запланированным этапом: это отдельное
расширение, требующее согласования. EPUB алгоритмы остаются upstream-owned.

Решения: [действующие](doc/fork-decisions.md). Проверки: [beta10](doc/validation/beta10-device.md).
Журнал: [beta10](doc/reports/patch-series/README.md). Данные: [каталог](test%20%26%20diagnostic/README.md).
История: [архив](doc/archive/README.md). Старые pending-статусы оттуда не восстанавливать.
