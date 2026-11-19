# Beta9 worker and shared contracts

This checkpoint imports the beta9 engine worker, EngineDrain activity support,
plain engine values, query states and font defaults from
`be47258850ffdc3295257475afdf8b8f6cb53176`.
The existing adapter now uses those shared values, without changing its
whole-book rendering mode. Publication font validation uses the common resource
bound; a compile-time test checks it against Blaze's limit.

The upstream activity concurrency test passes locally (g++ C++17/pthreads).
New worker and query-contract tests are registered in desktop CMake and the
focused native CI lane. They require CI's Qt environment; no local Qt or Kobo
SDK is installed. No device validation has been performed for this checkpoint.

EngineDrain is available but is not yet connected to the reader save lifecycle.
The query values do not yet feed ReaderSession or chrome. The actual Engine,
ReaderSession, Hybrid adoption and FB2 whole-book dispatch integration remain
pending, as do the G1–G9 final gates. This is not a device handoff.

## CI follow-up

Runtime `c9b675b390d4d73cd1206844252566bc79addb81`, run `44601310965`:
Qt5 build/package/contents passed. Native configuration failed because the new
model tests had no area labels; no native runtime result was obtained. The
follow-up assigns layout/sleep labels and documents the test entry points.
Rerun pending; this does not weaken the registration guard or omit tests.
