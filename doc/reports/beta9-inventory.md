# beta9 inventory — post wireless / sync-exclusion checkpoint

Runtime source: `2c5d2c554b50181b1d03aa4ee2e14779c3866104`;
pinned upstream: `be47258850ffdc3295257475afdf8b8f6cb53176`.
All Git-tracked fork paths compared against the upstream archive; scratch build
outputs and the dirty root are not inputs. Counts include docs/uploads:

- identical: 1102
- different: 247
- fork-only: 158
- upstream-only: 34

[TSV](beta9-inventory.tsv): 290 nonidentical runtime/build/assets/test paths.
A byte/path inventory is not a claim that every retained difference is justified.

Wireless consumer group: Qt5 build268 / [CI45050356339](https://github.com/Mutaracha/kobalt_ru/actions/runs/45050356339)
SUCCESS (native and Qt5 jobs). Device artifact14765951556, 37417243 bytes.
[Details and device boundaries](beta9-wireless.md). Inventory counts include
tracked documentation as of runtime commit; subsequent handoff docs are not
counted as runtime changes. Whole alignment remains OPEN.

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

The exhaustive immutable-tree [beta9 → beta10 handoff ledger](beta9-delta/README.md)
is the current divergence log: all 439 nonidentical tracked paths, both Git OIDs,
modes, conservative review status and next-update action. The 290-row TSV above
is only the legacy runtime/build/assets/test subset, not a semantic sign-off.

Wi-Fi-idle/vendor and sync folder exclusion are now imported and CI verified;
the former text listing them as missing was stale and is superseded. Remaining
Reader quiet/preload, chapter/search/navigation, native-save tracing, Android
brightness/picker, desktop/scenario monitoring and upstream tests are detailed
in B9-02 through B9-09. Full alignment gate remains OPEN. Protected EPUB algorithms
and deferred freeze are not implicitly fixed by any package checkpoint.
