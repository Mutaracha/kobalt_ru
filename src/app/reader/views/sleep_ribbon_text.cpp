#include "sleep_screen.h"
#include "support/strings.h"
namespace kobalt::reader {
QStringList sleepRibbonRemainingLines(const QString &remaining) {
    if (remaining.isEmpty())
        return {};
    const auto pattern = strings::get("sleep.time_left");
    const int slot = pattern.indexOf(QStringLiteral("%1"));
    if (slot >= 0) {
        const auto prefix = pattern.left(slot), suffix = pattern.mid(slot + 2);
        if (remaining.startsWith(prefix) && remaining.endsWith(suffix) &&
            remaining.size() >= prefix.size() + suffix.size()) {
            const auto duration = remaining.mid(prefix.size(), remaining.size() - prefix.size() - suffix.size()).trimmed();
            const auto caption = (prefix + suffix).trimmed();
            if (!duration.isEmpty() && !caption.isEmpty()) {
                // Keep the language's order (RU: caption first; EN: time first).
                if (prefix.trimmed().isEmpty())
                    return {duration, caption};
                return {caption, duration};
            }
        }
    }
    // Older saved snapshots may have been produced in another language.
    return {remaining};
}
}
