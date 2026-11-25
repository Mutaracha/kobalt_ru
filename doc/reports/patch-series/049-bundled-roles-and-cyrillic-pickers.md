# Ordered059: bundled Russian roles, Cyrillic pickers and close caption

## Decisions and asset identity

Runtime Russian roles now use **NV Literata** (default body/sleep/export), **NV Publica**
(former Legible Next generic-sans role) and **KF Sable** (Bookish/former Ancizar role).
NV Literata/Publica are already present in pinned beta10. Sable was absent from that
pin: user upload720156fa supplies four files with actual SFNT family **KF Sable**,
not NV Sable. We preserve names and bytes, including Lora-derived copyright and OFL
metadata, rather than relabel the fonts. Four faces bundled from that upload, recorded
in the existing supplemental manifest and top-level asset hashes. Lora OFL terms are
retained as Sable-OFL; acknowledgment identifies the ebook-fonts derived source.
Sable registers in RU only; its packaged files can serve RU after a language switch.

Separate added Literata/Lora faces, notices and acknowledgment entries are removed
from the final package. Historical058 introduces them and059 removes them; all patches
remain applied in order. Original uploaded samples stay unchanged. Noto Serif was
already a built-in fallback, not a third additional font set: it retains that service
role but no longer the new primary reading role. NV Publica replaces that role.
Old names Libron/Literata, Legible Next/Noto Serif and Ancizar/Lora resolve to the new
RU roles at engine/preference/preset loading, while arbitrary custom names and English
roles are unchanged. Literal Kobalt remains Instrument Serif. Noto Serif Display
Condensed and Shantell Sans Italic/Bold Italic remain the approved RU extra UI families.

## Picker filtering and duplicate prevention

The shared ordering helper used by book/default/interface/header/footer/dictionary
pickers now checks actual font cmap coverage through FreeType, requiring all66 Russian
letters including Ёё. This is a **selector-only** filter: fonts remain registered for
old books, embedded styles and script/math fallback. At least one inspected face/file
must have the full Russian alphabet to keep the family. Unknown/unreadable/no-file
entries are not misreported as proven missing glyphs. Coverage cached by path/size/mtime
with a bounded cache; English bypasses the check. This is Russian-alphabet coverage,
not a claim of every Cyrillic language or advanced shaping correctness.

Known old/replaced family names, six previously excluded families, Kalam and Instrument
Serif are also excluded by RU exact-name rules even when selected; Latin Kobalt still
renders directly and is not governed by a font picker. Separate Literata/Lora are gone,
so there is only the original NV Literata entry. KF/NV existing counterpart deduplication
is preserved. Noto Serif returns to specialist fallback classification and its old role
name is not offered as a misleading primary alias. English picker behavior stays upstream.

## Close caption only: slightly larger, not vertically shifted

The Russian close-caption run is now90% of nominal design pixels rather than85%.
This is approximately5.9% larger than the previous prefix. Common baseline painting,
vertical returnTitleOffset, Instrument Kobalt nominal size and global85% Noto decorative
scale are unchanged. Measurement and painting use the same mixed runs. Visual acceptance
still needed; do not claim a measured device optical match based only on tests.

## Automated checks and package

Final runtime **dc721a04d319ef42c362fd7695ec6d792cae6bce**,
[CI45661021878](https://github.com/Mutaracha/kobalt_ru/actions/runs/45661021878) all jobs PASS:
full ordered validation/inventory, native RU control tests including actual engine role
resolution/new font presence/old font absence/cmap filtering, presentation mixed-run
sizing, selected cache/FB2/annotation suites, real A1/A2+device-font replay; Qt5 compile/
check-package/configured refresh-qemu/package collection. English control executable
is now explicitly run with `en` in CI, in addition to the RU ctest invocation. Prior
reports should not infer English control execution merely from RU/EN branches in test
source; presentation itself loops over both locales.

[Qt5 artifact15045412170](https://github.com/Mutaracha/kobalt_ru/actions/runs/45661021878/artifacts/15045412170),45435508bytes.
Host cache evidence15045137574,12308bytes. Tests are selected host tests, not every
repository regression or a device screenshot acceptance.

Initial45660701393 stopped at export validation: supplemental-font JSON was changed
without updating its enclosing asset-manifest hash. Fixed and checked before final
successful run. Intermediate45660940410 superseded. No runtime compatibility guard
or cache-reader algorithm change; any cache benefit from a smaller/different font set
is unproven. Diagnostics uploadf4a708c is analyzed separately in050.

Device gate: RU picker has one Literata, Sable available for Bookish, Publica for the
selected generic role, Kalam/non-Russian fonts absent in all pickers. Check close
caption size with unchanged Kobalt, RU sleep and export. Keep caches; font changes may
legitimately trigger one re-pagination. No mandatory new stopwatch matrix.
