# FB2 reliability audit against KOReader

## Reference and scope

Reference KOReader commit: `21cc368d388378b6fc8ecdbfc879d8f89c17eca1`.
Reviewed `frontend/apps/reader/modules/readerrolling.lua` (independent logical
XPointer across reflows), `readertypography.lua` (metadata language before render,
explicit embedded language policy), `frontend/document/credocument.lua` (CREngine
properties), and `frontend/ui/data/css_tweaks.lua` (FB2 note classification).
These are architectural references, not a claim that KOReader guarantees every
book or that its Lua frontend can be transplanted into Kobalt.

Our shared CREngine already provides parsing, shaping, hyphenation, embedded
languages and note layout. We retain the upstream EPUB path and the accepted
FB2 note CSS, refresh policy and language-to-DOM binding. No new settings,
translations, engine update or FB2 ZIP support are introduced.

## Changes in this pass

### Stable book identity at the format boundary

The previous FB2 probe searched for `<FictionBook` anywhere after an XML
prefix. Thus XML with another root and an FB2 tag inside a comment could be
misclassified; a real book beginning with a comment without XML declaration was
not recognized. The byte probe now skips whitespace, comments, PIs and a doctype
and checks the actual first element. It preserves windows-1251 bytes and keeps
the existing 64 KiB probe window and entity-declaration guard. It is recognition,
not a complete validating XML parser or a new security guarantee. UTF-16 and
namespace-prefixed roots are not added in this pass.

### Preview language and exact bookmark restoration

KOReader keeps full language tags and an independent logical bookmark. Our FB2
preview retained its bookmark after build 181, but still truncated metadata tags
to two characters and discarded an explicit FictionBook root language override.
The synthetic preview now retains both the complete metadata tag and root lang.
An unresolvable/out-of-range mapped offset fails with the existing error rather
than silently clamping to a different reading position. This changes only the
FB2 synthetic preview; it does not redesign its existing flattened markup.

New Cyrillic cases cover prose, nested cite/section, poem verse and subtitle
fallback; a supplementary Unicode character before the inline bookmark checks
CREngine codepoint rather than UTF-16 offset accounting. Font-size reflows must
retain the preview bookmark/text suffix without changing the source state.
Regional metadata and an explicit root override are checked separately.

### Bounded single-resource transfer

The FB2 virtual-resource helper previously checked file size only before reading
and then followed EOF. It now caps delivery at the expected size and rejects
mid-stream size changes, premature EOF, file errors and cancellation. Tests grow
and truncate a separate fixture from the sink callback, alongside existing
lease/error-precedence tests. This is Kobalt integration hardening, not a copied
KOReader fix. It does not detect every same-size in-place rewrite or filesystem
race; immutable snapshots/content revalidation would be a different contract.

## Validation

Pending native EPUB/FB2 regressions and Qt5 build/package CI. Local Qt/CMake/Docker
are unavailable. New tests are included in the already-selected engine.fb2 and
model.local-resources checks. Existing reported-book language/height tests,
notes, cache, positions and shared EPUB regressions remain enabled.

Earlier device acceptance of build 181 remains evidence for that binary, not
acceptance of these new changes. Device validation of this pass is separate.
