# System font temporary-path hypothesis

Source inspected: full ordered runtime including059, same relevant path/hash mechanisms
as fd9a70ac. No runtime fix made in this investigation.

Confirmed code chain:
- platforms/kobo/common/reader_environment.h: fonts() calls systemFontFiles with
  KOBALT_SYSTEM_FONT_DIRECTORY and forwards resulting systemFiles to engine inputs.
- src/assets/system_fonts.cpp: QTD wrappers are decompressed to FontCopies; plain
  files retain original paths, QTE files are skipped.
- src/assets/assets.cpp: FontCopies owns a QTemporaryDir named kobalt-assets-XXXXXX;
  copies live below <random root>/<SHA256 bytes>/system/<filename> and are removed
  with the owner. Content digest does not stabilize the random parent directory.
- src/assets/font_catalog.cpp: systemFiles become BuiltIn catalog entries with their
  full paths. Lower-priority duplicate families can be suppressed; therefore not
  every decoded system file necessarily reaches the engine registry.
- src/engine/blaze/adapter/engine_document.cpp: initializeFonts registers catalog files.
- crengine/src/lvfntman.cpp: GetFontListHash starts each face contribution from
  face.file_path.getHash(), then folds metadata, sums faces and includes fallback string.
  Registration order is explicitly immaterial. Selected size/instance caches and old
  LVFontDef::getHash bias logic are not operands of this implementation.

Reproduction: extracted the actual GetFontListHash body into a standalone C++17 probe,
using minimal registry structs and the lString8 polynomial (STRING_HASH_MULT=31).
Compiled with g++ -Wall -Wextra and ran assertions. Identical metadata/fallback and
paths differing only in private root AAAAAA/BBBBBB yielded
1126879720 vs939780552. Identical inputs remained equal; reversing two face entries
remained equal. This tests the actual arithmetic body, not a full Qt/FreeType startup
or disk-layout round trip. Qt build tools unavailable locally.

Conclusion: a concrete restart-dependent hash-instability mechanism is confirmed.
If at least one decoded QTD firmware face survives source precedence into registration,
unchanged font bytes can invalidate disk layouts solely because of a new private root.
This needs no user-supplied fonts. It explains retained-process stability but is not yet
proven to account for every recorded device mismatch. Existing uploaded diagnostics
lack registered per-face paths/operands; embedded_font_count=0 in final state narrows
that state only, not the entire archive. Native Qt font-family output is not the engine
registry and must not be used to rule system fonts in or out.

Closing normally removes private copies, not necessarily corrupting any cache. The
next launch hashes new paths against saved old paths. No evidence here of a shutdown
write-order defect. Other possible operands: registered face set/metadata, fallback
string, document font scope. Style-sample cache already strips the private prefix in
src/reading/style_sample_cache.cpp; CREngine layout hash does not.

Next implementation should provide stable content-aware identity for owned system
copies without conflating arbitrary files sharing a basename or suppressing real font
updates. Keep private file lifetime/Qt mappings intact and cache compatibility guards
strict. Add two independently owned-copy engine/cache regression and per-face identity
diagnostics; do not infer hardware success from this standalone arithmetic probe.
