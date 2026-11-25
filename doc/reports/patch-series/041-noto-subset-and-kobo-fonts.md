# Noto Serif Display Condensed subset and uploaded Kobo font replay

User decision supersedes the pending full-Monotype-family task: Russian decorative serif becomes Noto Serif Display Condensed, only necessary faces; keep literal Kobalt branding unchanged. Uploaded originals: c29e6494 and3385e761. Device paths clarified by user: Windows volume-root `.adds/kobalt` and `fonts`, corresponding on Kobo to `/mnt/onboard/.adds/kobalt` and `/mnt/onboard/fonts`. No host-specific absolute Windows path is compiled in.

## Ordered055

`055-ru-noto-display-subset.patch` follows presentation050, before build060. It replaces the **final full-series** Russian locale serif with the supplied static Regular+Italic faces. Qt and private FreeType selection both resolve this exact family before the generic Instrument Serif branch; private italic resolves the real Italic file, not a slanted Regular substitute. The SFNT family name is exactly `Noto Serif Display Condensed`.

Only:
- NotoSerifDisplay_Condensed-Regular.ttf —430852bytes;
- NotoSerifDisplay_Condensed-Italic.ttf —447160bytes.

Total878012bytes (857.4KiB). No Bold/BoldItalic/Light/LightItalic/Medium/MediumItalic is added to runtime manifest or final source asset directory. Original supplied files stay intact under test & diagnostic. MTCORSVA.TTF157360bytes and proprietary Monotype notice/acknowledgment are removed from the final prepared source/package; the OFL notice and Noto acknowledgment replace them. Net raw font change+720652bytes. Package compression differs from raw size; compared with b23990b3 artifact the zipped package grows355205bytes (~347KiB).

The literal Kobalt role stays regular Instrument Serif in About/opening/exit, with both Instrument files still available. Heading role remains nonitalic as in beta10; existing explicit italic brand requests (e.g. activity/library message text) now obtain the real Noto Italic face in RU. Opening text had been forced upright by fork presentation050:055 restores the upstream beta10 condition `!fallback || automatic` for the opening label, while errors are upright and the separate literal brand stays unchanged. The helper preserves selected italic in both QFont and QSS. The close prefix remains upright, as in upstream; the user's explicit brand exclusion is respected.

Regression coverage checks the Russian family role, true italic request, both manifest font payloads available, Corsiva absent from staged runtime assets and literal About/exit wordmark still Instrument Serif. Existing RU control geometry/private text and fork presentation regressions pass with the new font.

The standalone `platforms/control-beta10/ru.patch` is the earlier comparison baseline and still contains its original font payload.055 operates on the final active series, removing it there; do not distribute RU-only baseline as this updated package. Historical patches/diagnostic archives are not rewritten to erase past Corsiva evidence. This change does not claim standalone RU overlay now packages Noto.

## Actual Kobo-font replay

Seven uploaded files (Fira Code Regular/Medium/Bold; Fira Sans Regular/Italic/Bold/BoldItalic) are copied only into the ignored host replay tree, not the installable package. Uploaded FiraSans-Regular differs byte-for-byte from the previous repository fixture. Both font file identity and complete family inventory therefore mattered to host equivalence; their difference alone is **not** an established explanation for device startup invalidation.

CI45596424824 host results, using actual archived A1/A2, pinned selected device settings and all seven uploaded fonts:

| Book | Warm1/warm2 reasons | Style current=saved | Global hash | Node hash |
|---|---|---|---|---|
| A1 | 0/0 |4238856906|2c9c7eab|e6227a08|
| A2 | 0/0 |249516262|2c9c7eab|228b6c6c|

Content geometry1240x1613; changed width1240→1260 gives reasons16, styles unchanged. Thus device font files still do not reproduce the cross-process mismatch on Linux. The host stylesheet836494821 still differs from device1662611122; full platform/settings/startup lifecycle equivalence is not claimed. The next bounded FB2 investigation remains node/global decomposition at device save/check and startup ordering. No broad device timing rerun is requested.055 is a UI typography change, not an FB2 speed fix; preload stays enabled and compatibility guards untouched.

## Validation/package

Runtime **2c1914e32a36d1726d83667c08feb42219fe031a**. **CI45596424824 all jobs PASS**: full ready-series validation/apply/inventory, native selected regressions plus actual Kobo-font replay, Qt5 compile/check-package/configured qemu-refresh/package collection. Local entire ordered apply to pristine pinned beta10 PASS. Actual-source replay executes on Linux, not Kobo hardware.

[Qt5 artifact15012959489](https://github.com/Mutaracha/kobalt_ru/actions/runs/45596424824/artifacts/15012959489), Kobalt-ru-beta10-series-qt5,44944509bytes.
[Host evidence artifact15013189028](https://github.com/Mutaracha/kobalt_ru/actions/runs/45596424824/artifacts/15013189028),8924bytes.
Derived check-run notices saved in041-kobo-font-replay-evidence.json (check139143576073).

Device visual gate only: check Russian settings headings, a normal opening label (italic as beta10) and About/close Kobalt (unchanged Instrument Serif). Also look for clipping in narrow tabs after the font replacement; build/test pass is not visual device acceptance. Do not clear book caches or re-run timing matrix for this UI change. Session-status toggle and remaining Hybrid correctness gates are still pending; full-Monotype integration is withdrawn, not completed.
