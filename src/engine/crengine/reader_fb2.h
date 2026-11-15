#pragma once
#include <QByteArray>
namespace kobalt::fb2 {
// Format marker only. Upstream canonical inpage_footnotes owns the note policy.
inline QByteArray cachePolicy(bool enabled) { return enabled ? QByteArray(":fb2-v7:") : QByteArray(); }
}
