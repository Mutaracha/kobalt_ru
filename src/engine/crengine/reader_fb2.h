#pragma once
#include <QByteArray>
#include <QString>
class ldomNode;

namespace kobalt::fb2 {
void markNumericNoteTitles(ldomNode *root);
// Format semantics shared by upstream alignment/beautifier passes.
bool preserveAlignment(ldomNode *node);
bool preserveBeautifier(ldomNode *node);
bool inlineElement(const QString &name);

// Format marker only. Upstream canonical inpage_footnotes owns the note policy.
inline QByteArray cachePolicy(bool enabled)
{
    return enabled ? QByteArray(":fb2-v8:") : QByteArray();
}
} // namespace kobalt::fb2
