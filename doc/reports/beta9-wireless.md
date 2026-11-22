# beta9 wireless and folder exclusion consumer group

Pinned upstream: `be47258850ffdc3295257475afdf8b8f6cb53176`.
Previous verified fork runtime: `34811f2f` / Qt5 266.

## Implemented together — CI PASS, device pending

- Whole upstream Qt5 `wifi-idle/` module, header/API, optional diagnostic module,
  verified callback installer, whole hooks.cpp, vendored NickelFix d735967
  signature/memory/detour/heartbeat helpers and original MIT license.
- CMake sources, upstream fix/diagnostic options, both independent Qt5 tests.
  Existing fork engine build recipe, source filtering and provenance retained.
- Whole upstream `nickelfix.h` ownership detection: installed/loaded NickelFix
  owns the fix. Fingerprints, required symbols and runtime state guard the
  correction; unknown firmware, Bluetooth unknown/on, active/recent KOSync,
  connection setup, interactive reading and native sync retain native policy.
  This code never substitutes its own radio-off operation.
- Whole upstream sync_exclusion h/cpp and matching lifecycle test. Both Kobo
  controllers install and remove the exclusion only at the existing supported-
  firmware initialization/uninstall points. The receipt restores the prior
  value only if the user has not changed it. Invalid/custom regex is preserved.
- Original vendor MIT text is also embedded via assets/notices/NickelFix-MIT.

No new settings or user-facing strings. A/B hardware diagnostics are OFF by
upstream default; no polling timer is created in the ordinary package. No
battery-saving, real-radio or sleep-drain outcome is established by mock tests.

## Checks

Local offline suite and source allowlist checks PASS. No Qt SDK locally.
Qt5 CI must run pewter.wifi-idle and pewter.wifi-idle-log and build the real ARM
installer/vendor sources; focused native includes the new sleep folder-exclusion
test together with previous KOSync/annotations/sleep/FB2 checks.

Device NOT RUN: back up both the Kobalt profile and Nickel's
`.kobo/Kobo/Kobo eReader.conf` before testing. Known and unknown firmware, Bluetooth on/off/unknown, active
KOSync and native sync, reconnect/standby and both plugin load orders; verify
NickelFix owns the fix when installed. Folder exclusion should keep wallpapers
out of My Books and preserve an existing calibre/custom expression on uninstall.

## Scope boundary

The beta.10 progress-bar replacement is PLANNED under D04 at the user-specified
4a9576ae017684d290dd2311f4f5195e5e043274. Its changes were inspected only to record
provenance and consumers. No beta.10 runtime or progress-bar code is imported here.
Reader opening/preload, chapter/navigation and other inventory groups remain open.

Initial CI 45050260780 / 80223bd2 rejected malformed CMake nesting in the
contextual merge of the build-option loop. The whole upstream foreach block is
restored; no test or compile option removed. Local offline/source checks did not
configure the actual Kobo project and did not establish its CMake validity.

The planned beta.10 commit adds `header_footer.progress_bar_bottom` / `_top`
(English Bottom / Top) and the upstream `Layout::progressBarTop` / `BarEdge`
contract. Reuse the already accepted RU «Внизу» / «Вверху» wording when it is
ported. Remove the fork bar renderer rather than layering this contract on top
of it. No menu page/time-to-end strip is authorized by that planned replacement.

## Verified checkpoint

Runtime `2c5d2c554b50181b1d03aa4ee2e14779c3866104`, Qt5 build268.
[CI45050356339](https://github.com/Mutaracha/kobalt_ru/actions/runs/45050356339)
completed SUCCESS: focused native regressions and real Qt5 ARM package/checks.
The Qt5 job took 5m08s. Device package artifact14765951556 (37417243 bytes):
https://github.com/Mutaracha/kobalt_ru/actions/runs/45050356339/artifacts/14765951556
Native results artifact14765892558. No device acceptance claimed; Android/Qt6
not run. Initial failed CMake merge superseded by this complete successful run.
Full log download was blocked by the runner log host/network; do not infer a new
native test count from build266. Registered test targets are not individual
hardware assertions.
