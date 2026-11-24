> АРХИВ: исторический контекст, не текущий план и не инструкция установки.
> Старые pending/next не возобновлять. Текущий источник — FB2_FORK_PLAN.md в корне.

# Physical page buttons: pinned beta9 vs fork

## Baseline and result

Checked upstream `main` via GitHub API: it resolves to the same pinned beta9
`be47258850ffdc3295257475afdf8b8f6cb53176`. Its Qt5 native_ui.cpp contents were
fetched separately, not inferred from the fork. **The Libra 2 landscape correction
is not redundant in this source.** This is a static result, not a device verdict
or a claim about an author's unpublished/newer revision.

## Path

- Kobo common reader_host_services.cpp exposes nativePageButtonDirection,
  nativePageButtonsRotated and the common positional `pageButtons::paging()`.
- Qt5 native_ui.cpp maps F23/F24 with Nickel's invert preference XOR orientation.
  Upstream swaps in InvertedPortrait and InvertedLandscape, for every model.
  Fork additionally queries Device::isIo (Libra 2), replacing only its landscape
  polarity. Native Qt6 has no fork delta here and uses its firmware mapping.
- Reader::eventFilter consumes both key press/release; non-autorepeat press calls
  pageButton, passing the physical key. This path is upstream-owned.
- Reader::pageButton first handles visible paged controls/seek preview. For saved
  book actions it uses the same native rotation flag in `pageButtons::held()`;
  absent saved actions, it keeps the native direction. Consequently the Libra 2
  correction is not canceled by the shared positional/held machinery.
- `reverseLandscapeButtons` used by the Appearance rotation controls detects
  `Device::isSpaKoboBW`, NOT `isIo`; it does not replace this correction.

Rotation flag, P / L / IP / IL:

| source | Portrait | Landscape | InvertedPortrait | InvertedLandscape |
|---|---|---|---|---|
| upstream Qt5 (all models) | false | false | true | true |
| fork Qt5 Libra 2 | false | true | true | false |
| fork Qt5 other models | false | false | true | true |

Upstream also recognizes directional arrow keys; fork Qt5 restricts this native
path to F23/F24. This is a separate retained difference, not the Libra 2 polarity
correction. Do not silently conflate/remove them on the basis of the author report.
Existing model appearance_settings tests check the four flags for Libra 2/other
models; hardware direction, firmware key generation and both landscape poses
cannot be proved by that test.

## Disposition

Keep the narrow correction pending a specific different upstream SHA or a device
example demonstrating it unnecessary. No button behavior changed in this pass.
The duplicate page_button_mapping.h include in native_ui.cpp was removed only.
