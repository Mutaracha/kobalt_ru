# Links and search checks

Label `links`. Build `mac-tests`, then run `ctest --preset mac -L links` from `platforms/desktop/`. `ctest --preset mac -N -L links` lists its tests.

These checks cover link classification, footnote panels, direct link taps, link reveal, links in the bookmark corner, engine search and search results in the reader. The `engine` checks run on CREngine directly, and the `desktop` and `lifecycle` checks drive the production reader. Word selection in the reader is in [Input](input.md).

The native suite launches no external browser. Physical touch accuracy still needs device validation.

The links engine regression includes a synthetic InDesign EPUB 2 matrix for
`._idFootnotes > ._idFootnote` (individual referenced blocks, neighbours,
continuation, off/on and popup). For a supplied publisher EPUB, invoke
`kobalt-links-tests --publisher-notes PATH` with the headless Qt environment
through `run-test.cmake`, using an explicit temporary root and a longer timeout.
The fork device workflow runs that opt-in check against the unchanged book
provided in commit 02953e6, separately from the portable suite. No commercial
text/fonts are embedded in generated fixtures or the application package.
