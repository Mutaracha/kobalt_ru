# Hybrid: combined cold-open evidence and 7f47164 device follow-up

> Поэтапный отчёт: статусы относятся к указанному runtime. Текущее состояние и
> пакет — [сводный журнал](README.md), действующие проверки — [beta10](../../validation/beta10-device.md).

## Scope and measurement boundaries

Inputs: previous ace3c3c and 4f2cecd1 diagnostic archives, the stopwatch protocol in `034-reboot-cache-review.md`, and `test & diagnostic/7f47164/diagnostics.zip` uploaded in 30bafb0b. The latest archive has 9172 events. Its device timestamps are 2026-11-24; these are recorded device time, not an independent date assertion. The latest startup identifies unmodified runtime 7f47164f.

The test titles were renamed before the first diagnostics, so the series can be compared. Accurate/Hybrid copies have distinct fingerprints. These are device observations on two books, not a randomized benchmark. Do not pool stopwatch and logged durations, retained opens and disk hits, or interrupted opens into a single mean.

## Cold opening without parsed cache

`session.status=reading` / `opening_timings.total_ms` is used below. It is time to the reading session, not merely engine parsing or the loading screen.

| Series | Book | Accurate, s | Hybrid, s | Reduction | Speed ratio |
|---|---|---:|---:|---:|---:|
| ace3c3c | 1 | 8.621 | 5.121 | 40.6% | 1.68× |
| ace3c3c | 2 | 27.481 | 11.483 | 58.2% | 2.39× |
| 4f2cecd1 | 1 | 9.611 | 5.121 | 46.7% | 1.88× |
| 4f2cecd1 | 2 | 26.633 | 9.773 | 63.3% | 2.73× |
| 7f47164 | 1 | 9.250 | 4.862 | 47.4% | 1.90× |

The latest Accurate observation follows an interrupted 22.890 s engine open and is not a clean controlled timing pair; it corroborates the magnitude, but should not tighten the earlier range. Latest successful Accurate open itself takes 5.536 s at engine level, with model load 5253 ms, parse 232 ms, paginate 4221 ms. Hybrid has no parsed-cache hit and no retained hit; model load is 1785 ms, parse 262 ms, prefix pagination 1016 ms, engine open 1.927 s. This is specifically the partial-startup benefit rather than XML parse acceleration.

Independent stopwatch readings (±0.5 s each): book1 10.36→5.87 s (43.3%, 1.76×); book2 27.37→10.58 s (61.3%, 2.59×). They agree with the logged range but are not combined statistically with it.

**Practical summary:** for these two FB2s, first cold opening saves roughly 3.5–4.5 seconds on book1 (about 41–47%, 1.7–1.9×) and 16–17 seconds on book2 (about 58–63%, 2.4–2.7×). No claim of a universal speed ratio for all books. Full layout still runs later in idle time: the feature reduces waiting before reading rather than removing that work.

## Latest cache and counter follow-up

- User confirms the first book opens faster on 7f47164 and the counter no longer shows incorrect prefix totals while counting.
- All seven `document-cache-save` events in this archive have `saved=true`; previous main-series book1 saves failed 7/7. Not all seven new saves are book1, but its latest successful reading cycles at 17:50–17:52 also close with successful saves.
- At 17:50:35 the full-layout book1 reading is 9250 ms, no cache hit. At 17:50:56 retained reopen is 2674 ms (not evidence of a disk hit).
- At 17:51:35 Hybrid book1 cold reading is 4862 ms, no cache hit, startup slice active; close saves successfully. At 17:52:11 retained reopen is 2704 ms (again memory reuse, not cold opening).
- After the next startup, the last engine open at 17:53:07 has `document_cache_hit=true`, `retained_book_hit=false`, model load 6207 ms. The user identifies these last runs as book1. This confirms disk-cache reuse is now reachable; it does **not** establish a stable ~5 s reboot-to-visible-page result. The archive contains no corresponding completed `session reading` total for this last startup, and a disk hit can still require layout work.
- Thus cache save/reuse and counter behavior improved, while broad timing acceptance remains narrower than saying every cached reopening is fast.

## Next build

Exit-title `Kobalt` explicitly selects Instrument Serif, while Russian `Закрыть` retains the locale UI face. The private renderer retains Instrument Serif assets in Russian so the requested font is actually drawable, not merely a QFont family label. Other locale serif roles are unchanged.

The full-series device build now passes `KOBALT_VERBOSE_DIAGNOSTICS=OFF`: the upstream marker-backed Debug checkbox is no longer disabled by a forced-verbose build. Detailed recording can be enabled/disabled immediately and the marker persists across restart. With it disabled, the normal compact warning/error log remains; this is not a promise of absolutely zero filesystem logging. A fresh profile starts without detailed recording; an existing `diagnostics-enabled` marker is respected. Added regression checks both marker states and added the diagnostics test to native CI. Device acceptance of this next build remains pending.

## Build result

Runtime/package commit: `eafb6161a39518dd6ac750e640bceb37d15b27e0`. CI run 45456569246: all three jobs PASS (ordered series validation, native Linux regressions including diagnostics persistence and mixed-font presentation, Qt5 compile/check-package/qemu refresh, verified package collection). Artifact 14952120554, `Kobalt-ru-beta10-series-qt5`, 44,588,419 bytes:
https://github.com/Mutaracha/kobalt_ru/actions/runs/45456569246/artifacts/14952120554

The preceding 46614c78 run compiled/passed its native and Qt5 checks but package collection failed because the collector still expected the forced-diagnostic filename. eafb6161 corrects the collector and its unit fixture to the non-forced `Kobalt-qt5.zip` filename; it does not change the installed library behavior.
