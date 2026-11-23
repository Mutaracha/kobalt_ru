# beta10 RU — review of 60 screenshots from 4b59de0

## Evidence and scope

Originals: `test & diagnostic/56843a0/screen_001.png` through `screen_060.png`.
Source commit `4b59de041c4df8ffad16ea5fe037a91fc9cb1dfd`, identified by user as
runtime56843a0. All 60 inspected as numbered contact sheets; 009, 020 and 041 also
inspected at original size to isolate the geometry/percent defects.
No device logs, live session, other orientations or UI scaling levels supplied.
A contact-sheet pass is not a pixel-perfect certification of all 60 screens.

User authorizes returning previous label/sleep text presentation fixes and
reducing the new statistics font. Work remains in the isolated original-beta10
RU overlay, not the entire fork. No vertical margin, progress-edge, sleep/wake,
snapshot storage, refresh, battery data or reading engine changes.

## Findings and corrections

| Evidence | Confirmed observation | Mechanism / correction |
|---|---|---|
| 001, 004–006, 009 | «РАСКЛАДКА» ink cropped in the bottom tab bar | `navigation.cpp` draws in H cap height; this is NOT the `Button` painter previously fixed for Debug. Shared `localeLabelInkRect` preserves baseline but expands to actual Cyrillic ink. Applied to bottom tabs, stacked Button and Activity shortcut in chapter_graph. |
| 009–014 | Corsiva statistics figures optically oversized relative to units/labels | Scoped RU statistics font = 80% previous requested pixels, used for both measurement and painting, including StatsCell QSS, time figures, records and achievement numbers. Brand titles remain unchanged. 80% is a review candidate, not user-accepted size. |
| 041 Widgets | «1 ч 31 мин» clipped at right edge of Today tile | Previous sleep single-line fitting was absent in original control. Restored bounded RU fit down to half original pixelSize, then elide at the fitted size; same renderer measures and paints. This applies to the actual full-screen design and gallery, not a gallery-only hack. |
| 041 Pill/Caption/Night | doubled `2%%` | Eleven more inherited fork templates still owned a literal percent sign although beta10 supplies it in percentArgument/sleepPercentText. Removed suffixes to match beta10 EN contract; added build/native guards. Battery `common.percent` fix from previous package retained. |
| 020 hardware buttons | Long inversion option title ends with ellipsis | Enabled existing wrapLabel presentation only on the RU inversion checkbox; no button-mapping logic changes. Native test inspects the actual dialog row. |
| Code audit: Bookmark ribbon | Previous two-row remaining-time fit absent | Restored pattern-derived caption/time split, reserved two detail rows in RU, fixed battery anchor. Legacy other-language remaining text is retained as one line. No sleep lifecycle port. Screenshot set does not show the full ribbon runtime. |
| Code audit: Widgets remaining row | Caption/time could overlap with longer values | RU time constrained to its own region after the two-line caption, fitted via same helper. English baseline path unchanged. |
| Code audit: translated rich text | Explicit newline must not become whitespace in translated sleep captions | Escape first, then convert translated newlines to `<br>`; no arbitrary HTML from strings. |

No whole-fork sleep_designs replacement. The old presentation fix is adapted to
the beta10 percentage/snapshot APIs; old raw-number percent calls are NOT restored.
The 80% statistics correction is not a global Corsiva/brandFont reduction.

## Complete screenshot coverage register

| Screens | Inspected scope / other observations |
|---|---|
| 001–008 | Reading controls, typography and layout, header/footer preview, face picker. Bottom-tab crop confirmed. Font picker and layout values visible; no reason to change vertical-margins behavior. |
| 009–014 | Chapter/book/session statistics, activity overview/calendar/books/records/achievements. Numeric scope corrected; no algorithm/counting changes. Russian plurals such as «9 дн.» remain upstream one/many logic, not solved by font fitting. |
| 015–024 | Screen/control settings, columns, refresh, navigation, hardware buttons, touch zones, chapters, links, menu style. Debug rail text visible; some long option captions intentionally elide. Need new-device pass for enabled/disabled states; no unsupported blanket “all labels fit” claim. |
| 025–038 | Defaults, typography samples, restore dialogs, advanced text, normalization. English Alice/Styling sample paragraphs are upstream examples, not missing `.strings` translation. No blind replacement of those samples. |
| 039–043 | System, hidden features, sleep gallery pages. Widget overflow and sleep percent contract confirmed. Gallery thumbnail review alone cannot certify full-screen fit. |
| 044–048 | Sleep reset, time/percent formats, secondary numbering and appearance. Remaining-time dialogs readable in supplied orientation. |
| 049–057 | Appearance/font/weight choices, time/percentage formats, system/dictionary rows. `SAMPLE TEXT` and `Определение.` are upstream examples. First/last-page custom-numbering previews need native/device review, not new translated text. |
| 058–060 | Developer rendering, debug diagnostics/system info. No language picker added. No obvious new crop found at contact-sheet scale. |

Not shown: Caveat playful theme, landscape, 125/150% UI, charging/powered-off
variants, long book titles/authors, all sleep designs at full size. These remain
unverified; do not infer success from the supplied portrait screenshots.

## Regression checks for future upstream updates

New focused test `component.ru-control-ui` (also executable with `en`):
- actual bundled-family/preset selection, statistics 80% RU vs 100% EN;
- percent-template single-sign contracts, including sleep and accessibility;
- Cyrillic tab raster against a roomy reference at 212/300 PPI;
- bounded duration fit, statistics measurement/QSS consistency;
- render Widgets/Bookmark/Caption/Night from a snapshot resembling screen041.

The workflow runs separate RU and EN processes (not unsupported live retranslation)
and publishes raster samples separately from the installation package. They are
synthetic headless outputs, NOT user/device evidence and NOT visual acceptance.
Focused native checks passed as recorded below. Local CMake strings,
production references, runtime manifest hashes and strings rebuild checks pass.
Full native suite not requested/run. The current environment has no Qt/Docker.

Upgrade checklist: regenerate from the new exact upstream; compare templates
and argument formatting, not just `%N` counts; check every cap-height clipping
call site; maintain identical font for fit/measurement/painting; retain bounded
fit and stable status-region geometry; review portrait/landscape at 212/300 PPI
and large UI, numeric widths (1/11/111/9999), one/two/long time values and sleep
charging/off states. If upstream supplies an equivalent fix, drop that hunk
instead of stacking duplicate geometry/formatting policy.

## Verification outcome / package handoff

| Check | Result / evidence |
|---|---|
| Strings generation/rebuild, 1808 key/slot parity, production references, manifest assets | Local PASS |
| Inject old `sleep.percent_read = прочитано %1%` | Rejected by new CMake guard, PASS; restored after test |
| Exact pristine apply + 36 output SHA-256 values | Local PASS |
| Qt5 compile/link + original check-package | [CI45238612205](https://github.com/Mutaracha/kobalt_ru/actions/runs/45238612205), PASS, source9228e406 |
| Focused native compile/link and RU **and** EN executions | [CI45241167262](https://github.com/Mutaracha/kobalt_ru/actions/runs/45241167262), PASS, sourcecc3c9e77 |
| Full native suite / physical Kobo visual acceptance | NOT RUN / pending user screenshots |

[Installation artifact14850976282](https://github.com/Mutaracha/kobalt_ru/actions/runs/45238612205/artifacts/14850976282),
`Kobalt-beta10-original-ru-qt5-control`, 44 997 129 bytes Actions archive.
Inside: `build/control-beta10/build/kobo-qt5/Kobalt-qt5.zip`.

[Headless synthetic samples14853145717](https://github.com/Mutaracha/kobalt_ru/actions/runs/45241167262/artifacts/14853145717),
`beta10-ru-presentation-samples`, 163 157 bytes. These are not device screenshots;
CI rendered/saved the samples successfully. Archives were not downloaded/viewed
in Arena (blob-host access restrictions observed earlier), so no visual pass is
inferred from artifact existence. Native raster/metric assertions themselves pass.

Package source9228e406 vs latest test sourcecc3c9e77 compared by the exact overlay
manifest: **only `tests/model/ru_control_ui.cpp` differs**. Production source/assets
are byte-identical. Qt5 packaging was intentionally not repeated for test-only
lookup/assertion corrections; `[native-only]` commits skip that job.

### Harness failures corrected before the native PASS

- Desktop validation requires an area for every new test: added `settings` mapping
  and a one-line test scope comment.
- DescribedOption has no Q_OBJECT: find QCheckBox by name, then dynamic_cast.
- The percent test must populate the book-title `%2` before counting literal
  percent signs; unresolved placeholder `%2` is not a duplicate sign.
- GitHub blob logs cannot be fetched from Arena; bounded multiline annotations
  expose the error tail via API without GitHub's per-step count/4 KB truncation.

Those earlier failed runs are not passes and are not device evidence. The final
native PASS above is after these harness corrections, with the actual runtime
ink-box/fitting/statistics code unchanged from the packaged revision.

## Повторные снимки устройства и уточнение подписи

Получены 11 повторных снимков; оригиналы сохранены в
`test & diagnostic/control-beta10-round2/`. Они документируют проверку
подписей «РАСКЛАДКА»/«ОТЛАДКА», уменьшенных цифр статистики, переноса
подписи инверсии и превью сна. Это не безусловная пользовательская приёмка
размера 80%. Полноэкранный сон технически невозможно снять; пользователь
сообщает, что он выглядит так же, как превью. Это сообщение пользователя,
не прямое полноэкранное растровое доказательство; повторно снимки не требуются.

По прямому согласованию заменена только строка `buttons.invert_landscape`:
«Инвертировать кнопки в альбомной ориентации.» Перенос RU `wrapLabel`
сохранён, включая маленькие экраны. EN и геометрия не менялись.
Manifest обновлён; все 36 хешей export, проверка ключей/аргументов/ссылок
и strings generation/rebuild (Ninja) PASS. Первая попытка strings fixture
с Makefiles дала известный timestamp-dependent сбой; отдельный Ninja scratch
прошёл. Новый пакет Qt5 для этой строки пока не собран; прежние ссылки на
пакет относятся к прежней длинной подписи.
