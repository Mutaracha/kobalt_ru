# Журнал beta10: состояние и evidence

Текущая очередь — [план](../../../FB2_FORK_PLAN.md), не старые next/pending внутри
поэтапных отчётов. Технические отчёты здесь сохраняют результаты на указанном SHA;
ранние формулировки «не реализовано» не описывают последующие сборки.

| Runtime | Результат |
|---|---|
| df8e910a | Full-series startup/raw FB2 приняты пользователем. |
| ace3c3ce | 034 initial prefix/background/fallback, CI PASS; первая device серия cold speed. |
| 4f2cecd1 | RU typography/«Быстро», CI PASS; в device-серии найдены cache-save=false книги1 и prefix total первых страниц. |
| 7f47164f | Disabled-cache Render swap и initial frame coordinates исправлены; CI45449776496 PASS. Пользователь подтвердил ускорение книги1/правильный count, saves/disk reuse появились. |
| 46614c78 | Instrument Serif exit-wordmark/switchable diagnostics; native и Qt5 проверки прошли, collection не прошёл из-за имени ZIP. Не конечный пакет. |
| eafb6161 | Collector исправлен; CI45456569246 все jobs PASS, [artifact14952120554](https://github.com/Mutaracha/kobalt_ru/actions/runs/45456569246/artifacts/14952120554). Последний проверенный пакет, device UI/log-toggle pending. |

## Основные материалы

- [Сводка cold Hybrid и последних логов](034-hybrid-summary-and-7f47164.md).
- [Исправление кэша и первых кадров](034-cache-footer-fix.md).
- [Подробный reboot/stopwatch разбор](034-reboot-cache-review.md), [числа](034-stopwatch.json).
- [Выборка последнего лога](034-7f47164-summary.json).
- [Механизм 034](fb2-startup-experiment.md): стартовый префикс, не общий section engine.
- [Текущий device checklist](../../validation/beta10-device.md).
- [Данные](../../../test%20%26%20diagnostic/README.md), [архив до beta10](../../archive/README.md).

Сводный эффект на двух книгах: примерно 41–47% и 58–63% сокращения холодного
ожидания. Не универсальный benchmark; retained, disk и cold измерения раздельны.
Фоновая полная вёрстка остаётся. Cache hit сам по себе не означает готовый layout.

Документационная консолидация не меняет runtime и не создаёт новый пакет.

## Очерёдность остатка работ

[Оценка FB2 Hybrid: выигрыш против сложности](fb2-hybrid-cost-benefit.md).
Сначала приёмка UI/cache, ограниченный cached-layout разбор и correctness gate;
затем полное семейство шрифта и переключаемая строка состояния меню.
Расширение prefix/сохранённой середины/секционного движка отложено, не feature promise.
Переключение строки состояния согласовано, ещё не реализовано. Нового бинарника нет.
