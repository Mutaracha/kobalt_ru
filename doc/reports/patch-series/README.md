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

## Device follow-up 46ecb8f

[Раздельные H/A логи и ручные замеры](035-eafb6161-device-review.md): exit-wordmark
принят, cache save/read подтверждены. Задержка A1 включает явную Home preload,
отменяемую при ручном tap (7.4/13.2 s ожидания), и отдельную ~4.5 s repagination.
Подготовлены 035 (skip speculative raw-FB2 preload) и RU «Kobalt закроется».
Runtime 4d9985e2 / CI45477082472 все jobs PASS; [пакет](https://github.com/Mutaracha/kobalt_ru/actions/runs/45477082472/artifacts/14959364274).
Device-проверка 035/новых сообщений pending. Новое семейство Corsiva/menu-status ещё не включены.

## 4d9985e2: эксперимент завершён, preload возвращается

[Отчёт и ограничения](036-4d9985e2-device-review.md). No-preload подтверждён событиями
skipped, но новые времена 11.20/22.32 s не закрывают cached-layout defect. OFF persistence
и wording приняты. По решению пользователя 035 удалён из active series, а не стал
постоянным отключением FB2 preload. Runtime f87e2a7f: About shared wordmark исправлен, CI45482986925 все jobs PASS,
[пакет](https://github.com/Mutaracha/kobalt_ru/actions/runs/45482986925/artifacts/14963356876). Device visual pending; preload FB2 восстановлен.

## FB2: наблюдение layout-кэша, runtime1da4445c

[Отчёт037](037-fb2-layout-observation.md): 036 добавляет checkRenderContext operands/reasons,
не ослабляет guards. CI45491097786 all jobs PASS. Узкая device-проверка A1→A2→A1
с прежним cache/layout и логами ON; не очередная cold/reboot-матрица. Скорость ещё не исправлена.

## 1da4445c: device style-hash evidence

[Отчёт038](038-1da4445c-layout-evidence.md). Manual A1/A2: reasons2; повтор A1 disk-hit
reasons0/paginate10ms; после startup preload A1 снова reasons2. Размеры/flags/stylesheet/
policy совпадают на каждой проверке. Следующий шаг — host hash decomposition и
cross-process воспроизведение, не новый широкий device-тест. Guard остаётся неизменным.

## Host probe037: отдельные процессы

[Отчёт039](039-host-cross-process-cache-probe.md), [точные hash excerpts](039-host-cache-probe-evidence.json).
CI45498201010 и45499862360 all jobs PASS. Short/long actual-text fixtures: warm reasons0,
width isolation reasons16. Device mismatch не воспроизведён; production не менялся,
нового device-запуска не требуется. Следующий шаг — приблизить source/fonts/typography/order.

## Noto subset + uploaded Kobo fonts

[041](041-noto-subset-and-kobo-fonts.md): runtime2c1914e3/CI45596424824 all jobs PASS.
Only Noto Regular+Italic; Corsiva removed from final package, Instrument wordmark unchanged,
opening italic semantics restored. Actual Kobo font replay warm stable, device FB2 issue open.

## Header-anchored layout input snapshots039

[042](042-layout-input-observation.md): current/saved node/global/style inputs, optional
existing doc props, unchanged compatibility formula. CI45599360375 all jobs PASS;
833c8e28 device run pending. Prime A1/A2, normal exit+reboot, A1/A2 and diagnostics.
Noto subset and enabled preload retained, no cache clear. This is not a speed fix.

## Upload214bf213; optical scale056

[043](043-214bf213-font-inventory-and-optical-scale.md): valid device snapshots narrow
initial reflow to font_list primitive; late disk hits all match9/34ms paginate. User reboot
chronology recorded separately from one explicit startup.056 scales RU Noto role85%,
wordmark untouched, hint «Подготовка к чтению». CI45607993683 PASS, RU45609563281 PASS.

## Hybrid correctness first test batch044

[044 report](044-hybrid-correctness-gates.md): user confirms late fast A1/A2 after reboot;
cache read path was not fixed during diagnostics. Transition to correctness-only Hybrid
gate. Added pre-start stop, park/preserve prefix, resume, parked-close/independent rebuild.
CI45613247416 all jobs PASS; tests only, device-update not requested. Remaining notes/
body-image/resource-lease/session-count checks are not declared complete.

[045 Hybrid note/body-image parity](045-hybrid-notes-images.md): test-only, CI45617219756 PASS. Remaining resource-backed revoke/session count gate open.
