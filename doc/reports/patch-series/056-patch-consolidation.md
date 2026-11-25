# Консолидация патчей без переработки runtime

Эталон: **5b45f2c7**, CI45686709893; метаданные старой серии доступны на **b45add66**.
Новая серия:29 →7 patches. Локальное сравнение: **1551/1551 файлов совпадают** по путям, байтам и executable/link mode;179 итоговых отличий от upstream имеют одного владельца.

## Соответствие старых слоёв итоговым

| Старый патч | Новые владельцы его сохранившихся файлов |
|---|---|
| 010-ru | 010-ru-localization, 040-reader-controls-settings, 050-presentation, 060-build-integration |
| 020-fb2-publication | 020-publication-resources, 060-build-integration |
| 021-fb2-resources | 020-publication-resources, 060-build-integration |
| 022-fb2-engine | 030-engine-integration, 060-build-integration |
| 023-fb2-platform-inputs | 020-publication-resources, 034-session-hybrid, 040-reader-controls-settings |
| 030-comics-layout | 030-engine-integration, 060-build-integration |
| 031-comics-engine | 020-publication-resources, 030-engine-integration, 034-session-hybrid, 040-reader-controls-settings, 060-build-integration |
| 032-comics-integration-refresh | 010-ru-localization, 030-engine-integration, 034-session-hybrid, 040-reader-controls-settings, 050-presentation, 060-build-integration |
| 033-fb2-session-cache | 034-session-hybrid, 060-build-integration |
| 034-fb2-startup-experiment | 030-engine-integration, 034-session-hybrid |
| 036-fb2-cache-layout-observation | 030-engine-integration, 034-session-hybrid |
| 037-fb2-cross-process-cache-probe | 034-session-hybrid |
| 038-fb2-archived-source-replay | 034-session-hybrid |
| 039-fb2-layout-input-observation | 030-engine-integration, 034-session-hybrid |
| 040-reader-controls | 010-ru-localization, 040-reader-controls-settings, 050-presentation, 060-build-integration |
| 044-fb2-hybrid-cancellation-tests | 030-engine-integration |
| 045-fb2-hybrid-note-image-tests | 030-engine-integration |
| 046-fb2-session-hybrid-gates | 034-session-hybrid |
| 050-fork-presentation | 010-ru-localization, 040-reader-controls-settings, 050-presentation, 060-build-integration |
| 055-ru-noto-display-subset | 010-ru-localization, 050-presentation |
| 056-ru-serif-optical-scale | 010-ru-localization, 040-reader-controls-settings, 050-presentation |
| 057-ru-shantell-italic | 010-ru-localization, 040-reader-controls-settings, 050-presentation |
| 058-ru-reading-font-roles | 010-ru-localization, 030-engine-integration, 040-reader-controls-settings, 050-presentation |
| 059-ru-bundled-fonts-and-picker | 010-ru-localization, 040-reader-controls-settings, 050-presentation |
| 059a-system-font-cache-identity | 030-engine-integration |
| 059b-reading-menu-status | 010-ru-localization, 040-reader-controls-settings, 050-presentation, 060-build-integration |
| 059c-sleep-letter-wordmark | 050-presentation |
| 059d-settings-and-letter-polish | 010-ru-localization, 040-reader-controls-settings, 050-presentation |
| 060-build-integration | 060-build-integration |

Полное соответствие каждого файла — `platforms/patch-series/equivalence/file-map.json`. Старый патч мог трогать несколько областей; shared integration-файлы теперь целиком лежат у одного владельца. Это не заявление, что все строки файла принадлежат одной функции.

## Что сохранено

- Та же upstream-база, та же полная функциональность и тесты; исходные файлы не редактировались.
- Итоговые RU-шрифты перенесены в010; промежуточные Literata/Lora/Caveat не добавляются с последующим удалением.
- Общая engine/font/cache identity коррекция находится в030, а не под локализацией.
- Общие reader/settings/status wiring040; Instrument/sleep/brand presentation050; CMake060.
- Старые патчи не дублируются в активной папке. Полностью восстановимы через Git snapshot b45add66; previous-series.json содержит их paths/hashes. Исторические отчёты неизменны.
- Исторический standalone control RU overlay сохранён отдельно; активный010 — финальная локализация полной серии, не дополнительный слой поверх control.

## Проверка

verify_equivalence.py сравнивает весь чистый export с reference-tree.json и отвергает missing/extra/changed bytes/modes/links. Регрессионные тесты проверяют сам verifier. Nested-export regression в CI применяет всю серию и выполняет сравнение до сборочной provenance.
ELF/ZIP будут иметь новый commit/provenance и не объявляются побайтно равными старому пакету. Функционального рефакторинга, изменения guards кэша, шрифтов или настроек не было.

## Приёмка и открытые наблюдения

Пользователь принял последние UI-доработки5b45f2c7: вкладки, language popup и формулировку Letter. Menu-status ранее подтверждён с первой минуты. Кэш: несколько перезапусков, включая reboot/Accurate, без повторения задержки; длительное наблюдение остаётся открытым. Отложенные EPUB recheck/неподтверждённый freeze не превращены в выполненные задачи. Непроверенные экраны не принимаются автоматически.
