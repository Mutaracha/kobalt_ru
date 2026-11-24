> АРХИВ: исторический контекст, не текущий план и не инструкция установки.
> Старые pending/next не возобновлять. Текущий источник — FB2_FORK_PLAN.md в корне.

# Note editor — verification checkpoint

Runtime 09307e421d649527cdcaa6854255e335f9861409; build 257.
Native/Qt5 CI verified; device acceptance pending. Supersedes the build 254
foundation package for testing the editor.

## Scope

Whole pinned-upstream NoteEditor; existing shared HighlightSession writer and
retry behavior. Reader draft/saved actions, note preview card, save completion and
error callbacks. Existing fork palette captions/section headers retained. Fresh
selection still uses the current toggle-mode tap to open actions; quick mode and
upstream delayed menu reopening remain a separate port. No new setting or second
note store. Five new RU strings approved in session; EN pinned upstream. Cancel
uses the existing common.cancel caption instead of importing unapproved Discard.

## Automated coverage selected

- ui.noteEditorPassageLines: whole-line passage quote bounds, short/long.
- ui.noteEditorSaveState: multiline draft, one save, controls locked while saving,
  retained text/error, retry, close and cleared keyboard bounds.
- desktop.annotationNotes: new passage + Cancel, save failure injected by SQLite
  trigger, preserved editor draft and retry, noted highlight menu, appearance
  Apply/Cancel, range adjustment, clearing note without deleting highlight.
- Existing annotations/annotationSelection/annotationGripMenus, palette caption
  bounds, shared row/choice/keyboard checks and selected EPUB/FB2/lifecycle tests.

Not a full upstream suite or all gesture/sleep/exit matrices. The first native
configuration failed missing test-area metadata (fixed without removing tests).
The second run found old two-button geometry and premature menu-on-release test
assumptions; both tests now assert the actual new layout/current gesture path.

## Device checklist — not run

Back up the full profile first, following doc/storage.md.

1. In annotation mode select a disposable passage and tap it to open actions.
   Add note must save the highlight before opening a multiline editor. Cancel
   closes the keyboard and leaves the saved passage without a new note.
2. Add a Cyrillic note with two lines; Save. Reopen it through Edit note, verify
   exact text; Cancel a modified draft and verify the stored text did not change.
3. Check note preview/icon in the Annotations list; change sorting and page away /
   back. Open a noted highlight; Color/Style must preserve the note on Apply and
   cancel unsaved appearance changes without altering stored style.
4. Clear a note and Save: only the note disappears, not the highlight. Test after
   book close/reopen, in EPUB and FB2, light/dark and both viewport orientations.
5. On Kobo specifically confirm native multiline keyboard appears and accepts
   Cyrillic/text deletion. Optional firmware QTextEdit receiver availability and
   keyboard placement are device gates, not established by a desktop pass.
6. Verify menu actions do not pass taps through to the page; handle dots still
   respond where drawn. No Settings-freeze reproduction requested.

## Result

[CI 45037956822](https://github.com/Mutaracha/kobalt_ru/actions/runs/45037956822) **SUCCESS**: native 11m00s;
Kobo Qt5/offline/package/contents 4m51s.
[Qt5 package](https://github.com/Mutaracha/kobalt_ru/actions/runs/45037956822/artifacts/14760328232),
artifact 14760328232 (37377574 bytes). Device/Android/Qt6 unverified; Qt6 skipped.
Full upstream suite not run. No Settings-freeze or new gesture fix claimed.
