# Cache and initial partial-frame correction

> Поэтапный отчёт: статусы относятся к указанному runtime. Текущее состояние и
> пакет — [сводный журнал](README.md), действующие проверки — [beta10](../../validation/beta10-device.md).

## Changes

- Render's automatic swap is skipped while `ldomDocCache` is disabled. Adapter cache scopes end after loading and are reopened for explicit saves; attempting an automatic swap between these scopes previously set the document's permanent `_maperror` flag when cache creation failed. This correction does not clear real write errors, relax source identity checks, or keep process-global cache state enabled between jobs.
- Shared whole-book coordinate capture now runs for initial and neighbour frames as well as chapter metadata. A partial document retains its provenance even when the current native position has no fraction yet (zero progress until measurable). Its local prefix page count is not a whole-book total. Existing rough/exact counts and full-ready flags remain supplied by the engine.
- Resource-backed close/reopen regression now exceeds the 1 MiB automatic-swap threshold; the previous 128 KiB padded fixture tested lookup but missed automatic Render swapping.

## Validation status

Full ordered 010–060 apply to pristine 1796a325: PASS. Python packaging/manifest tests: 12 PASS; nested export test cannot resolve the enclosing Git archive from the ignored local snapshot (same local environment limitation as before); equivalent full ordered application checked separately. Native compilation and runtime tests are delegated to CI because local Qt/CMake tools are absent.

These are source-level corrections of confirmed unsafe paths, not a claim that book1 is fixed on hardware. Device acceptance is still required: both real books, Accurate and Hybrid, close then reboot then reopen; first two Hybrid pages must not show the prefix total as the full total. The existing real-book archives remain the device inputs. No new user logs requested yet. New full Monotype family is not part of this cache/footer correction.
