# Diagnostics upload 5bbff30: opening analysis

Input: doc/diagnostics/ from 5bbff30. All 16,447 events.jsonl records parse;
no malformed JSON lines. The retained log is a tail (starts at sequence 2730),
not the complete startup/timing history. session-context startup confirms
source_commit=d5c64a5, source_modified=false, Qt5.2.1, firmware4.38.23828.
Use recorded UTC as source data, not as the documentation's assumed current date.

## Four observed openings

Ordered by occurrence in this file, NOT matched to the user's books #1/#2/#3.
Seconds, rounded. Reader shown is the app's diagnostic endpoint, not an external
measurement of the physical panel becoming clean/usable.

| Open | open-end | reader-shown total | model_load | paginate counter | UI build | show span |
|---|---:|---:|---:|---:|---:|---:|
| A | 4.644 | 8.769 | 4.366 | 3.694 | 1.293 | 1.875 |
| B | 1.304 | 5.300 | 1.287 | 0.016 | 1.262 | 1.844 |
| C | 3.356 | 7.780 | 3.298 | 2.176 | 1.497 | 1.918 |
| D | 4.789 | 9.055 | 4.083 | 2.960 | 1.439 | 1.852 |

All four: document_cache_hit=false, retained_book_hit=false. layout_cache_hit
true for A/B/C, false for D (layout_cache_stored=true). Thus layout inspection
cache is not evidence that CREngine restored its parsed document. Four misses
do NOT establish that document caching never works: no same-book/policy
reopening matrix is available here. Five document-cache-save events report
true,false,true,true,true;
false is a generic return, not an errno or proof of disk-write failure. The
current saveDocumentCache has several early returns. Need explicit miss/save
reason diagnostics before claiming a cache bug or changing cache identity.

Settings UI construction is consistently 0.529-0.541s and Appearance UI
0.308-0.313s. Total window/UI build is 1.26-1.50s. These are useful targets
for lazy construction of unopened panels, on the GUI thread, preserving their
behavior. show span is consistently 1.84-1.92s but must be decomposed before
calling it unnecessary waiting. Recorded phase/counter scopes overlap and
resource counters can cover multiple operations; do not add them as disjoint
costs or equate open-end with screen readiness.

Final session/state identify Sin City vol4 as the last book. Session resource
snapshot: 32MiB budget reached, 242 evictions, 419 loads, 417 loaded resources,
about32.1MB transferred, load counter2.55s, paginate2.96s, no failed_images and
no request_failures. This suggests examining eager resource traversal and cache
churn, not blindly increasing the budget. Counters do not prove repeated loads
of the same image or a leak. Do not sum I/O time with enclosing pagination.
Other openings have no identity field here; do not assign guessed book names.

## Next performance work

1. Explain parsed-cache miss and save=false reasons, retaining existing IDs,
   compatibility policy, reading data and privacy (no full book paths in events).
2. Compare same publication/appearance across reopenings using those reasons.
3. Audit lazy Settings/Appearance panel construction; widgets remain GUI-thread
   objects. Do not move widget creation to a worker or remove features.
4. Inspect image-heavy eager loads and resource reuse; bounded memory stays.
5. Verify first usable frame and physical refresh separately from open-end.

## Wording / policy decisions

User approved description (EN/RU) without further review:
- reader.comics_reading_mode_description:
  Hides margins, headers and footers, ignoring the current layout settings.
  Убирает поля и колонтитулы, игнорируя текущие настройки раскладки.
Progress selector Off/Bottom/Top and Выключено/Внизу/Вверху also approved.
Base values remain editable and preserved; ignoring does not mean erasing.

Refresh investigation must retain damage-region computation and test graphically
appropriate waveform selection before forcing a flash on every comic turn.
Pixel damage cannot by itself model e-ink history. Conservative full cleanup
remains a fallback; neither Nickel's exact algorithm nor partial-update quality
is proved by this log. No runtime/UI changes or speedup are claimed here.
