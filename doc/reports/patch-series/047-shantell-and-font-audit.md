# RU Shantell Sans Italic, font sample organization and Cyrillic audit

## Runtime change: ordered057

RU Playful handwriting role changes from Caveat (the former RU replacement of Kalam)
to **Shantell Sans Italic**. Supplied real400 Italic and700 Bold Italic faces are packaged, byte-identical to the
uploads. Bold Italic was added by the user in87285198 during report preparation and
incorporated before final handoff. Other weights remain samples only. Existing private
interface extra-weight behavior is otherwise unchanged.

Central sizedFont requests italic for RU Shantell, including formerly upright Caveat
callers. Root reader QSS explicitly preserves that slant and ListRow QSS no longer
unconditionally overwrites the provided font's italic flag. Tests cover RU/EN preset,
QFont italic/measurement, polished child QLabel, classic reset and legacy saved Caveat
custom-choice migration. Existing explicit italic use remains italic, not double-slanted.
The private renderer selects the real supplied Italic face even in callers that clear
a QFont's italic flag; only the two italic faces are registered for this family. No invented
Regular file or modified font metadata. Other families' explicit styles are unchanged.

Caveat Regular/Bold, its notice, acknowledgment and manifest entries are removed from
the final full-series assets. Historical control overlay/patch010 and archived originals
retain provenance; they are not the final packaged asset inventory. Old managed files
are governed by existing asset update/manifest behavior; no deletion of unrelated
user-installed fonts. English Kalam, classic Sans, Noto decorative sizing, literal
Instrument Serif Kobalt and book font settings are not replaced by this change.
Old RU custom `ui_font_face=Caveat` maps to Shantell; switching locales remains scoped.
SIL OFL notice included, obtained from google/fonts ofl/shantellsans/OFL.txt via GitHub API.

## Uploaded originals organized

Current standalone uploads now live under `test & diagnostic/fonts/`:

- `Shantell Sans/` — all six supplied static faces;
- `Noto Serif Display Condensed/` — all eight supplied faces;
- `Kobo/fonts/` — seven actual-device Fira font files used by replay;
- `Literata/` — six newly supplied faces plus OFL, all Russian letters present; samples only.
- `legacy/monotype-corsiva.zip` — original archive, not a runtime font.

No payload bytes changed. Old→new map:047-font-sample-moves.json. Catalog/checksums
updated, Shantell uploads added to catalog, CI's real-device-font replay path updated.
Historical fonts attached to archived diagnostics were deliberately not separated from
their evidence folders. There is no new automatic inclusion of sample files in builds.

## Method and evidence

`doc/tools/audit-cyrillic.py` uses actual Unicode cmap tables through fonttools, including
TTC faces.133 runtime-source faces (129 font files; TTC expands) and35 loose sample
faces scanned; exact paths/styles/hashes/missing letters in047-font-coverage.json.
Criterion is **all66 Russian letters including Ёё**. This is not proof of all Cyrillic
languages or shaping correctness. Cyrillic0400–052F counts are retained for nuance.
ZIP-contained historical samples are not scanned as loose fonts; do not infer their
coverage from filenames. System/user fonts outside supplied files are not inventoried.

Reproduce after full-series export:

```
python -m pip install fonttools
python doc/tools/audit-cyrillic.py /path/to/full-series/assets/fonts
python doc/tools/audit-cyrillic.py 'test & diagnostic/fonts'
```

### Fonts lacking Russian Cyrillic, and actual uses

Every face of the families below has0/66 Russian letters in its own cmap. Their
ability to display Russian through fallback does not mean the selected face supports it.

| Family | Actual role / source |
|---|---|
| Instrument Serif Regular/Italic | Latin Kobalt wordmark in both locales; decorative serif in English. RU decorative serif already uses Noto Display. `ui/style/fonts.cpp`, private_text.cpp; styles heading/brand/wordmark. Keep the Latin wordmark unchanged. |
| Kalam Regular/Bold | English Playful preset only; no longer the RU handwriting default. `app/reader/settings/interface_style.h`, assets/bundled_fonts.cpp. A manual English/custom choice can still need fallback for Russian. |
| Libron, all4 | Default book face (`reading/appearance_settings.h`), Kobalt text preset and Publisher-row samples; sleep-letter default/selected-body fallback, annotation HTML export embeds Libron. `reading/text_presets.cpp`, `app/reader/views/sleep_designs.cpp`, reader_annotations.cpp, `storage/annotation_export.cpp`. Russian body text uses fallback; changing it would be a separate typography/layout decision. Export fallback also depends on the receiving browser's fonts. |
| NV Ancizar Serif, all4 | Bookish text preset and core reading-font picker. `reading/text_presets.cpp`, assets/fonts/ebook-core.json. |
| NV Legible Next, all4 | Default publisher-sans substitution (`engine/font_defaults.h`) and core font picker. A Russian sans request may thus fall back rather than use these letterforms. |
| NV Adelph, NV Aleo, NV Andada, NV Basker, NV Castoro, NV Yrsa, each4 | Supplemental reading-font picker (`assets/fonts/ebook-supplemental.json`, bundled_fonts.cpp/font_catalog.cpp); no hard-coded default UI role. Can also be manually chosen as custom interface fonts if eligible. |
| Noto Naskh Arabic; Noto Serif Armenian/Bengali/Devanagari/Georgian/Hebrew/Tamil/Thai | Intentional script-specific fallbacks, not Russian fonts. `assets/bundled_fonts.h`, ui/style/fonts.cpp/private_text.cpp. Do not replace merely because they lack Cyrillic. |
| Noto Sans Math | Intentional math glyph fallback, not general Russian text. |

### Relevant families that do cover all66 Russian letters

Shantell Sans Italic; supplied Shantell weights; Noto Serif Display Condensed;
IBM Plex Sans/Mono; Playpen Sans; Noto Serif; CJK fallback faces (coverage does not
make them desirable primary Russian fonts); NV Jost, NV Bitter, NV Charis,
NV Garamond, NV Literata, NV Palatium, Sourcerer, Cartisse, NV Alegreya,
NV Halcyon, NV Publica, NV Scarlet, NV Technical. All35 loose supplied/historical
sample faces scanned have66/66, including uploaded Fira Sans/Code. Different families
cover different extended Cyrillic subsets; see evidence rather than assuming equivalence.

No additional font replacements made on the basis of this audit. In particular Libron
and the reading presets are reported, not silently changed, because that would affect
book layout and cache compatibility beyond the requested UI handwriting replacement.

## Validation / device gate

Final runtime **3aeb9efbbcd279ada6a52845f7601db04c3929db**,
[CI45629224994](https://github.com/Mutaracha/kobalt_ru/actions/runs/45629224994) **all jobs PASS**:
full ordered apply/validation/inventory, native selected tests including FB2 engine/session
and RU/EN control UI, actual A1/A2+uploaded-device-font replay at moved paths; Qt5
compile/check-package/configured refresh-qemu/package collection.
[Qt5 package15029143933](https://github.com/Mutaracha/kobalt_ru/actions/runs/45629224994/artifacts/15029143933),45030076bytes.
Host evidence15028919763,12313bytes. Earlier45627279171 also passed before Bold Italic.
New Literata samples and Shantell OFL uploaded during the final run are preserved and
audited/catalogued only; they do not change this runtime. Supplied Shantell OFL matches
the packaged notice after CRLF/LF normalization (original upload preserved).
Manifest hashes and absence of Caveat final font assets checked locally. Full-series
runtime package, not the standalone historical RU control overlay, is the deliverable.
On device: select RU Playful, verify Shantell italic in ordinary controls/settings and
text fitting; switch Classic/English to check isolation. Noto85% visual acceptance
remains separate. No timing matrix or cache clearing; replacing a bundled font may
legitimately invalidate an old layout once through the unchanged font-inventory guard.
