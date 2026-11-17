#pragma once
#include <algorithm>
#include <cstring>

namespace kobalt::strings {
struct Entry
{
    const char *key;
    const char *value;
};

namespace detail {
inline const Entry *find(const Entry *begin, int count, const char *name)
{
    const auto *entry =
            std::lower_bound(begin, begin + count, name, [](const Entry &item, const char *key) {
                return std::strcmp(item.key, key) < 0;
            });
    if (entry == begin + count || std::strcmp(entry->key, name) != 0)
        return nullptr;
    return entry;
}
} // namespace detail
} // namespace kobalt::strings
