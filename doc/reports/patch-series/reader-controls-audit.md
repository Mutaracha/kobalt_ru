# 040 reader controls — pre-port audit

Status: implemented and CI accepted as 040; physical-device toolbar review pending.
Base: pristine beta10 `1796a32559dbb2912bd565990d1c7a0ca7ac4e52`
plus ordered patches 010–033. Retained reference:
`ca50dd358386e9fccc5122b464f74003caa51e75`.

## Preserve upstream swipe handling

The prefix already has `pageNavigation::reverseSwipesKey =
"reverse_page_swipes"` in `src/reading/page_navigation.h`.
`Reader::showNavigationChoice` in `reader_dialogs.cpp` exposes
`rrReverseSwipeDirection`, persists independently of the navigation mode and
rolls the checkbox back if saving fails. The page swipe callback in
`reader_build.cpp` combines RTL with this preference using XOR before
`turnPage`. Annotation gestures use the same callback.

Do not introduce the fork's second `invert_page_swipes` preference or
`swipeDirection` helper. Do not change physical-button inversion, tap zones,
Hybrid, chapter-start spread policy, dark refresh, i.MX animations or BOOX.

## Selective toolbar port

Port the retained dark-theme/comics quick actions, checked state and visibility
rules from `fork_reader_actions.cpp`, including safe comics toggling around
appearance preview and busy state. Keep the existing Layout comics setting
from 032. Wire the actions through Reader construction/refresh and
ToolbarComposition/Navigation, including restoration of borrowed button style,
size and paint mode when Navigation is destroyed.

Port only necessary icon/button state rendering: comics state through icon ink,
not an inverted button background, and no redundant checked highlight for the
dark-theme button. Include comics vector geometry and the Fluent dark-theme
glyph with its MIT notice and acknowledgments entry in the introducing patch.
Do not reorder unrelated ControlIcon enumerators.

Preserve beta10's `Activity` feature and its position in feature name/icon/key
arrays when adding the two optional toolbar feature controls. The retained
fork's shorter arrays cannot replace upstream wholesale.

## Presentation review

Review translated navigation tab ink/baselines and the centered session caption
selectively against 010 to avoid duplicating its existing RU fixes. Do not port
removed chapter progress labels/ticks, their setting, or top variant. Keep
ordinary upstream progress behavior.

## Validation required before ready

- Clean ordered application and full prefix CI on native/Qt5.
- Toolbar visibility in normal/Zen/settings states and hidden-feature gating.
- Dark toggle checked state follows display settings; comics toggle follows
  Layout state, saving and preview/busy rules.
- Navigation teardown restores shared action presentation.
- Upstream Activity and independent swipe controls remain present.
- No mixed RU/EN hyphenation dictionary or obsolete Libra2/Hybrid changes.

All patches are one ordered stack; shared helpers are allowed. Source and test
registration belong to 040, not a deferred build-integration patch.


## Implemented prefix and evidence

040 ports the quick actions and their checked-state rendering, feature controls,
Navigation ownership/restoration and centered session caption. The RU ink
adjustments already provided by 010 (`localeLabelInkRect`) remain untouched;
the older fork button/text painter was not substituted for newer upstream code.
Upstream icon caching, styled icons and palette handling are preserved.
The redundant fork swipe preference and QObject inheritance were not ported.

`tests/model/reader_controls.cpp` checks independent feature preferences with
Activity retained, the upstream swipe preference key, borrowed action paint
mode/style/size restoration, distinct comics on/off glyphs and dark glyph
availability. This component target and its ctest entry are registered by 040;
the ordered workflow explicitly builds and runs it alongside prior regressions.
This is not an end-to-end hardware test of toggling during active repagination.

The first CI run found an incorrect namespace in the new test only;
`ToolbarComposition` is upstream `kobalt`, not `kobalt::reader`. Corrected without
removing or weakening the regression. Final [CI 45361473628](https://github.com/Mutaracha/kobalt_ru/actions/runs/45361473628)
on `87530065b2ad6a7655d1b2bdf7a2f451fa008ff1` passed all three jobs:
clean prefix checks, native tests and Qt5 build/package/qemu refresh.
Artifact **Beta10-series-prefix-qt5**, `14904792738` (44,561,916 bytes), contains
010–040. Hardware normal/Zen/hidden-feature checks remain in DEVICE-TEST.md.
