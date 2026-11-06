#pragma once
#include "support/strings.h"
#include "support/debug_language.h"


namespace kobalt {
// Resolve the interface locale once at startup. The order is:
// diagnostics .lang_en/.lang_ru markers, KOBALT_LANGUAGE, Kobo config,
// then the operating-system locale. Every unknown value falls back to English.
strings::Locale resolveLocale(const QString &diagnosticsDirectory = QString::fromLatin1(debugLanguageDirectory));
// Read a Kobo-style INI file. A Russian entry in ApplicationPreferences/
// ExtraLocales wins first (localization patches always set it, while old
// patches leave CurrentLocale at "C"); CurrentLocale is consulted afterwards.
// A missing file or key is an English fallback, never a startup failure.
strings::Locale localeFromKoboConfig(const QString &confPath);
} // namespace kobalt
