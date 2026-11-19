#include "check.h"
#include "reader_inputs.h"
#include "reading/reader_session.h"
#include "reading/rendering_mode.h"
#include "publication/local_resources.h"
#include "app/messages/opening_message.h"
#include <QEventLoop>
#include <QFile>
#include <QGuiApplication>
#include <QTemporaryDir>
#include <QTimer>
#include <atomic>
#include <cstdio>

using namespace kobalt;

namespace {
void waitUntil(const std::function<bool()> &ready)
{
    if (ready())
        return;
    QEventLoop loop;
    QTimer poll, deadline;
    poll.setInterval(5);
    QObject::connect(&poll, &QTimer::timeout, [&] {
        if (ready())
            loop.quit();
    });
    deadline.setSingleShot(true);
    QObject::connect(&deadline, &QTimer::timeout, [&] {
        require(false, "Timed out waiting for an FB2 session operation");
    });
    poll.start();
    deadline.start(20000);
    loop.exec();
}

void exact(const ReaderSession &reader)
{
    const auto state = reader.state();
    require(reader.error().isEmpty() && !reader.image().isNull(), "FB2 frame is unusable");
    require(!state.contains("book_fraction") && !state.contains("rough_pages")
                    && !state.value("continues_before").toBool() && !reader.continuesAfter(),
            "FB2 acquired a partial chapter or estimated book state");
    require(reader.bookPage() == reader.page() && reader.bookPages() == reader.pages()
                    && !reader.bookPagesEstimated()
                    && reader.progressFraction() == pageProgressFraction(reader.page(), reader.pages()),
            "FB2 display coordinates are not exact whole-book coordinates");
}

void checkMode(const QString &path, int mode)
{
    renderingMode::setOverride(mode);
    auto observed = std::make_shared<std::atomic_int>(0);
    const PositionCodec codec{
        [](const Engine &, const QString &anchor, QString *) { return anchor; },
        [observed](const Engine &engine, QString *) {
            const auto info = engine.pageInfo();
            require(info.bookPage.exact() && info.bookPages.exact()
                            && engine.bookFraction() < 0 && engine.roughPages().pages == 0
                            && !engine.fullDocumentReady() && !engine.fullDocumentStarted(),
                    "Hybrid session scheduled a partial/full replacement document for FB2");
            ++*observed;
            return engine.position();
        },
        [](const QString &anchor) { return anchor; }
    };
    {
        ReaderSession reader(test::readerFonts(), test::readerPlaceholder());
        auto lease = std::make_shared<SourceLease>();
        OpenRequest request{ path, path, QSize(600, 800), {}, {}, {}, lease,
                             localResourceFactory(path, lease), codec };
        bool opened = false;
        reader.open(std::move(request), [&](OpeningResult result) {
            require(result.ready(), qPrintable(openingMessage(result)));
            opened = true;
        });
        waitUntil([&] { return opened && !reader.busy(); });
        exact(reader);
        require(reader.pages() > 5, "FB2 session fixture is too short to exercise background turns");
        for (int turn = 0; turn < 4; ++turn) {
            require(reader.turn(1), "Cannot turn an FB2 session page");
            waitUntil([&] { return !reader.busy(); });
            exact(reader);
        }
        const auto anchor = reader.state().value("position");
        const auto generation = reader.layoutRevision();
        QJsonObject patch;
        patch["font_pixels"] = 34;
        reader.requestAppearance(reader.requestedAppearance().edited(appearance::Patch(patch)),
                                 QSize(800, 600), ReaderSession::hybridChapterMode);
        waitUntil([&] { return !reader.busy(); });
        exact(reader);
        require(reader.fontSize() == 34 && reader.image().size() == QSize(800, 600)
                        && reader.state().value("position") == anchor
                        && reader.layoutRevision() > generation,
                "Hybrid FB2 appearance request lost reflow, source anchor or layout generation");
        QVector<int> mapped;
        bool located = false;
        reader.bookmarkPages({ anchor.toString() }, [&](QVector<int> pages) {
            mapped = pages;
            located = true;
        });
        waitUntil([&] { return located; });
        require(mapped.size() == 1 && mapped.first() == reader.page(),
                "FB2 session bookmark page is not the zero-based whole-book display page");
        ReaderSession preview(test::readerFonts(), test::readerPlaceholder());
        bool previewed = false;
        const auto pixels = reader.image();
        preview.openPreview(reader, QSize(600, 800), [&](QString error) {
            require(error.isEmpty(), qPrintable(error));
            previewed = true;
        });
        waitUntil([&] { return previewed && !preview.busy(); });
        exact(preview);
        require(reader.state().value("position") == anchor && reader.image() == pixels,
                "FB2 session preview moved or repainted the reading source");
        require(observed->load() >= 6, "The test did not observe the worker's FB2 engine frames");
    }
    bool drained = false;
    ReaderSession::drain([&] { drained = true; });
    waitUntil([&] { return drained; });
}
} // namespace

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);
    QTemporaryDir directory;
    require(directory.isValid(), "Cannot create FB2 session fixture directory");
    const auto path = directory.path() + "/session.fb2";
    QFile file(path);
    require(file.open(QIODevice::WriteOnly), "Cannot create FB2 session fixture");
    QByteArray book("<FictionBook xmlns='http://www.gribuser.ru/xml/fictionbook/2.0'>"
                    "<description><title-info><book-title>Session boundary</book-title>"
                    "<lang>ru</lang></title-info></description><body><section>");
    for (int i = 0; i < 240; ++i)
        book += "<p>Body passage " + QByteArray::number(i)
                + " with enough text to exercise page turns, reflow and a semantic preview.</p>";
    book += "</section></body></FictionBook>";
    require(file.write(book) == book.size(), "Cannot write FB2 session fixture");
    file.close();
    renderingMode::setBuildQuietMilliseconds(0);
    checkMode(path, 0);
    checkMode(path, 2);
    renderingMode::setOverride(-1);
    std::puts("PASS: Accurate/Hybrid FB2 sessions retain exact whole-book pages, reflow and preview ownership");
}
