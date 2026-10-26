#pragma once
#include "support/strings.h"

class QString;

namespace kobalt {
// Resolve the interface locale once at startup. The order is:
// KOBALT_LANGUAGE for tests, the Kobo eReader.conf selection when present,
// then the operating-system locale. Every unknown value falls back to English.
strings::Locale resolveLocale();
// Read ApplicationPreferences/CurrentLocale from a Kobo-style INI file.
// A missing file or key is an English fallback, never a startup failure.
strings::Locale localeFromKoboConfig(const QString &confPath);
} // namespace kobalt
