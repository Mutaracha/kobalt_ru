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

EngineDrain was initially imported without save lifecycle wiring; the follow-up
now connects the accepted annotation preparation and highlight save query chains.
The query values do not yet feed ReaderSession or chrome. The actual Engine,
ReaderSession, Hybrid adoption and FB2 whole-book dispatch integration remain
pending, as do the G1–G9 final gates. This is not a device handoff.

## CI follow-up

Runtime `c9b675b390d4d73cd1206844252566bc79addb81`, run `44601310965`:
Qt5 build/package/contents passed. Native configuration failed because the new
model tests had no area labels; no native runtime result was obtained. The
follow-up assigns layout/sleep labels and documents the test entry points.
Rerun pending; this does not weaken the registration guard or omit tests.

## Accepted annotation saves

`AnnotationDraft::prepareSave` is now the complete beta9 implementation. A shared
EngineDrain lives in the asynchronous range callback through its continuation.
The corresponding beta9 block in `HighlightSession::save` retains a shared drain
through the range, overlap and chapter queries and the persistence enqueue.
Existing storage/close barriers remain responsible for actual disk writes.
No rendering mode, user strings, storage schema or framebuffer policy changed.

Added draft tests cover successful, rejected, stale and destroyed-owner replies:
the accepted query can enter/checkpoint while paused, keeps suspend blocked during
its continuation, and releases its drain when the query callback is discarded.
The beta9 native lifecycle save/suspend test is included without unrelated USB
scan changes. The focused workflow also runs existing highlight-session tests.
These follow-up runtime changes require their own native/Qt5 CI result. They do
not integrate Engine/ReaderSession Hybrid or diagnose the old post-frame freeze.
