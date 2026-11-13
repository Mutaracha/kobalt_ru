#pragma once
#include <lvstream.h>
#include <QSet>
#include <QString>
class LVDocView;

namespace kobalt {
// Fork-only effective styles. Applied after publisher/user CSS; never persisted.
const char *comicPageCss();
const char *comicSpreadCss();
// Explicit OPF center placement takes precedence over bitmap aspect ratio.
QSet<QString> singleComicPages(const LVContainerRef &container);
void markComicImages(LVDocView *view);
void markChapterLeadingImages(LVDocView *view);
}
