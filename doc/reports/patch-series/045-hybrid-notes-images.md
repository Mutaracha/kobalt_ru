# Hybrid FB2: notes and body-image parity

Test-only ordered045 extends fb2_engine with a long FB2 fixture containing a
properly namespaced note reference, notes body, early and late body-image references
to an embedded PNG. Both inpage_footnotes=false/true are opened against an independent
Accurate full reference. Conservative full fallback is permitted, not treated as failure.

Assertions: equal first rendered frame; if partial, unknown whole-book count then
successful full build/adoption; equal final page count; equal non-null binary cover;
resolvable body/note/late-image-marker anchors with equal position and rendered frame
after handoff. This covers these fixture passages, not all FB2 structures, image formats,
clickable-note popup behavior, or physical Kobo rendering. The body image is a tiny PNG;
it tests reference survival/parity, not large-image scaling or memory pressure.

Initial CI45616140465 failed at the reference search assertion. Inspection showed the
test incorrectly assumed searchText returned a final result in one call; it deliberately
uses bounded incremental batches. Test fixed to consume returned cursor until hit/done,
with a10000-batch safety bound and query/inpage diagnostic. No production change was
needed. This is not a reader defect or a relaxed parity assertion.

Revision5f8ff5320bd095d7169f14f4a77f440df94d2eac, CI45617219756 **all jobs PASS**:
full ordered apply/validation/inventory; native selected tests including expanded
fb2-engine; archived A1/A2 replay with uploaded device fonts; Qt5 build/check-package/
configured refresh-qemu/package collection. This does not mean every repository test ran.

No device update or cache clearing needed: production behavior unchanged. Keep7566324a
for pending Noto visual acceptance.044 cancellation/park/close tests remain included.

Coverage audit found generic resource-drain/lease tests in reader_session.cpp, but that
target is not executed by the current selected CI lane. Their EPUB coverage cannot be
reported as FB2 coverage. Remaining bounded work: resource-backed FB2 lease revoke/close
and ReaderSession final/pending count publication; add selected deterministic tests,
not a broad unrelated legacy test target or speculative acceleration.
