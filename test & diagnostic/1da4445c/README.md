# 1da4445c — device layout-check evidence

Оригинальный diagnostics.zip: upload74e25306; payload не изменён.
3772 events, runtime1da4445c, logs ON. Запрошенный ручной порядок A1→A2→A1.
Новых stopwatch values пользователь не сообщил. Logged session totals не stopwatch.
Первые manual A1/A2: cache hit, retained=false, reasons2 (style-hash mismatch).
Повтор A1: disk hit, reasons0, paginate10ms. После нового startup preload A1 снова
reasons2. Background preload не считать ручным UI-открытием.
Все пять сохранений успешны. Причина внутри style-hash ещё не установлена.
Анализ: doc/reports/patch-series/038-1da4445c-layout-evidence.md.
