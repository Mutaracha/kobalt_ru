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

A subsequent **one-off Qt6 adapter/package build** also passed at fork commit
`44b6f882e8235316580184cfc72b028a82ab0c97`
([CI 42507680309](https://github.com/Mutaracha/kobalt_ru/actions/runs/42507680309)).
No application-source change was needed for that build. Physical Qt6-device
behavior remains unverified; ongoing development remains Qt5-first.

## Withdrawn experiments — not current differences from beta7

An experimental manual book-cache cleanup action was removed from the fork
and deferred until after the beta period. A separate “quiet frontlight swipe”
experiment and its temporary diagnostics were also removed; the fork uses the
upstream light-gesture/feedback controls instead. These are not proposed as
current features, and their removal should not be read as removal of an
upstream beta7 capability.

## Proposals for upstream — not implemented in this fork

### A. Keep English embedded; load additional languages from external assets

Would you consider keeping only the English reference/fallback strings in the
binary and loading optional translations from `.adds/kobalt/assets/lang/` on
Kobo? This is the same directory shown as `.adds\kobalt\assets\lang` over USB
on Windows.

The current fork compiles both English and Russian tables into the executable;
external language packs are a **proposal**, not a description of the current
implementation. The goal is to let translators distribute and update a language
without rebuilding the application, while the reader remains usable when a pack
is missing, outdated or damaged.

Suggested contract:

- One data-only UTF-8 pack per locale, for example `ru.json`, with a locale ID,
  native display name and a format/compatibility version. The exact file format
  is open for discussion; packs should not contain executable code.
- Resolve the requested locale at startup, load the selected pack once before
  constructing the UI and use embedded English as a per-key fallback. Avoid
  file reads or parsing in paint, page-turn and individual string-lookup paths.
- Validate types, size limits and substitution placeholders. Reject unsafe
  paths/malformed packs; unknown or missing keys must not produce blank UI or
  crash the reader. Report invalid packs without exposing credentials or book
  content in diagnostics.
- Preserve a stable English key/reference table for diagnostics and translation
  tooling. Keep CI validation for distributed translations even though they are
  no longer compiled into the application.
- Support locale-appropriate plural rules, not just an English/Russian switch.
  Decide how users select among installed packs and how firmware locale aliases
  map to them.
- Define an update policy for bundled versus user-installed packs, so an app
  update neither silently destroys local translations nor assumes old packs
  contain every newly introduced key.

This would make localization independently maintainable. It does not imply
that language switching must happen live in already-created widgets.

### B. Borderless comics reading and chapter ticks

The approved per-book **Comics reading mode** is now implemented, default off.
It applies an effective zero-margin, single-column profile and hides the header,
footer and progress strip. Ordinary margin/chrome controls remain editable:
their latest saved values are ignored while enabled and used again on disabling.
The mode does not leak into global defaults or overwrite stored base geometry.
It participates in retained-layout and parsed-cache compatibility. Ordinary
books keep their existing parsed-cache policy.

Raster graphics use block layout, auto dimensions and viewport caps, preserving
intrinsic aspect ratio without cropping; this removes the inline baseline and
leading-image protection padding for the graphics profile. Natural borders or
aspect-ratio mismatch are not a reason to stretch or crop artwork. No CBZ/CBR,
new direction controls, fit-width selector or automatic spread was added.

The supplied reproduction is now available: Kobalt build 94 / d7b7d55 and
`doc/Spice and Wolf v01.kepub.epub/`. The reduced manual EPUB under
`tests/manual/books/comics/` retains the original XHTML/CSS/PNG bytes and fixes
references to removed resources. `engine.comics` uses synthetic art with the
same dimensions/inline structure to measure both orientations and 96/300 PPI,
effective margins, editable base geometry, cache/reopen and restoration. Device
verification against Nickel remains separate from these automated checks.

The lower progress strip also has TOC chapter boundaries without another
setting. They share the fill scale, deduplicate invalid/repeated page/pixel
positions, extend above filled progress and invalidate with footer/reflow state.
No book parsing takes place in paint.

Opening performance remains under investigation. FB2 now explicitly skips
EPUB embedded-font enumeration, while necessary shared font initialization and
generic-family fallbacks are preserved. No measured opening-time speedup is
claimed; see `doc/coolreader-opening-analysis.md`.

## Historical implementation: manual rendering-cache cleanup

**Status:** this was implemented experimentally, then removed before the beta6
merge and deferred until a stable release. It is **not available in the current
beta7 fork**. The following describes the final historical implementation, not
an API or lifecycle contract that should be restored unchanged.

References:
[worker/lifecycle rework 99f1af3](https://github.com/Mutaracha/kobalt_ru/commit/99f1af33e1f7d2a85f03d1633ea8c66658c7d268),
[final error/wake handling 2f127ba](https://github.com/Mutaracha/kobalt_ru/commit/2f127ba),
[removal b602d57](https://github.com/Mutaracha/kobalt_ru/commit/b602d57037aeb2c56f1cf08b0b8046635079b42a).

### User-facing flow

A “Clear book cache” list action lived in Defaults and opened a confirmation
modal. It explained that disposable caches would be removed, saved reading data
would remain and Kobalt would close afterwards. Confirmation initiated the
ordinary asynchronous close-save path; it did **not** delete files directly
from the button callback.

### Save-before-delete boundary

The reader tracked `None`, `Requested`, `Running`, `Complete` and `Failed` states.
In `Requested`, the normal close pipeline first completed progress, annotations,
native reconciliation and sync handling. Only after that save acknowledgement
could `finishExitSaved()` commit to cleanup. A save failure kept caches intact
and reused the existing save retry/cancel interaction. Cancelling the exit also
cancelled the uncommitted cleanup request.

### Retire the engine before touching its cache

At the terminal boundary, the reader disabled interaction and released the
appearance preview. `ReaderSession::clearRenderingCaches()` invalidated pending
GUI continuations and retired the session, then queued a FIFO job on the shared
engine worker. That job called `ReaderModel::discardDocument()` before deletion:
release page images and source indexes, close/reset the engine and source, and
clear the document's cache identity. This closes mapped DOM/cache resources
before deleting their files. Retired work could not start rendering or save the
document cache again on exit and thereby recreate what had just been deleted.

The worker retained the engine and paths independently of the window;
`QPointer`-guarded UI completion avoided callbacks into a destroyed reader.
Filesystem deletion was a blocking operation on the worker, not an interactive
main-thread operation.

### Precisely limited deletion scope

`storage::clearRenderingCaches()` removed only:

- the profile's CREngine document-cache directory, `.cache/`;
- the disposable layout-inspection file `cache/reopen-layout.json`, and its
  parent directory only when empty.

It did not delete source books, preferences, the reading database, annotations,
book positions or reading statistics. Missing cache targets counted as success.
An empty profile path or a symlinked cache-directory target was rejected rather
than traversed into unrelated data. Success required the targeted cache entries
to be absent afterwards. The operation was **not transactional**: partial
filesystem deletion could fail and was not represented as a successful cleanup.

### Sleep, USB, shutdown and errors

- Sleep/USB/shutdown before the terminal boundary cancelled the deletion request
  and took the ordinary transition/save path.
- Once cleanup was committed, transitions waited for the worker barrier rather
  than mapping or saving a retired DOM again.
- Sleep after commitment postponed completion/error UI until wake. It did not
  resume reading from the discarded document.
- Successful cleanup closed Kobalt. On deletion failure, the already-saved
  reading data remained intact; an error was shown before closing. Retrying
  cleanup required reopening a book, not continuing the retired session.
- A forced process exit could leave disposable caches partially removed. The
  design ordered durable saving before deletion; this was not a blanket
  guarantee against unrelated storage failure or power-loss corruption.

### Coverage and why this is historical

Regression tests were added for cache-only deletion, preservation of durable
files, missing targets, filesystem failures, symlink rejection, save failure /
cancellation, save-before-delete ordering and reader lifecycle interactions.
That describes the test code added at the time, not a new execution of those
removed tests against beta7.

The important lesson is that a cache button is not just a recursive directory
removal: it must coordinate the durable-save boundary, engine mappings, queued
work and close/sleep/USB behavior. The experiment was withdrawn to keep beta
integration focused on upstream progress/lifecycle and to avoid carrying a
parallel fork-specific close state machine. Any future implementation should
be redesigned against the then-current upstream ownership and storage contracts.

Comics/progress verification: d5c64a5 passed native regressions and Kobo Qt5
packaging in run 43064523543 (Qt5 artifact 13875852133). Device evidence and
opening-time benchmarks remain outstanding; this is not a speedup claim.
