#pragma once
#include <lvdocview.h>
#include <QtGlobal>
#include <QVector>
#include <QString>
#include <vector>
#include <algorithm>

namespace kobalt {
// Keep CREngine's pagination at one column. Public page numbers refer to whole
// screens, while source anchors remain the engine's original document positions.
class ColumnView final : public LVDocView
{
public:
    ColumnView() : LVDocView(32, true) { }

    void setComicSpreads(bool enabled) { comicSpreads = enabled; }
    bool comicRightToLeft() { return rightToLeft(); }
    int comicPageCount(int page) const
    {
        return page >= 0 && page < comicStarts.size()
                ? (page + 1 < comicStarts.size() ? comicStarts[page + 1] : comicInternalPages)
                        - comicStarts[page] : 1;
    }

    // Called only after pagination, never from the paint or page-turn path.
    // Pair only adjacent image-only pages with one known portrait bitmap each.
    // Prose, wide art, covers and ambiguous/multiple-image pages remain single.
    void rebuildComicSpreads()
    {
        comicStarts.clear();
        if (!comicSpreads || viewportWidth <= viewportHeight || columns != 1) return;
        comicInternalPages = LVDocView::getPageCount();
        QVector<int> portraits(comicInternalPages, 0), images(comicInternalPages, 0);
        std::vector<ldomNode *> pending{ getDocument()->getRootNode() };
        while (!pending.empty()) {
            auto *node = pending.back();
            pending.pop_back();
            if (!node || !node->isElement()) continue;
            if (node->isNodeName("head") || node->isNodeName("style") || node->isNodeName("script")) continue;
            if (node->isNodeName("img") || node->isNodeName("image")) {
                const int page = LVDocView::getBookmarkPage(ldomXPointer(node, 0), true);
                if (page >= 0 && page < comicInternalPages) {
                    ++images[page];
                    if (node->getAttributeValue("data-kobalt-comic-portrait") == U"1") ++portraits[page];
                }
            }
            for (int i = node->getChildCount() - 1; i >= 0; --i)
                pending.push_back(node->getChildNode(i));
        }
        QVector<bool> pairable(comicInternalPages, false);
        for (int page = 0; page < comicInternalPages; ++page) {
            const auto text = LVDocView::getPageText(false, page);
            pairable[page] = images[page] == 1 && portraits[page] == 1
                    && QString::fromUtf8(UnicodeToUtf8(text).c_str()).trimmed().isEmpty();
        }
        for (int page = 0; page < comicInternalPages;) {
            comicStarts.append(page);
            page += pairable[page] && page + 1 < comicInternalPages && pairable[page + 1]
                    && LVDocView::getPageFlow(page) == LVDocView::getPageFlow(page + 1) ? 2 : 1;
        }
    }

    void setColumns(int count) { columns = count; }

    int columnWidth() const { return width + 2 * overflow; }

    void setHangingPunctuation(bool enabled) { hangingPunctuation = enabled; }

    int columnOffset(int column)
    {
        return (columns > 1 ? sideMargin - overflow : 0)
                + (rightToLeft() ? columns - 1 - column : column) * (width + gutter);
    }

    void setColumnSpacing(int pixels) { spacing = pixels; }

    void setPageMargins(lvRect value)
    {
        margins = value;
        Resize(viewportWidth, viewportHeight);
    }

    void Resize(int w, int h)
    {
        comicStarts.clear();
        viewportWidth = w;
        viewportHeight = h;
        // Side margins belong to the screen; spacing separates the text columns.
        // Hanging punctuation also needs paper inside each engine viewport.
        // Share the gutter evenly and keep the text width and origins unchanged.
        // CREngine clamps its viewport to 80 pixels. Reduce whitespace first
        // on small screens so its text geometry still matches our columns.
        const int minimumWidth = 80;
        sideMargin = qMin(margins.left, qMax(0, (w - minimumWidth * columns) / 2));
        const int available = qMax(minimumWidth * columns, w - 2 * sideMargin);
        gutter = columns > 1 ? qMin(spacing, (available - minimumWidth * columns) / (columns - 1))
                             : 0;
        width = columns > 1 ? qMax(1, (available - gutter * (columns - 1)) / columns) : w;
        // CREngine gives these side margins to each paragraph as its usable overflow.
        // Its render context includes them (crengine-render-overflow.patch), so a
        // changed allowance lays the book out again, and a cached or retained layout
        // with the same allowance is reused.
        overflow = hangingPunctuation && columns > 1 ? qMin(sideMargin, gutter / 2) : 0;
        LVDocView::setPageMargins(
                columns > 1 ? lvRect(overflow, margins.top, overflow, margins.bottom) : margins);
        LVDocView::Resize(columnWidth(), h);
    }

    int getPageCount() { return comicStarts.isEmpty() ? (LVDocView::getPageCount() + columns - 1) / columns : comicStarts.size(); }

    int getCurPage(bool internal = false)
    {
        const int page = LVDocView::getCurPage(true);
        return internal ? page : screenPage(page);
    }

    bool goToPage(int page, bool internal = false, bool updateBookmark = true, bool regulate = true)
    {
        return LVDocView::goToPage(internal ? page : firstPage(page), true, updateBookmark,
                                   regulate);
    }

    ldomXPointer getPageBookmark(int page, bool precise = true, bool internal = false)
    {
        return LVDocView::getPageBookmark(internal ? page : firstPage(page), precise, true);
    }

    int getBookmarkPage(ldomXPointer pointer, bool internal = false)
    {
        const int page = LVDocView::getBookmarkPage(pointer, true);
        return internal || page < 0 ? page : screenPage(page);
    }

    int getPageFlow(int page) { return LVDocView::getPageFlow(firstPage(page)); }

    lString32 getPageText(bool wrap, int page = -1)
    {
        if (page < 0)
            page = getCurPage();
        lString32 text;
        for (int i = 0; i < pageColumns(page) && firstPage(page) + i < LVDocView::getPageCount(); ++i) {
            if (i)
                text += U"\n";
            text += LVDocView::getPageText(wrap, firstPage(page) + i);
        }
        return text;
    }

    void drawColumn(LVDrawBuf &buffer, int page, int column)
    {
        LVDocView::Draw(buffer, -1, firstPage(page) + column, false, false);
    }

    ldomXPointer getNodeByPoint(lvPoint point, bool strict = false, bool selection = false)
    {
        if (!comicStarts.isEmpty() && comicPageCount(getCurPage()) == 2) {
            const int physical = point.x < viewportWidth / 2 ? 0 : 1;
            const int column = rightToLeft() ? 1 - physical : physical;
            Position restore(*this, firstPage(getCurPage()) + column);
            point.x -= comicOffset(physical);
            return LVDocView::getNodeByPoint(point, strict, selection);
        }
        if (columns == 1)
            return LVDocView::getNodeByPoint(point, strict, selection);
        point.x -= sideMargin;
        const int physical = qBound(0, (point.x + gutter / 2) / (width + gutter), columns - 1);
        const int local = point.x - physical * (width + gutter);
        if (local < -overflow || local >= width + overflow)
            return {};
        const int column = rightToLeft() ? columns - 1 - physical : physical;
        const int target = getCurPage() * columns + column;
        if (target >= LVDocView::getPageCount())
            return {};
        Position restore(*this, target);
        point.x = local + overflow;
        return LVDocView::getNodeByPoint(point, strict, selection);
    }

    bool docToWindowPoint(lvPoint &point, bool bottom = false, bool fit = false)
    {
        if (!comicStarts.isEmpty() && comicPageCount(getCurPage()) == 2) {
            const int first = firstPage(getCurPage());
            const int target = getPageList()->FindNearestPage(point.y - (bottom ? 1 : 0), 0);
            if (target < first || target >= first + 2) return false;
            Position restore(*this, target);
            if (!LVDocView::docToWindowPoint(point, bottom, fit)) return false;
            point.x += comicOffset(rightToLeft() ? 1 - (target - first) : target - first);
            return true;
        }
        if (columns == 1)
            return LVDocView::docToWindowPoint(point, bottom, fit);
        const int first = getCurPage() * columns;
        const int target = getPageList()->FindNearestPage(point.y - (bottom ? 1 : 0), 0);
        if (target < first || target >= first + columns)
            return false;
        Position restore(*this, target);
        if (!LVDocView::docToWindowPoint(point, bottom, fit))
            return false;
        point.x += columnOffset(target - first);
        return true;
    }

    void getCurrentPageLinks(ldomXRangeList &list) override
    {
        if (columns == 1 && comicStarts.isEmpty()) {
            LVDocView::getCurrentPageLinks(list);
            return;
        }
        list.clear();
        const int page = getCurPage();
        const int first = firstPage(page);
        for (int i = 0; i < pageColumns(page) && first + i < LVDocView::getPageCount(); ++i) {
            Position restore(*this, first + i);
            ldomXRangeList links;
            LVDocView::getCurrentPageLinks(links);
            for (int j = 0; j < links.length(); ++j)
                list.add(new ldomXRange(*links[j]));
        }
    }

private:
    bool comicSpreads = false;
    QVector<int> comicStarts;
    int comicInternalPages = 0;
    int firstPage(int page) const
    {
        return comicStarts.isEmpty() ? page * columns
                : page >= 0 && page < comicStarts.size() ? comicStarts[page] : comicInternalPages;
    }
    int pageColumns(int page) const { return comicStarts.isEmpty() ? columns : comicPageCount(page); }
    int screenPage(int internal) const
    {
        if (comicStarts.isEmpty()) return internal / columns;
        const auto it = std::upper_bound(comicStarts.cbegin(), comicStarts.cend(), internal);
        return qMax(0, int(it - comicStarts.cbegin()) - 1);
    }
    int comicOffset(int physical) const
    {
        return (physical * viewportWidth / 2 + (physical + 1) * viewportWidth / 2 - viewportWidth) / 2;
    }
    bool rightToLeft()
    {
        return getDocument()
                && getDocument()->getProps()->getStringDef("doc.page-progression-direction")
                == U"rtl";
    }

    int columns = 1, width = 1, gutter = 0, spacing = 0, overflow = 0;
    bool hangingPunctuation = false;
    int viewportWidth = 1, viewportHeight = 1, sideMargin = 0;
    lvRect margins;

    struct Position
    {
        ColumnView &view;
        ldomXPointer bookmark;
        bool moved;

        Position(ColumnView &view, int page)
            : view(view),
              bookmark(view.getBookmark()),
              moved(page != view.LVDocView::getCurPage(true))
        {
            if (moved)
                view.LVDocView::goToPage(page, true);
        }

        ~Position()
        {
            if (moved)
                view.goToBookmark(bookmark);
        }
    };
};
} // namespace kobalt
