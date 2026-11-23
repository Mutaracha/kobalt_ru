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
All native checks are pending until the new CI completes. Local CMake strings,
production references, runtime manifest hashes and strings rebuild checks pass.
Full native suite not requested/run. The current environment has no Qt/Docker.

Upgrade checklist: regenerate from the new exact upstream; compare templates
and argument formatting, not just `%N` counts; check every cap-height clipping
call site; maintain identical font for fit/measurement/painting; retain bounded
fit and stable status-region geometry; review portrait/landscape at 212/300 PPI
and large UI, numeric widths (1/11/111/9999), one/two/long time values and sleep
charging/off states. If upstream supplies an equivalent fix, drop that hunk
instead of stacking duplicate geometry/formatting policy.

## CI harness integration note

First Qt5 package on 9df3a64b passed (CI45236979735), while the newly registered
native test could not configure: the upstream desktop harness requires every
new test to have a validation-area mapping. Added settings area mapping and
one-line test comment; this is test integration, not evidence of a passing test.
Blob log downloads are blocked in Arena; the workflow now exposes a single
multiline failure-tail annotation via the GitHub API. First per-line annotation
attempt was truncated by GitHub's annotation limit. New native/Qt5 checks pending.

Qt5 compile/link/check-package on 9228e406: PASS (CI45238612205).
Native compile reached the new test, not an application failure. Corrected its
QObject lookup: DescribedOption has no Q_OBJECT, so use findChild<QCheckBox*>()
then dynamic_cast, not Qt's findChild<DescribedOption*>(). Native rerun pending.
Failure-tail annotation bounded to 18×200 characters to fit GitHub's 4 KB limit.
No application changes in this test-harness correction; Qt5 package is reused.
