# 059b: persistent reading-menu status toggle

Runtime **97a5fc9ae9d65b2c2cc14c0622ef91977472787a**;
[CI45672903619](https://github.com/Mutaracha/kobalt_ru/actions/runs/45672903619) all jobs PASS.
[Qt5 artifact15050961898](https://github.com/Mutaracha/kobalt_ru/actions/runs/45672903619/artifacts/15050961898),45437261bytes.
Host cache replay evidence15050118893,12306bytes.

Ordered after059a cache identity, before060 integration. Previous cache-only hardware
package3c7c1b20/artifact15047034445 remains separately available; do not interrupt the
user's in-progress cache check or attribute its results to this newer UI binary.

## Behavior

An invisible QAbstractButton hit surface covers the existing normal menu status row
from clock through battery. No new strip/layout row and no geometry change to labels,
clock or battery. It supplies standard press/release cancellation, keyboard activation,
checkable accessibility state and existing translated Session/BookPage labels. Active
text is exposed in accessible description. It remains tappable when the initial
session-time caption is hidden before five minutes or Stats is disabled. Original
Zen/native-status visibility policy is retained; no permanent status outside the menu.

Successful clicks alternate global preference reading_menu_status between session and
book_page through existing checked preference writer. Missing/invalid mode defaults to
session; cleaner validates the two choices. Book layout/state cannot override it.
Write failure retains preferences and restores the button's checked state. Existing
settings write-error reporting is reused. No session restart/reset, Stats mutation,
header/footer preset mutation or typography/layout call on toggle.

Page mode delegates to headerfooter::content(Field::BookPage, chromeValues(Reading),
false). It inherits whole-book/rough/calculating/estimated and applicable printed or
screen labels from the existing chrome values and formatter, not prefix-local page
counts. Normal frame/menu refresh updates the caption; it does not request additional
pagination merely to switch the row. Existing upstream requestMenuPagination on menu
presentation/frame refresh is unchanged. Session mode retains existing five-minute/
Stats/hourglass behavior. Diagnostics expose resolved reading_menu_status alongside
menu_session_text.

## Checks actually run

- lifecycle.readingMenuStatusToggle: actual canvas clicks during first minute, two
  toggles, storage roundtrip, independent reader host/book reopening, day/night,
  clock/battery visibility and geometry, unchanged layoutStarts/position/reading time.
- lifecycle.readingTimeStatsVisibility: previous Stats hiding and continued collection.
- lifecycle.readingTimeStatus: icon separation, symmetric screen-centered status
  bounds, scale/density/radio combinations and minute updates.
- RU and EN control executable: same BookPage formatter for full, estimated, pending,
  screen and printed-label values, including long numeric values.
- Appearance model: default/invalid mode/global cleaning/cross-book precedence.
- Previous selected regression gates, font identity assertions, archived FB2 replay,
  Qt5 compile/check-package/configured refresh-qemu/package collection.

Not a full test matrix or physical visual acceptance. New lifecycle test recreates
reader hosts, not the OS process; persistence itself is checked on disk. Pending/full
formats are checked with Values fixtures, not a new forced background-adoption UI
scenario. Dedicated live page-turn/full-ready, failed preference-write and RU touch/
visual review of this new target remain useful device/follow-up gates. No screenshots
were captured locally: Qt build/runtime tooling unavailable in this sandbox.

## Intermediate failures and correction

45668476726: Linux CMake rejected missing area label for new lifecycle test; Qt5 built.
Added chrome area and one-line test contract.45670004178/45671560745: new toggle and
Stats tests passed; newly enabled pre-existing readingTimeStatus failed twelve data
rows. Its assertions expected the midpoint/free width of the asymmetric clock/radio
gap, but production updateSessionGeometry already uses symmetric screen-centered
bounds before this patch. Changed only those old assertions to exact symmetric
inset/width/center and retained no-overlap/clock/radio checks, not production geometry.
Failure details obtained through check annotations because raw Actions logs return EOF.
Final45672903619 passed all jobs. No disabled failing test or reduced data matrix.

## Device review after the separate cache check

Open the reading menu; tap anywhere along clock/caption/battery row twice. Check
page mode in the first minute, normal session caption later, current page after a
turn and background completion, same saved choice after another book/restart. Clock
and battery must stay put. Check RU wording/long text and day/night. Existing clipping
bounds protect icons; no claim that arbitrary-length labels fit unabridged.
