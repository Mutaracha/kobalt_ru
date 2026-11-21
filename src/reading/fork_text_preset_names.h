#pragma once
#include "text_presets.h"
#include "support/strings.h"

namespace kobalt::textpresets {
// Example names were saved in the UI language that first created the list.
// Localize only recognized shipped names at presentation time; never rename
// user presets, rewrite saved data, or discard edited typography.
inline QString displayName(const Preset &preset)
{
    if (!preset.example
        || (preset.id != "kobalt" && preset.id != "bookish" && preset.id != "modern"
            && preset.id != "classic"))
        return preset.name;
    const auto key = QByteArray("text_presets.example_") + preset.id.toUtf8();
    int count = 0;
    const auto *table = strings::entries(&count);
    const auto *english = strings::detail::find(table, count, key.constData());
    table = strings::detail::russianTable(&count);
    const auto *russian = strings::detail::find(table, count, key.constData());
    if ((english && preset.name == QString::fromUtf8(english->value))
        || (russian && preset.name == QString::fromUtf8(russian->value)))
        return strings::get(key.constData());
    return preset.name;
}
} // namespace kobalt::textpresets
