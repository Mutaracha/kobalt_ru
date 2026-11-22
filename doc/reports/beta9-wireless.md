# beta9 wireless and folder exclusion consumer group

Pinned upstream: `be47258850ffdc3295257475afdf8b8f6cb53176`.
Previous verified fork runtime: `34811f2f` / Qt5 266.

## Implemented together — CI pending

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

Device NOT RUN: known and unknown firmware, Bluetooth on/off/unknown, active
KOSync and native sync, reconnect/standby and both plugin load orders; verify
NickelFix owns the fix when installed. Folder exclusion should keep wallpapers
out of My Books and preserve an existing calibre/custom expression on uninstall.

## Scope boundary

The beta.10 progress-bar replacement is PLANNED under D04 at the user-specified
4a9576ae017684d290dd2311f4f5195e5e043274. Its changes were inspected only to record
provenance and consumers. No beta.10 runtime or progress-bar code is imported here.
Reader opening/preload, chapter/navigation and other inventory groups remain open.
