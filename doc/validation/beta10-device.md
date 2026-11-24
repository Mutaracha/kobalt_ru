# Beta10 — актуальная проверка устройства

Пакет: **[1da4445c / CI45491097786](https://github.com/Mutaracha/kobalt_ru/actions/runs/45491097786/artifacts/14965972454)**.
Все jobs серии/native selected regressions/Qt5/package/qemu PASS, не all-variant/device PASS.
Сохранить полный профиль при закрытом Kobalt; не очищать кэши/данные для обновления.

## FB2: целевой диагностический шаг

Из прежнего архива нельзя установить точное условие отказа от вёрстки: parsed cache
hit отмечается до Render, а CRLog не входит в ZIP. В новом пакете 036 записывает
`model_layout_check` и `fb2-cache-layout-check`, без изменения cache guards или скорости.
Описание полей: [отчёт037](../reports/patch-series/037-fb2-layout-observation.md).

- Включить подробные логи. Не менять файлы/layout/Accurate, не очищать кэши.
- Вручную открыть A1 → закрыть книгу в Home → A2 → закрыть → A1.
- Каждый раз дождаться готовности чтения; reboot между открытиями не нужен.
- Собрать diagnostics. Stopwatch необязателен: нужны операнды новой проверки,
  а не повтор сравнения абсолютных времён. Чередование уменьшает риск подменить
  disk-hit тест retained reopen; фактический путь определяется по логам.

Обновлённая диагностика обосновывает этот узкий запуск; широкая cold/reboot-матрица
по-прежнему не требуется. FB2 Home preload включён, как указал пользователь.
Известные задержки отмены preload не объявлены исправленными.

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
