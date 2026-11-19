#pragma once
#include <lvstream.h>
#include <lvdocview.h>
#include <QSet>
#include <QString>
class LVDocView;

namespace kobalt {
// Effective overlay only: never write these values back to saved appearance.
struct ComicPageLayout
{
    int columns;
    lvRect margins;
};
ComicPageLayout comicPageLayout(bool enabled, int baseColumns, lvRect baseMargins);
// Fork-only effective styles. Applied after publisher/user CSS; never persisted.
const char *comicPageCss();
const char *comicSpreadCss();
// Explicit OPF center placement takes precedence over bitmap aspect ratio.
QSet<QString> singleComicPages(const LVContainerRef &container);
void markComicImages(LVDocView *view);
} // namespace kobalt
