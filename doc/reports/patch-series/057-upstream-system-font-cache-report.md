# Layout cache invalidation across restarts: temporary system-font paths

## Symptom and diagnostic evidence

On Kobo, the same FB2 book could reopen slowly after restarting Kobalt even though the book, font files and typography settings had not changed. Subsequent openings within the same process were fast.

We correlated these diagnostic records:

- `startup`: application restart boundaries;
- `opening-phases`: model-load timing and `retained_book_hit`;
- `fb2-cache-layout-check`: saved versus current layout compatibility inputs;
- `document-cache-save`: whether cache writes succeeded.

In six checks where the parsed disk cache was found, the saved layout was rejected with `reasons=2`. The only differing primitive input was `font_list`; the `global` and `style` differences were derived from it. The other recorded compatibility inputs matched. Cache writes reported success. Fast subsequent openings used a retained in-memory book, so they did not demonstrate successful disk-layout reuse.

**The layout compatibility check itself is standard engine functionality, shared by EPUB and other formats handled by the engine. We added the diagnostic event named `fb2-cache-layout-check` to expose its saved and current inputs during this investigation. That additional instrumentation identified `font_list` as the only differing primitive input. The event name and detailed saved/current input dump are not present in stock beta10.**

Stock beta10 checks compatibility in `ldomDocument::checkRenderContext()` in `src/engine/blaze/crengine/src/lvtinydom.cpp`. `GetFontListHash(documentId)` already contributes to the style hash. Engine messages such as `checkRenderContext: Style hash doesn't match ...` and `CRE WARNING: cached rendering is invalid (style hash mismatch): doing full rendering` can indicate rejection, but do not identify the individual changed input. Whether those messages reach a diagnostic archive depends on logging configuration.

## Confirmed mechanism

Kobo system fonts with a `QTD` wrapper are decompressed through `FontCopies` into paths such as:

```text
…/kobalt-assets-XXXXXX/<SHA256 of decoded bytes>/system/<font filename>
```

The private directory name changes between launches. `LVFreeTypeFontManager::GetFontListHash()` used the full `face.file_path`, including that random directory. Identical font bytes could therefore produce a different font-list hash after restart if the decoded system face reached the engine registry. System faces shadowed by higher-priority bundled families do not necessarily reach that registry.

Changing only the private path reproduced a hash difference with all face metadata unchanged. Registration order is not the cause: this implementation sums per-face contributions independently of their order.

## Fix implemented

For each actually registered file from `sources.systemFiles`, the host calculates SHA256 over the complete file contents and supplies an explicit stable identity through `SetFontFileIdentity()`.

`GetFontListHash()` uses this identity instead of the pathname contribution for those files. Per-face metadata, document scoping and the fallback string remain part of the existing calculation. The real file-opening path and private-copy lifetime are unchanged. Other font sources retain their existing path semantics. Failure to read/hash a registered system file aborts font initialization rather than silently granting compatibility.

No layout compatibility guards were disabled or weakened, and FB2 preload was not removed. The fix is in the shared font/engine integration, not the FB2 parser. Existing layouts may need one legitimate rebuild when moving from path-based to content-based identity.

## Validation and EPUB follow-up

A real engine-registry regression recreates the font manager with two independently owned decompressed copies and verifies:

- identical decoded bytes under different temporary roots produce the same font-list hash;
- changed bytes with unchanged font metadata produce a different hash.

Selected regression tests, archived FB2 cross-process cache replay and the Qt5 build/package checks pass. On-device testing has not reproduced the long opening after several app restarts and a device reboot, including further testing in Accurate mode. Longer-term observation remains open. Older device logs lack per-face paths, so this mechanism cannot be claimed to explain every historical rejection.

For EPUB, please check reuse of the saved disk layout across app restarts, separately from parsed-cache hits and retained-book hits. Also verify that actual changes to system-font contents and EPUB-embedded fonts still invalidate the relevant layout. EPUB-specific on-device validation of this fix has not yet been established by our FB2 tests.
