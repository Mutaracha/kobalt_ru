> Продолжение: [повторяемый reboot/cache review 8e03668](034-reboot-cache-review.md).

> Поэтапный отчёт: статусы относятся к указанному runtime. Текущее состояние и
> пакет — [сводный журнал](README.md), действующие проверки — [beta10](../../validation/beta10-device.md).
> Пользователь подтвердил идентичность входов между сериями. После полного
> расчёта и reopen счётчик в проверенном сценарии обновляется правильно;
> прежний code-review риск публикации не является доказанным зависанием.
> Подтверждена утечка промежуточного total=46 на первые две страницы.

# 034 / ace3c3ce: первый device review

## Источники и границы

Пользователь добавил `cfa10b923449b72bb93f8639ab49996cff86c448`:
`test & diagnostic/ace3c3c/diagnostics.zip`. Анализируется его events.jsonl
(9044 события), session/context/state и пять пользовательских скриншотов.
Порядок сообщил пользователь: 1 HYBRID, reopen; 2 HYBRID, два reopen;
1 ACCURATE и 2 ACCURATE — первое и повторное открытие.
В начале лога есть другая книга; её события до 15:03 не входят в сравнение.
UTC в таблицах — буквально время устройства, не дата проведения анализа.

## Холодное открытие: выигрыш уже виден

Пары связаны по пользовательскому порядку, последовательности открытия,
размеру FB2 ресурса и признаку fb2_startup_slice. Это по одному наблюдению,
не рандомизированный benchmark. Настройки и CPU/FS cache не зафиксированы;
частота CPU в отдельных событиях различается. Проценты не обещают стабильный
результат на любой книге.

| Книга / режим | session reading UTC | logged total, ms | load_and_layout, ms | model_load, ms | model_parse, ms | model_paginate, ms |
|---|---|---:|---:|---:|---:|---:|
| 1 Hybrid | 15:03:39.008 | 5121 | 2470 | 1788 | 259 | 1025 |
| 1 Accurate | 15:07:33.508 | 8621 | 5973 | 5403 | 269 | 4185 |
| 2 Hybrid | 15:05:22.370 | 11483 | 8785 | 7954 | 2602 | 4799 |
| 2 Accurate | 15:08:28.778 | 27481 | 24245 | 22842 | 1297 | 20540 |

Разница logged total: книга 1 — 3500 ms (около 41%); книга 2 — 15998 ms
(около 58%). Разница foreground model_paginate: 3160 / 15741 ms.
Не складывать перекрывающиеся стадии и не приравнивать loading first-frame
к первому читаемому кадру. session reading — более подходящая граница, но всё
равно лог приложения, не внешний stopwatch полного экранного refresh.

План действительно активировался: книга 1 — 554991 символ, бюджет 22145,
43 отрендеренных/21 начальная видимая страница; книга 2 — 1956477 символов,
46/23 страницы, такой же бюджет. Не все эти символы эквивалентны исходному XML
itertext из предварительного отчёта: DOM/style/пробелы имеют другую метрику.

Фон завершился: 15:04:05.538 — 649 страниц; 15:05:56.288 — 2427 страниц.
У первой книги также повторная full build после reopen в 15:04:49.918.
Следовательно, полнота lifecycle/adoption ещё не принята только по cold speed.

## Повторные открытия: не выдавать retained reuse за disk-cache hit

| Книга / режим / повтор | model_load, ms | session total, ms | retained_book_hit | document_cache_hit |
|---|---:|---:|---|---|
| 1 Hybrid | 100 | 2430 | true | false |
| 2 Hybrid #1 | 152 | 3411 | true | false |
| 2 Hybrid #2 | 135 | 3330 | true | false |
| 1 Accurate | 82 | 2490 | true | false |
| 2 Accurate | 147 | 3380 | true | false |

model_parse/model_paginate из resource stats retained-модели описывают её
предыдущее построение, а не затраты текущего reopen. Их нельзя использовать
в качестве повторного parse/layout benchmark.

Для книги 1 cache save был false и в Hybrid, и в Accurate; для книги 2 есть
saved=true в обоих режимах. По одному false причина не установлена — у события
нет early-return reason. Нельзя объявлять это регрессией только 034 или чинить
проверки кэша наугад.

В конце есть **другой, не retained** cache-hit путь: 15:10:08.980,
document_cache_hit=true, model_load=19252 ms. Рядом model_parse=417 ms,
model_paginate=16430 ms. Это показывает: disk parsed-cache hit сам по себе
не гарантирует reuse совместимой вёрстки. Причина repagination этим архивом
не доказана; final session указывает 034-2-ACCURATE. Этот эпизод не добавляется
как ещё один обычный быстрый reopen из описанной пользователем основной серии.

## «Расчёт…» в верхнем колонтитуле

Да, это общий для EPUB/FB2 текст `header_footer.calculating`, который возвращает
`src/reading/header_footer.cpp::calculating()`. Отдельной FB2 надписи нет.
Скриншот `034-2-HYBRID · Расчёт…` соответствует ещё не опубликованным page counts;
минутная метка снимка сама по себе не доказывает зависание после full-ready.

Есть конкретный риск в 034: в событиях работы Hybrid встречается book_fraction=-1.
В ReaderSession::captureChapters публикация whole_book_ready/rough полей
находится внутри проверки `bookFraction() >= 0`. Native prefix без пригодного
якоря не должен из-за этого терять уведомление о полной готовности. Также
повторная полная сборка первой книги требует проверки adoption/retained lifecycle.
Это следующая граница диагностики; текущий font/translation follow-up не меняет
engine и не заявляет, что проблема уже исправлена. Считать 034 экспериментом,
а не завершённым production Hybrid для FB2.

## Русские шрифты и перевод

Предыдущее исправление ошибочно зафиксировало Instrument Serif в wordmarkFont,
хотя требовалась RU-only замена. Исправлено в 050: буквальный Kobalt теперь
использует locale Serif — Monotype Corsiva (MTCORSVA.TTF) для RU, Instrument Serif
для EN. Та же serif-роль возвращена заголовкам/«Закрыть» и opening status:
ошибочная подмена этих ролей на Sans отменена. setItalic(false) и normal-style
сохранены; MTCORSVA сама имеет каллиграфический наклон, и это не запрос Italic-face.
Обычные строки/подписи настроек остаются Sans. Изменение проверяется в обоих языках.

Caveat уже является RU default **в роли Kalam**, то есть у playful-пресета
(`settings/interface_style.h::themePreset`), и его Regular/Bold входят в RU assets.
Это не default вместо IBM Plex Sans для classic UI. На присланных скриншотах
обычные UI подписи, по ним нельзя установить поломку Kalam→Caveat. Сохраняем
исходный объём замены, не подменяем Sans, шрифт книги или явный custom choice
без отдельного указания. Регрессия подтверждает Caveat/Kalam по языку и что
classic UI не превратился в рукописный.

`settings.rendering_fast`: «Быстрее» → **«Быстро»** в canonical RU strings и 010
RU overlay. Текст описания «Быстрее всего» не меняется: запрос касается имени режима.

Скриншоты сохранены рядом с диагностикой в `test & diagnostic/ace3c3c/ui-review/`.

## Follow-up пакет (шрифты/перевод, без новых engine исправлений)

Runtime `4f2cecd18767944dbdcf1a7c80b5c485a575d84c`,
[CI45425030626](https://github.com/Mutaracha/kobalt_ru/actions/runs/45425030626):
full series checks, native regressions (включая оба языка), Qt5/package/qemu PASS.
Artifact **Kobalt-ru-beta10-series-qt5**, `14936239073`, 44,585,598 bytes:
https://github.com/Mutaracha/kobalt_ru/actions/runs/45425030626/artifacts/14936239073

Новый пакет исправляет роли serif/wordmark и название «Быстро». Проверенные
шрифтовые роли: headings/opening label/exit prefix+Kobalt — RU Monotype Corsiva,
EN Instrument Serif, italic=false; playful default — RU Caveat/EN Kalam;
обычный classic Sans UI не меняется. Это не повторная аппаратная приёмка и не
исправление отмеченного риска публикации whole_book_ready в эксперименте 034.
