> АРХИВ: исторический контекст, не текущий план и не инструкция установки.
> Старые pending/next не возобновлять. Текущий источник — FB2_FORK_PLAN.md в корне.

# beta9 inventory — завершение shared consumer alignment

Runtime `80ee8d791c32d51c80cff194fd21418c3a571c66`; upstream `be47258850ffdc3295257475afdf8b8f6cb53176`.
[Полная воспроизводимая дельта](beta9-delta-current/README.md):
344 nonidentical tracked путей, 1203 identical,
0 upstream-only. [TSV](beta9-inventory.tsv): 195 runtime/build/assets/tests
путей. Документация/evidence отдельно учтена в полном отчёте.

Shared consumer implementations перенесены; CI45060819419 / build289 native + Qt5 PASS.
Это не физическая device acceptance, не beta10, не EPUB Hybrid rewrite.

## Заключительная группа

Reader/coordinator quiet/preload/activation и Settings/Debug/status/percentages
больше не остаются запланированными consumers: код, интерфейсы, helpers и focused
regressions перенесены. Fork progress удалён целиком; используется beta9 bottom
on/off. Сохранённые дельты и ограничения перечислены в полном отчёте.

Все `src/sync` реализации/headers, Reader KOSync, shared header/footer model/panel,
Reader dialogs, opening interfaces, coordinator header и private font loader
совпадают с pinned upstream; coordinator cpp отличается только locale hook.

## Проверка границ

Algorithms EPUB Hybrid, frame/session behavior вне approved comics hook,
Libra2 mapping и waveform не переписаны в этой группе. Сам факт одинакового файла
или зелёной сборки не доказывает отсутствия device дефектов. Консервативный REVIEW
в exhaustive TSV предназначен для следующего pinned update, не blanket sign-off.

Исторические checkpoints и disproven dead-writer hypothesis сохранены в
[исходном handoff](beta9-delta/README.md) и Git history. Не переносить их старые
OPEN статусы на текущие уже восстановленные consumers.
