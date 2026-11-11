# Проверка следующей сборки: progress, comics refresh и диагностика

Этот план не объявляет прошедшими CI или проверку устройства. Нужен новый пакет
из текущего runtime commit после успешного Qt5/native CI; записать SHA/номер сборки.

## Подготовка

1. Сохранить старую diagnostics целиком; закрыть Kobalt, затем очистить только
   events.jsonl. Не удалять настройки, parsed/layout/resource caches и книги.
2. После блока закрыть Kobalt и сохранить diagnostics. Допустим общий
   архив A → B с указанным порядком и временем блоков. Сохранить events.jsonl, session-context.json, session.json,
   state.json вместе. Для сравнения кадров нужны и фото физического экрана.
3. Записать устройство/firmware, SHA, названия/форматы/размеры тестовых книг.
   Для каждого открытия записать номер попытки и секундомер: от нажатия до
   пригодной для чтения страницы. Новые opening_id уникальны в пределах процесса,
   после перезапуска различать их по startup/pid. Не путать open-end с reader-shown.

## A. Строка режима комиксов

Проверить EN/RU, portrait/landscape: утверждённая подпись не обрезается, строка
не имеет лишней минимальной высоты, индикатор/текст не перекрываются, область
нажатия работает. Режим запоминается для книги, default off в другой книге.
При enabled менять поля и колонтитулы: сохраняются, но не применяются. После
выключения возвращаются последние значения. Повторить после сна и reopen.

## B. Полоса прогресса

В Колонтитулах выбрать Выключено → Внизу → Вверху → Выключено. Старое включённое
состояние должно остаться внизу, старое выключенное — выключенным.
Проверить верхнюю полосу с header on/off и разными размерами текста, с footer
on/off, с нижними сносками и без, после смены шрифта и поворота. Сверху полоса расположена НАД колонтитулом; ticks направлены вниз.
Снизу ticks направлены вверх. Снизу нет резервации второй полосы. Fill и ticks обновляются при
каждом ходе вперёд/назад и jump. Менять положение на той же странице: не должно
оставаться старой полосы. Проверить миниатюру/предпросмотр и закрытие меню.
При режиме комиксов оба положения скрыты; выключение режима возвращает выбор.

## C. Комиксы и обновление экрана

Одинаковая последовательность страниц: тёмная → светлая → тёмная, затем назад,
переход к другой главе/странице, открытие/закрытие меню и возврат из image viewer.
Минимум 20 перелистываний без ручного очищающего обновления. Сравнить режим
комиксов off/on с неизменённой общей настройкой screen refresh. Сделать фото
физического экрана + обычный screenshot на проблемной паре страниц.

Qt5 now requests graphics quality waveform over existing submitted damage when
comics is active, rather than setting unconditional whole-screen reader_full_flash.
The hardware may still flash for this waveform; no promise of flash-free output.
Events comic-refresh-submit report actual submission-path bounds/first policy
for normal comic turns; absence does not prove no refresh (other paths e.g. an
explicit full-flash return earlier). Compare cleanliness, visible flash and turn
latency against Nickel. If ghosting remains, keep the exact sequence and firmware;
do not infer waveform correctness from the screenshot alone.

## D. Открытие и кэши — без очистки кэшей

На Kobo штатное закрытие книги означает выход из Kobalt в Nickel. Нет
внутреннего переключения книг. Каждый пункт ниже выполняется запуском книги
из системы; resident engine/retained hit не является обязательным результатом.
A и B можно проводить последовательно в одном events.jsonl, сохраняя порядок
и время начала каждого блока; отдельные файлы не обязательны.

Для каждой из трёх текстовых книг и одной манги:
1. Открыть, дождаться загрузки, закрыть штатно, подождать завершения сохранения.
2. Повторить 3 раза, не меняя шрифт/раскладку/колонтитулы/режим.
3. Для одной книги изменить размер текста один раз и повторить ещё два открытия.
4. Отдельно перезагрузить устройство и повторить. Каждый предыдущий
   reopen уже включает выход в Nickel и запуск Kobalt.

Сопоставлять open-start/opening-phases/open-end по opening_id и порядку попыток.
Смотреть retained_book_hit / layout_cache_hit / document_cache_hit отдельно.
Для document-cache-save смотреть reason: unkeyed, source-name-mismatch,
cache-unavailable, update-incomplete, rename-failed, resource-error, no-pages,
no-engine или saved. Это причины возврата нашей обёртки, не точные errno движка.
Новые причины промаха самого LoadDocument пока не инструментированы.

Не суммировать вложенные model/resource timings. Сравнивать reader-shown total,
build_settings/build_appearance/build/show spans и wall time. Никаких обещаний
speedup: отложенное создание панелей и изменение image-cache budget не входят
в этот патч и требуют отдельного безопасного изменения с тестами.

## E. Нижние сноски: ещё незавершённая задача

До изменения движка снять page screenshot при 100/150/200% интервала и двух
размерах шрифта для FB2 и EPUB: одна сноска, две, длинная с переносом. Нужны
исходные книги или минимальные примеры. Измерять последнюю строку основного
текста, разделитель и первую строку сноски. Сохранить tap/popup, selection и
переходы по номеру. В этом патче позиционирование сносок НЕ изменено.

Причина остаточного места подтверждена кодом lvdocview.cpp: renderer добавляет
h_avail-footnotes_height перед первой сноской. Просто убрать CSS margin или
изменить FOOTNOTE_MARGIN_REM недостаточно: нужен согласованный перенос draw,
hit testing и pagination/reservation, с регрессиями EPUB/FB2. Не переносить
сноски одним draw-only сдвигом, который ломает ссылки/выделение.


## Verification of 0533a90

[Run 43090289292](https://github.com/Mutaracha/kobalt_ru/actions/runs/43090289292)
SUCCESS: native regressions (including Top/Off routed UI, header/footer optional
field, comics, FB2/EPUB/cache and cache-save reason check) and Qt5 packaging /
offline/package checks PASS. Qt6 intentionally skipped.
Qt5 artifact [13888743153](https://github.com/Mutaracha/kobalt_ru/actions/runs/43090289292/artifacts/13888743153),
36,429,810 bytes; native results 13890060155. Device refresh quality is not
proved by CI. No opening-time speedup, footnote gap fix or lazy UI construction
is claimed. These last two remain open; test plan explicitly separates them.


## Follow-up device acceptance

The user confirms no ghosting; preserve the existing waveform behavior.
With comics enabled on ordinary FB2/EPUB, prose must remain readable, without
zero-leading overlap (including settings preview, reopen and mode off/on).
Landscape: two portrait source pages side by side; a wide source spread alone
at full viewport width; no rotation/stretch/crop. Check pair → wide → odd page,
backward turns, portrait/landscape round trips and saved position after reopen.
This supersedes the former one-source-page-in-both-orientations requirement.
