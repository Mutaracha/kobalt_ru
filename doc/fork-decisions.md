# kobalt_ru — действующие решения beta10

Это правила текущего форка, не очередь. Очередь: [план](../FB2_FORK_PLAN.md).
Источник — явные указания пользователя; консолидация не добавляет нового согласования.

## D01 — база и порядок

Upstream beta10 `1796a32559dbb2912bd565990d1c7a0ca7ac4e52` и вся серия 010–060.
Патчи применяются вместе по series.json; общие helpers и зависимости разрешены.
Исторический beta9 SHA в provenance нужен для происхождения дельты, не для сборки runtime.

## D02 — сохраняемые функции

Нативный raw FB2, comics, RU/CLDR/утверждённые строки, branding/fonts,
тёмная/comics toolbar, presentation аннотаций/сна и независимая инверсия swipe.
Comics — per-book default off, aspect-fit; временно игнорирует поля/chrome,
но не переписывает пользовательские настройки. Landscape pairing учитывает
wide/prose/standalone center; numbering/bookmarks/hits используют ту же геометрию.
У upstream сохраняются штатные функции, включая его progress и настройки кнопок.

## D03 — FB2 и Hybrid

Canonical inpage_footnotes default false, форматные notes/comments и native
run-in numeric titles; текст, IDs, anchors и корректная геометрия сохраняются.
Hybrid 034 разрешён как эксперимент начального префикса с фоновым full layout
и безопасным fallback. Prefix не сохраняется под ключом полного документа.
Не ослаблять fingerprint/name/source guards ради кэша. EPUB slicing/adoption/fallback
не менять ради FB2. Скорость, счётчик, кэш и lifecycle принимаются раздельно.

## D04 — RU и оформление

При расхождении ранних переводов использовать согласованные screenshot refinements
8bd576f, а не прежний 4b59de0. Канонические строки — assets/ru.strings и 010 RU overlay.
«Закрыть Kobalt», «Быстро», «Язык интерфейса». Вход языка — первый пункт Debug,
общий стиль Settings и самостоятельное владение dialog, не About. Выбор persistent,
без частичной live retranslation. ExtraLocales/C/POSIX и EN fallback сохраняются.
RU serif-роли — Monotype Corsiva, playful default вместо Kalam — Caveat;
обычный classic Sans и explicit custom choice не подменяются. Заголовки/opening
не запрашивают Italic. В строке закрытия латинское **Kobalt — Instrument Serif**,
русское «Закрыть» — locale face. Полное новое семейство Corsiva ещё ожидает интеграции.
Stock version/commit в About сохраняются, fork/build row отдельно.

## D05 — upstream ownership

Отдельная upstream инверсия физических кнопок заменяет прежний fork mapping;
две коррекции не складывать. Dark/i.MX анимация, BOOX refresh, KOSync,
общие lifecycle/storage и базовая progress policy — upstream, не второй механизм.
Сохранять утверждённые ribbon/annotation тексты и визуальное представление.

## D06 — диагностика и приёмка

В текущем пакете подробные логи переключаются в Debug; marker сохраняется.
Принудительный verbose не должен блокировать пользовательскую настройку.
OFF оставляет компактный warning/error log. CI PASS не равен device acceptance.
Оригинальные ZIP/книги/снимки сохраняются без перепаковки/перекодирования;
отчёты отделены от данных. Не очищать профиль/кэш для получения красивого замера.

Закрытые исключения собраны в [отдельном реестре](retired-decisions.md), а не в плане.
Исторические формулировки и одобрения: [архив](archive/README.md).
