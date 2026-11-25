# 059d: approved settings captions, language ChoiceSheet and Letter duration

## Device feedback before this patch

User screenshots from92a29364 show the Letter signature and working page status.
User explicitly confirms status switching works immediately after opening, before any
reading-time estimate. Several further reader restarts in Accurate mode have not
caused repeated cached-layout recomputation per user. This is positive ongoing device
feedback, not a new measured cache-log analysis; extended testing remains open.

Seven original screenshots preserved in `test & diagnostic/device-review-settings-polish/`,
with byte sizes/SHA256 in the diagnostic material catalog. Language-debug screenshot
shows runtime92a29364. Do not interpret screenshot page totals from different books or
font/layout choices as an independently established pagination defect.

## Approved changes

- RU settings.defaults becomes «Стандартные»; settings.about becomes «О программе».
  Rail uppercases them as before: СТАНДАРТНЫЕ / О ПРОГРАММЕ. No forced line break,
  abbreviation or renamed English section. Same string key also names the associated
  section where reused.
- All settings-rail labels, RU and EN, use94% of prior text pixels, rounded to integer
  device pixels. Nominal26 becomes24 at design density (rounding means not exactly6%
  at every density). Rail horizontal inset becomes80% of prior28 (22 design pixels).
  List-style settings, bottom navigation, row height and normal icon size unchanged;
  compact-height icon fitting uses the reduced caption metrics consistently.
  Rail still measures its widest visible caption. This reduces language disparity,
  not a promise of mathematically identical RU/EN width or an untested forced width.
- RU Letter phrase: «Осталось читать примерно %1.». Spaces inside highlighted duration
  become U+00A0 before HTML escaping, keeping ordinary duration units together. The
  entire duration can move to the next line if insufficient room remains. Highlight
  padding stays nonbreaking. Paragraph/font/signature sizing is unchanged.
- Language picker now built with makeChoiceSheet/presentChoiceSheet: shared heading,
  measured RadioList height, zero outer content-row spacing and border-only outer
  margins. Avoids nested notice padding and separated duplicate list/action rules.
  Keeps three existing choices, saved override vs effective locale, async write and
  submission lock, Cancel/backdrop behavior and saved/error feedback. On success hides
  list and refits compact heading/message/OK card. Existing object names retained;
  no live partial retranslation and no Nickel/native OS dialog dependency.

## Regression scope

Language widget test now asserts common heading, zero content spacing/border margins,
and measured list height, in addition to cancellation/reopening/async save/no live
locale switch. RU control test asserts approved captions and Letter wording; previous
bilingual signature-pixel, status-toggle/time/Stats lifecycle, font/cache and archived
FB2 regressions remain selected. Device width/spacing/nonbreaking-time visual review
is pending: no new screenshots are claimed from a host build.

Initial c92173e1/CI45685191767: Linux and Qt5 jobs passed; series regression rejected
intentional translation changes because the old010 overlay was asserted verbatim.
Updated exact expectations for these three approved keys only, keeping all other
reviewed translations checked. Locally re-ran nested full-series export regression
with archive lookup redirected to the real checkout (export directory is untracked);
all010–060 including059a/b/c/d applied and assertions passed. Final CI below.

## Final package

Runtime **5b45f2c74857180f0689f80b5e48a9134f58bec7**,
[CI45686709893](https://github.com/Mutaracha/kobalt_ru/actions/runs/45686709893): all jobs PASS,
ordered validation/inventory, Linux selected regressions and archived FB2 replay,
Qt5 compile/check-package/configured refresh-qemu/package collection.
[Qt5 artifact15056267436](https://github.com/Mutaracha/kobalt_ru/actions/runs/45686709893/artifacts/15056267436),45440784bytes.
Host cache evidence15057930772,12312bytes. Hardware visual review pending. Keeps all
previous059a cache identity/059b status toggle/059c Instrument Letter signature changes.
