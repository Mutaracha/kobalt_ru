# Build 106: device feedback and next changes

Baseline: 94 / d7b7d55; tested implementation: 106 / d5c64a5.
Evidence and exact opening samples: tests/manual/screenshots/kobo-qt5/build106-review/.
User confirms per-book/default-off mode, zero comic margins, correct aspect ratio,
and progress redraw. Ghosting remains a reported device defect; the screenshot
alone cannot establish whether it exists in the framebuffer or physical panel.

## Opening times

Seconds, medians; raw arrays are preserved in opening-times.json.

| Book | 94 first (2 trials) | 106 first (1 trial) | 94 repeat | 94 after reboot | 106 repeat |
|---|---:|---:|---:|---:|---:|
| #1 | 13.80 | 9.20 | 5.10 | 4.90 | 4.85 |
| #2 | 14.65 | 17.30 | 5.70 | 5.90 | 5.70 |
| #3 | 14.70 | 12.70 | 7.20 | 7.10 | 7.00 |

Repeat changes are -0.25, 0.00, -0.20 seconds; rebooted old values are even
closer. No demonstrated material general opening speedup. First trials are
mixed, unequal in count, and not controlled cold-cache trials. Reboot preserves
disk parsed/layout caches. Do not infer parsing or embedded-font cost from wall
time alone, or silently delete caches to manufacture a cold comparison. Need
format/identity/size and opening-phases/cache-hit logs for phase attribution.

## KOReader source audit (pinned)

KOReader eb8fe6a3e4b0a8aefce4f8875083ae922963c4e2:
frontend/apps/reader/modules/readerview.lua, paintTo (lines 263-297):
- CRe uses getDrawnImagesStatistics; image coverage >=0.075 OR coverage change
  >=0.075 requests a full flashing update on first paint of a new page when
  refresh_on_pages_with_images is not false. It remembers the previous coverage
  to clean the transition away from graphics, not only onto graphics.
- The separate paging/KOpt branch requests the analogous full update for
  Kaleido color waveform + hardware dithering and non-continuous viewing.
  Do NOT describe the CRe 7.5% rule as a universal CBR-specific algorithm.
- frontend/ui/uimanager.lua documents full as high-fidelity flashing refresh,
  maintains periodic partial-to-full promotion, resets the count on full.

Source links:
https://github.com/koreader/koreader/blob/eb8fe6a3e4b0a8aefce4f8875083ae922963c4e2/frontend/apps/reader/modules/readerview.lua#L263-L297
https://github.com/koreader/koreader/blob/eb8fe6a3e4b0a8aefce4f8875083ae922963c4e2/frontend/ui/uimanager.lua

Kobalt paintTurn currently uses the ordinary screen_refresh preference, independent
of comics. Proposed effective policy: full high-quality refresh on every committed
new comic page (forward/back/jump), on mode transitions and return from overlays;
no forced refresh on every incidental repaint. Keep ordinary saved screen_refresh
unchanged. Use the existing Kobo refresh broker, not vendor-specific ioctls in
Reader. Verify real flush/waveform and full-page damage, not only requestFlash
calls; compare a captured framebuffer and panel photograph. Dithering and waveform
capabilities are separate; do not port a Kaleido-only condition to every Kobo.

## Compact mode row: pending string approval

Keep approved title. Use shared row presentation with normal one-line minimum
when no description and appropriate two-line height when described, rather than
shrinking every settings checkbox, its hit area or only its widget class.
Proposed new reader.comics_reading_mode_description:
EN: Hides margins, headers and footers without changing your layout settings.
RU: Убирает поля и колонтитулы, сохраняя настройки раскладки.
Controls remain editable; effective overrides do not rewrite their base values.

## Progress position: pending string approval

Retain existing Progress bar title. One canonical selector: Off / Bottom / Top;
reuse existing Off string. Proposed header_footer.progress_bottom / progress_top:
EN Bottom / Top; RU Внизу / Вверху. Existing enabled state maps to Bottom;
missing position defaults to Bottom. Preserve custom/preset/text independence.
Top strip goes below header (also works with header disabled), minimal symmetric
clearance, same TOC ticks/scale, and no second bottom reserve. Do not reuse the
24px bottom strip unchanged at the top. Track snapshot/damage/reflow/cache and
preview geometry; comic mode suppresses either position without losing it.
No assets or schema fields are changed before the wording is approved.

## In-page notes

CREngine f335f7a97e240f505bb8a0dfb721fcb7912a7824 lvpagesplitter.cpp defines
FOOTNOTE_MARGIN_REM=1 (root font size), used for reservation before notes.
This is NOT identical to the configured body line spacing. Observed white space
also includes page-split remainder and separation-rule/text geometry. FB2 note
CSS is a separate layer. Do not reduce padding-bottom:0.5em on the notes body's
heading blindly: it is not the generic separator from the main text.

Next: measure actual last body line bottom, separator and first note line on the
same page, plus reserved/discarded remainder. Target the ordinary visible gap
between neighboring body lines, not a hardcoded pixel value or blindly 1.5rem
(which can enlarge rather than shrink the gap). Any positioning change must
update drawing, hit testing and pagination/reservation consistently, including
multiple/split notes and cached reopening. Include EPUB and FB2 regressions.
No patch to the generic engine spacing constant is claimed from screenshots.
