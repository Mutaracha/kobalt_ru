#include "support/locale.h"

#include <QFileInfo>
#include <QLocale>
#include <QSettings>
#include <QString>

namespace kobalt {
namespace {
constexpr auto koboReaderConf = "/mnt/onboard/.kobo/Kobo/Kobo eReader.conf";
} // namespace

strings::Locale localeFromKoboConfig(const QString &confPath) {
    const QFileInfo info(confPath);
    if (!info.isFile() || !info.isReadable())
        return strings::Locale::English;
    const QSettings settings(confPath, QSettings::IniFormat);
    const auto code = settings.value("ApplicationPreferences/CurrentLocale").toString();
    if (code.isEmpty())
        return strings::Locale::English;
    return strings::normalizeLocaleCode(code);
}

strings::Locale resolveLocale() {
    const auto forced = qgetenv("KOBALT_LANGUAGE");
    if (!forced.isEmpty())
        return strings::normalizeLocaleCode(QString::fromLatin1(forced));
    // On a Kobo the firmware config is authoritative when it exists. Desktop
    // and Android builds have no such file and follow the system locale.
    if (QFileInfo::exists(QString::fromLatin1(koboReaderConf)))
        return localeFromKoboConfig(QString::fromLatin1(koboReaderConf));
    return strings::normalizeLocaleCode(QLocale::system().name());
}
} // namespace kobalt
