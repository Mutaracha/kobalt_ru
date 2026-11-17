#include "publication.h"
#include "fb2.h"
#include "support/strings.h"
#include <QCryptographicHash>
#include <QtEndian>

namespace kobalt {
Publication::Fb2Inspection Publication::inspectFb2(QIODevice &source, qint64 length, QString *error,
                                                   OpeningResult *result)
{
    const auto fail = [error, result](const char *reason) {
        if (result)
            *result = OpeningResult::failed(OpeningFailureKind::Io, reason);
        if (error)
            *error = strings::get(reason);
        return Fb2Inspection::Failed;
    };
    const auto append32 = [](QByteArray &out, quint32 n) {
        const auto value = qToLittleEndian(n);
        out.append(reinterpret_cast<const char *>(&value), 4);
    };
    // Recognize single-file FB2 before the ZIP end-of-directory scan. A raw
    // XML stream has no ZIP record, so the EPUB checks below must not run first.
    if (length >= 32 && length <= qint64(maxBytes)) {
        if (!source.seek(0))
            return fail(strings::key("error.cannot_read_publication_directory"));
        const auto head = source.read(qMin(length, qint64(65536)));
        if (head.size() != qMin(length, qint64(65536)))
            return fail(strings::key("error.incomplete_publication_directory"));
        if (fb2::matchesHead(head)) {
            format_ = PublicationFormat::Fb2;
            protected_ = false;
            if (!source.seek(0))
                return fail(strings::key("error.cannot_read_publication_directory"));
            // A single file has no central directory to fingerprint. Hash the
            // length plus the complete contents so any edit invalidates caches.
            QCryptographicHash hash(QCryptographicHash::Sha256);
            QByteArray sizePrefix;
            append32(sizePrefix, quint32(length));
            append32(sizePrefix, quint32(quint64(length) >> 32));
            hash.addData(sizePrefix);
            while (!source.atEnd()) {
                const auto chunk = source.read(64 * 1024);
                if (chunk.isEmpty())
                    return fail(strings::key("error.incomplete_publication_directory"));
                hash.addData(chunk);
            }
            fingerprint_ = hash.result().toHex();
            const auto resourceName = QString::fromLatin1(singleFb2ResourceName);
            names_ = QStringList{ resourceName };
            sizes_.clear();
            sizes_[resourceName] = length;
            return Fb2Inspection::Accepted;
        }
    }
    return Fb2Inspection::NotMatched;
}
} // namespace kobalt
