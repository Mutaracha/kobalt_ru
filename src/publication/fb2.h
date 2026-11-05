#pragma once
#include <QByteArray>
#include <cstring>

namespace kobalt::fb2 {
// Raw-byte FictionBook recognition. The payload is never decoded here: a
// windows-1251 book keeps its encoding declaration and CREngine decodes it.
// Callers pass at most the first 64 KiB. The root must fall within this
// supported probe window; this is recognition, not full XML validation.
inline bool matchesHead(const QByteArray &head) {
    int at = 0;
    if (head.startsWith("\xEF\xBB\xBF"))
        at = 3;
    while (at < head.size() &&
           (head[at] == '\t' || head[at] == '\r' || head[at] == '\n' || head[at] == ' '))
        ++at;
    const auto prefix = head.mid(at);
    if (!prefix.startsWith("<?xml") && !prefix.startsWith("<FictionBook"))
        return false;
    // XML entity expansion (billion laughs) guard, mirroring cleanXml for EPUB.
    if (head.contains("<!ENTITY"))
        return false;
    const int tag = head.indexOf("<FictionBook");
    if (tag < 0)
        return false;
    const int after = tag + int(std::strlen("<FictionBook"));
    if (after >= head.size())
        return false;
    const char delimiter = head[after];
    return delimiter == ' ' || delimiter == '\t' || delimiter == '\r' || delimiter == '\n' ||
           delimiter == '>';
}
} // namespace kobalt::fb2
