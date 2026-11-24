# Закрытые исключения — не backlog

По указанию пользователя эти функции исключены из форка. Не возвращать их в план,
задачи, проверки реализации или сборку без нового прямого запроса. Этот реестр —
предохранитель от повторного предложения, а не перечень будущих улучшений.

- FB2.ZIP и ошибочный патч 024: не поддерживаются; распаковывать в обычный FB2.
- Собственный progress renderer/chapter labels/ticks, положение Top/Bottom и верхний
  вариант полосы; отдельная полоса page-count/time-to-book-end меню. Не путать
  с оригинальными штатными функциями beta10, которые сохраняются.
- Локальная Libra2 polarity/actionRotation correction: заменена upstream-опцией.
- F09/sample-cache settings filtering форка.
- Quiet-frontlight policy и quiet-gesture diagnostics форка.
- Собственные dark/i.MX animation и BOOX refresh overrides.
- Старые opening acceleration/freeze/Hybrid patches, не вошедшие в ordered серию.
  Это не запрет согласованного эксперимента 034 и не утверждение, что freeze исправлен.
- Отменённый adjacent-footnotes эксперимент, draw-only сдвиги и закрытая доработка
  зазора нижних сносок; отдельная fork KOSync policy.
- Избыточный Russian_EnUS.pattern и его fork-only notice/manifest/list entries.
  Обычный Russian.pattern и отдельный ru-en переводной словарь сохраняются.
- Отменённые исторические предложения не становятся обещаниями релиза из-за
  наличия старого pending в архиве. F07 checkable screens не исключены.

Основание: решения пользователя, ранее рассеянные в плане и реестре.
Исходные формулировки: archive/mixed-history/doc/fork-decisions.md и
archive/mixed-history/FB2_FORK_PLAN.md. Не использовать старые правила поверх текущих.
