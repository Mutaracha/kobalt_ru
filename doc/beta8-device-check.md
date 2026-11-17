# beta.8 — завершение проверки

Переход и задачи beta.8 завершены по решению пользователя. Иконка comics принята;
индексы сносок работают верно, текущие отступы оставлены без изменений.

Runtime: `68b3ac16d592207b467f0778cf1f6742867844a1`.
[Focused native и Kobo Qt5 package PASS](https://github.com/Mutaracha/kobalt_ru/actions/runs/44075570386).
Это не свидетельство all-variant прогона. Старые build-checklists удалены, а не
объявлены задним числом выполненными. Единственная открытая диагностика —
зависание после frame load; см. [план](../FB2_FORK_PLAN.md).

## Перед установкой

Полностью выйти из Kobalt в систему. Сделать резервную копию каталога профиля
Kobalt, включая базу `kobalt.sqlite`, настройки и имеющиеся журнальные файлы.
Не копировать открытую базу во время работы приложения. Удобнее сохранить всю
папку `.adds/kobalt` с выключенным Kobalt. Хранить копию отдельно от устройства.

Beta.8 переводит базу в schema 4. Старые сборки не читают обновлённую базу:
для отката нужны **старая программа и соответствующая резервная копия профиля**.
Первое открытие может потребовать перепагинации.

## Установка

1. Скачать артефакт, извлечь из него `Kobalt-qt5.zip`, затем распаковать этот ZIP.
2. По USB скопировать `.adds` в корень диска Kobo, затем `.kobo`, с объединением
   папок и без удаления других модификаций.
3. Безопасно извлечь Kobo и дождаться установки `KoboRoot.tgz`.

## Рефакторинг модулей — отдельная проверка

Runtime `d9536352eb0264127f1a53815b7533d413bb4884`:
[расширенный focused native + Kobo Qt5 package/contents PASS](https://github.com/Mutaracha/kobalt_ru/actions/runs/44089641669).
[Пакет Qt5](https://github.com/Mutaracha/kobalt_ru/actions/runs/44089641669/artifacts/14331407822).
Изменено размещение алгоритмов, не настройки/вид/формат хранения. Пользователь
подтвердил запуск на устройстве после первого этапа и разрешил продолжить.
Это startup-проверка, не новая полная визуальная/функциональная приёмка;
all-variant не проводился. Предыдущая
пользовательская приёмка иконки и сносок сохранена, но не переименована в новую.
Карта модулей и оставшихся интеграционных участков: [fork-delta](fork-delta.md).

### Второй этап: RU-policy, CSS и single-file FB2

Вынесены `string_locale.h` / `string_table.h`, `reader_fork_styles.*`,
`publication/fb2_resources.*`. Схемы, salts, настройки и ресурсы EN/RU не меняются.
Собранный CSS нижних сносок побайтово совпадает с предыдущим этапом; новые
native assertions фиксируют digest трёх перенесённых фрагментов. К native CI
добавлена `model.local-resources`, включая границы worker-only transfer.
Пользователь затем подтвердил запуск второго этапа на устройстве и проверку FB2:
обнаружены два ещё не описанных недочёта. Это не полная функциональная приёмка;
исправления отложены пользователем до завершения рефакторинга.

Runtime второго этапа: `69c70aa357209d0ef9ffce5afc68b42d74492a5f`.
[CI №171: native, offline checks и Qt5 package/contents PASS](https://github.com/Mutaracha/kobalt_ru/actions/runs/44096144408).
[Qt5 artifact](https://github.com/Mutaracha/kobalt_ru/actions/runs/44096144408/artifacts/14333753710).
Native включает новые CSS digest и transfer-boundary assertions, RU table/fallback/
plural/key-mode и прежние focused regressions. Не all-variant. При проверке пакета
сравнить запуск/EN-RU, FB2/EPUB notes и off/on, reopening/cancellation; это checklist,
не уже выполненная физическая проверка.

### Третий этап: общий draw buffer и fork comics paint

`PageDrawBuffer`/EquationImage/IllustrationProbe перенесены без дублирования в
`page_draw_buffer.*`; алгоритмы comics paint/request tracking/visible priority —
в `engine_document_comics.cpp`. До переноса общий буфер совпадал с pinned beta.8;
после переноса сверены тела методов и helpers. Нет изменения настроек, CSS,
схем, cache salts и waveform. Native/Qt5 CI этого этапа прошёл; устройство
пока не проверено.
Два новых FB2-недочёта не считаются исправленными и не закрыты прохождением CI.

Runtime третьего этапа: `9e03e25d31bfc000a88048c01f102b16e57d6a8f`.
[CI №172 / 44100128546](https://github.com/Mutaracha/kobalt_ru/actions/runs/44100128546)
**PASS**: expanded focused native (включая image-proportions, mathml и buffer
contract в comics), offline checks, Qt5 package/contents.
[Qt5 artifact](https://github.com/Mutaracha/kobalt_ru/actions/runs/44100128546/artifacts/14336795926).
Отдельный RTL-сценарий, all-variant и физическая проверка этого runtime не проводились.
Проверять обычную EPUB/FB2 страницу и обложку, светлую/тёмную тему, comics
single/pair/center и image-viewer hits; это невыполненный device checklist,
не расширение предыдущего подтверждения запуска.

### Совместная проверка этапов 3+4

Четвёртый этап завершает вынос effective layout, raw FB2 inspection, request trace,
progress geometry и fork toolbar presentation. CI pending. Проверка на устройстве
pending: пользователь согласовал совместный прогон этапов 3+4.
Проверить FB2/EPUB открытие/повторное открытие, обычный рендер/обложку/изображения,
comics single/pair/center и image-viewer hits, смену ориентации, on/off с изменением
полей/колонок, top/bottom/off progress, EN/RU вкладки и toolbar/theme/hide toggle.
Отдельные RTL/all-variant не выполнены. Два недочёта FB2 не считаются исправленными.
