# Annotation foundation package — device checklist

## Scope

Runtime candidate `4e014dc57c04ec35a5d3dd3e0cba7f73fa91b698`.
Build 254; native and Qt5 CI verified. Use only the final package linked below,
not intermediate build 253. Physical-device acceptance is pending.

This dependency group restores shared upstream list rows, annotation storage /
session and note-aware BookmarkList. It does not yet add Reader note-editor
commands or quick-annotation gestures. Full alignment remains open. Existing
full note records, compact SQLite records and excerpt reads share one pipeline;
no new note store/schema or EPUB Hybrid algorithm change.

Use a full profile backup per doc/storage.md before testing. Do not hand-edit a
live profile or overwrite the only saved annotations just to obtain a note sample.

## Device checks (not yet run)

1. Existing EPUB and FB2: open Bookmarks and Annotations. Confirm chapter, excerpt,
   date and page captions remain legible; existing translated color/style labels
   in the annotation palette are still present.
2. Several pages of annotations: page forward/back, change sorting, reopen list,
   rotate portrait/landscape. Rows must remain selectable; no stale excerpt or
   note from a previously displayed page. Delete only a disposable annotation.
3. If a safely backed-up profile/book already has annotations carrying notes,
   verify note preview/icon below excerpt, no overlap with metadata or next row,
   no blank note block on entries without notes. Repeat light/dark. The package
   does not offer a new way to create/edit notes yet.
4. Open a bookmark/annotation after EPUB Hybrid start and after full pagination
   adoption: page captions and target navigation must stay consistent. Also
   check a plain FB2 bookmark. This exercises consumers, not modified rendering.
5. Long choice lists / short landscape: Next/Previous must expose all rows and
   retain selected choice, with controls inside the card. Check existing settings
   selectors; no new wording/settings needed.
6. Open/close reading menu: removed page/book-time strip never appears; clock,
   battery and centered session time remain. Libra 2 button behavior unchanged.

Not requested: reproducing the deferred freeze. If a new example occurs, retain
its diagnostics and action sequence separately; no fix is claimed by this package.

## Result

[CI 45033749297](https://github.com/Mutaracha/kobalt_ru/actions/runs/45033749297) **SUCCESS**:
native 9m55s; Kobo Qt5 build/offline/package/contents 5m01s.
[Qt5 device package](https://github.com/Mutaracha/kobalt_ru/actions/runs/45033749297/artifacts/14758934067),
artifact 14758934067, 37359704 bytes. Qt6 skipped; Android/full suite/device NOT RUN.
Focused CTest now runs note/list/choice-sheet/keyboard checks, highlight-store,
annotation-database/highlight-session and previous selected engine/lifecycle
checks. Full desktop.annotation* interaction suites were not selected.


CI and physical-device outcomes must be recorded separately. Qt6 and Android are
not part of the normal Qt5-first run. UI/model passes are not proof of firmware
keyboard ABI or device touch/lifecycle behavior.
