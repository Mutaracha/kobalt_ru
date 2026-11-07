# Интеграция beta6 — рабочий статус

## База и выполненная подготовка

- Fork до обновления: `2f127ba`; upstream: `v1.0-beta.6`, `7ff437e`.
- Общая база подтверждена командой `git merge-base`: `3d8514c`.
- Подготовительный коммит `b602d57`: удалены fork quiet-light implementation и cache cleanup (включая helper, worker retirement, UI, специальные тесты). Данные устройства не удаляются.
- Согласованные уточнения RU и предварительная проверка длины: [отчёт](translation-length-beta6.md). Полный новый словарь ещё не внесён.

## Пробное слияние

Пробный `git merge --no-commit --no-ff v1.0-beta.6` выполнен, конфликты изучены и слияние отменено без потери подготовительных изменений. В рабочем дереве нет незавершённого merge. Конфликтные маркеры не являются готовым кодом и не отправлялись.

Группы ручного переноса:

1. **Перемещения:** `src/interfaces/reader` → `src/app/reader`, общие controls → `src/ui`; Kobo `qt5/qt6` → `nickel-qt5/nickel-qt6`. Перенести fork_debug и return_title_text с исправлением include paths. Старые settings_panel/bookmark_list/sleep_screen/reader_refresh нельзя просто сохранить рядом с новой реализацией.
2. **Сохранение/открытие:** взять upstream progress/save; в native_publication оставить FB2 guard против применения EPUB chapter location. Сохранить resolveLocale в accepted opening boundary и ранней инициализации.
3. **FB2:** совместить engine_document/source/links/alignment/local_resources/publication и appearance state с beta6. Применять общую эволюцию EPUB formatting; сохранить default-on нижние сноски, preprocessing и cache-policy salt только для FB2.
4. **UI:** новые настройки beta6 совместить с общей фабрикой оформления строк, инверсией свайпов, нижними сносками и неизменной строкой About/debug gesture. Сохранить общую baseline русских вкладок и search/theme order. Новую upstream обработку animation support не заменять старым Boolean.
5. **Sleep:** взять новые режимы и расчёт времени beta6, сохранив только необходимую небольшую RU/layout дельту. Удалить ложное утверждение «Все варианты показывают прогресс» из обеих локалей.
6. **Assets/build:** объединить RU generation/parity с новыми embedded assets; обновить manifest до beta6, сохранив Russian hyphenation entries. В build.sh совместить diagnostics/check-mode upstream с KOBALT_FORK_NAME; сборку Qt5 проверять в CI.
7. **Тесты:** сохранить и адаптировать FB2/locale/baseline tests вместе с новыми upstream tests. Не восстанавливать удалённые cleanup/quiet tests. Не считать package CI выполнением native lifecycle/UI suites.

## Проверки подготовки

- Штатный `kobalt_check_string_parity`: PASS (ключи и кратность параметров; порядок параметров может отличаться в переводе).
- `git diff --check`: PASS.
- Production offline suite воспроизводит прежний timestamp flake strings-test.cmake:69. Production scripts не изменялись.
- Полный offline suite с изолированной копией strings-test (ожидание 5 секунд + touch входного fixture): PASS.
- Первый Qt5 CI подготовки обнаружил забытые cacheRow layout calls; исправлены отдельным изменением. Повторный CI [42396464431](https://github.com/Mutaracha/kobalt_ru/actions/runs/42396464431) для `720d58a` — PASS, 4m52s; artifact `Kobalt-qt5-device`. Это подготовительная сборка, не beta6.
- Native Qt/UI/device tests не запускались: SDK отсутствует. Интеграция beta6 пока **не завершена**.

## Уточнение архитектурного приоритета

Пользователь подтвердил: во время активной разработки upstream важнее уменьшать
стоимость следующих переносов, чем число добавленных строк. Поэтому разрешён
более объёмный, но связный код специфики форка с небольшим числом явных точек
подключения. Это уточняет прежнее требование минимального вмешательства:
минимизировать нужно рассеянные правки и повторное разрешение конфликтов, а не
строки любой ценой.

- Следовать новой структуре upstream; не оставлять рядом старые экраны и каталоги.
- Изолировать форматные политики FB2 и локализацию/debug, сохраняя явное владение.
- Общие настройки, оформление и сохранение брать из upstream, не замораживать их
  копии внутри FB2-модулей. Форматные отличия подключать к общей реализации.
- Не вводить универсальный plugin framework или множество мелких hooks без
  конкретной необходимости. Для каждой точки подключения фиксировать назначение
  и регрессионную проверку.
- Разделять структурный перенос и изменения поведения при проверке diff.

Это согласованное требование к дальнейшей реализации; архитектурная переработка
и интеграция beta6 пока не объявляются выполненными.

## Кандидат переноса (2026-11-07)

После пробного merge выполнено фактическое разрешение конфликтов и перенос кода
в новую структуру. Историческое описание отменённого пробного слияния выше
сохранено; оно больше не описывает текущий этап. Текущий этап — кандидат beta6,
требующий Qt5 compile/package verification и заключительного аудита.

Форматные FB2 helpers остаются рядом с EngineDocument; общий appearance/formatting
идёт из beta6. Fork presentation/About attachment вынесены в fork_settings, а
разделение ribbon caption — в sleep_ribbon_text. Новые UI/переводы требуют
проверки на устройстве; успешная генерация строк не доказывает отсутствие clipping.
