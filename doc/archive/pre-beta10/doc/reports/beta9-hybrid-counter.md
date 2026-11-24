> АРХИВ: исторический контекст, не текущий план и не инструкция установки.
> Старые pending/next не возобновлять. Текущий источник — FB2_FORK_PLAN.md в корне.

# EPUB Hybrid: baseline beta9 и отложенная проверка beta10

Upstream `be47258850ffdc3295257475afdf8b8f6cb53176`, runtime
`80ee8d791c32d51c80cff194fd21418c3a571c66`, архив в пользовательском
`faed04cb7396651f0d21f9a6e6539b84c59be30e`:
`test & diagnostic/80ee8d7/diagnostics hibrid.zip` (байты не изменены).

## Наблюдение и приёмка

Пользователь подтвердил успешное открытие своих тестовых книг на устройстве.
Отдельно: после двух перезапусков EPUB сохраняет оценочный `~`, колонтитул не
обновляется. Full-document-ready в архиве дважды сообщает 1000 страниц:
19:57:54.104Z и 20:02:59.201Z по UTC timestamps архива. Последний Full document
завершён примерно за 6.9 s, completed=true; state.json содержит rough_pages=1042,
оба estimated=true, whole_book_failed=false. Snapshot сам по себе не доказывает
причину; повторная проверка пользователя подтверждает видимый симптом.

## Сверка исходников (не runtime upstream reproduction)

- `src/engine/blaze/adapter/reader_slices.cpp` целиком одинаков: roughPages,
  wholeBookEstimateRecord, карта счётчиков и признаки оценочности.
- `src/engine/blaze/adapter/reader_source.cpp`: prepareFullDocument целиком одинаков,
  включая перенос whole.read в pageMap после готовности. Вне этой функции есть
  FB2/comics изменения; весь файл не объявляется одинаковым.
- `src/reading/reader_session.cpp`: captureChapters, фоновая публикация counts,
  вызов updated и showFullDocument одинаковы; дельта файла — include atomic и
  image(page) → frameImage для comics. Header имеет дополнительный include.
- `src/app/reader/window/reader_build.cpp`: session->updated → refresh одинаков.
- `src/app/reader/window/reader_settings.cpp`: chromeValues одинаков.
- `src/reading/header_footer.cpp`, `.h`, `src/reading/rendering_mode.cpp` одинаковы.

В этой цепочке нет отсутствующего upstream-переноса. Точная причина не установлена;
не называть это доказанным upstream bug без запуска чистой beta9.

## Решение / следующая проверка

Deferred до beta10, без собственного patch/fallback/reset cache. На beta10 открыть
тот же EPUB с теми же параметрами, дождаться full-document-ready, проверить
снятие `~` без перелистывания, после перелистывания и после повторного запуска.
Не переносить этот симптом как намеренную доработку форка. Алгоритмы Hybrid
остаются upstream-owned. Открытие книг принято; счётчик не принят как исправленный.
