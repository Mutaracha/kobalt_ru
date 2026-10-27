#include "storage/reader_storage.h"
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QTemporaryDir>

using namespace kobalt;

static void expect(bool value, const char *message) {
    if (!value)
        qFatal("%s", message);
}

// Write a disposable cache tree and durable profile files, then prove the
// helper removes only the caches and keeps every reading-data record.
int main(int argc, char **argv) {
    QCoreApplication app(argc, argv);
    QTemporaryDir directory;
    expect(directory.isValid(), "temporary directory");

    storage::Paths paths;
    paths.directory = directory.path();
    paths.bookPrefix = "book-";

    // Disposable targets: CREngine parsed books and the layout cache.
    QDir().mkpath(paths.documentCache() + "/nested");
    {
        QFile parsed(paths.documentCache() + "/nested/book.dat");
        expect(parsed.open(QIODevice::WriteOnly) && parsed.write("cached pages") == 12, "write document cache");
    }
    QDir().mkpath(QFileInfo(paths.layoutCache()).absolutePath());
    {
        QFile layout(paths.layoutCache());
        expect(layout.open(QIODevice::WriteOnly) && layout.write("{\"verdicts\":1}") == 14, "write layout cache");
    }

    // Durable data that must survive: settings, database, annotations, state.
    const QStringList durable{paths.preferences(), paths.readingSpeed(), paths.selection(), paths.database()};
    for (const auto &path : durable) {
        QFile file(path);
        expect(file.open(QIODevice::WriteOnly) && file.write("keep") == 4, "write durable file");
    }
    QDir().mkpath(paths.annotations());
    {
        QFile annotation(paths.annotations() + "/note.json");
        expect(annotation.open(QIODevice::WriteOnly) && annotation.write("{}") == 2, "write annotation");
    }
    const auto bookState = paths.bookState("file:///mnt/onboard/Books/novel.fb2");
    {
        QFile state(bookState);
        expect(state.open(QIODevice::WriteOnly) && state.write("progress") == 8, "write book state");
    }

    expect(storage::clearRenderingCaches(paths), "clear existing caches");
    expect(!QFileInfo::exists(paths.documentCache()), "document cache removed");
    expect(!QFileInfo::exists(paths.layoutCache()), "layout cache removed");
    expect(!QFileInfo::exists(QFileInfo(paths.layoutCache()).absolutePath()), "empty cache directory removed");
    for (const auto &path : durable)
        expect(QFileInfo::exists(path), "durable file preserved");
    expect(QFileInfo::exists(paths.annotations() + "/note.json"), "annotation preserved");
    expect(QFileInfo::exists(bookState), "reading progress preserved");

    // Missing caches are a no-op success so the action stays retryable.
    expect(storage::clearRenderingCaches(paths), "clear absent caches");

    // An existing target that cannot be removed as a directory tree is a
    // failure, not a silent skip. A regular file cannot be removeRecursively'd.
    QFile blocker(paths.documentCache());
    expect(blocker.open(QIODevice::WriteOnly) && blocker.write("not a directory") == 15, "create blocking file");
    expect(!storage::clearRenderingCaches(paths), "unremovable cache reported");
    blocker.close();
    expect(blocker.remove(), "remove blocking file");

    // Once the blocker is gone and a real cache directory exists again, the
    // same action clears it and reports success.
    QDir().mkpath(paths.documentCache());
    {
        QFile rebuilt(paths.documentCache() + "/book.dat");
        expect(rebuilt.open(QIODevice::WriteOnly) && rebuilt.write("cached") == 6, "rebuild document cache");
    }
    expect(storage::clearRenderingCaches(paths), "retry succeeds after blocker is gone");
    expect(!QFileInfo::exists(paths.documentCache()), "cache removed on retry");

    return 0;
}
