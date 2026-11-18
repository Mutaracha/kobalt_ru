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
    // Do not mistake a tag mentioned in a comment, PI or another XML root
    // for the document element. Keep this byte-based for legacy encodings.
    if (head.contains("<!ENTITY"))
        return false;
    for (;;) {
        while (at < head.size() &&
               (head[at] == '\t' || head[at] == '\r' || head[at] == '\n' || head[at] == ' '))
            ++at;
        if (head.mid(at, 4) == "<!--") {
            const int end = head.indexOf("-->", at + 4);
            if (end < 0) return false;
            at = end + 3;
        } else if (head.mid(at, 2) == "<?") {
            const int end = head.indexOf("?>", at + 2);
            if (end < 0) return false;
            at = end + 2;
        } else if (head.mid(at, 9) == "<!DOCTYPE") {
            // A declaration may contain quoted '>' or an internal subset.
            // Entity declarations remain disallowed by the guard above.
            int depth = 0;
            char quote = 0;
            int end = at + 9;
            for (; end < head.size(); ++end) {
                const char ch = head[end];
                if (quote) {
                    if (ch == quote) quote = 0;
                } else if (ch == '\'' || ch == '"') quote = ch;
                else if (ch == '[') ++depth;
                else if (ch == ']') --depth;
                else if (ch == '>' && !depth) break;
                if (depth < 0) return false;
            }
            if (end == head.size()) return false;
            at = end + 1;
        } else break;
    }
    if (head.mid(at, 12) != "<FictionBook")
        return false;
    const int tag = at;
    const int after = tag + int(std::strlen("<FictionBook"));
    if (after >= head.size())
        return false;
    const char delimiter = head[after];
    return delimiter == ' ' || delimiter == '\t' || delimiter == '\r' || delimiter == '\n' ||
           delimiter == '>';
}
} // namespace kobalt::fb2
