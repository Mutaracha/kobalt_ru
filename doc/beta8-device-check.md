# beta.8 — завершение проверки

Переход и задачи beta.8 завершены по решению пользователя. Иконка comics принята;
индексы сносок работают верно, текущие отступы оставлены без изменений.

Runtime: `68b3ac16d592207b467f0778cf1f6742867844a1`.
[Focused native и Kobo Qt5 package PASS](https://github.com/Mutaracha/kobalt_ru/actions/runs/44075570386).
Это не свидетельство all-variant прогона. Старые build-checklists удалены, а не
объявлены задним числом выполненными. Актуальные ошибки FB2 и старая диагностика
зависания после frame load перечислены в [плане](../FB2_FORK_PLAN.md).

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
progress geometry и fork toolbar presentation. CI PASS. Проверка на устройстве
pending: пользователь согласовал совместный прогон этапов 3+4.
Проверить FB2/EPUB открытие/повторное открытие, обычный рендер/обложку/изображения,
comics single/pair/center и image-viewer hits, смену ориентации, on/off с изменением
полей/колонок, top/bottom/off progress, EN/RU вкладки и toolbar/theme/hide toggle.
Отдельные RTL/all-variant не выполнены. Два недочёта FB2 не считаются исправленными.

Runtime финального этапа: `0e3e9f98bddc9e1ac219b04fe76d895c30e11f4b`.
[CI №173 / 44102979954](https://github.com/Mutaracha/kobalt_ru/actions/runs/44102979954)
**PASS**: focused native EPUB/FB2 regressions, включая новые assertions layout/hash/trace,
offline checks и Kobo Qt5 build/package/contents. Qt6 пропущен.
[Пакет Qt5 для совместной проверки этапов 3+4](https://github.com/Mutaracha/kobalt_ru/actions/runs/44102979954/artifacts/14336659474).
[Native results](https://github.com/Mutaracha/kobalt_ru/actions/runs/44102979954/artifacts/14338460061).
Устройство, отдельный RTL и all-variant не проверены; два FB2-недочёта не исправлялись.

### Пользовательская приёмка финального рефакторинга

Пользователь подтвердил runtime `0e3e9f98bddc9e1ac219b04fe76d895c30e11f4b`
по совместному checklist 3+4: FB2/EPUB открытие, comics/ориентация, восстановление
полей, progress top/bottom и панель чтения. Рефакторинг принят и закрыт.
Новые сообщения FB2 (smart chapter refresh, наложение при paragraph normalization,
Nickel status/percent) — отдельные ошибки, а не отмена приёмки выноса модулей.

### Проверка исправления Nickel FB2

Runtime `5eb8a600f71b5f46794a8c469783577f2ca01eed`.
[CI №175 / 44111989597](https://github.com/Mutaracha/kobalt_ru/actions/runs/44111989597)
**PASS**: focused native, включая session.reading-book с FB2 identity и activity-only
writeback, model.screen-refresh, engine.fb2 с новым chapter/eligible Smart contract;
offline checks и Kobo Qt5 build/package/contents. Qt6 пропущен, all-variant не выполнен.
[Qt5 artifact](https://github.com/Mutaracha/kobalt_ru/actions/runs/44111989597/artifacts/14340793511).
[Native results](https://github.com/Mutaracha/kobalt_ru/actions/runs/44111989597/artifacts/14342042111).
Device-проверка исправления Nickel pending. Исходный баг Smart/normalization не
воспроизведён на книге пользователя; синтетический тест не закрывает эти пункты.
Предыдущий CI №174 остановился на старом bool assertion теста NativeSaveResult;
он исправлен, №175 полностью прошёл. Новых production rendering изменений нет.

### Проверка исходной книги пользователя

Пользователь добавил `test & diagnostic/1. Виртуальный свет.fb2` коммитом
`d8cbfab5`, который сохранён в истории без перезаписи. Проверка runtime
`116024cd86de9c9085d18d9c53ddf4014fd3079a`:
[CI №176 / 44114141676](https://github.com/Mutaracha/kobalt_ru/actions/runs/44114141676)
**PASS**, focused native + Qt5 package/contents. Дополнительный `--reported-fb2`
прогон проверяет нумерованные главы исходной книги (chapter page 1) и соседние
параграфы screenshot passage главы 9: непересечение DOM boxes и одинаковые
пагинация/paint при удалении уже нулевых gaps в нескольких высотах, font 48px,
line spacing 130%, viewport width 1180. Это ограниченный regression, не проверка
всех настроек и не доказательство отсутствия аппаратного/отрисовочного дефекта.
[Актуальный Qt5 пакет](https://github.com/Mutaracha/kobalt_ru/actions/runs/44114141676/artifacts/14343185395).
[Native results](https://github.com/Mutaracha/kobalt_ru/actions/runs/44114141676/artifacts/14342472260).
Наложение и пропуск Smart на этих параметрах не воспроизведены; нужны точные
параметры пользователя/диагностика после повторения и конкретный переход главы.
Production-исправление этого пакета относится только к Nickel activity-only FB2.


## FB2: изоляция языка книги от предпросмотра — сборка №180

Runtime: `1b091187878695080a8dd8ebf42f6d7a84bc9d19`.
[CI №180](https://github.com/Mutaracha/kobalt_ru/actions/runs/44135643021):
focused native EPUB/FB2 regressions и Kobo Qt5 build/package/contents **PASS**.
[Скачать Kobalt-qt5-device](https://github.com/Mutaracha/kobalt_ru/actions/runs/44135643021/artifacts/14350784926).
Qt6, Android, all-variant и физическое устройство не проверены.

Подтверждённый механизм: открытие другого документа меняло глобальный fallback
языка CREngine; при повторном форматировании некоторые абзацы исходной книги
получали другую высоту, чем при pagination. До исправления replay исходной книги
с Fira Sans давал разницу 94 px после английского preview. Теперь metadata-язык
FB2 закреплён на FictionBook DOM до pagination; явные root/локальные lang
сохраняются. Регрессии проверяют переключение ru↔en и normalization spacing off/on,
включая повторное форматирование до и после repaint. Использован тестовый Fira
Sans, не доказанно идентичный пользовательскому файлу; viewport оценён по скриншоту.
Это исправление воспроизводимого рассогласования, не подтверждение устранения
конкретного наложения главы 8 на ридере.

После резервного копирования и установки по инструкции выше:

1. Открыть «Виртуальный свет» с прежними настройками. FB2 cache v9 автоматически
   требует нового parse/layout, вручную удалять кеш/профиль не нужно.
2. Проверить прежний участок главы 8 («На другое утро»), в частности строку
   «Дальше самый трудный участок пути…». Номер страницы после перепагинации
   может отличаться от 172/720.
3. Открыть оформление, дождаться preview, закрыть без изменений. Пролистать
   несколько страниц вперёд/назад и снова проверить участок. Повторить после
   выхода в систему и нового открытия книги.
4. Если наложение сохранилось, прислать скриншот и events.zip этого прогона,
   указав сборку №180; задача остаётся открытой до device-подтверждения.

Сноски, размеры/отступы, refresh waveforms и принятые Nickel/Smart исправления
не менялись. Старое зависание после frame load этим изменением не закрыто.


## FB2: сохранение позиции предпросмотра — сборка №181

Runtime `c0ff0115eae5557b4f093176b1ca9c100d4416d4`.
[CI №181](https://github.com/Mutaracha/kobalt_ru/actions/runs/44154435468): focused
native EPUB/FB2 и Qt5 build/package/contents **PASS**.
[Скачать Kobalt-qt5-device](https://github.com/Mutaracha/kobalt_ru/actions/runs/44154435468/artifacts/14360732277).
Исходная потеря позиции и область теста: [отчёт](reports/fb2-preview-position.md).
Пользователь проверил сборку №181 на устройстве и подтвердил решение всех
поставленных проблем FB2. Qt6/Android/all-variant не проводились.

После резервного копирования и установки по инструкции выше:

1. Открыть книгу посреди раздела, например прежний участок главы 8. Запомнить
   первые слова видимой страницы.
2. Открыть «Текст», дождаться предпросмотра. Изменить толщину 0 → 1 → 2 → 0.
   Должен сохраняться тот же фрагмент, без прыжка к началу раздела. Сравнивать
   исходный текст, а не побайтовое изображение: толщина меняет рисунок букв.
3. Закрыть меню: позиция основной книги должна сохраниться. Снова открыть
   «Текст» и проверить первую правку ещё раз.
4. Если скачок сохранился, прислать скриншоты и events.zip со сборки №181.

Новая перепагинация/удаление кеша для этого изменения не требуются; исправлена
позиция отдельного preview. Исправление изоляции языка сборки №180 сохранено.


### Итог device-приёмки FB2

После проверки сборки №181 пользователь подтвердил: все поставленные проблемы
FB2 решены. Закрыты наложение строк, скачок фрагмента предпросмотра при первой
правке, ранее принятые Nickel progress и Smart chapter refresh. Дополнительный
прогон или новая сборка для этой фиксации не требуются. Старая диагностика
зависания после frame load остаётся отдельным открытым пунктом плана.
