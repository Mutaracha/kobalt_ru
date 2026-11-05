#pragma once
#include <QByteArray>

namespace kobalt::fb2 {
// Parsed FB2 cache identity. Bump only for incompatible parser/preprocessing
// changes, never for a release or a code-only move. EPUB adds no marker.
inline QByteArray cachePolicy(bool enabled) {
    return enabled ? QByteArray(":fb2-v1:") : QByteArray();
}
} // namespace kobalt::fb2
