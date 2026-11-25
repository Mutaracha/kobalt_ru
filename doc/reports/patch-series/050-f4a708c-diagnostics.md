# Uploadf4a708c: repeated startup font-inventory incompatibility

Source: original `test & diagnostic/fd9a70ac/diagnostics.zip`, uploaded in f4a708c3;
actual startup runtime fd9a70ac67aa86593b4877947b831e3deeba0f6e. User reports one book,
one long opening after restart (probably second/third), then text-setting changes and
fast reopens. The archive contains rotated/preceding observations, including a3.9MB
book before and around the first startup; do not reinterpret all archived events as
new manual taps in the reported one-book test. Selected original events in
050-f4a708c-evidence.json; originals unmodified.

Five explicit startups:14:54:47,15:05:05,15:12:17,15:15:17,15:21:00 UTC. Pid214 is
reused; monotonic clocks reset. Session labels/folder names do not identify a unique
process by themselves. Local time for Smolensk is UTC+3; no stopwatch measurements
were supplied, and the exact user-described second/third tap cannot be uniquely matched.

## Disk checks

Six parsed disk-cache-hit checks have valid header-anchored saved inputs and reasons2.
In **every one**, the only differing primitive input is font_list; global/style are
derived differences. Node/styles/page dimensions/rendering policy otherwise match
within each saved/current pair. This includes checks after the later explicit restarts,
not just the initial transition from an older package. The recorded font_list changes:
727799554→235126840;235126840→1636766358;1636766358→3436494476;
3436494476→3974028370;3974028370→1350992438. The initial235126840 inventory appears
in two checks for different resources. The hash names a registered-face/fallback
aggregate, not the selected font or a font count. It does not identify the changed
face/path or prove whether registration order, files or some other operand changed.

| UTC opening-phases | Disk/retained | model_load_ms |
|---|---|---:|
|14:55:15.756|disk hit; archive startup/background path|17707|
|14:56:26.536|disk hit|4551|
|15:11:35.119|disk hit|6258|
|15:12:31.761|disk hit; preceding preload|6375|
|15:15:31.340|disk hit; preceding preload|6081|
|15:21:15.452|disk hit; preceding preload|6379|

These model timings are not tap-to-reading times. The clear slower **manual** reading
session in the later group is15:11:37.315: total10503ms, paginate3753ms, reasons2,
retained=false.14:56:28.724 is total8041ms/paginate3672ms. Do not state either is
certainly the user's specific second/third open.

## Later quick reopens are not proof of compatible disk reuse

Five opening-phases observations are retained_book_hit=true, with model_load_ms
77/88/94/109/105. Corresponding later session totals are about2.8–3.0s. Their resource
statistics may retain the earlier build's paginate_ms/layout-check result: those fields
must not be charged again to a fresh retained reopen. They do not constitute new disk
layout checks with reasons0. Subsequent settings/reflows also produce five cold-model
checks with reasons7/no valid saved snapshot; these are not the same case as a valid
old disk header rejected solely on font_list. Eleven cache-save events all saved=true.

Thus the user-observed quick reopens are supported, but this upload does **not** close
cross-startup layout reuse: the font inventory repeatedly differs after restart in the
same binary. Earlier successful post-reboot observations in upload214bf213 remain
valid for those runs, not a guarantee for this later configuration. Do not blame the
later manual text-setting changes for an earlier slow opening.

## Scope and next step

No cache-reading or admission fix in059. The package changes fonts/pickers/caption and
will itself alter registered inventory legitimately. A smaller built-in-role set is
not proof of a fix. Keep the font-inventory issue open as reproducible device evidence;
next investigation should identify per-face/fallback hash operands across startup,
without changing hash arithmetic, weakening safety guards, clearing caches or removing
FB2 preload. Do not expand into unrelated acceleration or demand another timing matrix
for the visual font package. Menu-status feature is still queued, not implemented here.
