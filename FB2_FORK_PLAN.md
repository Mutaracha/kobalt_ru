# kobalt_ru — действующий план beta10

Единственная активная очередь. История сборок не является очередью задач.
База: upstream `1796a32559dbb2912bd565990d1c7a0ca7ac4e52`, полная ordered серия
010–060 (включая 033/034), все патчи применяются вместе.
Последний CI-проверенный бинарник: **92a29364**, [CI45675458450](https://github.com/Mutaracha/kobalt_ru/actions/runs/45675458450),
[пакет Qt5](https://github.com/Mutaracha/kobalt_ru/actions/runs/45675458450/artifacts/15051583108).
CI selected native/Qt5/package PASS; не полная аппаратная приёмка.
Docs-only обновления не меняют этот бинарник.

## Текущий шаг

- [049: актуальные RU fonts/pickers](doc/reports/patch-series/049-bundled-roles-and-cyrillic-pickers.md):
  NV Literata/KF Sable/NV Publica вместо добавленных Literata/Lora и promoted Noto Serif.
  Noto fallback сохранён; Noto Display/Shantell retained; close caption90%, baseline/wordmark unchanged.
- [050: новые логи](doc/reports/patch-series/050-f4a708c-diagnostics.md) подтвердили повторные
  font_list mismatches после рестартов. [051: проверка причины](doc/reports/patch-series/051-system-font-path-investigation.md),
  [052: исправление идентичности](doc/reports/patch-series/052-system-font-identity-device-package.md).
  Пользователь не воспроизвёл задержку в нескольких открытиях, включая после reboot.
  Краткая проверка положительная; длительную проверку продолжаем в combined92a29364.
- [053: menu-status](doc/reports/patch-series/053-reading-menu-status.md):059b реализован,
  native/lifecycle/Qt5 gates PASS. Физическая визуальная приёмка и live pending/turn check отдельно.
  [054: feedback и Letter signature](doc/reports/patch-series/054-letter-signature-and-device-feedback.md):
  Close caption визуально принят; подпись Letter Kobalt переведена на Instrument Serif.
  059c CI PASS, новая device visual/длительная cache-проверка pending.
- Bounded Hybrid gates046 сохранены. Дополнительного acceleration/cache clearing нет.

## Очередь — порядок закрытия

Оценка FB2-задач и критерии остановки: [выигрыш/сложность](doc/reports/patch-series/fb2-hybrid-cost-benefit.md).
Не оптимизировать ради долей секунды с усложнением восстановления/якорей; корректность
не приносится в жертву скорости. Неизмеренный выигрыш не считать доказанным.

| Порядок / ID | Статус | Следующее действие / критерий закрытия |
|---|---|---|
| 1 / B10-UI | partial device accepted | Exit-wordmark, close caption, OFF persistence и cleanup wording приняты. About wordmark исправлен через shared Instrument Serif role; CI PASS/device visual pending. |
| 2 / B10-CACHE | done: observed save/read lifecycle | 46ecb8f: все 12 saves=true; измеренные H1/H2/A1/A2 имеют disk hits, retained отдельно. Низкая стабильная задержка A1 не заявлена: это B10-CACHED-LAYOUT. |
| 3 / B10-CACHED-LAYOUT | short device check positive; extended check pending | Recurrent font_list mismatch in050; temporary system-copy path instability reproduced in051 and corrected in052. Several opens including after reboot without reproduced delay per user; continue longer in combined build, no new log reasons0 claim. |
| 4 / B10-HYBRID-COVERAGE | done: bounded host tests PASS | 044–046 prefix/full anchors/count/notes/images, park/cancel/close and resource-backed session/lease gates. CI45636420364 PASS; no claim of full device matrix. |
| 5 / B10-FONTS | CI PASS, device visual pending | 055/056 Noto subset85%; 057 RU Shantell Sans Italic instead of Caveat. English Kalam/classic/Instrument brand unchanged. Report047 cmap audit and sample organization complete. 059/049 supersedes058: NV Literata/KF Sable/NV Publica, cmap picker filter, standalone Literata/Lora removed; CI PASS. |
| 6 / B10-MENU-STATUS | implemented: CI PASS, device review pending | Общий для всех книг toggle существующей строки состояния: часы + батарея остаются, время текущего сеанса ↔ страница в книге по нажатию. Persistent global preference, формат/estimated/pending как у BookPage колонтитула. Подробный контракт ниже. |

## B10-MENU-STATUS — согласованный контракт

- Нажатие на существующую строку «Часы, батарея и время текущего сеанса» меняет
  только альтернативное содержимое времени сеанса: **страница в книге**.
  Следующее нажатие возвращает время текущего сеанса; часы/батарея не исчезают.
- Режим — общая настройка приложения, не per-book. Сохраняется между книгами
  и перезапусками; исходный режим при отсутствии настройки — время сеанса.
- Использовать ту же семантику и formatter BookPage, что у колонтитула:
  whole-book page/pages, estimated/calculating и применимые publisher/screen labels.
  Не показывать local prefix total как полный; не запускать layout ради строки.
- Не менять настройку/содержимое самих колонтитулов и не возвращать отдельную
  удалённую полосу под toolbar. Чтение сеанса и Stats продолжается в обоих режимах.
- Переключение должно быть доступно и в начале сеанса: нынешнее скрытие подписи
  до пяти минут не должно делать tap target недоступным. Сохранить общую политику
  показа/скрытия строки и штатных панелей, не добавлять постоянно видимую полосу.
- Проверки: два нажатия, первая минута, смена книги/restart, full и partial/pending,
  обновление после page turn/full-ready, RU/EN и день/ночь, длинные значения/hit area;
  отсутствие лишнего reflow и сброса длительности сеанса. Визуальная приёмка отдельно.

## Отложено — не блокирует закрытие текущей реализации

| ID / область | Причина / условие возврата |
|---|---|
| Дополнительный tuning prefix / saved-middle / секционный FB2 engine | Измеренной дополнительной пользы нет; риск anchors/dependencies/adoption высок. Только отдельный сценарий с доказанным заметным выигрышем и новым согласованием. |
| Расширенная corpus/stress matrix | Выполняется только ограниченный correctness gate выше; расширять по конкретным дефектам/новым структурам, не по всем сочетаниям. |
| B10-EPUB-RECHECK | Проверка прежнего counter-наблюдения без собственного изменения EPUB алгоритмов. Beta9 evidence не доказывает beta10-дефект. |
| B10-FREEZE | blocked: нужны воспроизводимый beta10-сценарий, время/source и логи. Не объявлен исправленным, speculative legacy fixes не переносить. |

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

### Superseding font decision

Задача полной Monotype Corsiva family **отозвана** последним решением пользователя.
В final active series055 Noto Regular+Italic уже интегрированы (CI PASS), device visual
pending; не добавлять остальные шесть faces, не возобновлять full-Monotype task.
Standalone RU-only overlay остаётся историческим comparison baseline с прежним шрифтом;
обновлённый устанавливаемый пакет — полная ordered series с055.
