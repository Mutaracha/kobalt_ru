#pragma once
#include <QString>
#include <functional>
class QObject;

namespace kobalt {
inline constexpr auto debugLanguageDirectory = "/mnt/onboard/.adds/kobalt/diagnostics";
enum class DebugLanguage { Automatic, English, Russian };
// Hidden .lang_en/.lang_ru markers; English wins if both exist.
// Legacy unprefixed markers are read only when no hidden marker exists.
DebugLanguage debugLanguage(const QString &directory);
// Worker-only write helper. Never touches other diagnostics/profile files.
bool writeDebugLanguage(const QString &directory, DebugLanguage language);
// Completion on the GUI thread, dropped if context has gone away. No GUI waits.
void writeDebugLanguageAsync(const QString &directory, DebugLanguage language, QObject *context,
                             std::function<void(bool)> finished);
} // namespace kobalt
