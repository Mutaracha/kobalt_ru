#pragma once
class LVDocView;

namespace kobalt {
// Fork-only effective styles. Applied after publisher/user CSS; never persisted.
const char *comicPageCss();
const char *comicSpreadCss();
void markComicImages(LVDocView *view);
void markChapterLeadingImages(LVDocView *view);
}
