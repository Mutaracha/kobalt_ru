#pragma once
#include "string_table.h"
#include <QByteArray>
#include <QString>
#include <atomic>

namespace kobalt::strings {
// Fork policy only. The shared entry point owns English fallback and key display.
enum class Locale { English, Russian };

namespace detail {
inline const Entry *russianTable(int *count)
{
    static const Entry values[] = {
#include <kobalt_strings_ru.inc>
    };
    *count = int(sizeof(values) / sizeof(values[0]));
    return values;
}
} // namespace detail

inline std::atomic_int &localeStore()
{
    static std::atomic_int value(int(Locale::English));
    return value;
}

inline void setLocale(Locale locale)
{
    localeStore().store(int(locale));
}

inline Locale locale()
{
    return Locale(localeStore().load());
}

// Nickel stores codes such as "ru", "ru_RU" and occasionally "ru-RU". Only the
// language prefix matters; an unknown language safely resolves to English.
inline Locale normalizeLocaleCode(const QString &code)
{
    return code.left(2).toLower() == QLatin1String("ru") ? Locale::Russian : Locale::English;
}

namespace detail {
inline const Entry *localizedEntry(const char *key)
{
    if (locale() != Locale::Russian)
        return nullptr;
    int count = 0;
    const auto *table = russianTable(&count);
    return find(table, count, key);
}
} // namespace detail

// Russian plurals follow CLDR: one (1 книга), few (2 книги), many (5 книг).
// English keeps its existing one/base split. The base key holds the many form.
inline QByteArray pluralSuffix(int count)
{
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
    return suffix;
}
} // namespace kobalt::strings
