#include "debug_language.h"
#include "support/main_thread.h"
#include "support/qt_compat.h"
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QPointer>
#include <QRunnable>
#include <QSaveFile>
#include <QThreadPool>
#include <utility>

namespace kobalt {
DebugLanguage debugLanguage(const QString &directory) {
    if (directory.isEmpty()) return DebugLanguage::Automatic;
    const QDir dir(directory);
    if (QFileInfo(dir.filePath("lang_en")).isFile()) return DebugLanguage::English;
    if (QFileInfo(dir.filePath("lang_ru")).isFile()) return DebugLanguage::Russian;
    return DebugLanguage::Automatic;
}
bool writeDebugLanguage(const QString &directory, DebugLanguage language) {
    mainthread::BlockingOperation operation("debug-language-write");
    if (directory.isEmpty()) return false;
    const QDir dir(directory);
    const auto remove = [&dir](const char *name) {
        const auto path = dir.filePath(QString::fromLatin1(name));
        const QFileInfo info(path);
        return (!info.exists() && !info.isSymLink()) || QFile::remove(path);
    };
    if (language == DebugLanguage::Automatic) {
        const bool en = remove("lang_en");
        const bool ru = remove("lang_ru");
        return en && ru;
    }
    if (!QDir().mkpath(directory)) return false;
    // Commit the new marker before removing the old one: a failed create must
    // not silently discard a working override. Report any cleanup failure.
    const auto target = dir.filePath(language == DebugLanguage::English ? "lang_en" : "lang_ru");
    const QFileInfo info(target);
    if (info.isSymLink() || (info.exists() && !info.isFile())) return false;
    QSaveFile marker(target);
    if (!marker.open(QIODevice::WriteOnly) || !marker.commit()) return false;
    return remove(language == DebugLanguage::English ? "lang_ru" : "lang_en");
}
namespace {
QThreadPool &languagePool() {
    struct Pool {
        QThreadPool threads;
        Pool() {
            threads.setMaxThreadCount(1); // Serialize choices even across reader lifetimes.
            threads.setExpiryTimeout(0);
            const auto drain = [this] {
                mainthread::AllowBlocking allow("debug language at application shutdown");
                mainthread::BlockingOperation operation("debug-language-shutdown");
                threads.waitForDone();
            };
            QObject::connect(qApp, &QCoreApplication::aboutToQuit, &threads, drain, Qt::DirectConnection);
            QObject::connect(qApp, &QObject::destroyed, &threads, drain, Qt::DirectConnection);
        }
    };
    static Pool pool;
    return pool.threads;
}
class SaveLanguage final : public QRunnable {
public:
    SaveLanguage(QString directory, DebugLanguage language, QObject *context, std::function<void(bool)> finished)
        : directory(std::move(directory)), language(language), guard(context), finished(std::move(finished)) {}
    void run() override {
        const bool saved = writeDebugLanguage(directory, language);
        // The worker owns its storage operation; closing the dialog or reader
        // neither waits for it nor gives it a dangling UI pointer.
        kobalt::qt::invoke(qApp, [guard = guard, finished = finished, saved] {
            if (guard && finished) finished(saved);
        }, Qt::QueuedConnection);
    }
private:
    QString directory;
    DebugLanguage language;
    QPointer<QObject> guard;
    std::function<void(bool)> finished;
};
}
void writeDebugLanguageAsync(const QString &directory, DebugLanguage language, QObject *context,
                             std::function<void(bool)> finished) {
    languagePool().start(new SaveLanguage(directory, language, context, std::move(finished)));
}
} // namespace kobalt
