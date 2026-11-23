# 023: raw-FB2 platform inputs and note preference migration

Applied strictly after 010/020/021/022 to pristine beta10. This is a prepared stage, not complete FB2 or device acceptance.

## Ported

- Kobo book eligibility: case-insensitive `.fb2` and the three retained FictionBook MIME aliases. Preview volumes, missing local files and `.txt.epub` exclusions stay in place.
- Raw FB2 does not use EPUB database/Adobe/LCP checks. All original beta10 EPUB guards remain, including unavailable protection metadata and Qt6 API checks. Format eligibility is not proof of valid book contents: publication opening still validates the file through 020/021.
- Qt5 and Qt6 NativePublication use the existing local publication source for FB2 without importing EPUB chapter/container positions. Explicit native annotation bookmarks remain rejected rather than silently redirected to the start. Kobalt's own saved DOM position is handled by the existing reading state.
- Legacy `fb2_inpage_footnotes` migrates to `inpage_footnotes` in global preferences and saved-book opening state. A canonical boolean, including false, wins. Malformed legacy values are discarded and the obsolete key is removed. Upstream notes implementation remains shared with EPUB.
- Existing nickel-book-menu and appearance-settings regression targets extended; no existing upstream tests removed. The series CI now builds and runs these alongside publication/resource/engine tests.

## Validation

Local: all nine series tooling tests PASS; actual nested export applies 010 through 023 and asserts both platform branches and the saved-book migration are present. `git diff --check` on generated patch PASS. No local Qt toolchain is available.

Native C++ runtime tests, Qt5 compilation/package and device behavior remain pending CI/hardware. Qt6 NativePublication has not been cross-compiled in this workflow; desktop Qt6 tests exercise shared menu code, not firmware ABI integration.

## Remaining FB2 scope

FB2.ZIP is out of scope, not an unported fork feature: the old fork did not support it through the system library. The planned 024 archive stage was an erroneous scope expansion and has been removed. No system-library workaround or new archive format support is requested. Desktop/Android integration and device behavior still require verification, not a new archive implementation. No opening acceleration, Hybrid fix, comics or device key mapping is included here.

## CI repair

CI 45267840438 failed before compilation in both SDK jobs: single quotes around the new ctest regex prematurely closed the Docker shell argument. Use double quotes inside that argument. Test selection also includes the actual `platform.nickel-book-menu` group (not `model`). `--no-tests=error` prevents an empty selection from looking successful. A Python regression runs `bash -n` on the actual workflow script.

The separate legacy Kobo workflow built its Qt5 device package successfully, but its native regression job failed compiling `tests/model/ui.cpp`: duplicate `const auto actual` declarations. Remove the duplicate only, preserving the image/clipping assertion. That workflow builds the current legacy checkout, not the beta10 patch export; success there does not validate the new series.


## Confirmed CI

Run 45270389485 on 7facfcb9: all three series jobs PASS, including native
FB2 engine, platform.nickel-book-menu and model.appearance-settings, plus Qt5
build/check-package. Legacy workflow 45270389509 also PASS. 023 is now ready as
a CI-verified series stage, not as blanket hardware/Qt6 firmware acceptance.
