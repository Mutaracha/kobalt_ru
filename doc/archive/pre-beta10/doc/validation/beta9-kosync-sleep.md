> АРХИВ: исторический контекст, не текущий план и не инструкция установки.
> Старые pending/next не возобновлять. Текущий источник — FB2_FORK_PLAN.md в корне.

# KOSync, sleep/storage and annotations — device handoff

## Runtime and automated evidence

Candidate runtime `34811f2f42b64c73a557fa532a453c34851953db`, Qt5 build 266,
CI [45048708215](https://github.com/Mutaracha/kobalt_ru/actions/runs/45048708215).
CI SUCCESS; physical-device acceptance pending. The artifact below belongs to
this exact runtime, not an earlier Qt5-only pass.

Pinned upstream `be47258850ffdc3295257475afdf8b8f6cb53176`.
KOSync source/model/page/window handlers and upstream tests restored whole;
checkpoint/deferred handoff and native dialog resume consumers included. Shared
storage/assets and native sleep/USB lifecycle restored whole. Fork ribbon's
localized two-line text and fitting retained. No duplicate annotation writer or
sleep-snapshot implementation introduced. EPUB Hybrid algorithms unchanged.

The prior annotation candidate c8192851 passed native and Qt5 in 45043939833.
Intermediate b05249b9 and d8c94b34 produced valid Qt5 packages, but native test
build failed on missing fixture/build dependencies. They are not the recommended
combined verification result. The test fixture/headers/target definitions were
restored without removing assertions. Run 265 then executed 484 tests; its two
failures exposed missing invalid Rendering-value normalization and an old private
KOSync retry helper. The whole upstream sanitizer (with legacy-note migration
retained) and private KOSync helpers were restored for this candidate. kosyncDialogResume is explicitly registered.

## Automated selection

- Existing EPUB/FB2/comics/theme/settings, annotation/quick/note/drag and lifecycle
  focused checks remain enabled.
- Whole model.kosync, model.sync-coordinator, component.kosync-ui;
  lifecycle.kosync* including retained login drafts, sleep password clearing,
  modal-input resume and checkpoint/deferred handoff in whole/chapter modes.
- model.storage-assets, model.profile-storage, platform.native-lifecycle and
  platform.kobo-power-screen, including explicit assets verification/revocation,
  legacy-cache preservation, USB cancellation/remount and home-screen composition.
- Qt5 offline checks, both flush ABIs including drag cleanup, package build and
  package contents validation. Android Java tests included in source but NOT RUN;
  Qt6 device build/full upstream suite NOT RUN.

## Device checklist — NOT RUN

Back up the full `.adds/kobalt` profile before installation; use a disposable book
and test account. Do not submit passwords, keys or account files in diagnostics.

1. **KOSync:** enter an invalid server, empty username/password and confirm the
   respective validation without network submission. Fail sign-in deliberately;
   confirm the password draft survives for an explicit retry. Confirm success,
   logout, leaving the Sync page and sleep clear the draft. Open/close/sleep sync
   and manual sync must preserve the local passage and approved status wording.
   Compare progress after chapter-mode and whole-book reading, not local chapter
   page percentage. Confirm native Wi-Fi/password dialogs do not leave sync stuck.
2. **Sleep:** test chosen design, private-book mode, sleep/wake and power-off,
   portrait/landscape and light/dark. Keep fork ribbon's two remaining-time lines
   and no overlap/clipping. Outside the reader, check the last Kobalt book uses
   the selected home sleep design; a Nickel-only book keeps Nickel's screen.
3. **USB/storage:** close/reopen after USB editing; check profiles and book
   positions survive. Test cancelling USB before export and a completed remount.
   Do not deliberately damage required assets without a recovery installation.
   The new `.cache-blaze` namespace must not delete legacy `.cache` contents.
4. **Annotations:** test both toggleable/default and quick modes. A new selection
   opens actions; a grip drag hides them and reopens after about 1.5 seconds.
   Pressed grip grows and stays hittable. Quick mode hides Annotate and leaves
   temporary mode after save/clear. Confirm no taps leak through the card.
5. **Notes/refresh:** save and reopen a two-line Cyrillic note, retry/cancel and
   remove only the note. Check drag updates and one final cleanup for ghosting;
   normal page turns/comics must retain their existing waveform behavior.

Freeze investigation remains deferred until a new example. This package does
not claim a freeze fix. Complete-repository semantic alignment is a separate
open gate documented in ../reports/beta9-inventory.md.

## Result

CI 45048708215 SUCCESS: native 9m06s, Qt5/offline/package/contents 7m37s.
[Qt5 artifact 14764857536](https://github.com/Mutaracha/kobalt_ru/actions/runs/45048708215/artifacts/14764857536),
37408180 bytes, not expired at handoff. All selected 484 tests passed; no failed
test was removed to obtain the pass. Physical Kobo, Android, Qt6 and the complete
upstream suite NOT RUN. This document is not another binary build.
