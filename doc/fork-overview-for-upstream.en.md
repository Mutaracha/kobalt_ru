# kobalt_ru: changes relative to upstream Kobalt v1.0-beta.7

I maintain a Kobo-oriented fork of Kobalt with native FictionBook 2 support,
a Russian interface and several reading/UI adjustments. This is an overview
for discussion, not a pull request. Thank you for the original application.

## Scope of this comparison

This inventory compares upstream **v1.0-beta.7**
(`e86533c80e8aa41bfc7d79c9e445cbd4a6405039`) with fork build
**`dddace7644e80745e8b5160b1c74da5b0f4730d9`**. Later commits document the build
and add screenshots; they should not be mistaken for a different tested binary.

The fork has merged beta6 and beta7. Live appearance previews, collapsible
chapter groups, hardware-button mapping, the shared About page, standby
controls, KOSync improvements and upstream progress/lifecycle work are **upstream
features**, not contributions claimed by this fork. The items below describe
the remaining fork-specific functionality and supporting implementation.

## 1. Native, single-file FictionBook 2 support

- Open ordinary `.fb2` files without conversion, using the existing CREngine
  backend rather than a second rendering engine.
- Integrate FB2 opening with Kobo's native library and the existing “Open with
  Kobalt” action, including the default-reader path.
- Recognize raw FB2 XML before EPUB ZIP validation. Preserve the original byte
  stream so CREngine can decode UTF-8 and Windows-1251 declarations correctly.
- Adapt the publication/resource layer to a single-file publication. Use a
  full-content fingerprint for invalidation rather than assuming an EPUB
  archive or its central directory.
- Use the FB2 stylesheet from the pinned CREngine/KOReader code, with FB2
  metadata hidden from the reading flow and support for titles, poetry,
  epigraphs, tables, inline formatting, images and covers.
- Reuse the existing reading/session infrastructure for contents, internal
  links, bookmarks, annotations, cover/sleep presentation and appearance settings.
- Keep Kobalt's own FB2 positions. Do not interpret Nickel EPUB bookmarks as
  FB2 locations; native progress integration uses time/percentage rather than
  importing or exporting incompatible structural positions.

**Limits:** single `.fb2` files only, within the existing publication-size
limit. `.fb2.zip` and DRM support are not included. This is primarily a Kobo
Qt5 fork; shared platform code is adapted, but that is not a claim of equivalent
Android or Kobo Qt6 device validation.

## 2. Bottom-of-page footnotes for FB2 and EPUB

The fork enables CREngine's existing in-page footnote mechanism, following
KOReader's standard stylesheet profiles. It does not introduce an application-side
footnote pagination or drawing engine.

- Add a **Footnotes at the bottom of the page** option under the book-content
  section of Styles. It defaults to enabled, is persisted with appearance
  settings and is restored to enabled by the appearance reset.
- Support standard FB2 secondary bodies named `notes` and `comments`.
- Support EPUB note semantics through the engine's CSS/hint pipeline, including
  `epub:type` note/footnote/endnote/rearnote and corresponding document roles.
- Add a deliberately narrow InDesign EPUB 2 rule for individual
  `._idFootnotes > ._idFootnote` blocks. Do not classify the entire note group,
  source links or backlinks as footnote bodies.
- Do not promote arbitrary numeric links or chapter destinations to page notes.
  Popup-note recognition and in-page CSS classification remain distinct.
- Use smaller note text, based on KOReader's reference profiles. Numeric FB2
  note titles are reduced and raised; brackets are removed from those titles
  only. Source-reference labels and nonnumeric note titles are preserved.
- Retain tap-to-open note popups, including when in-page notes are disabled,
  subject to the reader's normal link-control setting.
- Allow the engine to continue long notes across pages. Repagination keeps
  the source-text position rather than relying on an old page number.
- Map FB2 inline/block vocabulary to the existing safe popup HTML subset so
  emphasis, verse lines, titles and bylines retain their structure.

This is support for recognized note markup, not a guarantee for every publisher's
custom note format.

## 3. FB2 appearance, previews and cache compatibility

- Apply the shared typography/layout/normalization pipeline to FB2 rather
  than maintaining a copy of the old EPUB settings implementation.
- Extend normalization/alignment classification for FB2 semantic elements:
  ordinary prose remains adjustable while poetry, epigraphs, titles, notes
  and related special blocks are protected from inappropriate prose rules.
- Select the format stylesheet **before parsing/classifying** the document,
  including when an engine instance previously opened the other format.
- Provide FB2 section-preview support without treating the source as an EPUB
  archive, and preserve the shared anchor-preview behavior introduced upstream.
- Separate FB2 cache identity from EPUB and include the relevant format/note
  policy in cache validation. Old incompatible parsed data is rebuilt on open;
  the change is not a reason to delete user settings or reading progress.
- Keep fork appearance preferences in the content-change/default/reset paths.

## 4. Russian localization and locale selection

- Add a complete Russian string table while keeping English as the reference
  and runtime fallback. Include the new beta7 strings and review long labels
  in their actual reader/settings context.
- Extend the CMake string tooling to generate both language tables and reject
  key or placeholder mismatches. No Python dependency is added to this build
  path.
- Add Russian one/few/many plural selection for count-bearing UI, including
  library, chapter and annotation/bookmark-related labels.
- Resolve the locale before constructing the reader UI. On Kobo, recognize
  Russian entries in `ApplicationPreferences/ExtraLocales`, including devices
  whose localization patch leaves `CurrentLocale` at `C` or `POSIX`.
- Accept both QSettings representations of a comma-separated `ExtraLocales`
  value: an INI string list and a quoted string. Use `CurrentLocale` and the
  existing environment/system fallback path when appropriate.
- Keep canonical diagnostic string mappings in English regardless of the
  displayed interface language; localization does not change stored book data.

### Hidden language override for testing

Five quick taps on the fork build line in About open English / Russian /
Automatic selection. The choice is stored as `.lang_en` or `.lang_ru` inside
the diagnostics directory, with an explicit override taking priority over
automatic detection. Automatic removes the override. Both-marker precedence
is deterministic, and persistence failures are reported rather than shown as
success. The language changes on the next reader opening, not by retranslating
the currently open widget tree.

## 5. Russian hyphenation

Bundle a Russian/English-US hyphenation pattern resource for CREngine and
update the asset manifest, acknowledgments and license notices. This is a
reading-language resource in addition to the interface translation.

## 6. Reading controls

### Quick dark-theme action

Add a half-filled-circle action to the reading toolbar. It uses the same
preference and application path as the existing dark-theme setting, so the
button and checkbox remain synchronized. Persistence failures restore the
previous state. Its state is expressed by the current palette rather than an
additional inverted checked-button fill. The action participates in the
upstream adaptive toolbar layout.

### Optional swipe inversion

Add a persistent **Invert page-turn swipes** preference, disabled by default.
It reverses horizontal page-turn swipes while respecting the book's RTL/LTR
direction. It does not reverse tap zones, hardware buttons or light gestures,
and does not enable swipes when the selected navigation mode disables them.
The explanatory label reflects the current direction.

## 7. Localized UI geometry and presentation

- Measure bottom-navigation labels using the translated text, not just the
  Latin capital `H`. Use a common ink envelope and baseline so Cyrillic
  descenders are not clipped or independently shifted between tabs.
- Apply the existing Settings-row presentation at construction through a shared
  helper: square indicators, common label/description metrics, horizontal
  insets, minimum height, pressed fill and dividers. This includes the new
  footnote and swipe options rather than styling them afterward through a
  fragile registry of controls.
- Use the selected interface Sans for the Settings heading and the surrounding
  text of the exit caption. Keep the word “Kobalt” in Instrument Serif;
  measure and paint mixed-font runs on one baseline.
- Split the sleep bookmark ribbon's remaining-time caption into two lines.
  This is a presentation adjustment, not a change to the time-estimation model.
- Add the small reusable control hooks needed by these surfaces, such as
  optional checked highlighting and a configurable last radio-row divider.
  Existing defaults remain unchanged for unrelated controls.

## 8. Fork identification without replacing upstream About

Preserve the upstream version/commit presentation. Append a separate
`kobalt_ru · UTC build time` line using the existing label styling. Hook the
fork metadata and hidden language selector into the shared upstream About
page used by Library and Settings; do not maintain a second About screen.

## 9. Validation and maintenance support

- Add a dedicated Kobo Qt5 device-package workflow with offline checks,
  cross-compilation and package-content validation.
- Extend focused native coverage for FB2 opening/encoding/formatting, EPUB/FB2
  notes, cache round trips, locale/plural handling, settings, shared About,
  translated UI geometry and appearance lifecycle.
- Check that temporary anchor previews restore position, page/state and note
  fragments, including drawing/cache behavior in both preview modes.
- Compare note styling against a reference KOReader CSS setup and run an
  additional opt-in regression using the original publisher EPUB. Keep that
  book outside the distributed application and outside mandatory portable
  synthetic fixtures.
- Keep device captures and supplied test materials with provenance/checksums;
  document fork module ownership, storage compatibility and the update process.
- Isolate format-specific and branding helpers so future upstream beta merges
  can reuse upstream screens, formatting and lifecycle code.

The checked build passed the dedicated
[CI run 42500439074](https://github.com/Mutaracha/kobalt_ru/actions/runs/42500439074),
including the focused native selection, separate publisher-book invocation and
Kobo Qt5 package checks. That is **not** a claim that every desktop test, Android,
Qt6 or every real-device visual scenario has been validated. Screenshots of
this binary were subsequently supplied in
[commit 2cdb862](https://github.com/Mutaracha/kobalt_ru/commit/2cdb86297e9af6465e0eef5961071808a32c2d50).

## Withdrawn experiments — not current differences from beta7

An experimental manual book-cache cleanup action was removed from the fork
and deferred until after the beta period. A separate “quiet frontlight swipe”
experiment and its temporary diagnostics were also removed; the fork uses the
upstream light-gesture/feedback controls instead. These are not proposed as
current features, and their removal should not be read as removal of an
upstream beta7 capability.
