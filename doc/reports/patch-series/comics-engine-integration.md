# Комиксы: последовательные патчи 030 → 031 → 032

> Поэтапный отчёт: статусы относятся к указанному runtime. Текущее состояние и
> пакет — [сводный журнал](README.md), действующие проверки — [beta10](../../validation/beta10-device.md).

## Реализованный объём

031-comics-engine подключает effective comics layout к EngineDocument, source
markers для OPF center pages, drawing/spreads/RTL/hit identity, visible-image
request tracing, frame completion, preview и mode-sensitive parsed cache.
Сохраняются исходные поля и настройка колонок: режим накладывается только на
эффективную геометрию. Выключение восстанавливает последние базовые значения.

`PageDrawBuffer` вынесен из текущего beta10 engine_document в общий header без
подмены старым fork-буфером: сохранены row-stride для zero-copy колонок, таблицы,
visible image counters, нормализация текста, dark image policy и clipping.
Обычная zero-copy отрисовка beta10 остаётся прежней веткой, comics — отдельной.

032-comics-integration-refresh добавляет:

- per-book настройку `rrComicsReadingMode` в Layout с двумя новыми EN/RU строками;
- requested appearance, content-mismatch сохранение независимого выбора, запрет
  наследования режима следующей книгой через глобальные предпочтения;
- ReaderSession capture через frameImage, чтобы публикуемый spread включал обе
  видимые картинки, не ожидая загрузки всей очереди соседних страниц;
- draft/apply/reflow геометрию ReadingPage: нулевые поля и скрытые chrome strips,
  не перезаписывая сохранённый header/footer layout;
- Qt5 graphics refresh через существующий repairPolicy только над submitted damage.
  Это не новый whole-screen flash и не замена upstream animation/dark policy.

Новая beta10 `chapter_starts_spread`, tick placement и отдельные progress edges
сохранены. Graphics mode скрывает оба progress edge, а после выхода возвращаются
исходные upstream настройки. Старые fork chapter labels/ticks не возвращаются.

## Регистрация и тесты

В 031 зарегистрирован полный `engine.comics` synthetic suite: geometry/rotation,
prose, centered pages, cache, RTL, visible/deferred image pairs, bounded requests.
Дополнен `model.resource-cache` тестами trace/priority/queue/reset. Контракт количества
appearance keys обновлён в том же вводящем патче с учётом beta10 chapter-start key.

В 032 добавлен `component.comics-ui`: checkbox, hidden chrome, draft images и
включение/сброс graphics property. `model.appearance-settings` проверяет per-book
изоляцию, canonical false, Requested/Patch и content mismatch. Qt5 fake-display
тест расширен проверкой graphics waveform и возврата к обычной политике; CI строит
его в Kobo SDK и запускает под qemu (`pewter.qt5-refresh`). Старые adapter tests
для i.MX/dark/animation не удалены.

## Проверки перед CI

- 10 Python series tests PASS, включая фактический export 010–032.
- CMake EN/RU key/placeholder/production-reference check PASS (1810 пар ключей).
- CMake runtime asset manifest hashes PASS.
- Generated patches: git diff --check PASS.
- Native C++ runtime, Qt5 package и qemu adapter tests PASS: CI 45278312375.

В предыдущем 030 CI обнаружено несовпадение const API LVCreateMemoryStream в
тестовом fixture; исправлено без отключения теста. Сборка beta10 test package
ранее проходила, но collection использовал неверное имя ZIP: upstream по умолчанию
включает diagnostics и создаёт Kobalt-qt5-diagnostic.zip. Имя исправлено, diagnostics
явно ON; результат остаётся Beta10-series-prefix-qt5.zip с provenance.

## Границы

Toolbar composition/кнопки comics и dark остаются 040; сейчас режим доступен через
Layout. Qt6 firmware/Android и физическое устройство не объявляются проверенными
по native/Qt5 CI. FB2.ZIP, Russian_EnUS.pattern, собственные Hybrid/opening fixes,
устаревший Libra2 mapping не включаются. Полный форк всё ещё требует 040–060.


## Подтверждённый результат

Коммит проверяемого содержимого: `08c66e4d7f7f6f259ff45c1bfc91ffcc1e056ff6`.
[Actions 45278312375](https://github.com/Mutaracha/kobalt_ru/actions/runs/45278312375):
все три jobs SUCCESS, включая:

- raw FB2 engine/resources/archive и меню открытия;
- model.appearance-settings и model.resource-cache;
- engine.comics-layout, engine.comics, engine.columns, engine.chapter-parity;
- component.comics-ui;
- Qt5 build/check-package;
- pewter.qt5-refresh под qemu в Kobo SDK;
- публикацию `Beta10-series-prefix-qt5` (artifact 14868097752).

Дополнительная ошибка первого 032-прогона: отсутствовал stylePanelCheckbox из
смешанного старого UI-файла. В 032 добавлены helper declaration/implementation и
масштабирование tick к фактическому размеру индикатора; следующий CI PASS.
Тесты не отключались и compile errors не обходились.

030/031/032 переведены в ready как проверенные этапы серии. Это завершение
подготовки группы 030–032, но не hardware acceptance и не готовность 040–060.
Следующий шаг по запросу пользователя — 040; в этой работе он не начинался.
