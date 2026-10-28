#include "check.h"
#include "support/locale.h"
#include "support/strings.h"

#include <QCoreApplication>
#include <QFile>
#include <QTemporaryDir>
#include <QTextStream>

using namespace kobalt;

namespace {
void normalizeCodes() {
    using strings::Locale;
    require(strings::normalizeLocaleCode(QStringLiteral("ru")) == Locale::Russian, "ru must select Russian");
    require(strings::normalizeLocaleCode(QStringLiteral("ru_RU")) == Locale::Russian, "ru_RU must select Russian");
    require(strings::normalizeLocaleCode(QStringLiteral("ru-RU")) == Locale::Russian, "ru-RU must select Russian");
    require(strings::normalizeLocaleCode(QStringLiteral("RU")) == Locale::Russian, "RU must select Russian");
    require(strings::normalizeLocaleCode(QStringLiteral("en")) == Locale::English, "en must select English");
    require(strings::normalizeLocaleCode(QStringLiteral("en_GB")) == Locale::English, "en_GB must select English");
    require(strings::normalizeLocaleCode(QStringLiteral("fr_FR")) == Locale::English, "unknown locale falls back to English");
    require(strings::normalizeLocaleCode(QString()) == Locale::English, "empty locale falls back to English");
}

void translatedLookup() {
    using strings::Locale;
    strings::setLocale(Locale::Russian);
    require(strings::locale() == Locale::Russian, "Russian locale was not stored");
    require(strings::get("common.cancel") == QString::fromUtf8("Отмена"), "Russian lookup returned the wrong copy");
    require(strings::get("library.title") == QString::fromUtf8("Библиотека"), "Russian library title mismatch");
    strings::setLocale(Locale::English);
    require(strings::get("common.cancel") == QStringLiteral("Cancel"), "English lookup returned the wrong copy");
    require(strings::get("missing.unknown.key") == QStringLiteral("missing.unknown.key"),
            "An unknown key must resolve to itself");
}

void keyDisplayMode() {
    using strings::Locale;
    strings::setLocale(Locale::Russian);
    strings::showKeys(true);
    require(strings::get("common.cancel") == QStringLiteral("common.cancel"), "Key mode must show the key, not Russian copy");
    const auto count = strings::get("library.count");
    require(count == QStringLiteral("library.count [%1]"), qPrintable(QString("Key mode lost slots: %1").arg(count)));
    strings::showKeys(false);
    require(strings::get("common.cancel") == QString::fromUtf8("Отмена"), "Key mode did not switch back to Russian copy");
}

void russianPlurals() {
    using strings::Locale;
    strings::setLocale(Locale::Russian);
    const struct Case {
        int count;
        const char *expected;
    };
    const Case cases[]{{1, "1 книга"},    {2, "2 книги"},     {5, "5 книг"},      {0, "0 книг"},
                       {11, "11 книг"},   {21, "21 книга"},   {22, "22 книги"},   {25, "25 книг"},
                       {101, "101 книга"}, {111, "111 книг"}, {102, "102 книги"}};
    for (const auto &test : cases) {
        require(strings::plural(test.count, "library.count") == QString::fromUtf8(test.expected),
                qPrintable(QString("Russian plural for %1").arg(test.count)));
    }
    require(strings::plural(1, "annotations.summary") == QString::fromUtf8("1 аннотация"),
            "Russian annotation singular mismatch");
    require(strings::plural(3, "annotations.summary") == QString::fromUtf8("3 аннотации"),
            "Russian annotation few-form mismatch");
    require(strings::plural(5, "annotations.summary") == QString::fromUtf8("5 аннотаций"),
            "Russian annotation many-form mismatch");
    strings::setLocale(Locale::English);
    require(strings::plural(1, "library.count") == QStringLiteral("1 book"), "English singular mismatch");
    require(strings::plural(2, "library.count") == QStringLiteral("2 books"), "English plural mismatch");
}

void koboConfig() {
    using strings::Locale;
    QTemporaryDir directory;
    require(directory.isValid(), "temporary directory");
    const auto confPath = directory.path() + "/Kobo eReader.conf";
    {
        QFile file(confPath);
        require(file.open(QIODevice::WriteOnly | QIODevice::Text), "create Kobo config fixture");
        QTextStream stream(&file);
        stream << "[ApplicationPreferences]\nCurrentLocale=ru_RU\n";
    }
    require(kobalt::localeFromKoboConfig(confPath) == Locale::Russian, "ru_RU config must select Russian");
    {
        QFile file(confPath);
        require(file.open(QIODevice::WriteOnly | QIODevice::Text), "rewrite Kobo config fixture");
        QTextStream stream(&file);
        stream << "[ApplicationPreferences]\nCurrentLocale=en_GB\n";
    }
    require(kobalt::localeFromKoboConfig(confPath) == Locale::English, "en_GB config must select English");
    {
        QFile file(confPath);
        require(file.open(QIODevice::WriteOnly | QIODevice::Text), "rewrite Kobo config without locale");
        QTextStream stream(&file);
        stream << "[Reading]\nSideloadedMode=true\n";
    }
    require(kobalt::localeFromKoboConfig(confPath) == Locale::English, "Missing CurrentLocale must fall back to English");
    {
        QFile file(confPath);
        require(file.open(QIODevice::WriteOnly | QIODevice::Text), "rewrite Kobo config with an old-patch placeholder");
        QTextStream stream(&file);
        stream << "[ApplicationPreferences]\nCurrentLocale=C\nExtraLocales=ru\n";
    }
    require(kobalt::localeFromKoboConfig(confPath) == Locale::Russian,
            "ExtraLocales=ru must select Russian even when CurrentLocale is C");
    {
        QFile file(confPath);
        require(file.open(QIODevice::WriteOnly | QIODevice::Text), "rewrite Kobo config with ExtraLocales only");
        QTextStream stream(&file);
        stream << "[ApplicationPreferences]\nExtraLocales=ru_RU\n";
    }
    require(kobalt::localeFromKoboConfig(confPath) == Locale::Russian,
            "ExtraLocales=ru_RU without CurrentLocale must select Russian");
    {
        QFile file(confPath);
        require(file.open(QIODevice::WriteOnly | QIODevice::Text), "rewrite Kobo config with a locale list");
        QTextStream stream(&file);
        stream << "[ApplicationPreferences]\nCurrentLocale=C\nExtraLocales=en_GB, ru-RU\n";
    }
    require(kobalt::localeFromKoboConfig(confPath) == Locale::Russian,
            "A Russian token inside a comma-separated ExtraLocales must select Russian");
    {
        QFile file(confPath);
        require(file.open(QIODevice::WriteOnly | QIODevice::Text), "rewrite Kobo config with non-Russian locales");
        QTextStream stream(&file);
        stream << "[ApplicationPreferences]\nCurrentLocale=en_GB\nExtraLocales=fr_FR\n";
    }
    require(kobalt::localeFromKoboConfig(confPath) == Locale::English,
            "ExtraLocales without a Russian token must follow CurrentLocale and stay English");
    {
        QFile file(confPath);
        require(file.open(QIODevice::WriteOnly | QIODevice::Text), "rewrite Kobo config with the C placeholder only");
        QTextStream stream(&file);
        stream << "[ApplicationPreferences]\nCurrentLocale=C\n";
    }
    require(kobalt::localeFromKoboConfig(confPath) == Locale::English,
            "CurrentLocale=C without ExtraLocales must fall back to English");
    require(kobalt::localeFromKoboConfig(directory.path() + "/missing.conf") == Locale::English,
            "Missing config file must fall back to English");
}

void environmentOverride() {
    using strings::Locale;
    qputenv("KOBALT_LANGUAGE", "ru_RU");
    require(kobalt::resolveLocale() == Locale::Russian, "KOBALT_LANGUAGE=ru_RU must force Russian");
    qputenv("KOBALT_LANGUAGE", "fr");
    require(kobalt::resolveLocale() == Locale::English, "Unknown KOBALT_LANGUAGE must fall back to English");
    qunsetenv("KOBALT_LANGUAGE");
}
} // namespace

int main(int argc, char **argv) {
    QCoreApplication app(argc, argv);
    normalizeCodes();
    translatedLookup();
    keyDisplayMode();
    russianPlurals();
    koboConfig();
    environmentOverride();
    // Do not leak the selector into any later in-process use.
    strings::setLocale(strings::Locale::English);
    std::puts("PASS: locale normalization, translated lookup, key mode, Russian plurals and config resolution");
}
