# 050 — подготовка последнего функционального патча

Статус: **in progress — первый runtime-срез 050 подготовлен; весь 050 не завершён**.
Проверяемый вход: pristine beta10 `1796a32559dbb2912bd565990d1c7a0ca7ac4e52`
плюс ready 010–040 из `41c11e37d6da34255d875109638fe1aea2799283`.
Retained reference: `ca50dd358386e9fccc5122b464f74003caa51e75`.
Сравнение выполнено по Git trees, не по грязному корню checkout.

050 — последний функциональный номер текущего плана. 060 остаётся отдельным
этапом итоговой упаковки/provenance/CI; функциональные зависимости и регистрация
тестов не откладываются в 060. Все патчи применяются одной упорядоченной серией,
отдельная работоспособность частей не требуется.

## Остаток после 010–040

### Язык и CLDR — обязательная часть 050

В префиксе есть `string_locale.h`, RU/EN resource tables и RU ink helpers из
010, но **нет** `support/locale.*`, `support/debug_language.*` и
`settings/debug_language_dialog.*`. Значение localeStore сейчас по умолчанию
Russian. Поэтому наличие перевода не означает, что переключение языка уже
перенесено.

Нужно перенести согласованную цепочку выбора: hidden markers `.lang_en` /
`.lang_ru`, затем KOBALT_LANGUAGE, Kobo config и системная локаль, сохранив
правила fallback/legacy markers из retained. Выбор English/Русский/Automatic —
первой action row Debug, подпись «Язык интерфейса», стандартный Settings style.
Диалог и запись принадлежат settings/support, не About. Запись async,
завершение через GUI context; отмена не пишет markers. Применение только на
границе запуска/открытия новой reading session, без частичного live retranslate.

Entry points: desktop/main.cpp, android/main.cpp и Kobo ReaderCoordinator.
Новые source files должны попасть в source lists/globs **в 050** на всех
платформах; наличие файла само по себе не доказывает компиляцию в Qt5 core.
Проверить consumers plural/date/чисел поверх нового beta10, не заменять их
старыми файлами целиком. Сохранить forced verbose diagnostics.

Тесты: strings_locale (EN/RU 1/2/5/11/21, даты и normalize), markers priority,
Automatic/English/Russian, отмена, отсутствующий/недоступный каталог, async
completion/context lifetime; UI entry только первым пунктом Debug, не About.
Однократная потеря реакции touch после language form ранее не воспроизводилась;
не называть её исправленной без установленной причины.

### About / branding

В префиксе нет fork About row и её `kobaltForkName` / `kobaltBuildTime`.
Перенести build row, translation keys и configure-time значения вместе.
Пустой fork name не должен добавлять строку в generic build; timestamp — UTC,
не строка времени, зашитая агентом. Не возвращать language chooser в About.
Шрифты/brand сначала сверить с 010: это не повод повторно копировать manifest
или откатывать новые upstream assets. README пользователя не изменять.

### Аннотации

Выборочные retained hunks: секции Color / Style / Actions, видимые подписи
swatches, согласованные подписи Cancel / Dictionary. Сопоставить с новым
beta10 `annotationActionsFor`, `showAnnotationActions`, saved appearance menu.
Сохранить новую upstream логику draft/actions/selection; не копировать весь
reader_annotations.cpp или highlight_palette.h.

Тесты должны проверять наличие подписей и raster/geometry на EN/RU, selected /
unselected, узкой ширине и разных PPI. Старый fork diff содержит также удаления
upstream тестов: они не являются частью одобренного оформления.

### Сон

Оставлены для переноса ранее запрошенные F07: две строки remaining в ribbon,
языковой порядок caption/duration, bounded text fitting с elision и сохранение
battery space. helper `sleepRibbonRemainingLines` отсутствует в 010–040.
Нельзя вместе с оформлением менять sleep refresh/cache/opening pipeline.
Неподходящая строка из snapshot другого языка должна сохраняться как один
текст, без ошибочного разбора. Проверить пустой estimate, длинные EN/RU,
battery on/off, все designs и portrait/landscape. Device-приёмка не заявлена.

### Пресеты и return title

`fork_text_preset_names.h` отсутствует. Встроенные известные имена должны
локализоваться при отображении без изменения сохранённых имени/типографики.
Переименованные пользователем и неизвестные presets не переводить. В retained
`tests/model/text_presets.cpp` помимо полезных новых assertions есть удаление
PPI/rounding regression loop — **не переносить это удаление**.

`return_title_text.h` также отсутствует. Проверить необходимые sizeHint/paint
hooks против нового beta10, сохранить arrow placement и RU ink из 010. Не
возвращать fork chapter-progress labels/ticks/setting/top variant.

## Уже покрыто / исключено

- Dark/comics quick actions, icon geometry, Fluent notice/acknowledgment — 040;
  не добавлять второй вариант механизма и не переупорядочивать ControlIcon.
- Checkbox helper/пропорциональный tick — 032; не дублировать его ради языка.
  Возможный `RadioList::setBottomDivider` для language modal переносить только
  вместе с действительным consumer и тестом в 050.
- Русские resources, font/presentation fixes из 010 сохраняются; новые строки
  выравниваются EN/RU с приоритетом ранее утверждённых screenshots.
- `Russian_EnUS.pattern` и его fork-only manifest/list/notice entries исключены.
  `Russian.pattern` и переводной ru-en dictionary не удалять.
- FB2.ZIP, собственный Hybrid, Libra2 mapping и opening acceleration не входят.
- Upstream reverse_page_swipes, Activity, chapter spread, dark/i.MX animation и
  BOOX поведение должны остаться без регрессии.

## Порядок подготовки и критерий ready

1. Языковая инфраструктура + Debug entry + startup consumers и тесты.
2. Preset/annotation/sleep/return-title presentation hunks и их тесты.
3. About configure/build row и только ещё отсутствующие assets/strings.
4. Сверка остатка по inventory и provenance каждого перенесённого hunk.
5. SHA256 патча, clean apply 010–050, локальные series tests, native suites,
   Qt5 build/check-package/qemu и новый Beta10-series-prefix-qt5 artifact.

050 теперь prepared в series.json: в патче первая языковая часть. Это ещё не
полный 050 и не завершение переноса. После ready 050 — 060 full-stack
packaging/CI и отдельная аппаратная проверка.


## Первый runtime-срез 050

Подготовлены support locale/markers, canonical English diagnostics table,
RU CLDR suffixes и fallback, async settings-owned language dialog, стандартная
первая Debug action row и startup/session hooks desktop/Android/Kobo. Убрана
жёсткая установка Russian из контрольного 010; default/fallback теперь English,
а Russian выбирается устройством/ENV/markers. Формы library count используют
CLDR вместо литерала «1 книга» (включая 21/101). Остальные CLDR consumers ещё
нужно пройти вместе с presentation-остатком.

Регистрируются `model.strings-locale` (normalization, tables, dates/numbers,
plural forms, config, markers, async lifetime) и `component.debug-language-ui`
(отмена без записи, повторное открытие, async choice и отсутствие live смены
locale). Qt5 компилирует helper через Kobo source glob; desktop core имеет
явную регистрацию support sources. Выбор языка не добавлен в About.

В language modal оставлены штатные beta10 RadioList dividers вместо переноса
старой реализации RadioList, чтобы не потерять новые two-column/layout rules.
Общий Settings action style берётся из текущего beta10 listAction.

Локальные 10 series tests проходят. В первом CI Qt5 build/package/qemu прошёл,
но native configure остановился на незарегистрированных area labels двух новых
тестов; labels добавлены, тесты не убраны. Итоговый native/runtime результат
следующего прогона пока pending. Не считать первый успешный Qt5 job приёмкой
языковой формы на устройстве.

Legacy device workflow остаётся отключённым. Ordered workflow больше не
собирает binary на report-only изменения и отменяет устаревший прогон своей
группы при новом push, чтобы не тратить время на заменённые сборки.


### Итог проверки первого среза

[CI45372368929](https://github.com/Mutaracha/kobalt_ru/actions/runs/45372368929)
на `b9713968a1c495de66ca0efcb310dc00e16901ed` завершился успешно:
series validation, native suites (включая model.strings-locale и
component.debug-language-ui), Qt5 build/check-package и qemu refresh.
Артефакт `Beta10-series-prefix-qt5`, ID `14910157343`, 44,572,875 bytes.
Это префикс с **частичным prepared 050**, не финальный полный форк;
проверка запуска и language UI на устройстве не выполнена.

Не менять 050 на ready по этому результату: остаются прочие CLDR consumers,
About/annotation/sleep/preset/return-title hunks и их проверки. Для продолжения
патч расширять поверх того же входа 010–040, а не накладывать новый diff поверх
уже применённого 050. Текущие source/test changes и CMake registration находятся
в самом 050; legacy workflow остаётся отключённым.
