# beta9 inventory — post KOSync/sleep checkpoint

Runtime source: `d8c94b3423ca1d0c9678e8104ae2df6232a11929`;
pinned upstream: `be47258850ffdc3295257475afdf8b8f6cb53176`.
All Git-tracked fork paths compared against the upstream archive; scratch build
outputs and the dirty root are not inputs. Counts include docs/uploads:

- identical: 1083
- different: 248
- fork-only: 155
- upstream-only: 52

[TSV](beta9-inventory.tsv): 308 nonidentical runtime/build/assets/test paths.
A byte/path inventory is not a claim that every retained difference is justified.

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
  Sleep captures/details and wallpaper refresh follow upstream. Fork-only
  two-line ribbon/fit-to-width text is presentation, not a storage policy.

## Protected boundaries checked against c8192851

No changes to reader_slices.cpp, reader_session.cpp, page_button_mapping.h or
highlight_palette.h in the KOSync/storage group. The session delta from upstream
remains the comics frameImage hook. EPUB Hybrid algorithms are not rewritten.
No freeze remedy or physical-device waveform/keyboard acceptance is claimed.

## Remaining full-repository gate

The requested KOSync/sleep/storage consumer group is distinct from exact platform
parity. Upstream-only files still include Qt5 Wi-Fi-idle patches/vendor detours,
sync folder exclusion, Android brightness/picker helpers, desktop engine monitor,
scenario resource reporting and new independent engine tests/fixtures. Remaining
Reader diffs also include status/debug/percentage controls, chapter/navigation
consumers and quiet preloading, besides intentional fork presentation. These
cannot all be described as cosmetic merely because the current CI passes.

The exhaustive semantic disposition of those retained/missing consumers is
still OPEN; this report does not certify complete beta9 alignment. Frozen EPUB
algorithms and deferred freeze investigation remain explicit exclusions. Keep
this gate visible before new fork functionality, rather than treating a package
for device testing as proof of whole-repository parity.
