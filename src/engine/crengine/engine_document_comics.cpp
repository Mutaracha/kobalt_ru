#include "engine_document.h"
#include "column_view.h"
#include "reader_source.h"
#include "engine_activity.h"
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

} // namespace kobalt
