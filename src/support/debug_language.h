#pragma once
#include <QString>
#include <functional>
class QObject;

namespace kobalt {
inline constexpr auto debugLanguageDirectory = "/mnt/onboard/.adds/kobalt/diagnostics";
enum class DebugLanguage { Automatic, English, Russian };
// Empty regular marker files only; if both exist, English wins deterministically.
DebugLanguage debugLanguage(const QString &directory);
// Worker-only write helper. Never touches other diagnostics/profile files.
bool writeDebugLanguage(const QString &directory, DebugLanguage language);
// Completion on the GUI thread, dropped if context has gone away. No GUI waits.
void writeDebugLanguageAsync(const QString &directory, DebugLanguage language, QObject *context,
                             std::function<void(bool)> finished);
} // namespace kobalt
