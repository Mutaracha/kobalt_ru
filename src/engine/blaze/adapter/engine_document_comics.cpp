#include "engine_document.h"
#include "page_draw_buffer.h"
#include <QPainter>
#include "column_view.h"
#include "reader_source.h"
#include "engine/engine_activity.h"
#include "publication/resource_cache.h"
#include "support/diagnostics.h"

namespace kobalt {
QImage EngineDocument::frameImage()
{
    auto result = image(page());
    if (!comicsMode_ || !engine_ || !source_ || !source_->resources
        || engine_->comicPageCount(page()) != 2)
        return result;
    const auto names = comicImageRequests_.value(page());
    int pending = 0;
    for (const auto &name : names)
        pending += source_->resources->pendingImage(name);
    if (pending > 0 && names.size() <= 2) {
        engineActivity().checkpoint();
        if (loadNextImage())
            result = image(page());
        engineActivity().checkpoint();
    }
    if (diagnosticsActive()) {
        int remaining = 0;
        for (const auto &name : names)
            remaining += source_->resources->pendingImage(name);
        diagnostic("comic-frame-images",
                   { { "page", page() },
                     { "visible_resources", names.size() },
                     { "pending_before", pending },
                     { "pending_after", remaining } },
                   false);
    }
    return result;
}

bool EngineDocument::beginComicImageRequests(int pageNumber)
{
    const bool tracked = comicsMode_ && source_ && source_->resources
            && engine_->comicPageCount(pageNumber) == 2;
    if (tracked)
        source_->resources->beginImageRequests();
    return tracked;
}

void EngineDocument::finishComicImageRequests(int pageNumber)
{
    // Neighbour drawing must not erase the visible page's request identity.
    // A decoded image may bypass Decode() on later draws, so retain the union
    // until reflow/source change rather than replacing it with an empty trace.
    if (!comicImageRequests_.contains(pageNumber) && comicImageRequests_.size() >= 8) {
        for (const auto key : comicImageRequests_.keys()) {
            if (key != page() && key != pageNumber && !cache_.contains(key)) {
                comicImageRequests_.remove(key);
                break;
            }
        }
    }
    auto &names = comicImageRequests_[pageNumber];
    for (const auto &name : source_->resources->endImageRequests())
        if (!names.contains(name))
            names.append(name);
}

bool EngineDocument::loadVisibleComicImages()
{
    auto &resources = *source_->resources;
    bool batch = false;
    // A spread is one displayed screen. Load only its requested images, not
    // the neighbour queue, before ReaderSession captures the next frame.
    if (comicsMode_ && engine_ && engine_->comicPageCount(page()) == 2) {
        const auto names = comicImageRequests_.value(page());
        if (names.size() <= 2) {
            for (const auto &name : names) {
                if (!resources.prioritizePendingImage(name))
                    continue;
                batch = true;
                resources.loadNextImage();
                if (!resources.error().isEmpty())
                    break;
            }
        }
    }
    return batch;
}

EngineDocument::ComicPaint EngineDocument::paintComicPage(int pageNumber, QImage &result,
                                                          PageDrawBuffer &buffer)
{
    if (auto *node = engine_->singleComicNode(pageNumber)) {
        const auto source = node->getObjectImageSource();
        const auto rect = engine_->singleComicRect(pageNumber);
        const bool rotate = engine_->rotatedComicNode(pageNumber) != nullptr;
        result.fill(darkMode_ ? Qt::black : Qt::white);
        // Scale to the final viewport before the quarter-turn: no additional source-sized
        // bitmap copy, second resampling or artwork cropping is introduced.
        QImage layer(rotate ? rect.height() : rect.width(), rotate ? rect.width() : rect.height(),
                     QImage::Format_RGB32);
        if (layer.isNull())
            return ComicPaint::Failed;
        layer.fill(darkMode_ ? Qt::black : Qt::white);
        PageDrawBuffer part(layer, darkMode_, true, normalizeTextColors_, &imageInversion_);
        part.Draw(source, 0, 0, layer.width(), layer.height(), false);
        QPainter painter(&result);
        painter.translate(rect.x() + (rotate ? rect.width() : 0), rect.y());
        if (rotate)
            painter.rotate(90);
        painter.drawImage(0, 0, layer);
        buffer.hasImages = true;
        buffer.imageRegions.append(rect);
        return ComicPaint::Painted;
    }
    if (columns_ != 1 || engine_->comicPageCount(pageNumber) != 2)
        return ComicPaint::NotHandled;
    result.fill(Qt::white);
    QImage layer(size_, QImage::Format_RGB32);
    if (layer.isNull()) {
        return ComicPaint::Failed;
    }
    QPainter painter(&result);
    for (int i = 0; i < 2; ++i) {
        PageDrawBuffer part(layer, darkMode_, true, normalizeTextColors_, &imageInversion_);
        engine_->drawColumn(part, pageNumber, i);
        const int physical = engine_->comicRightToLeft() ? 1 - i : i;
        const int left = physical * size_.width() / 2;
        const int right = (physical + 1) * size_.width() / 2;
        const int offset = (left + right - size_.width()) / 2;
        const QRect clip(left, 0, right - left, size_.height());
        const int vertical = part.imageRegions.size() == 1 && !part.hasText
                ? (size_.height() - part.imageRegions.first().height()) / 2
                        - part.imageRegions.first().y()
                : 0;
        const auto bounds = part.imageRegions.size() == 1
                ? part.imageRegions.first().translated(offset, vertical).intersected(clip)
                : QRect();
        engine_->setComicVerticalOffset(pageNumber, i, vertical, bounds);
        painter.setClipRect(clip);
        painter.drawImage(offset, vertical, layer);
        buffer.hasImages |= part.hasImages;
        buffer.hasText |= part.hasText;
        for (const auto &region : part.imageRegions)
            buffer.imageRegions.append(region.translated(offset, vertical).intersected(clip));
    }
    return ComicPaint::Painted;
}

} // namespace kobalt
