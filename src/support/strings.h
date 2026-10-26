#pragma once
#include <QString>
#include <QStringList>
#include <QRegularExpression>
#include <algorithm>
#include <atomic>
#include <cstring>

namespace kobalt::strings {
// English stays the reference table and the fallback for every locale.
enum class Locale { English, Russian };

struct Entry {
    const char *key;
    const char *value;
};

namespace detail {
inline const Entry *englishTable(int *count) {
    static const Entry values[] = {
#include <kobalt_strings.inc>
    };
    *count = int(sizeof(values) / sizeof(values[0]));
    return values;
}
inline const Entry *russianTable(int *count) {
    static const Entry values[] = {
#include <kobalt_strings_ru.inc>
    };
    *count = int(sizeof(values) / sizeof(values[0]));
    return values;
}
inline const Entry *find(const Entry *begin, int count, const char *name) {
    const auto *entry = std::lower_bound(begin, begin + count, name,
                                         [](const Entry &item, const char *key) {
                                             return std::strcmp(item.key, key) < 0;
                                         });
    if (entry == begin + count || std::strcmp(entry->key, name) != 0)
        return nullptr;
    return entry;
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
inline void showKeys(bool enabled) {
    keyDisplay().store(enabled);
}
inline std::atomic_int &localeStore() {
    static std::atomic_int value(int(Locale::English));
    return value;
}
inline void setLocale(Locale locale) {
    localeStore().store(int(locale));
}
inline Locale locale() {
    return Locale(localeStore().load());
}
// Nickel stores codes such as "ru", "ru_RU" and occasionally "ru-RU". Only the
// language prefix matters; an unknown language safely resolves to English.
inline Locale normalizeLocaleCode(const QString &code) {
    return code.left(2).toLower() == QLatin1String("ru") ? Locale::Russian : Locale::English;
}
// Mark deferred display-copy keys without translating them at the producer.
inline constexpr const char *key(const char *value) {
    return value;
}
inline QString get(const char *key) {
    const Entry *entry = nullptr;
    if (locale() == Locale::Russian) {
        int count = 0;
        entry = detail::find(detail::russianTable(&count), count, key);
    }
    if (!entry) {
        int count = 0;
        entry = detail::find(detail::englishTable(&count), count, key);
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
// Russian plurals follow CLDR: one (1 книга), few (2 книги), many (5 книг).
// English keeps its existing one/base split. The base key holds the many form.
inline QString plural(int count, const char *baseKey) {
    QByteArray suffix;
    if (locale() == Locale::Russian) {
        const int ten = count % 10, hundred = count % 100;
        if (ten == 1 && hundred != 11)
            suffix = "_one";
        else if (ten >= 2 && ten <= 4 && !(hundred >= 12 && hundred <= 14))
            suffix = "_few";
        else
            suffix = "_many";
    } else if (count == 1) {
        suffix = "_one";
    }
    QByteArray key = baseKey;
    key += suffix;
    return get(key.constData()).arg(count);
}
} // namespace kobalt::strings
