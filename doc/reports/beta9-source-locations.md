# Beta9 source-location implementation

## Scope

Imports `publication/location.h` and `adapter/reader_locations.{h,cpp}` byte for
byte from beta9 `be47258850ffdc3295257475afdf8b8f6cb53176`. The concrete adapter
exposes the upstream source-location methods; its cached spine names are cleared
when configure replaces the underlying view. This is the position-mapping part
of the future Engine boundary, not the Engine/ReaderSession/Hybrid migration.

The mapping follows source XHTML element indexes, traverses generated layout
wrappers without counting them as source elements, excludes pseudo content and
counts code points without the upstream-defined whitespace/format characters.
Restoration distinguishes exact, approximate and unresolved results. Calls do
not navigate or persist state. The caller must supply a non-null Location output.

FB2 has no EPUB DocFragment, so it does not claim an XHTML Location or spine item.
Its existing engine-anchor save/preview paths are unchanged. No saved profile,
position codec, schema, strings, rendering mode or framebuffer policy is changed.
A generic Engine header with unimplemented methods is deliberately not added.

## Validation

Upstream location-round-trip test is imported with its four existing fixtures,
plus boundary checks for invalid inputs, query immutability, reflow and source
reuse through EPUB/FB2/EPUB. The test is registered in desktop CMake's engine
layout area and explicitly built/run by the focused native workflow. Qt5 still
builds the new implementation for device compatibility. Local environment has
no CMake, Qt or Kobo SDK; native and Qt5 CI results are pending.

The wider G1–G9 gates, full upstream suite, sanitizers, Android, Hybrid and device
validation remain unrun. This is not a device testing handoff.
