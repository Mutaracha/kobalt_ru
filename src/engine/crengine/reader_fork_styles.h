#pragma once

namespace kobalt::forkstyles {
// These fragments are spliced into the common stylesheet at explicit boundaries.
// Preserve their bytes and ordering: this extraction must not invalidate caches.
const char *fb2LineHeightSelectors();
const char *fb2NoteSelectors();
// Includes the accepted EPUB anchor labels as well as numeric FB2 run-in titles.
const char *noteLabelsCss();
} // namespace kobalt::forkstyles
