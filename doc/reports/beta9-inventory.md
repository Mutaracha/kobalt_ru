# beta9 inventory — post wireless / sync-exclusion checkpoint

Current implementation snapshot: `e04c5d8446d6dc71cf13c01d65767b39054b59a6`;
pinned upstream: `be47258850ffdc3295257475afdf8b8f6cb53176`.

Current exhaustive [delta + verification](beta9-delta-current/README.md):
389 nonidentical tracked paths;215 different/165 fork-only/9 upstream-only,
1159 identical. [TSV](beta9-inventory.tsv) is the 232-path runtime/build/assets/test
subset, not semantic approval. Historical [beta9→beta10 input](beta9-delta/README.md)
is retained separately. Full alignment OPEN; latest implementation CI45055269191
SUCCESS. Latest verified handoff: build280 /e04c5d84, native and Qt5 PASS.

## Closed implementation groups

- Annotations: shared rows/store/database/list and NoteEditor whole upstream;
  Reader annotation actions/quick mode/drag/reopen restored with captioned fork
  palette, section headings and approved Cancel/Dictionary wording. Qt5 drag
  branch follows upstream; retained comics/page-turn policy is not overwritten.
- KOSync: every src/sync implementation and public header, page, schedule sheet,
  notice, Reader KOSync handlers and corresponding model tests match upstream
  byte-for-byte. Upstream Reader checkpoint/save (retaining the fork lazy-Appearance test report
  flag `appearance_controls_built`) plus API consumers support
  whole-book percentage, failed-login retry, final-exchange upload tracking,
  modal-input resume and durable Android handoff. New RU errors explicitly
  approved. No independent fork KOSync policy or second writer remains.
- Sleep/storage: shared sleep preferences/snapshot pipeline already matches;
  home sleep owner, native lifecycle, last-read helper, asset verification,
  reader storage and custom pin guard restored whole upstream. Boot/remount
  verification and release/restore callbacks wired into both Kobo adapters.
  The upstream preferences sanitizer is restored with the existing note-migration
  hook. Sleep captures/details and wallpaper refresh follow upstream. Fork-only
  two-line ribbon/fit-to-width text is presentation, not a storage policy.

## Protected boundaries checked against c8192851

No changes to reader_slices.cpp, reader_session.cpp, page_button_mapping.h or
highlight_palette.h in the KOSync/storage group. The session delta from upstream
remains the comics frameImage hook. EPUB Hybrid algorithms are not rewritten.
No freeze remedy or physical-device waveform/keyboard acceptance is claimed.

## Remaining full-repository gate

The exhaustive immutable-tree [current beta9 → beta10 handoff ledger](beta9-delta-current/README.md)
is the current divergence log: all 389 nonidentical tracked paths, both Git OIDs,
modes, conservative review status and next-update action. The 232-row TSV above
is only the legacy runtime/build/assets/test subset, not a semantic sign-off.

Wi-Fi-idle/vendor/sync exclusion were already verified. Search/chapter/lazy
consumers, Android/standalone opening, diagnostic/resource/desktop reporting,
new upstream engine tests and dead native-save helper cleanup are imported.
Build278 on8486d267 passes native and Qt5; Android/Qt6 are not executed by that
workflow. Exact rotation lock and close-timeout now have their upstream consumers
and tests in e04c5d84, with CI45055269191 SUCCESS.

Remaining implementation gate: Reader quiet/preload; Debug/Settings/status and
percentage/header-footer consumers. Approved62 strings are applied; their mere
presence is not completion of those consumers. Per-file mixed hunks remain REVIEW
until justified by decisions. Protected EPUB algorithms and deferred freeze are
not implicitly fixed by any package checkpoint.
