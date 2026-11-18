#include "fb2_resources.h"
#include "support/strings.h"
#include <QElapsedTimer>
#include <QFile>

namespace kobalt::fb2 {
bool transferFile(const QString &path, qint64 expectedSize,
                  const std::shared_ptr<SourceLease> &lease, const ResourceCache::Sink &sink,
                  QString *error, qint64 &sinkNs)
{
    const auto cancelled = [&lease] { return !lease || lease->revoked(); };
    auto fail = [&](const char *reason) {
        if (error && error->isEmpty()) {
            *error = strings::get(reason);
        }
        return false;
    };
    if (cancelled()) {
        return fail("error.reading_session_closed");
    }
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly) || file.size() != expectedSize) {
        return fail("error.a_book_resource_changed_during_reading");
    }
    QElapsedTimer deadline;
    deadline.start();
    qint64 transferred = 0;
    while (transferred < expectedSize) {
        if (cancelled()) {
            return fail("error.reading_session_closed");
        }
        if (deadline.elapsed() >= 5000) {
            return fail("error.native_publication_transfer_timed_out");
        }
        const auto bytes = file.read(qMin(qint64(ResourceCache::blockBytes), expectedSize - transferred));
        if (bytes.isEmpty()) {
            return fail("error.could_not_load_a_book_resource");
        }
        QElapsedTimer work;
        work.start();
        if (!sink(bytes.constData(), bytes.size())) {
            return false;
        }
        sinkNs += work.nsecsElapsed();
        transferred += bytes.size();
        if (file.size() != expectedSize)
            return fail("error.a_book_resource_changed_during_reading");
    }
    if (cancelled())
        return fail("error.reading_session_closed");
    if (file.error() != QFile::NoError || file.size() != expectedSize || !file.atEnd())
        return fail("error.a_book_resource_changed_during_reading");
    return true;
}
} // namespace kobalt::fb2
