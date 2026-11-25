# kobalt_ru — действующий план beta10

Единственная активная очередь. История сборок не является очередью задач.
База: upstream `1796a32559dbb2912bd565990d1c7a0ca7ac4e52`, полная ordered серия
010–060 (включая 033/034), все патчи применяются вместе.
Последний проверенный бинарник: **fd9a70ac**, [CI45636420364](https://github.com/Mutaracha/kobalt_ru/actions/runs/45636420364),
[пакет Qt5](https://github.com/Mutaracha/kobalt_ru/actions/runs/45636420364/artifacts/15034715785).
CI: series/native/Qt5/package/qemu PASS; не полная аппаратная приёмка.
Docs-only обновления не меняют этот бинарник.

## Текущий шаг

[048: RU reading roles](doc/reports/patch-series/048-ru-reading-font-roles.md).
058: RU Literata вместо Libron, Noto Serif вместо NV Legible Next, Lora вместо
NV Ancizar Serif. По4 static faces Literata/Lora, real supplied bytes+OFL. Пресеты,
engine default/generic roles, sleep defaults/registration и annotation export согласованы.
Старые builtin family names при RU open/load mapped; custom names/EN preserved.
Шесть families Adelph/Aleo/Andada/Basker/Castoro/Yrsa и replaced role names скрыты
из RU pickers, engine files остаются. Noto Serif RU доступен как обычный шрифт.
Runtimefd9a70ac/CI45636420364 все jobs PASS; visual device acceptance pending.
Новый timing/cache-clear не нужен; typography/font inventory может законно вызвать reflow.
Bounded Hybrid gates complete (046), Shantell Italic/BoldItalic retained (047).
Следующий feature — persistent reading-menu status toggle после visual feedback.

## Очередь — порядок закрытия

Оценка FB2-задач и критерии остановки: [выигрыш/сложность](doc/reports/patch-series/fb2-hybrid-cost-benefit.md).
Не оптимизировать ради долей секунды с усложнением восстановления/якорей; корректность
не приносится в жертву скорости. Неизмеренный выигрыш не считать доказанным.

| Порядок / ID | Статус | Следующее действие / критерий закрытия |
|---|---|---|
| 1 / B10-UI | partial device accepted | Exit-wordmark, OFF persistence и cleanup wording приняты. About wordmark исправлен через shared Instrument Serif role; CI PASS/device visual pending. |
| 2 / B10-CACHE | done: observed save/read lifecycle | 46ecb8f: все 12 saves=true; измеренные H1/H2/A1/A2 имеют disk hits, retained отдельно. Низкая стабильная задержка A1 не заявлена: это B10-CACHED-LAYOUT. |
| 3 / B10-CACHED-LAYOUT | paused: observed lifecycle accepted | User confirms late fast A1/A2 after reboot, disk hits all inputs match9/34ms. Earlier font_list primitive differs, exact inventory cause unknown. No read-path fix claimed; return only on reproducible recurrence. |
| 4 / B10-HYBRID-COVERAGE | done: bounded host tests PASS | 044–046 prefix/full anchors/count/notes/images, park/cancel/close and resource-backed session/lease gates. CI45636420364 PASS; no claim of full device matrix. |
| 5 / B10-FONTS | CI PASS, device visual pending | 055/056 Noto subset85%; 057 RU Shantell Sans Italic instead of Caveat. English Kalam/classic/Instrument brand unchanged. Report047 cmap audit and sample organization complete. 058/048 RU reading roles+picker exclusions, CI45636420364 PASS. |
| 6 / B10-MENU-STATUS | planned: implementation | Общий для всех книг toggle существующей строки состояния: часы + батарея остаются, время текущего сеанса ↔ страница в книге по нажатию. Persistent global preference, формат/estimated/pending как у BookPage колонтитула. Подробный контракт ниже. |

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
