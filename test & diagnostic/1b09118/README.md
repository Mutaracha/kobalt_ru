# Device evidence — build 180 (runtime 1b091187)

Original uploads from user commit `9b7748d`; PNG and ZIP bytes are unchanged.

The user traversed chapter 8 and changed text settings in the reading menu;
the previous overlapping-paragraph defect did not recur in this run.

Screenshots 001–004 show Text preview at thickness +0, +1, +2 and +3.
At +0 the visible passage includes «При остановке корзину…»; at +1 the
preview starts «Шеветте снилось…» and remains on that passage at +2/+3.
This is a change of source passage, not merely different line breaking.
The reading-menu counter stays 181/720. No paragraph overlap is visible here.

The event archive records preview-start at 18:28:16.926Z and preview-end
at 18:28:17.081Z with failed=false. Subsequent render-end events at
18:28:20.392, 24.919, 28.348 and 31.984 report page=0, failed=false.
These events do not identify which setting was changed or independently
map each render to a screenshot; thickness values come from the screenshots
and the user's description.

Source inspection suggests investigating the unedited main-page to edited
preview transition in `Reader::syncAppearancePreview()` and section-start
extraction in `EngineDocument::openFb2Preview()`. This is not yet a regression-
tested root cause, nor evidence that metadata-language isolation failed.

“Open/close appearance” in the earlier handoff meant the reading menu's Text
panel with preview, not a specific setting; changing settings there exercises
the intended scenario too.
