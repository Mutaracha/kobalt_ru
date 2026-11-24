# Текущая дельта kobalt_ru

Источник истины для runtime — pristine beta10
`1796a32559dbb2912bd565990d1c7a0ca7ac4e52` + полная [ordered серия](../platforms/patch-series/README.md).
Не считать dirty root tree или старую beta9 карту собранным текущим runtime.

| Группа | Владелец |
|---|---|
| 010 | RU-only strings/CLDR/fonts/locale, reusable overlay |
| 020–023 | FB2 recognition/publication/resources/engine/platform semantics |
| 030–032 | Comics layout/engine/session/UI |
| 033 | Parsed-document session cache lifecycle |
| 034 | Experimental cold new-book FB2 prefix + background full model |
| 040 | Retained reader controls/swipe/toolbar |
| 050 | Approved presentation/About/annotation/locale hooks |
| 060 | Full-stack build/provenance/package |

Все применяются вместе; registration/test changes принадлежат вводящему патчу.
Последний binary/source/CI и незавершённые проверки — [план](../FB2_FORK_PLAN.md).
[Beta10 migration map](reports/beta10-fork-map/README.md) — исторический вход для
разбора происхождения, не конкурирующая очередь. Прежняя beta9 карта и накопительный
дневник — [архив](archive/README.md). Исключённые функции — отдельный закрытый реестр.
