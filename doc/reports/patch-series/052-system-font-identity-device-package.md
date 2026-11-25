# 059a: stable system-font identity — device-check package

Runtime: **3c7c1b20ccd052c35bd922523f8d5d00dea29ae9**.
Ordered full stack:059 Russian font roles/picker/caption, then
059a-system-font-cache-identity, then060 build integration.

## Correction

Host supplies an explicit SHA256 content identity for each registered font in
sources.systemFiles. GetFontListHash uses that identity instead of its file pathname;
all per-face metadata, document scoping, additive order independence and fallback
string remain part of the existing hash. Font open paths and private-copy lifetime
are unchanged. No special basename/path-prefix guessing or trusted digest parsed
from a directory name. This covers plain and decompressed systemFiles consistently;
all bytes are read and hashed, failure aborts font initialization rather than granting
compatibility. Other sources retain existing path semantics. No bypass of layout
admission, no cache clearing, no removal of preload.

Existing saved layouts that included temporary paths may legitimately fail once on
upgrade; after rebuilding, identical firmware bytes have stable identity. This fixes
the demonstrated mechanism, not a claim that all device startup delays are eliminated.
No device acceptance yet, and older device logs do not identify individual font paths.

## Tests

Added real engine-registry assertions to platform.font-sources:
- two independent FontCopies owners produce different private paths;
- separate EngineDocument lifetimes recreate the font manager and produce identical
  GetFontListHash for identical decoded font bytes;
- valid font bytes with unchanged face metadata but extra trailing content produce
  a different hash (full-content identity, not basename/face metadata alone).
Existing wrapper cleanup/plain/QTD/QTE coverage in platform.system-fonts retained.
CI now builds and runs both tests alongside selected previous regression gates.
This is registry recreation within a host process, not a physical reader reboot or
an end-to-end layout roundtrip through QTD copies. Existing archived FB2 cross-process
cache probe remains separate coverage of unchanged cache admission/rejection.

[CI45666186368](https://github.com/Mutaracha/kobalt_ru/actions/runs/45666186368):
ordered validation, Linux compilation/selected regressions/new font-source assertions/
archived FB2 replay, Qt5 compile/package checks all PASS.
Raw log download failed EOF in this environment; workflow configuration and successful
job steps establish test invocation, not an invented extracted individual test log.

[Qt5 device-check artifact15047034445](https://github.com/Mutaracha/kobalt_ru/actions/runs/45666186368/artifacts/15047034445),45440475bytes.
Host cache evidence artifact15048301544,12311bytes.

## Device check

Keep caches and existing book/font files. First open after installation may reflow.
After it has opened, return to library, restart Kobalt normally and reopen the same
book without changing typography/orientation. If a long opening recurs, export logs
and approximate opening time. No stopwatch matrix requested. Distinguish parsed disk
cache hit, accepted layout and retained-book hit as in050.

All059 visual/font changes are included. Queued persistent reading-menu status toggle
is not included here; keep the cache correction as an independently identifiable
hardware-check package before proceeding with that UI feature.
