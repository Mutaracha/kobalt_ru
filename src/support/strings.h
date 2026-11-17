#pragma once
#include <QString>
#include <QStringList>
#include <QRegularExpression>
#include "string_locale.h"
#include <atomic>

namespace kobalt::strings {
namespace detail {
inline const Entry *englishTable(int *count) {
    static const Entry values[] = {
#include <kobalt_strings.inc>
    };
    *count = int(sizeof(values) / sizeof(values[0]));
    return values;
}
} // namespace detail

// Diagnostics persist the canonical English key/value mapping, so it keeps the
// English table regardless of the active interface locale.
inline const Entry *entries(int *count) {
    return detail::englishTable(count);
}
inline std::atomic_bool &keyDisplay() {
    static std::atomic_bool enabled(qgetenv("KOBALT_SHOW_STRING_KEYS") == "1");
    return enabled;
}

// Tests can exercise real widgets without coupling assertions to editable copy.
inline void showKeys(bool enabled)
{
    keyDisplay().store(enabled);
}
// Mark deferred display-copy keys without translating them at the producer.
inline constexpr const char *key(const char *value)
{
    return value;
}
inline QString get(const char *key) {
    const Entry *entry = detail::localizedEntry(key);
    if (!entry) {
        int count = 0;
        const auto *table = detail::englishTable(&count);
        entry = detail::find(table, count, key);
    }
    if (!entry)
        return QString::fromLatin1(key);
    const QString value = QString::fromUtf8(entry->value);
    if (!keyDisplay().load())
        return value;
    QString result = QString::fromLatin1(key);
    // Retain argument slots so key mode also exercises formatted messages.
    const QRegularExpression placeholders("%[1-9][0-9]?");
    auto matches = placeholders.globalMatch(value);
    QStringList seen;
    while (matches.hasNext()) {
        const auto slot = matches.next().captured();
        if (!seen.contains(slot))
            seen.append(slot);
    }
    if (!seen.isEmpty())
        result += " [" + seen.join(", ") + "]";
    return result;
}
// Locale-specific inflection is owned by string_locale.h.
inline QString plural(int count, const char *baseKey)
{
    QByteArray key = baseKey;
    key += pluralSuffix(count);
    return get(key.constData()).arg(count);
}
} // namespace kobalt::strings
