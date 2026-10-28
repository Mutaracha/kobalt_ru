#pragma once
#include "support/strings.h"

class QString;

namespace kobalt {
// Resolve the interface locale once at startup. The order is:
// KOBALT_LANGUAGE for tests, the Kobo eReader.conf selection when present,
// then the operating-system locale. Every unknown value falls back to English.
strings::Locale resolveLocale();
// Read a Kobo-style INI file. A Russian entry in ApplicationPreferences/
// ExtraLocales wins first (localization patches always set it, while old
// patches leave CurrentLocale at "C"); CurrentLocale is consulted afterwards.
// A missing file or key is an English fallback, never a startup failure.
strings::Locale localeFromKoboConfig(const QString &confPath);
} // namespace kobalt
