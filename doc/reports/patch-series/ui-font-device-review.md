# Device review: font roles after complete series

## Hardware evidence

The user tested full-series runtime `df8e910a` and confirmed successful book
startup and raw FB2 opening, behaving as expected. Those scenarios are accepted
on hardware; this is not blanket acceptance of cache timing, sleep, annotations,
all controls or the quarantined legacy workflow.

Original user screenshots and context:
`test & diagnostic/df8e910a-ui/screen_003-11.png` and `screen_001-22.png`.

## Requested correction and mechanism

The exit prefix must use the RU patch's UI font. Menu headings and opening
copy must not be italic; literal Kobalt must consistently use the brand face.
Opening explicitly selected italic brandFont, while many menu headings used
brandFont's RU Monotype Corsiva substitution. Corsiva remains visually slanted
even with QFont::italic false, so simply clearing that flag would not separate
the roles. The old exit prefix also relied on an inherited font family.

Correction stays in introducing patch 050:
- Explicit upright UI heading style, using interfaceFont (IBM Plex Sans in the
  default RU interface). Applied to exit prefix and menu/sub-sheet titles,
  chapter/bookmark/word/Activity titles, image viewer and Settings titles.
- Exit run measurement and painting use the same UI face for non-brand text.
- Separate wordmarkFont uses regular Instrument Serif for literal Kobalt in
  exit runs, About wordmark and the opening brand, independent of UI locale.
  RU font registration retains Instrument Serif in addition to Corsiva, so the
  wordmark is a supplied face rather than a missing-family fallback.
- Opening descriptive label uses upright interface text in normal, automatic
  and failure states. Wordmark is styled separately; caption/hint keeps its
  ordinary monospace caption role.
- Existing Serif/Corsiva policy for other uses such as sleep/statistics was
  not globally replaced. Reader engine, FB2/cache and package loader unchanged.

## Verification / package

Runtime `3e652fdc720e88711ce0d15529a0748284c9ba28`,
[CI45389989110](https://github.com/Mutaracha/kobalt_ru/actions/runs/45389989110):
full series validation, 13 Python checks, native regressions, Qt5 package/qemu
and package collector PASS. Presentation tests assert the exit-prefix font
family/slant and locale-independent wordmark role in both languages. These
checks supplement, not replace, screenshot/device review.

Artifact **Kobalt-ru-beta10-series-qt5**, `14917769837`, 44,587,044 bytes:
https://github.com/Mutaracha/kobalt_ru/actions/runs/45389989110/artifacts/14917769837

The manifest's intermediate note that the UI correction awaited CI is superseded
by this final result; the runtime patch/checksum did not change after this run.
This evidence-only update avoids another identical binary build.

Outstanding: user review of corrected fonts on the device and other previously
unrun hardware scenarios. About/source-commit.txt for this binary identify
`3e652fdc`, not any subsequent documentation-only handoff commit.
