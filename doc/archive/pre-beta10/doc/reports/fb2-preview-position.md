> АРХИВ: исторический контекст, не текущий план и не инструкция установки.
> Старые pending/next не возобновлять. Текущий источник — FB2_FORK_PLAN.md в корне.

# FB2 Text preview: source-position check

## Scope and evidence

Runtime inspected: `1b091187878695080a8dd8ebf42f6d7a84bc9d19` (build 180).
Device evidence: user commit `9b7748d`, `test & diagnostic/1b09118/`.
The initial investigation below is a source/history and source-fixture check.
No local CMake, Qt SDK or Docker is available. The subsequent implementation and
CI-built binary are described separately at the end.

## Finding

The first edit switches from the reading document to a synthetic FB2 document
whose source position has been discarded. This explains a different passage,
not merely different hyphenation of the same passage.

1. `Reader::syncAppearancePreview()` uses `session` until
   `appearanceEdit.edited() && appearanceEdit.ready()` becomes true; afterwards
   it uses `appearanceEdit.preview()`. The unedited `usePage` path can show the
   main reading image directly, whereas the edited path requests `anchoredPage`.
2. `EngineDocument::openFb2Preview()` resolves the source XPointer only to find
   the containing section/body. It then iterates children **from index zero**,
   collecting p/v text, not from the source paragraph/offset.
3. It creates a new synthetic section, explicitly removes `settings["position"]`
   and opens the result without a mapped bookmark. The preview starts at its
   beginning. Anchored rendering later cannot recover an anchor that was lost
   during construction.
4. Subsequent thickness edits operate on that same preview document. There is
   no second switch of source passage, matching +1/+2/+3 in the screenshots.

## Check against the actual book

An independent XML traversal of the canonical FB2 located the paragraph starting
«При остановке корзину…», selected its containing section and reproduced the
p/v selection and 8192-character soft limit of `openFb2Preview()`.

Result:

- Current paragraph: zero-based index **22** among selected p/v children.
- First selected paragraph: «Шеветте спилось, что сильный боковой ветер…»
  (the spelling here is the literal source text).
- Selected paragraphs: **23**, accumulated characters: **8199**.
- The current paragraph is included, but its location is not restored.

Thus the changed passage is the beginning chosen by the extraction algorithm,
consistent with the screenshot after the first edit. This does not establish
pixel-level equivalence: original native viewport, font and UI interaction were
not replayed during this check.

## History

The original native FB2 implementation, commit
`82eda3be0bf21de08b183cce977c6844cb8da467`, already collected p/v children from
index zero and removed the saved position in `openFb2Preview()`, then in
`reader_source.cpp` under `ReaderModel`. The later extraction into `reader_fb2.cpp`
(`d97b9deb`) did not introduce this loss.

The Text preview implementation in `b659ed43` already selected the separate
preview only after an edit and had the unedited `usePage` fast path. The recent
language-binding change (`3355e388`, compilation correction `1b091187`) does not
change fragment selection or position restoration.

This establishes an older position-loss mechanism, **not** that the user must
have seen it in every earlier build. Visibility depends on reading position,
section boundaries and whether the unedited preview uses the current page.
No historical binary bisect was performed, so the first visibly affected
release is not claimed.

## Existing test gap and next step

`renderFictionBook()` checks only that `openPreview()` produces pages.
`requireAnchoredPreviewRestores()` checks that drawing an anchored image does
not mutate its input document; it does not check bookmark mapping from a reading
FB2 into the newly created preview. The ru/en height-drift test addresses a
separate cross-document language problem, not this mapping.

The initial implementation recommendation was to retain/map the source text node and its offset into
the preview and use the same starting passage before and after the first edit.
Do not hide the jump by always showing a section beginning or by draw-only
cropping. A regression must start part-way through a multi-paragraph section,
including an inline element and nonzero text offset, and assert the selected
passage across thickness 0→1→2→0, while preserving main reading state. Native and UI checks were still pending at the time of that recommendation.


## Implementation and validation — build 181

Runtime: `c0ff0115eae5557b4f093176b1ca9c100d4416d4`.
[CI 44154435468](https://github.com/Mutaracha/kobalt_ru/actions/runs/44154435468)
**PASS**: focused native EPUB/FB2 regressions, offline checks and Qt5
build/package/contents. Qt6 skipped; physical device validation is pending.

`openFb2Preview()` now collects paragraphs starting at the containing source
paragraph, not at the section beginning. It retains the entire first paragraph
and computes the source text-node offset including preceding inline text in
CREngine codepoints. After opening the synthetic DOM, it resolves the marked
first paragraph and maps that offset into the preview bookmark. This is not
pixel cropping and does not move the main book. Subsequent paragraphs are
collected through nested containers with the existing soft text budget. The
pre-existing flattening of selected paragraph markup is not redesigned here.
No storage/cache policy, strings, main-book layout or refresh waveforms changed.

New `previewPosition()` in `tests/model/fb2.cpp` opens a multi-paragraph FB2,
positions the source at nonzero offset inside emphasis after 40 earlier
paragraphs, creates the preview and changes thickness 0→1→2→0. It asserts the
source text suffix is retained, the mapped bookmark stays unchanged, anchored
painting succeeds and the main source position/state/image remain unchanged.
The old section-start implementation would select the earlier paragraph and
fail this assertion; a separate pre-fix CI run of the new test was not made.

This is engine-level regression coverage, not a new end-to-end UI interaction
test. The existing focused UI/lifecycle checks also pass in this run. On the
reader, compare the visible passage before the first thickness edit, at +1/+2,
and after returning to 0, with the book open in the middle of a section.
