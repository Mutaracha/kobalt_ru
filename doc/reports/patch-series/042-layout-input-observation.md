# FB2: current/saved style-hash input observation

## Motivation and scope

Device1da4445c proved reasons2 (style-hash mismatch) while same-process disk reuse works and a later process startup rejects again. Host synthetic short/long and archived A1/A2 with all seven uploaded Kobo fonts remain stable across processes. These results justify component-level device evidence, not another uninstrumented timing matrix or removing compatibility checks.

New ordered039 records the operands already used in calcStyleHash/calcGlobalSettingsHash. The wrapper around each global operand returns the same uint32 value to the original multiply/add formula; image scaling hash is computed once and reused. No extra DOM traversal, duplicate font-inventory walk, extra Render call, fallback substitution, or weakening of any compatibility predicate is introduced. FB2 preload remains enabled; Noto subset055 and unchanged Instrument Serif wordmark remain in the package.

## Fields

`model_layout_check` and FB2 layout-check event now include:

- `inputs`: current hash operands at first rejected check (or first check if none fail), including node/global/final style, font inventory, shaping mode, fractional setting, monospace scale/fallback adjustment, base weight/optical/DPI, language/hyphenation, image/space/CJK scaling, flags and interline. The existing checks/rejected/reasons and saved/current header values remain.
- `saved_inputs`: snapshot associated with the saved header, or empty object if unavailable.
- `saved_inputs_valid`: all26 operands were present/in uint32 range, saved rendered=true and saved final style matches the actual cached header render_style_hash. This is a diagnostic confidence flag, **never** a cache-admission condition.

Font inventory and node values are hashes, not font paths/source text. Language field is TextLangMan's existing aggregate, not a newly guessed book language. Global base_weight400 is not necessarily the requested nominal font weight450; variable/synthetic weight selection can be represented elsewhere. Node hash still aggregates node style/font identities, not just CSS. Different input totals identify a category but are not automatically spurious changes.

## Persistence and compatibility

Snapshots use optional existing document properties under `kobalt.layout-observation.v1.*`, stored as nonnegative Int64 to preserve uint32 values on32/64bit platforms. No cache header/layout version, fingerprint, rendering policy or serialization schema is changed. Old caches load normally without snapshots; missing/unanchored telemetry is reported unavailable, not a reason to reflow.

`updateRenderContext` records the operands it just used. An old compatible cache without a valid snapshot is primed once using the successful current check and marked stale for normal close-time metadata saving. This can cause an additional cache write, but not pagination or an extra save worker. The diagnostics are not literally zero-I/O. Incompatible old layout follows its pre-existing reflow path and then records new operands. All writes/reads stay on the existing engine/document lifecycle; no new global callback ownership or background thread is added. Internal engine observation also applies to other document formats using this check, although the dedicated event is emitted only for FB2.

## Tests and results

The resource-backed FB2 session test independently reconstructs the global and combined style hashes from exported operands with uint32 arithmetic; it compares them to the actual check hash. Warm opens require a valid header-anchored snapshot surviving close/cache save and independent-process reload. Cold/warm/changed-source and changed-width tests remain; altered width must still produce reasons16. Replay uses actual archived A1/A2 and the seven user fonts with asserted nominal typography.

**Runtime833c8e2804ef33230f24359c807cea20dcab0695; CI45599360375 all jobs PASS**: series validation/full ordered apply/inventory; native selected regressions and uploaded-font replay; Qt5 compile/check-package/configured refresh-qemu test/package collection. Local full ordered apply to pristine1796a325 also PASS.

Ten API-visible host notice observations were compared against previous CI45596424824: exact same current/saved style hashes, reasons, stylesheet and geometry. This corroborates unchanged arithmetic on these fixtures; it is not proof for all possible inputs. GitHub exposes fewer notice excerpts than the full set of12 JSON observations; the full host artifact retains all files. Native tests assert their own operands independently rather than relying on annotation count. Stable host A1 style4238856906/global748453547/node3861019144, A2 style249516262/global748453547/node579562604.

[Host JSON artifact15015441623](https://github.com/Mutaracha/kobalt_ru/actions/runs/45599360375/artifacts/15015441623),12317bytes. [Derived visible notices](042-layout-input-evidence.json), check139153830920.
[Qt5 package15014992792](https://github.com/Mutaracha/kobalt_ru/actions/runs/45599360375/artifacts/15014992792), Kobalt-ru-beta10-series-qt5,44944869bytes.

No device result for039 exists yet. The cause of the mismatch is **not fixed or identified within the component inputs** until the device run is analyzed. No speed claim or cache-cleaning recommendation.

## Narrow device gate

Install833c8e28; preserve current files/fonts/Accurate/layout and all caches. Detailed logging ON. No page turns or layout changes required.

1. Manually open A1, wait until readable; close book to Home and allow closure to finish.
2. Manually open A2, wait until readable; close book to Home and allow closure to finish. These two opens initialize optional snapshots under this runtime.
3. Close Kobalt normally and reboot the device, preserving logs.
4. Manually open A1 then A2, waiting for readable state and closing each back to Home as above.
5. Export diagnostics after the final close. Stopwatch optional; source of truth is current/saved input comparison, including internal preload events which are not manual UI-opening times.

No cold A/B, no cache clearing, no extra book matrix. If a first old-cache observation says saved_inputs_valid=false that is expected; subsequent saved/reloaded contexts should have a baseline. If a snapshot remains unavailable, diagnose metadata persistence/anchoring rather than infer a font/layout cause. The broad Hybrid correctness gate/menu-status toggle remain pending, and full Monotype integration stays withdrawn. Noto visual acceptance has not yet been explicitly reported.
