# Тестовые и диагностические данные beta10

Исходные uploads сохранять без изменения bytes, ZIP не перепаковывать. Runtime,
результаты и текущие задачи — разные сущности. Документация: [план](../FB2_FORK_PLAN.md),
[проверки](../doc/validation/beta10-device.md), [журнал](../doc/reports/patch-series/README.md).
Даты внутри логов — часы устройства, а не дата независимого измерения.

| Каталог / файл | Назначение и статус |
|---|---|
| `08c66e4d/` | Смешанная переходная диагностика beta9 b62cf07 и pristine beta10 1796a32. Оставлена целиком для provenance; не приёмка current full-series. Пакеты 41c11e3/b62cf079 признаны неработающими на устройстве, не устанавливать. |
| `df8e910a-ui/` | Full-series beta10: пользователь подтвердил запуск/raw FB2; UI screenshots — отдельное evidence, не общий PASS. |
| `ace3c3c/` | 034: первая серия cold Hybrid/Accurate, retained повторы и UI review. |
| `4f2cecd1/` | Вторая серия, stopwatch/reboot: обнаружены отказ cache-save книги1 и ложный prefix total. Историческое наблюдение, исправлено позже. |
| `7f47164/` | Диагностика также содержит 4f2cecd1; успешные saves/disk reuse книги1, пользователь подтвердил исправление счётчика. Последний disk hit не имеет session reading total. |
| `FB2-034-test-inputs.zip` | Пользовательский согласованный набор холодных копий; оригинальный архив не менять. |
| `1. Виртуальный свет.fb2`, `Sorokin_*.fb2` | Реальные исходные книги, opt-in device regression, не portable CI fixtures и не installer assets. |
| `monotype-corsiva.zip` | Новое полное семейство, интеграция ещё pending; не считать включённым в eafb6161. |
| `archive/pre-beta10/` | Все отделённые beta9/8/7 и старые beta5 evaluation evidence. Не текущий backlog. |

[Контрольные суммы всех исходных файлов](SHA256SUMS), включая архивные, проверяются
из корня репозитория: `sha256sum -c 'test & diagnostic/SHA256SUMS'`.
`catalog.json` фиксирует обнаруженные startup versions/commits, размеры и hashes.
README/catalog/SHA256SUMS не включают сами себя в checksum-набор.

Новые данные добавлять отдельной runtime-папкой с контекстом сценария. До диагностики
включить подробные логи в Debug; не удалять профиль/кэши. Source commit брать из
startup/About, а не из имени upload commit. Текущий бинарник eafb6161 ещё не имеет
нового пользовательского diagnostic upload. Общие portable fixtures и opt-in
`tests/manual/books` остаются на месте: они не являются результатами старой сборки.
