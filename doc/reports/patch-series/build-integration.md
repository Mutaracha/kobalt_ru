# 060 — complete ordered series packaging

> Поэтапный отчёт: статусы относятся к указанному runtime. Текущее состояние и
> пакет — [сводный журнал](README.md), действующие проверки — [beta10](../../validation/beta10-device.md).

Status: **done / ready in CI; hardware acceptance pending**.
Final build source: `df8e910a714e45132ad430d88fcda30b46acd0ee`.
[Final CI 45378133723](https://github.com/Mutaracha/kobalt_ru/actions/runs/45378133723):
all three jobs PASS. All patches 010–060 are ready; the full-stack apply command
is exercised, not just prefix verification. Local Python tests: 13 PASS.

## Device artifact to use

**Kobalt-ru-beta10-series-qt5**, artifact `14911449860`, 44,582,183 bytes.
[Artifact download page](https://github.com/Mutaracha/kobalt_ru/actions/runs/45378133723/artifacts/14911449860).
The Actions download wraps an inner installer `Kobalt-ru-beta10-series-qt5.zip`.
Use this final run, not earlier prepared/prefix or legacy device artifacts.
The legacy device workflow remains disabled.

The artifact contains:
- Installer ZIP with upstream Qt5 `.kobo/KoboRoot.tgz`, runtime assets and
  `.adds/kobalt/series-provenance.json` installed beside them.
- External identical `series-provenance.json`: full source commit/version,
  upstream pin, retained reference, ordered patch IDs/SHA256, manifest checksum
  and CI run URL. The installed/external copies are compared before publication.
- `source-commit.txt`, `installed-library.sha256`, installation README and
  `SHA256SUMS` covering all other artifact files.

## Integration boundaries and checks

060 adds only installed provenance and its CMake packaging dependency. It does
not change the loader, firmware install paths or reader functionality.
`package.py` exports pristine beta10 plus the complete ordered stack, refuses
planned/prepared/incomplete hardware builds, supplies upstream-compatible
source-info.json (commit **and version**), and validates the generated package.
The Kobo About commit is the workflow source commit, not the pristine upstream
pin; the exported source is reproducible, so the dirty flag is OFF.

Checks: native regression suites from 020–050; EN/RU strings/assets validation;
Qt5 build/check-package; qemu refresh; installer tar exact library path,
owner/group0:0, mode0755; ELF32 little-endian ARM header; matching installed
provenance; checksums. Unit fixtures reject incomplete/prepared stacks and
incorrect metadata/mode; these synthetic fixtures are not hardware evidence.
The first attempt omitted the version field in source-info.json; upstream's
release-version validator caught it. Fixed without bypassing the validator.
The complete prepared build passed CI45377119064; final all-ready build above
also passed after enabling strict all-ready export.

Source/test registrations remain in their introducing patches. Redundant mixed
RU/EN hyphenation data, FB2.ZIP, obsolete Libra2 mapping, own Hybrid fixes,
progress ticks/labels and legacy opening accelerators were not reintroduced.
Generic upstream packaging without series provenance still works as before.

## Hardware handoff

Backup profile and rollback installer, verify checksums, then unpack the inner
installer ZIP at the Kobo root. Safely eject and wait for installation/restart.
First verify actual Kobalt startup with an EPUB, then raw FB2. Only after startup
check cache reopen, comics, toolbar, Debug language, annotations, presets and
sleep. See `platforms/patch-series/DEVICE-TEST.md` in the artifact.

If Nickel opens instead, collect source-commit.txt, model/firmware, installed
series-provenance.json and available startup/crash diagnostics before changing
or deleting the profile. Successful CI/package checks do not establish that the
prior startup failure is fixed. Qt6/Android hardware is outside this acceptance.
Documentation-only handoff commits do not identify a new tested binary.
