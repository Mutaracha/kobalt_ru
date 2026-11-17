#pragma once
#include "resource_cache.h"
#include "source_lease.h"
#include <memory>

namespace kobalt::fb2 {
// Synchronous worker-only transfer of the single virtual FB2 resource.
// Keep the lease, bounded chunks, timeout, error precedence and sink accounting
// identical to the local ZIP path. Does not own or extend the opening lifetime.
bool transferFile(const QString &path, qint64 expectedSize,
                  const std::shared_ptr<SourceLease> &lease, const ResourceCache::Sink &sink,
                  QString *error, qint64 &sinkNs);
} // namespace kobalt::fb2
