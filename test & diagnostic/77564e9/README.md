# Runtime 77564e9 — original device evidence

- `diagnostics epub.zip`: user upload ffbaab3; earlier opening-history analysis.
- `diagnostics freeze.zip`, `diagnostics epub hibrid.zip`, screen_001/002/005/006/007/008:
  user upload 68c77d8; bytes preserved unchanged. Archive labels are user labels,
  not a claim that every contained session uses that format or that the final
  state.json captures the freeze instant.
- Curated analysis, limitations and fix validation:
  [device issues report](../../doc/reports/77564e9-device-issues.md).

User reported the combined Settings/buttons checklist passed before the separate
freeze report. Hybrid verification confirms EPUB partial opening and whole-book
background build/adoption; it does not enable partial FB2.

## Fonts supplied in adjacent branch commit 9d20cd4

`MTCORSVA.TTF` and the four `Caveat/*.ttf` files were imported byte-for-byte
from `9d20cd4729c4fa5fbd5370ef7c1ec39519d74a04`, without its other tree changes.
User approved RU-only Corsiva/Instrument Serif and Caveat/Kalam replacements.
Family/style and Cyrillic coverage inspected with fontTools. Caveat is OFL 1.1;
MTCORSVA has a proprietary Monotype notice (not an application-license grant).
Only Corsiva and Caveat Regular/Bold are bundled for this control test.
