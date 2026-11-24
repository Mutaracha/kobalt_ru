> АРХИВ: исторический контекст, не текущий план и не инструкция установки.
> Старые pending/next не возобновлять. Текущий источник — FB2_FORK_PLAN.md в корне.

# 77564e9 — device follow-up, upload 68c77d8

## Provenance and scope

Original archives/screenshots: `test & diagnostic/77564e9/`, added by user commit
`68c77d863b98190c4d137a79d1db9a153428d2bc`. Runtime remains 77564e9.
All times below are archive UTC, not an inference about the current date.
User previously completed the combined panels checklist successfully. Subsequent
freeze is a separate open defect, not covered by that acceptance.

## EPUB Hybrid — confirmed, no algorithm change

`diagnostics epub hibrid.zip/events.jsonl`:
- 16:36:15.355: EPUB, chapter_mode_requested=true, first/last spine item=4.
- 16:36:21.262: next slice, first/last=5.
- 16:36:22.295: background whole-book instance, bounds=-1 (not evidence of a
  fallback by itself).
- 16:36:29.429: full-document-ready, from_cache=false, pages=1000, parks=3.
- 16:36:54.260: render-end successful, loaded_pages=1000, book_fraction=-1,
  page=30: full document adopted on subsequent navigation.
No engine-opening-fallback or whole-book-failed in this EPUB run. Saved mode
hybrid, inpage_footnotes=false. Initial partial view had 23 loaded pages.
The archive also contains an earlier FB2 session: chapter request is true but
bounds remain -1 as intended; this is NOT evidence of partial FB2 support.

## Freeze in Settings / Styling — deferred until a new example

Latest user decision: defer investigation until a new example. No freeze fix
claimed; the evidence below is retained as history, not an active fix queue.

Freeze archive includes current events and two history files. The relevant
pre-reboot process is pid 216; next startup is 15:19:18.913.
- 15:16:54.711: Settings opened, build 0.070 ms (already constructed).
- 15:17:03–53: successive Styling samples/windows finish; feature ids
  0,1,2,5,3,4. Last panel-open: 15:17:53.399, sample 392 ms, paint 572 ms.
- Through 15:18:22.483: touch begin/end and synthesized mouse press/release
  reach the main thread; display-flush operations finish (last 15:18:22.806).
- Suspend calls in this interval also return. No recorded uncompleted sample
  render or main-thread operation explains a permanent engine wait.
- Then a new startup; no book-close from that process in this interval.

This does not disprove the reported unresponsive UI. It rules out claiming a
proven permanent event-loop deadlock from this log. Receiver widget, active modal,
grabber and accepted/ignored state are not recorded; no stack dump is present.
Possible modal/input-state or retained overlay issue remains a hypothesis, not a
confirmed root cause. Do not disable standby, add forced flashes or change EPUB
Hybrid algorithms as a speculative remedy. Last state.json is not a capture at
the instant of the freeze and must not be treated as its widget tree.
Next diagnostic step: identify the exact last Styling window/control and whether
close/back, page buttons or power still responded; if reproduced, capture modal /
mouse-grabber / receiver evidence. No freeze fix claimed in this checkpoint.

## Screens 1–2 — opening typography, pending

English uses Instrument Serif Italic. The bundled Instrument Serif has no
Cyrillic; private-text script fallback resolves Russian to Noto Serif Italic.
Both use the same requested opening-label pixel size, but their glyph shapes,
width and perceived weight differ. Reducing only the nominal size cannot turn
Noto's shapes into Instrument's. Need a narrowly scoped opening-label choice /
size proof, not a global fallback change or synthetic horizontal compression.
No font/size modification in this checkpoint; user request remains open.

## Screens 5–6 — preset names, fix

These are Text Presets, not UI light/dark themes. Names of shipped examples are
persisted in text_presets entries in the language used at creation. Switching
interface language changes strings but previously left those saved names alone.
New fork-local displayName resolves known EN/RU shipped names by example id at
presentation time, across strip, gallery, chooser, action header, rename prefill
and delete prompt. Does not rewrite storage, alter typography/order, translate
user presets, or replace custom-renamed examples. No new wording.
Regression covers EN→RU, saved RU→EN, no mutation and custom/unknown entries.

## Screen 8 — verified functional meaning, approved wording revision

vertical_spacing_scope=protection reaches LVDocView::setVerticalSpacing's
onlyProtection flag. Drawing AND coordinate conversion gate on
RN_PAGE_PARAGRAPH_PROTECTION_GAP. Page splitting sets that flag when paragraph
protection moved justifiable lines that otherwise fit on the previous page.
It does not merely adjust the orphan line; Center/Expand applies to eligible
page content to compensate for the gap caused by widow/orphan protection.
With scope=all ordinary suitable near-full prose pages are eligible too.
Normal/off applies neither correction. With widow/orphan protection disabled,
protection-only scope normally has no such gaps to adjust.
Latest user-approved RU label: «Только зазоры от висячих строк», implemented
in assets/ru.strings. Description and EN unchanged; algorithms unchanged.
Tests/model/vertical_spacing.cpp explicitly checks protection-only adjustment,
no effect when protection is disabled and cached metadata round-trip.

## Screen 7 — version, fix

assets/manifest.json still held v1.0-beta.8; branch builds fall back to that
metadata through platforms/ci/version.sh and release-version.cmake. Corrected to
v1.0-beta.9. Do not spoof only About or replace the fork source-commit/UTC lines.
This corrects packaging metadata; it does not declare remaining beta9 alignment
work complete or create a tag/release.

## Validation

Implementation and focused CI pending at initial publication. Native Qt SDK /
Docker unavailable locally. No device acceptance claimed for these fixes.


Initial CI 44931369775: Kobo Qt5/offline/package PASS; native compilation stopped
because a newly added test variable reused an existing `restored` identifier in
main(). Renamed the new fixture variable; no assertion removed. Follow-up CI
required. Local strings generator/parity/rebuild checks pass with scratch CMake
and Ninja; default Make run failed its regeneration check (not claimed as PASS).
FontTools coverage confirms Instrument Serif Italic, NV Ancizar Serif Italic and
Libron Italic lack the Cyrillic text; Noto Serif Italic covers it. Bundled faces
are unmodified. Opening typography and freeze remain unresolved, not silently
included among the two fixes.


Verified follow-up runtime `8f1fdf4d8b57c039dd5660cd183c96276aa4b2ae`:
[CI 44932175305](https://github.com/Mutaracha/kobalt_ru/actions/runs/44932175305)
SUCCESS, focused native (including model.text-presets locale regression) and
Kobo Qt5/offline/package/contents. Qt6 skipped; no full-suite/Android/local Qt
or on-device verification of the fixes. Artifacts API: native results 14718710551
(14890 bytes), [Qt5 package](https://github.com/Mutaracha/kobalt_ru/actions/runs/44932175305/artifacts/14718635775)
14718635775 (37361585 bytes). Raw result archive not downloaded/inspected.
Only preset-name display and manifest version are fixed in this binary.


## User clarification and measured opening comparison

User clarified the last interaction: changed line-spacing / vertical-page-spacing
options, closed that setting's form, then the book stopped responding to touch
and would not enter sleep. Recovered only via long power-button hold. This is
not merely a slow sample render, nor user confirmation that sleep worked.
Kernel idle-suspend events in the log are not the user-visible reader sleep
transition. Investigate the vertical-spacing form close/apply path, modal/input
ownership and appearance commit/cancel state. No root cause or fix proven yet.

Same runtime 77564e9, EPUB full document resource_count=60 (exclude the other
EPUB with resource_count=523 and all FB2/retained-memory runs):

| Recorded run, UTC | Mode | prepare_ms | load_and_layout_ms | total_ms |
|---|---|---:|---:|---:|
| Nov 20 21:25:11, first book opening after startup | Accurate | 4838 | 10065 | 16969 |
| Nov 21 16:36:15, first book opening after startup | Hybrid | 4818 | 4528 | 11408 |
| Nov 20 21:13:35, process already used for other books | Accurate | 1216 | 9453 | 12595 |

First pair: observed saving 5561 ms / 32.77% total; load/layout saving 5537 ms /
55.01%. Against the already-warm process Accurate run: 1187 ms / 9.42% total.
Do not present this as a controlled benchmark: different dates, process/cache
states and layout choices (state snapshots show differing margins/line spacing/
normalization). Accurate hit disk document cache; initial Hybrid slice did not.
Hybrid full background build was ready at 16:36:29.429, with subsequent adoption
confirmed; it shifts work beyond first readable display, not eliminates it.

Latest user decision supersedes the proposal: «Только зазоры от висячих строк»
is approved and applied. Keep English and the explanatory description unchanged;
this copy revision does not authorize algorithm changes.
