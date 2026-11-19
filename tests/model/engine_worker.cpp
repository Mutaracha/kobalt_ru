#include "engine/engine_worker.h"
#include "check.h"
#include <QCoreApplication>
#include <QMetaObject>
#include <QThread>
#include <QTimer>

using namespace kobalt;

// Checks foreground accounting, idle scheduling, thread ownership and worker shutdown.
int main(int argc, char **argv)
{
    qputenv("KOBALT_KOBO_EMULATION", "1");
    QCoreApplication app(argc, argv);
    require(!engineSlowdownEnabled() && engineSlowdown() == 1,
            "Engine emulation must not slow ordinary production jobs");
    const auto before = engineWorkRequests();
    auto *idle = engineWorkerForIdle();
    require(idle && engineWorkRequests() == before,
            "Idle scheduling must not count as foreground input");
    auto *worker = engineWorker();
    require(worker == idle && engineWorkRequests() == before + 1,
            "Foreground scheduling did not register exactly one engine request");
    const auto expected = engineWorkRequests();
    QTimer::singleShot(3000, &app, [] { require(false, "Engine worker callback timed out"); });
    require(QMetaObject::invokeMethod(worker, [&, expected] {
        require(QThread::currentThread() == engineWorkerState()->thread,
                "Engine work ran outside its owning thread");
        require(engineWorker() == worker && engineWorkRequests() == expected,
                "Worker continuation counted itself as waiting user work");
        require(engineWorkQuietMilliseconds() >= 0, "Engine quiet time is negative");
        QMetaObject::invokeMethod(&app, [&] { app.quit(); }, Qt::QueuedConnection);
    }, Qt::QueuedConnection), "Cannot queue engine work");
    const int result = app.exec();
    require(engineWorkerState()->stopped.load(), "The shared worker did not finish on app quit");
    return result;
}
