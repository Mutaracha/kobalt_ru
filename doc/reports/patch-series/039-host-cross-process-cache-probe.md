# FB2: separate-process host probe, no reproduction yet

## Change and scope

New ordered **037-fb2-cross-process-cache-probe.patch** extends only `tests/model/fb2_cache_session.cpp`. It runs the same executable as separate cold/warm1/warm2/changed-width children sharing source/cache/font paths, serializing saved reading state between them. Each child opens through actual resource-backed ReaderSession, drains the close-time save and releases retained objects. Thus globals and in-memory font/document instances cannot survive between these probes. Existing same-process cold/warm/changed-source checks remain after a separate temporary-cache reset. Only a generated test temporary cache is deleted; no device/user cache operation is introduced.

The test captures existing CRLog messages (`Calculating style hash`, `Updating render properties`, `checkRenderContext`) into JSON alongside opening resource stats. The temporary CRLOG receiver lives through worker drain, accepts bounded hash-only output and is restored before exit. No renderer/hash/guard/cache-format or preload change is made. The host-only test source is not part of the production executable.

CI now preserves four JSON files as a small `FB2-host-cache-probe` artifact and publishes hash/operand excerpts as check-run notice annotations; this allows retrieval through the GitHub API even where redirecting full job logs is unavailable. A checked-in derived evidence JSON contains those exact parsed notices, not invented run output.

## Executed experiments

1. **ca4e9b45 / CI45498201010, all jobs PASS.** Initial fixture: 60 short paragraphs plus a 2MiB comment (cache thresholds exceeded, but not large actual text). In both warm independent processes reasons=0. Global hash=f577c6a6 and node hash=074c3075 are stable. Changed width alone gives reasons16; current/saved style remains201348428.
2. **f6c68d26e5688d1c9192577e3fa7a004ae6d7a61 / CI45499862360, all jobs PASS.** Increased to6000 paragraphs: actual text exceeds250000 characters, matching the device's automatic fast-shaping eligibility rather than faking size through comments. Both warm children again reasons=0. Global hash=f577c6a6, node hash=2ced0f61, combined style506298336 are identical across warm1/warm2. Changed width504→524 gives reasons16, while style/stylesheet remain equal. Mandatory parsed hit and actual close/save/reopen assertions pass. Changed-source isolation checks still pass.

The changed-width check is mandatory. Warm context compatibility is currently observational rather than an unconditional assertion across all fixture/font environments. Cold reasons7/policy_match=false describe initialization without existing rendered layout, not a regression. The cold log shows globalHash changing from pre-render7ea52b53 to post-renderf577c6a6, as expected from the existing `already_rendered` branch of calcGlobalSettingsHash; it is not evidence of the device's cross-startup fault.

[Derived evidence](039-host-cache-probe-evidence.json), from check-run138814210414 and138819968582 annotations. Latest [host artifact14968754597](https://github.com/Mutaracha/kobalt_ru/actions/runs/45499862360/artifacts/14968754597), 2878bytes. Latest Qt5 artifact14970054143, 44,591,347bytes, was generated successfully but is **not a requested device update**: production behavior is unchanged from1da4445c, so installing it does not add a device fix or more telemetry.

## Conclusion and limits

The plain host fixture does **not** reproduce the device mismatch, even across separate executable/font-manager lifetimes with large actual text. Therefore no claim of random per-process hashing, fixed repagination, or a safe bypass follows. These are same-host Linux tests, not cross-architecture save/load, not a Qt5 model-test execution (Qt5 job compiles/packages and runs its configured refresh/qemu check), and not a recreation of the device's full fonts/settings/documents.

The supplied device state uses Fira Sans, font_pixels56, font_weight450, line_spacing120, reading_ppi300, word_spacing80, hyphenation auto, Russian book text and other real reader settings. Current generated fixture is English/default host typography. The device stylesheet equality rules out only its own aggregate check; it does not prove global font inventory/node font/global language inputs equal. Source books and sideloaded font availability must be verified before claiming an exact host replay.

Next bounded step: replay the actual A-book source and relevant device typography/font inputs on host if available, and compare node/global components at save/check under startup/preload versus manual-open ordering. If host reproduction still fails, component-level device telemetry is justified, but should be selected from these results, not another broad stopwatch matrix. Preserve current style-hash guards and FB2 preload. No new device run is requested now; remain on1da4445c. Full Hybrid correctness gates/new Monotype family/menu-status toggle remain separate queued work.
