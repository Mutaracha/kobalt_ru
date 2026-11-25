# RU reading roles and font-picker exclusions (ordered058)

## User decision implemented

Only Russian UI locale changes these built-in roles:

| Role / previous built-in | Russian | English |
|---|---|---|
| Default body / Libron | Literata | Libron |
| Publisher generic sans / NV Legible Next | Noto Serif | NV Legible Next |
| Bookish preset / NV Ancizar Serif | Lora | NV Ancizar Serif |

Noto Serif is intentionally a serif replacement for the former generic-sans role,
exactly as requested; not Noto Sans and not Noto Serif Display Condensed. The latter
remains the separate85% decorative UI role. Literal Kobalt and Shantell remain unchanged.

A shared locale_font_roles helper supplies defaults at runtime, not at static startup.
Production default-role callers use it in engine settings/generic fallback selection,
text/style presets, Publisher-row samples, default-font settings/dialogs, library defaults
and sleep-letter default. Engine open maps the three old built-in family names (also
KF aliases) to replacements in RU saved book state; unrelated custom family names and
Publisher Fonts' empty face stay unchanged. Old default preferences and loaded text
presets resolve through the mapping. No user JSON file is globally rewritten in place.
Switching to English uses English roles; an explicitly saved new Literata/Lora choice
is a user choice, not silently reverse-mapped. Legacy constexpr names are retained only
for compatibility with old tests/out-of-tree callers; production uses locale accessors.

Sleep drawing registers supplied Literata faces with the private renderer. Annotation
HTML export selects supplied Literata-Regular bytes in RU and Libron in EN; CSS/data
font-face share a neutral `Kobalt Export Body` alias, with the OFL notice no longer
misidentifying every embedded font as Libron. Human-visible export text is not translated
as part of this font task.

## Assets and original samples

Four static faces each of supplied Literata and Lora are bundled: Regular, Italic,
Bold, Bold Italic. Their source files are copied byte-for-byte from
`test & diagnostic/fonts/Literata/` and `.../Lora/`; Medium/Medium Italic stay samples.
OFL notices, acknowledgment entries and all runtime manifest sizes/hashes included.
Original Lora upload86144f66 remains in its supplied dedicated folder and is now catalogued.
These supersede the previous audit047's samples-only status for Literata. The existing
NV Literata is not mistaken for the supplied family named exactly `Literata`.

`048-font-coverage.json` scans the final source-font inventory with the existing cmap
audit tool. All eight newly bundled faces cover all66 Russian letters including Ёё;
this is not proof of every Cyrillic language or visual/shaping quality.

## Picker exclusions, not destructive deletion

In Russian `orderedReadingFonts` removes NV Adelph, NV Aleo, NV Andada, NV Basker,
NV Castoro, NV Yrsa (also KF/unprefixed family-name variants). Selecting one previously
is not an exception to hiding it. The replaced Libron/Legible Next/Ancizar entries are
also hidden so they cannot misleadingly offer a different name for a mapped role.

The underlying engine's registered font inventory and font files remain available:
old books using one of the six excluded families can still open; no user font file
or saved custom preset is deleted. English lists are unchanged. These are exact family
name rules, not substring filtering. A manually supplied font with the same exact
excluded family name is also hidden in RU; unrelated custom names stay visible.
The filter covers main reading and settings/interface/header/footer/dictionary pickers
through their existing shared ordering helper.

Noto Serif is promoted from specialist-only fallback classification in RU so the main
picker does not remove it when it is not selected. In English the specialist behavior
stays unchanged. Other language/script fallbacks retain their existing classification.
No new UI preference, book-format support or acceleration feature is added.

## Automated checks and device boundary

RU/EN control regressions assert role defaults, old-family migration/custom-name
preservation, source availability scoped by locale, actual EngineDocument opening
using each old role with resulting fontFace equal to the replacement, fresh built-in
presets, Noto picker classification and exclusions even for a selected old family.
Existing highlight-store/annotation HTML export regression was updated to the neutral
embedded-font license label and explicitly added to selected native CI execution.

Initial revision4318667e/CI45634771746 all jobs PASS: full series/native selected
regressions, actual A1/A2+device-font replay, Qt5 compile/check-package/configured
refresh-qemu/package. Final runtime **fd9a70ac67aa86593b4877947b831e3deeba0f6e**,
[CI45636420364](https://github.com/Mutaracha/kobalt_ru/actions/runs/45636420364) **all jobs PASS**,
including Noto picker-classification regression, actual RU/EN engine font resolution,
annotation export, all selected existing tests and uploaded-source cache replay.
[Qt5 package15034715785](https://github.com/Mutaracha/kobalt_ru/actions/runs/45636420364/artifacts/15034715785),45749333bytes;
host evidence15034845533,12305bytes. This is not the entire repository test matrix.

A physical visual check is still needed: RU default reading, Bookish/Lora, Publisher
font mode's generic text, list visibility and sleep/export samples. New typography and
new font inventory can legitimately invalidate old disk layouts once. Do not clear cache,
weaken compatibility guards or attribute re-pagination to a cache-read regression.
Device timings are not requested. Existing books may move to a different page count
because this request deliberately changes their previously selected built-in typeface.
