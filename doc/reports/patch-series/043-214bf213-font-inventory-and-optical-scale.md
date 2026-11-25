# 214bf213 device evidence; Noto optical balance and opening hint

## Source and chronology

User upload214bf213, original `test & diagnostic/2c1914e3/diagnostics.zip` (folder label is not runtime identity). Startup identifies **833c8e2804ef33230f24359c807cea20dcab0695**, verbose=true. User reports both A1/A2 opened quickly after reboot and supplied screen_002-17.png, showing the RU Noto prefix optically too large against the literal Kobalt brand. No new stopwatch numbers were supplied. Source payloads are retained unchanged; selected original events are preserved in043-214bf213-evidence.json.

Archive time range11:22:06.449–11:38:38.067UTC. There is one explicit startup at11:22:45.956. All subsequent shown observations use pid213, with increasing monotonic time. Late11:38 observations have no separate startup event in the available archive. Therefore the user's after-reboot feedback is recorded, but the late fast pair is not independently proven to be a new process by these events. Clarify timing without rejecting the user's observation or requiring an automatic new run.

## FB2: actual changed input is now identified

Both rejected checks have valid header-anchored saved snapshots. The only differing **primitive** operand is:

- current `font_list`727799554 vs saved4054671644;
- consequently global245838870 vs saved2218558512;
- consequently combined style changes.

Node hash and all other captured operands match. Width/height, stylesheet aggregate, document flags and rendering-policy comparison also match. This is no evidence of the user changing layout settings or turning pages. `font_list` is the engine's registered-face/fallback aggregate, not a count and not merely the selected family name. It includes registered face paths/indices/names/axis ranges/slant and the fallback chain. The aggregate alone cannot identify exactly which face registration/file/path changed. Updating installed font assets or inventory may legitimately invalidate layout; nondeterministic startup inventory is another unproven possibility. Do not weaken this guard or call it a solved defect.

| UTC check | Path | Reasons | Outcome |
|---|---|---:|---|
|11:22:59.481|background A1 preload|2|font-list-only primitive mismatch; ready preload ends11:23:00.105|
|11:23:19.942 phase|manual A1 retained|not a new check|retained=true; session2711ms, inherited resource pagination3658ms is not fresh work|
|11:23:50.443|manual A2 disk hit|2|font-list-only primitive mismatch; paginate16946ms, session21742ms|
|11:38:02.045|manual A1 disk hit, retained=false|0|all26 saved/current inputs match; paginate9ms, session4032ms|
|11:38:14.007|manual A2 disk hit, retained=false|0|all26 match; paginate34ms, session4632ms|

All six cache-save events in the archive are saved=true, including one preceding the explicit startup. Internal preload is not an automatic UI-opening time. Session totals are not manual stopwatch measurements. Late fast opens demonstrate real disk-layout reuse, not a retained facade. They do not establish why earlier snapshots had another font aggregate or prove every reboot now stays compatible.

No cache engine, cache-admission, preload, or font-inventory hash change is made in this follow-up. Current instrumentation is sufficient to compare future captures if needed. No new broad timing matrix or cache clearing is justified now.

## Noto optical scale: ordered056

The user requests smaller Noto everywhere in its decorative interface role while preserving literal Kobalt. New056 applies an **85% optical scale** in RU `brandFont`; `styles::heading` and `styles::brand` use the resulting actual QFont pixel size rather than overwriting it with the unscaled design size. Both Regular and Italic use the same reduction. English Instrument Serif, literal wordmarkFont, classic Sans, reading engine typography and user-selected reading fonts are unchanged. Existing statistic-specific design sizing remains before the common role reduction, so it also receives the requested reduction; no claim that every widget uses the same absolute font size.

The mixed close title stores its nominal wordmark size separately before QSS polish: Noto is scaled once and Instrument Kobalt stays at the original nominal size. This avoids either shrinking Kobalt or applying the reduction twice after reading a scaled QLabel font. Tests cover both locales,100→85 RU vs100→100 English, QSS pixel-size agreement, italic requests, mixed-run prefix/wordmark sizing and the adjusted statistics font/measurement expectation.

The15% choice is optical calibration to the screenshot, not a claim of equal SFNT cap-height metrics or visual device acceptance. Actual boldness, x-height and glyph shapes differ. Device visual check is still required for close title and other Noto headings/tabs/opening label. Original Instrument brand pixels remain unchanged.

## Opening wording

RU `opening.reopening`: **«Подготовка к чтению»**, replacing «Повторное открытие книги». The setter changes a non-fallback/non-delayed opening hint; the new text is neutral and does not assert a failed retry or a particular cache path. Canonical standalone RU strings, its patch and manifest output hash are synchronized. Fork root assets/ru.strings has a separate ordering/content baseline, so its raw hash must not be used for the standalone canonical file.

Initial standalone check45607993706 failed because the hash was incorrectly calculated from the fork root strings rather than the standalone canonical strings. Corrected in fe89ce4a; local verify.py pristine apply/all36 output hashes/canonical RU/EN contracts PASS. Full-series runtime binary was already valid; this correction changes verification metadata only. Standalone CI rerun45609563281 **PASS**.

## Verified full-series package

Runtime **7566324aa6672d8e8960b93152da25941549ecbd**, **CI45607993683 all jobs PASS**: full series/ordered apply/inventory, native selected regressions and actual A1/A2+Kobo-font replay, Qt5 compile/check-package/configured refresh-qemu/package collection. Local entire ordered apply PASS. Local font/QSS/measurement tests do not replace visual acceptance.

[Qt5 artifact15018104913](https://github.com/Mutaracha/kobalt_ru/actions/runs/45607993683/artifacts/15018104913), Kobalt-ru-beta10-series-qt5,44944678bytes.
Host evidence artifact15018442882,12318bytes. Font files/subset unchanged from055; only role sizing and wording change. No cache-speed-fix claim.

Next device action is visual-only: close-title balance, other Noto widgets/tab clipping, neutral opening hint; preserve files/fonts/layout/cache. If the user can clarify which pair followed their reboot, record it; do not demand another run just to fill this chronology gap. Cached-layout lifecycle has positive device evidence, with the remaining unexplained inventory-change question bounded separately. Hybrid correctness gate and menu-status toggle remain pending; full Monotype-family task remains superseded.
