# 030 — первый слой комиксов поверх 010–023

> Поэтапный отчёт: статусы относятся к указанному runtime. Текущее состояние и
> пакет — [сводный журнал](README.md), действующие проверки — [beta10](../../validation/beta10-device.md).

Статус: ready по CI 45278312375 (08c66e4d). Сам по себе 030 — слой геометрии;
пользовательский режим подключён следующими 031/032, проверенными в том же CI.

## Реальный патч

`030-comics-layout.patch` содержит:

- `reader_comics.cpp/.h`: временная effective geometry (одна колонка, нулевые поля без перезаписи исходных настроек), форматный CSS, bounded OPF metadata для `rendition:page-spread-center`, классификация bitmap portrait/standalone.
- `column_view_comics.cpp` и comic hooks в `column_view.h`: пары соседних image-only страниц, standalone/rotation geometry, RTL placement и hit-test mapping. Режим по умолчанию выключен. Состояние группировки сбрасывается при Resize/rebuild.
- Новый `engine.comics-layout`: геометрия on/off, OPF center-page/relative/percent-encoded paths, ordinary/missing references, повреждённый и слишком большой package, null container и обычное column mapping.
- Source registration использует существующие beta10 adapter globs для desktop/Kobo/Android; тест/ZIP fixture/area зарегистрированы в CMake в этом же патче. CI дополнительно собирает/запускает comics-layout, columns и chapter-parity.

## Адаптация к beta10

Не заменён старой версией весь ColumnView. При конфликте двух добавлений сохранён
beta10 `chapterColumns()` и `groupColumns(..., chapterStartsSpread)`, включая forced-break
логику. В новых hit-test ветках старый RAII `Position` заменён на beta10 `ScopedColumn`,
сохраняющий фактическую физическую колонку, а не ближайшую текстовую закладку.
Это перенос comics hunks, не откат новой chapter-spread функциональности upstream.

## Проверки и ограничения

Локально: чистое последовательное применение и десять Python-проверок серии.
Нового Qt toolchain локально нет; native runtime и Qt5 compile/package должны пройти CI.
OPF/layout unit test не заменяет pixel/RTL/anchor/spread runtime tests полного движка.
Не объявлять этот патч готовой поддержкой комиксов по одному clean apply.

## Следующий порядок внутри 030-группы

- 031-comics-engine: подключение к EngineDocument/reader_source, image-request trace,
  visible-image completion, painting, preview/cache lifecycle и полные synthetic comics tests.
- 032-comics-integration-refresh: appearance/session/UI wiring и graphics refresh.
  Учитывать текущую beta10 refresh architecture, не копировать старые animation/dark hunks.
- 040: согласованный toolbar; не примешивать туда ещё не перенесённые renderer зависимости.

Это зависимые последовательные слои одного полного стека, не автономные пользовательские
модули. Hybrid/opening acceleration и устаревший Libra2 mapping не возвращаются.
Russian_EnUS.pattern остаётся исключённым.


## Завершение группы 030–032

CI 45278312375: prefix application, native comics/layout/UI/resources/appearance,
обычные columns/chapter-parity, Qt5 build/package и qemu refresh PASS.
[Объём 031/032 и аппаратные ограничения](comics-engine-integration.md).
