> АРХИВ: исторический контекст, не текущий план и не инструкция установки.
> Старые pending/next не возобновлять. Текущий источник — FB2_FORK_PLAN.md в корне.

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
FB2 synthetic preview. The subsequent heading fix below replaces its flattened markup.

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

Runtime: `09a7acdcb81713e6058309bb525947e7832d0a60`.
[CI №182 / 44342485883](https://github.com/Mutaracha/kobalt_ru/actions/runs/44342485883)
**PASS**: focused native EPUB/FB2 regressions, including the new probe, Cyrillic
preview and changing-file cases, plus offline checks and Qt5 build/package/contents.
[Qt5 package](https://github.com/Mutaracha/kobalt_ru/actions/runs/44342485883/artifacts/14443824738).
Qt6 skipped, physical device/all-variant not run. Local Qt/CMake/Docker
are unavailable. New tests are included in the already-selected engine.fb2 and
model.local-resources checks. Existing reported-book language/height tests,
notes, cache, positions and shared EPUB regressions remain enabled.

Earlier device acceptance of build 181 remains evidence for that binary, not
acceptance of these new changes. Device validation of this pass is separate.


## Follow-up: semantic heading preview

The supplied FB2 exposed a separate preview defect: flattening a chapter-title
paragraph removed its title ancestors and inline bold markup on the first edit.
The FB2 fragment now retains semantic ancestors and inline descendants, unwrapping
engine-generated boxes and omitting generated pseudo content. The source DOM is
not modified. Fragment size/depth/node limits remain bounded; the original anchor
is mapped by codepoint offset through the retained text descendants.

Tests cover a synthetic strong/title/margins fixture and «На другое утро» in the
reported book, preserving heading font/margins, text/anchor, repaint and source
state across thickness 0→1→2→0. Existing Cyrillic inline/verse/cite cases remain.
The initial test crash was a null font in the new probe (text nodes have no
getFont()); the probe now resolves the parent element before reading the font.

Runtime `06cd5e129b2799a0a11cd90a9270f6fb8086b785`:
[CI 44362073516](https://github.com/Mutaracha/kobalt_ru/actions/runs/44362073516)
**PASS** for the focused native matrix, reported-book integration and Qt5 package
build/contents. [Device package](https://github.com/Mutaracha/kobalt_ru/actions/runs/44362073516/artifacts/14454752686).
Physical-device verification is pending; EPUB implementation and refresh were
not changed. Temporary direct/gdb CI diagnostics were removed; the short native
failure-tail annotation remains for environments unable to download redirected logs.
