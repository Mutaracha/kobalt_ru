# Beta10 — актуальная проверка устройства

Пакет: **[1da4445c / CI45491097786](https://github.com/Mutaracha/kobalt_ru/actions/runs/45491097786/artifacts/14965972454)**.
Все jobs серии/native selected regressions/Qt5/package/qemu PASS, не all-variant/device PASS.
Сохранить полный профиль при закрытом Kobalt; не очищать кэши/данные для обновления.

## FB2: целевой шаг выполнен

Пользователь прислал `test & diagnostic/1da4445c/diagnostics.zip` (74e25306).
[Результаты038](../reports/patch-series/038-1da4445c-layout-evidence.md): первые manual
A1/A2 reasons2 (style-hash), повтор A1 disk-hit reasons0 и paginate10ms; после нового
startup preload A1 снова reasons2. Это подтверждает работу новой диагностики и
возможность disk-layout reuse, но не закрывает причину межзапускного изменения hash.

Повторять A1→A2→A1, cold A/B или reboot-матрицу сейчас не нужно. Следующая работа
на host: hash constituents и separate-process reproduction. Новая runtime-правка
по этому upload ещё не сделана, ускорение не обещано. FB2 Home preload остаётся включён.

## Визуальная проверка при случае

«О приложении»: Kobalt — Instrument Serif regular, не Monotype Corsiva. RU-префикс
закрытия/заголовки остаются Monotype Corsiva. Исправление общее для About/opening/exit.
Полная новая Corsiva family и menu-status toggle ещё не включены.

## Уже принято

Exit-wordmark, cache-cleanup wording и persistence OFF после reboot подтверждены.
Сохранение/чтение кэша подтверждено, но повторная вёрстка — отдельная незакрытая проблема.
Не очищать данные и не ослаблять layout/source compatibility guards ради времени.

Источники: [4d9985e2](../reports/patch-series/036-4d9985e2-device-review.md),
[каталог](../../test%20%26%20diagnostic/README.md).
