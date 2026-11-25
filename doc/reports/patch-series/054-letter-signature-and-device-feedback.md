# Device feedback: accepted close caption, Letter signature, extended cache check

User supplied screen_002-19.png (reading menu) and screen_001-26.png (sleep gallery),
preserved unchanged under `test & diagnostic/device-review-letter/`, catalogued with
SHA256 and byte size. Exact screenshot runtime is not embedded in the PNG evidence;
do not infer it from blank session text in the first minute.

## User acceptance and cache observation

- User says «Закрыть Kobalt» now looks visually unified, without a distracting font
  difference. Keep caption90%, common baseline and Instrument Serif Kobalt unchanged.
- Letter sleep-screen signature must use the brand face, not the localized decorative
  serif. The supplied gallery shows the issue; other gallery/body/status fonts were
  not requested to change.
- User tested several openings of the same test book, including after device reboot,
  and could not reproduce a long opening. This is positive provisional device evidence
  after059a, not proof of layout reasons0 in a new log and not long-term closure.
  User explicitly requests an extended-duration check in the combined build.

## 059c implementation

Both paintLetter signature and letterAreas geometry now use ui::wordmarkFont at the
existing nominal11% frame-width pixel size (rounded as before). Instrument Serif
Regular is independent of the RU85% decorative scale. The literal Kobalt is unchanged.
The corresponding signature line metrics also bound the dynamic body repaint area,
so charging/status repaints and fresh renders use consistent geometry. Same painter
serves gallery preview, sleep and power-off. Letter body/closing/heading fonts remain
unchanged; no changes to the accepted return title, font inventory or cache algorithm.

Regression added to existing bilingual fork-presentation executable: render Letter
for EN and RU, sleep/power-off and light/dark; actual nonblank signature pixel crops
must equal the original English Instrument Serif reference. This checks rendered
pixels, not just the claimed QFont family. Existing lifecycle literal-placeholder
case and Letter light/dark partial-repaint-vs-fresh-render cases are explicitly run.
New combined build retains059a system-font identity and059b menu-status toggle.

## Extended device check

Keep existing book/font/cache files. Use the combined build normally over a longer
period, including ordinary app restarts and occasional device reboot as convenient.
No mandatory stopwatch matrix or cache reset. If a long open recurs, preserve the
archive and approximate time; distinguish retained-book hit from disk-layout reuse.
Do not promise every open is instant or label all older mismatches definitively fixed.
Check the Letter signature in gallery and sleep/power-off, then status-row taps and
saved choice. Hardware visual acceptance of059c/menu toggle remains pending.

## Verified combined package

Runtime **92a2936401383d57278575af613e0b8ce3198295**,
[CI45675458450](https://github.com/Mutaracha/kobalt_ru/actions/runs/45675458450): all jobs PASS.
Ordered full-series validation; Linux selected regressions including bilingual
signature pixels, Letter lifecycle cases, menu-status lifecycle/appearance/font identity
and archived FB2 replay; Qt5 build/check-package/configured refresh-qemu and collection.
This is selected host coverage, not the whole repository test matrix or device acceptance.

[Qt5 artifact15051583108](https://github.com/Mutaracha/kobalt_ru/actions/runs/45675458450/artifacts/15051583108),45440099bytes.
Host cache evidence15051433591,12319bytes. All patches including059/059a/059b/059c apply
together before060. No diagnostics/cache clear or re-install of older package requested.
