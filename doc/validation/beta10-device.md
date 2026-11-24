# Beta10 — актуальная проверка устройства

Пакет: **[f87e2a7f / CI45482986925](https://github.com/Mutaracha/kobalt_ru/actions/runs/45482986925/artifacts/14963356876)**.
Все jobs серии/native selected regressions/Qt5/package/qemu PASS, не all-variant/device PASS.
Сохранить полный профиль при закрытом Kobalt; не очищать кэши/данные для обновления.

## Текущий короткий шаг

Проверить «О приложении»: Kobalt — Instrument Serif regular, не Monotype Corsiva.
В строке закрытия бренд тоже Instrument Serif, а RU-префикс/заголовки сохраняют
Monotype Corsiva. Новый шрифтовой helper общий для About/opening/exit; отдельные
reading fonts и classic Sans не менялись. Полное новое семейство Corsiva и новый
menu-status toggle ещё не реализованы в этом пакете.

По решению пользователя FB2 Home preload возвращён: 035 — завершённый эксперимент,
не новая пользовательская опция. В финальной active series запрета FB2 preload нет.
Тайминги 4d9985e2 (без preload) нельзя приписывать f87e2a7f или новой branding-правке.

## Уже принято и повторно не требуется

- Exit-wordmark eafb6161 принят пользователем.
- RU cache-cleanup wording 4d9985e2 принято.
- OFF логов сохраняется после reboot: подтверждено пользователем и startup verbose=false.
- Cache save/read lifecycle подтверждён; медленная повторная вёрстка — отдельный дефект.

## Ограничение дальнейших замеров

Новая cold A/B или широкая reboot-матрица сейчас не требуется. Из текущего архива
уже видно: без preload parsed cache hit не исключает ~3.7/16.5 s repagination.
Следующий запрос device-данных — только под конкретную проверяемую причину или
при добавлении целевой диагностики. Не ослаблять compatibility/identity guards и
не объявлять preload единственной причиной задержки. Background work не равна
автоматическому UI открытию: пользователь все измеренные открытия выполняет вручную.

Источники: [eafb6161](../reports/patch-series/035-eafb6161-device-review.md),
[4d9985e2](../reports/patch-series/036-4d9985e2-device-review.md),
[каталог данных](../../test%20%26%20diagnostic/README.md). Реальные ZIP и screenshot
сохранены без изменения bytes. Перед воспроизведением дефекта включить подробные логи.
