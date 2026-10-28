#include "support/locale.h"

#include "support/qt_compat.h"

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
    // Localization patches always register their language in ExtraLocales,
    // while older patches leave CurrentLocale at the placeholder "C". The
    // value is a comma-separated list (e.g. "ru" or "en_GB, ru_RU"), so a
    // Russian token there is the most reliable sign of a Russian device.
    const auto extraLocales = settings.value("ApplicationPreferences/ExtraLocales").toString();
    for (const auto &token : extraLocales.split(QLatin1Char(','), kobalt::qt::skipEmptyParts)) {
        if (strings::normalizeLocaleCode(token.trimmed()) == strings::Locale::Russian)
            return strings::Locale::Russian;
    }
    const auto code = settings.value("ApplicationPreferences/CurrentLocale").toString();
    // "C" and "POSIX" mean no real locale selection, as shipped by old patches.
    if (code.isEmpty() || code == QLatin1String("C") || code == QLatin1String("POSIX"))
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
