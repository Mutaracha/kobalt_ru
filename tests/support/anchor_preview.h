#pragma once
#include "check.h"
#include "reader_source_index_probe.h"
#include "engine/crengine/engine_document.h"

namespace kobalt::test {
// Anchored previews must not lend page-list geometry or cached note images to reading.
inline void requireAnchoredPreviewRestores(EngineDocument &model)
{
    const int page = model.page(), pages = model.pages();
    const auto image = model.image(page);
    const auto position = model.position();
    const auto state = model.state();
    const int fragments = ReaderSourceIndexProbe::inPageFootnoteCount(model);
    require(!image.isNull() && fragments > 0, "Preview regression needs a painted page with notes");
    for (bool chapter : { true, false }) {
        require(!model.imageFromAnchor(chapter).isNull(), "Anchored note preview did not paint");
        require(model.page() == page && model.pages() == pages && model.position() == position
                        && model.state() == state,
                "Anchored note preview changed reading state or pagination");
        require(ReaderSourceIndexProbe::inPageFootnoteCount(model) == fragments
                        && model.image(page) == image,
                "Anchored note preview damaged bottom fragments or the reading image cache");
    }
}
} // namespace kobalt::test
