#pragma once
#include <QByteArray>
class ldomNode;
namespace kobalt::fb2 {
void markNumericNoteTitles(ldomNode *root);
// Format marker only. Upstream canonical inpage_footnotes owns the note policy.
inline QByteArray cachePolicy(bool enabled) { return enabled ? QByteArray(":fb2-v8:") : QByteArray(); }
}
