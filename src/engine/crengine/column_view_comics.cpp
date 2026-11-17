#include "column_view.h"
#include <vector>

namespace kobalt {
// Fork-only spread classification and image geometry; shared column layout stays upstream.
int ColumnView::comicPageCount(int page) const
{
    return page >= 0 && page < comicStarts.size()
            ? (page + 1 < comicStarts.size() ? comicStarts[page + 1] : comicInternalPages)
                    - comicStarts[page]
            : 1;
}

ldomNode *ColumnView::rotatedComicNode(int page) const
{
    const int internal = firstPage(page);
    return internal >= 0 && internal < comicCenters.size() ? comicCenters[internal] : nullptr;
}

ldomNode *ColumnView::singleComicNode(int page) const
{
    const int internal = firstPage(page);
    return comicPageCount(page) == 1 && internal >= 0 && internal < comicImages.size()
            ? comicImages[internal]
            : nullptr;
}

QRect ColumnView::singleComicRect(int page) const
{
    auto *node = singleComicNode(page);
    const auto source = node ? node->getObjectImageSource() : LVImageSourceRef();
    if (source.isNull())
        return { };
    // A center marker does not imply rotation: only a portrait bitmap turns.
    const bool rotate = rotatedComicNode(page) != nullptr;
    const auto fitted = (rotate ? QSize(source->GetHeight(), source->GetWidth())
                                : QSize(source->GetWidth(), source->GetHeight()))
                                .scaled(QSize(!rotate && source->GetHeight() > source->GetWidth()
                                                      ? viewportWidth / 2
                                                      : viewportWidth,
                                              viewportHeight),
                                        Qt::KeepAspectRatio);
    return QRect(
            QPoint((viewportWidth - fitted.width()) / 2, (viewportHeight - fitted.height()) / 2),
            fitted);
}

void ColumnView::setComicVerticalOffset(int page, int column, int offset, const QRect &bounds)
{
    const int internal = firstPage(page) + column;
    if (internal >= 0 && internal < comicVerticalOffsets.size()) {
        comicVerticalOffsets[internal] = offset;
        comicImageRects[internal] = bounds;
    }
}

void ColumnView::rebuildComicSpreads()
{
    comicStarts.clear();
    comicCenters.clear();
    comicImages.clear();
    comicVerticalOffsets.clear();
    comicImageRects.clear();
    if (!comicSpreads || viewportWidth <= viewportHeight || columns != 1)
        return;
    comicInternalPages = LVDocView::getPageCount();
    QVector<int> portraits(comicInternalPages, 0), images(comicInternalPages, 0);
    comicCenters.fill(nullptr, comicInternalPages);
    comicImages.fill(nullptr, comicInternalPages);
    comicVerticalOffsets.fill(0, comicInternalPages);
    comicImageRects.fill(QRect(), comicInternalPages);
    std::vector<ldomNode *> pending{ getDocument()->getRootNode() };
    while (!pending.empty()) {
        auto *node = pending.back();
        pending.pop_back();
        if (!node || !node->isElement())
            continue;
        if (node->isNodeName("head") || node->isNodeName("style") || node->isNodeName("script"))
            continue;
        if (node->isNodeName("img") || node->isNodeName("image")) {
            const int page = LVDocView::getBookmarkPage(ldomXPointer(node, 0), true);
            if (page >= 0 && page < comicInternalPages) {
                ++images[page];
                bool centered = false, linked = false;
                for (auto *parent = node; parent && !parent->isNodeName("DocFragment");
                     parent = parent->getParentNode()) {
                    centered |= parent->getAttributeValue("data-kobalt-comic-single") == U"1";
                    linked |= parent->isNodeName("a");
                }
                if (!linked)
                    comicImages[page] = node;
                const auto source =
                        centered && !linked ? node->getObjectImageSource() : LVImageSourceRef();
                if (!source.isNull() && source->GetWidth() > 0
                    && source->GetHeight() > source->GetWidth())
                    comicCenters[page] = node;
                if (node->getAttributeValue("data-kobalt-comic-portrait") == U"1")
                    ++portraits[page];
            }
        }
        for (int i = node->getChildCount() - 1; i >= 0; --i)
            pending.push_back(node->getChildNode(i));
    }
    QVector<bool> pairable(comicInternalPages, false);
    for (int page = 0; page < comicInternalPages; ++page) {
        const auto text = LVDocView::getPageText(false, page);
        const bool imageOnly = images[page] == 1
                && QString::fromUtf8(UnicodeToUtf8(text).c_str()).trimmed().isEmpty();
        pairable[page] = imageOnly && portraits[page] == 1;
        if (!imageOnly) {
            comicCenters[page] = nullptr;
            comicImages[page] = nullptr;
        }
    }
    for (int page = 0; page < comicInternalPages;) {
        comicStarts.append(page);
        page += pairable[page] && page + 1 < comicInternalPages && pairable[page + 1]
                        && LVDocView::getPageFlow(page) == LVDocView::getPageFlow(page + 1)
                ? 2
                : 1;
    }
}
} // namespace kobalt
