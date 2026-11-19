#include "engine/types.h"
#include "engine/query.h"
#include "engine/font_defaults.h"
#include "check.h"
#include <lvfntman.h>

using namespace kobalt;

// Checks query exactness, generation identity, search defaults and shared resource bounds.
int main()
{
    static_assert(maxEmbeddedFontBytes == LVFontManager::MaxEmbeddedFontBytes,
                  "Publication and Blaze must agree on the embedded font resource bound");
    PageInfo info;
    require(info.bookPage.state() == QueryState::PendingLayout,
            "An unknown book page must not become an exact zero");
    const LayoutGeneration old{1, 2}, next{1, 3};
    info.chapterPage = Query<int>::exact(4, old);
    info.bookPage = Query<int>::estimated(25, QueryBasis::Bytes, old);
    require(info.chapterPage.state() == QueryState::ReadyExact
                    && info.bookPage.state() == QueryState::ReadyEstimated,
            "Chapter exactness must not imply whole-book exactness");
    require(info.bookPage.basis() == QueryBasis::Bytes && info.bookPage.generation() == old
                    && info.bookPage.generation() != next,
            "An estimated page lost its basis or layout identity");
    auto failed = Query<int>::failed(QStringLiteral("not available"));
    require(failed.state() == QueryState::Failed, "A failed query became ready");
    SearchCursor cursor;
    require(cursor.block == 0 && cursor.offset == 0 && cursor.item == 0
                    && cursor.firstItem == -1 && cursor.lastItem == -1
                    && cursor.previousStart.isEmpty() && cursor.previousEnd.isEmpty()
                    && !cursor.prepareWholeBook,
            "The common search cursor lost its safe initial state");
    SearchHit hit;
    require(hit.page < 0 && hit.chapter.isEmpty(), "A new search hit claims a page/chapter");
    require(openingProgressPercent(OpeningStage::Content, 0) == 0
                    && openingProgressPercent(OpeningStage::Ready, 100) == openingBarEngine,
            "Upstream opening progress no longer reserves window handoff work");
}
