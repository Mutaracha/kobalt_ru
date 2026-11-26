# Повторно открытые дефекты: предпросмотр и FB2 Hybrid

Источник: пользовательские снимки и `14127a372e7e2c63df2fa348d0d461c3004ae0ac`,
`test & diagnostic/c1cbf4e/diagnostics.zip`. Архив изучен без изменения оригинала.
Это расследование, не исправление и не новая сборка.

## Предпросмотр

Пользователь: FB2, Раскладка, comics off → on → off. До переключения текст есть,
в comics текст есть без полей/колонтитулов, после возврата остаётся белое тело
страницы с верхним колонтитулом. Закрытие/повторное открытие настроек возвращает
текст. На проверенном EPUB-комиксе не воспроизводится. Это сужает проблему до
обновления/рендеринга предпросмотра; потеря текста самой книги не подтверждена.

Исследуемый путь: Reader::syncAppearancePreview → ReaderSession::appearancePage
(Layout) → EngineDocument::imageFromAnchor. Точная причина ещё не установлена.
Не подменять исправление сохранением старой картинки: новая картинка должна
соответствовать реально выбранным полям/режиму. Нужен regression off/on/off.

## Hybrid

Предыдущее общее объяснение полного кэша недостаточно для этого случая.
Лог подтверждает `chapter_mode=true`, а state содержит `rendering_mode=hybrid`.
Нельзя заключать, что настройка осталась Accurate.

Время ниже — UTC из устройства, не дата среды разработки:

- 15:20:30.182: cache_hit=false, saved_position=false, explicit_window=false,
  characters=788339, slice=true. Частичный FB2 старт действительно выбран.
- 15:20:30.934: fb2-startup-layout, pages=57, visible_pages=28.
- 15:20:30.951: fb2-startup-promote reason=fraction-jump.
- 15:20:33.109: повторная загрузка explicit_window=true, slice=false.
- 15:20:39.969: opening-phases model_load_ms=13886, document_cache_hit=false,
  retained_book_hit=false. То есть частичный старт перешёл в полный ещё внутри
  открытия, а не только через обычную фоновую подготовку после начала чтения.
- 15:25:19.525 и 15:26:06.099: cache_hit=false, saved_position=true, slice=false.
  Очистка кэша не стирает сохранённую позицию; текущая политика FB2 сознательно
  использует полный путь для таких открытий.
- 15:28:06.102, 15:29:02.924, 15:29:29.158: cache_hit=false,
  saved_position=false, characters=8236, slice=false. Маленький текст не проходит
  порог частичного старта; это отдельная причина полного числа страниц.

Код reader_source.cpp подтверждает guards cache/saved-position/window/size;
reader_slices.cpp::goToFraction безусловно переводит активный FB2 prefix в полный
документ. OpeningJob::restartBook вызывает goToFraction(0); это один из путей,
требующих целевого теста. Конкретный caller записи fraction-jump пока не доказан:
не выдаём сопоставление кода за трассировку вызова.

Диагностические события не выбирают режим. Из лога нельзя объяснить все случаи
одной причиной. Полный кэш в Hybrid допустим, но зафиксированное раннее promotion
нужно разбирать отдельно. Не ослаблять guards и не включать saved-middle slicing
под видом исправления. Следующий scope: воспроизведение/регрессии preview round-trip
и раннего fraction-jump, затем минимальные fixes с полным CI/device gate.

## Продолжение

Реализованные fixes, воспроизводящий red/green regression и финальный CI:
[069](069-fb2-preview-and-zero-fraction-fixes.md). Device acceptance остаётся отдельной.
