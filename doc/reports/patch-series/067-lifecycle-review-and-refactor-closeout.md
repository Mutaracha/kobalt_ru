# R10 — оценка lifecycle и завершение ограниченного цикла рефакторинга

## Решение R10

Оценка выполнена; runtime-рефакторинг lifecycle **deferred / не включён**.
Это не «реализованный и проверенный R10» и не исправление дефекта lifecycle.
Условие возврата — конкретная польза/дефект, отдельный scope и регрессии на
асинхронные переходы; для красоты исходников отдельную сборку не выпускаем.

Проверенные области и причины не менять их в этом цикле:

| Область | Связанные контракты / риск |
|---|---|
| ReaderSession::open, cancelSource, revokeSource, drain | revision/newestLayout, очереди worker/main thread, lease ownership, cancel vs revoke. Похожие cancel/revoke блоки заканчиваются разными действиями доступа к источнику; сокращение строк не оправдывает новый общий lifecycle API. |
| OpeningJob::bindResources, inspectSource, mergeSavedState | retained layout, hash/resources/CSS/appearance guards, FB2 probe, миграция сохранённых полей. Попытка объединить стадии пересекает исключённую область cache/Hybrid. |
| Reader::prepareShutdown, prepareUsb, releaseForUsb, prepareTransition, saveTransition | request generation, QPointer guards, mapping deadline, обязательные storage acknowledgements, retry budget, поздние callbacks, освобождение native resources. Сходные ветки не имеют одинаковой семантики. |
| AppearanceEditSession::open/resetPreview/discard/completeApply | preview generation, cancellation, retention/destination. Уже отдельный owner; ещё одна обёртка не даёт доказанной пользы. |

Существующие tests/reader_lifecycle.cpp, tests/model/native_lifecycle.cpp,
reader_session.cpp, reading_book.cpp и fb2_cache_session.cpp дают точки для будущих
целевых регрессий, но их наличие не доказывает полное покрытие всех переходов.
В этой docs-only оценке новые lifecycle tests не запускались; новые гарантии
корректности lifecycle не заявляются.

## Итоговая проверка серии

База сравнения перед R01: `b753ef28` (после одобренного аудита переводов).
Последний runtime: `66c21df1c9b4a914272aa9d1abef09c90fbd0b60` (R09).
Пользователь подтвердил R09: «проверил, продолжаем».

- Порядок сохранён: 010 / 020 / 030 / 034 / 040 / 050 / 060, все вместе.
- SHA256 всех семи patch files соответствует series.json.
- После R01–R09 1552 runtime paths против 1551 в базе; 18 путей отличаются
  (включая новый sleep_letter_text.h). Все 18 входят в явные amendments R01–R09.
- Ни одного изменения относительно этой базы в src/engine, src/storage,
  src/publication или assets. Также байт-в-байт прежние reader_session.cpp,
  opening_job.cpp, reading_book.cpp и appearance_edit_session.cpp.
- Изменения reader.cpp / reader_build.cpp относятся к menu-status R05, не к
  переходам открытия/закрытия. Эти различия не выдаются за нетронутые файлы.
- Повторно выполнены 15 локальных Python series/package/equivalence tests: PASS,
  вся ordered серия применена, 1552 runtime paths совпадают с текущим reference.
- [CI45833426741](https://github.com/Mutaracha/kobalt_ru/actions/runs/45833426741):
  все 3 jobs PASS — series, selected native/Linux и Qt5/package, включая FB2 replay.
  Это ограниченный CI набор, не полная аппаратная/stress-матрица.
- Нового бинарника после R09 нет. [Пакет R09](https://github.com/Mutaracha/kobalt_ru/actions/runs/45833426741/artifacts/15123082151)
  (45 429 910 байт) остаётся последним CI-проверенным и принятым на устройстве.

## Приёмка и оставшиеся ограничения

R01–R03, R06–R09 приняты пользователем на устройстве. R08 явно подтверждён по
всему предложенному списку. Для R04/R05 подтверждены шрифты и статусбар;
полные EN/RU/custom-name сценарии не приписываются пользователю.

Ограниченный цикл рефакторинга завершён: R01–R09 выполнены; R10 оценён и
осознанно не изменён. Это не закрывает расширенную EPUB/cache/stress-проверку,
невоспроизведённые наблюдения о зависании или будущую beta11 font-cache integration.
Согласование upstream font identity выполняется отдельно по beta11 checklist.
Новые cache/Hybrid алгоритмы и ускорение saved-middle не становятся следующей
автоматической задачей.
