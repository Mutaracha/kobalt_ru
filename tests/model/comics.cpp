#include "check.h"
#include "reader_inputs.h"
#include "reader_source_index_probe.h"
#include "reading/appearance_settings.h"
#include <QBuffer>
#include <QGuiApplication>
#include <QTemporaryDir>

using namespace kobalt;

static QRect inkBounds(const QImage &image)
{
    QRect bounds;
    for (int y = 0; y < image.height(); ++y)
        for (int x = 0; x < image.width(); ++x) {
            const auto pixel = image.pixel(x, y);
            if (qRed(pixel) > 200 && qGreen(pixel) < 40 && qBlue(pixel) < 40)
                bounds = bounds.united(QRect(x, y, 1, 1));
        }
    return bounds;
}

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);
    // Same proportions and inline/div structure as the supplied KCC page,
    // but synthetic solid art makes geometry independently measurable.
    QImage art(1160, 1680, QImage::Format_RGB32);
    art.fill(Qt::red);
    QByteArray png;
    QBuffer buffer(&png);
    require(buffer.open(QIODevice::WriteOnly) && art.save(&buffer, "PNG"), "Encode comic art");
    const QMap<QString, QByteArray> data{
        {"mimetype", "application/epub+zip"},
        {"META-INF/container.xml", "<container xmlns='urn:oasis:names:tc:opendocument:xmlns:container' version='1.0'><rootfiles><rootfile full-path='book.opf' media-type='application/oebps-package+xml'/></rootfiles></container>"},
        {"book.opf", "<package xmlns='http://www.idpf.org/2007/opf' version='2.0'><metadata/><manifest><item id='c' href='c.xhtml' media-type='application/xhtml+xml'/><item id='i' href='art.png' media-type='image/png'/></manifest><spine><itemref idref='c'/></spine></package>"},
        {"c.xhtml", "<html xmlns='http://www.w3.org/1999/xhtml'><head><style>@page{margin:0}body{margin:0;padding:0}</style></head><body><div style='text-align:center'><img width='1160' height='1680' src='art.png'/></div></body></html>"},
        {"art.png", png}
    };
    QMap<QString, qint64> lengths;
    for (auto it = data.cbegin(); it != data.cend(); ++it) lengths[it.key()] = it.value().size();
    auto resources = std::make_shared<ResourceCache>(data.keys(),
        [data](const QString &name, const ResourceCache::Sink &sink, QString *) {
            const auto bytes = data.value(name);
            return sink(bytes.constData(), bytes.size());
        }, std::make_shared<std::atomic_bool>(false));
    for (const int ppi : {96, 300}) {
        readingPpi.store(ppi);
        QTemporaryDir cache;
        require(cache.isValid(), "Create comic cache");
        EngineDocument model(test::readerFonts());
        const QByteArray fingerprint(64, 'a');
        model.setDocumentCache(cache.path(), fingerprint, "comics-test", 1024 * 1024);
        QString error;
        const int baseMargin = readingPixels(42), editedMargin = readingPixels(60);
        const int baseTop = readingPixels(33), editedTop = readingPixels(51), baseBottom = readingPixels(27);
        QJsonObject saved{{appearance::comicsModeKey, true}, {"margin_pixels", baseMargin},
            {"top_margin_pixels", baseTop}, {"bottom_margin_pixels", baseBottom},
            {"two_columns_landscape", true}};
        require(model.openResources(resources, lengths, QSize(1264, 1680), saved, &error), qPrintable(error));
        require(ReaderSourceIndexProbe::comicsGeometry(model), "Comic profile did not override physical geometry");
        auto bounds = inkBounds(model.image(0));
        require(bounds.height() == 1680 && bounds.top() == 0 && qAbs(bounds.width() - 1160) <= 1,
                qPrintable(QString("Comic fit/baseline: %1,%2 %3x%4 at %5 PPI")
                    .arg(bounds.x()).arg(bounds.y()).arg(bounds.width()).arg(bounds.height()).arg(ppi)));
        require(model.pages() == 1, "Single graphic page acquired an extra page");
        require(model.state().value("margin_pixels").toInt() == baseMargin, "Overlay overwrote saved margin");
        require(model.applyAppearance({{"margin_pixels", editedMargin}, {"top_margin_pixels", editedTop}}), "Edit while active");
        require(ReaderSourceIndexProbe::comicsGeometry(model) && model.margin() == editedMargin,
                "Active edit must save base without changing effective margins");
        require(model.applyAppearance({}, QSize(1680, 1264)), "Rotate comic");
        require(ReaderSourceIndexProbe::comicsGeometry(model), "Landscape changed physical engine geometry");
        bounds = inkBounds(model.image(0));
        require(qAbs(bounds.width() - 840) <= 1
                    && qAbs(bounds.height() - qRound(840.0 * 1680 / 1160)) <= 2,
                "Landscape portrait did not fit a half-screen without distortion");
        require(model.saveDocumentCache(), "Save comics parsed cache");
        const auto checkpoint = model.state();
        EngineDocument reopened(test::readerFonts());
        reopened.setDocumentCache(cache.path(), fingerprint, "comics-test", 1024 * 1024);
        require(reopened.openResources(resources, lengths, QSize(1680, 1264), checkpoint, &error), qPrintable(error));
        require(reopened.openedFromCache(), "Compatible comics checkpoint did not use parsed cache");
        require(ReaderSourceIndexProbe::comicsGeometry(reopened) && reopened.margin() == editedMargin,
                "Reopen lost mode or updated base settings");
        require(reopened.applyAppearance({{appearance::comicsModeKey, false}}), "Disable comic mode");
        require(reopened.margin() == editedMargin && reopened.state().value("top_margin_pixels").toInt() == editedTop
                    && reopened.state().value("bottom_margin_pixels").toInt() == baseBottom && reopened.state().value("columns").toInt() == 2,
                "Disable did not restore latest base layout");
        require(reopened.embeddedFontCount() == 0, "Synthetic EPUB acquired document fonts");
    }
    // Ordinary prose must retain positive line height even with comics enabled.
    // This covers accidental activation on both EPUB-like markup and FB2.
    for (const bool fictionBook : {false, true}) {
        QByteArray paragraphs;
        for (int i = 0; i < 40; ++i)
            paragraphs += "<p id='p" + QByteArray::number(i) + "'>" +
                    QByteArray("Words on separate readable lines. ").repeated(20) + "</p>";
        const auto bytes = fictionBook
                ? "<FictionBook xmlns='http://www.gribuser.ru/xml/fictionbook/2.0'><description><title-info><book-title>Prose</book-title><lang>en</lang></title-info></description><body><section>" + paragraphs + "</section></body></FictionBook>"
                : "<html><head><title>Prose</title></head><body>" + paragraphs + "</body></html>";
        EngineDocument prose(test::readerFonts());
        QString error;
        const QJsonObject proseSettings{{appearance::comicsModeKey, true}, {"font_pixels", 32}};
        if (fictionBook) {
            require(prose.open(bytes, QSize(800, 1000), proseSettings, &error), qPrintable(error));
        } else {
            auto proseData = data;
            proseData["c.xhtml"] = bytes;
            QMap<QString, qint64> proseLengths;
            for (auto it = proseData.cbegin(); it != proseData.cend(); ++it) proseLengths[it.key()] = it.value().size();
            auto proseResources = std::make_shared<ResourceCache>(proseData.keys(),
                [proseData](const QString &name, const ResourceCache::Sink &sink, QString *) {
                    const auto resource = proseData.value(name);
                    return sink(resource.constData(), resource.size());
                }, std::make_shared<std::atomic_bool>(false));
            require(prose.openResources(proseResources, proseLengths, QSize(800, 1000), proseSettings, &error), qPrintable(error));
        }
        const auto first = ReaderSourceIndexProbe::nodeBounds(prose, "p0");
        const auto second = ReaderSourceIndexProbe::nodeBounds(prose, "p1");
        require(first.height() > 32 && second.top() >= first.bottom(),
                qPrintable(QString("Comics prose bounds (FB2=%1): first y=%2 h=%3, second y=%4 h=%5")
                    .arg(fictionBook).arg(first.y()).arg(first.height()).arg(second.y()).arg(second.height())));
        require(prose.pages() > 1, "Comics prose collapsed into a single page");
    }

    // Mixed source sequence: portrait + portrait, a wide spread, and an odd portrait.
    // Page numbering and saved anchors must describe displayed screens, not halves.
    auto mixed = data;
    QImage wide(1680, 1000, QImage::Format_RGB32);
    wide.fill(Qt::red);
    QByteArray widePng;
    QBuffer wideBuffer(&widePng);
    require(wideBuffer.open(QIODevice::WriteOnly) && wide.save(&wideBuffer, "PNG"), "Encode wide art");
    mixed["wide.png"] = widePng;
    mixed["book.opf"] = "<package xmlns='http://www.idpf.org/2007/opf' version='2.0'><metadata/><manifest><item id='c' href='c.xhtml' media-type='application/xhtml+xml'/><item id='i' href='art.png' media-type='image/png'/><item id='w' href='wide.png' media-type='image/png'/></manifest><spine><itemref idref='c'/></spine></package>";
    mixed["c.xhtml"] = "<html xmlns='http://www.w3.org/1999/xhtml'><head><title>Mixed</title></head><body><div><img src='art.png'/></div><div><img src='art.png'/></div><div><img src='wide.png'/></div><div><img src='art.png'/></div></body></html>";
    QMap<QString, qint64> mixedLengths;
    for (auto it = mixed.cbegin(); it != mixed.cend(); ++it) mixedLengths[it.key()] = it.value().size();
    auto mixedResources = std::make_shared<ResourceCache>(mixed.keys(),
        [mixed](const QString &name, const ResourceCache::Sink &sink, QString *) {
            const auto bytes = mixed.value(name);
            return sink(bytes.constData(), bytes.size());
        }, std::make_shared<std::atomic_bool>(false));
    readingPpi.store(96);
    EngineDocument spread(test::readerFonts());
    QString error;
    require(spread.openResources(mixedResources, mixedLengths, QSize(1680, 1264),
                {{appearance::comicsModeKey, true}}, &error), qPrintable(error));
    require(spread.pages() == 3, "Mixed art did not become pair / wide / odd screens");
    auto pair = spread.image(0);
    require(qRed(pair.pixel(100, 100)) > 200 && qGreen(pair.pixel(100, 100)) < 40
                && qGreen(pair.pixel(1000, 100)) < 40, "Portrait pair missing one side");
    const auto wideBounds = inkBounds(spread.image(1));
    require(wideBounds.width() == 1680 && qAbs(wideBounds.height() - 1000) <= 1,
                "Wide source spread rotated, cropped or fitted to half the screen");
    require(spread.go(1), "Select wide screen");
    require(inkBounds(spread.imageFromAnchor()) == wideBounds, "Anchored preview sliced a graphic screen");
    const auto widePosition = spread.position();
    require(spread.applyAppearance({}, QSize(1264, 1680)), "Rotate mixed art to portrait");
    require(spread.pages() == 4, "Portrait must have one source image per screen");
    require(spread.goTo(widePosition), "Wide anchor lost after rotation");
    require(spread.applyAppearance({}, QSize(1680, 1264)), "Rotate mixed art back");
    require(spread.page() == 1 && spread.pages() == 3, "Spread anchor/page mapping changed after rotation");
    // KCC can store a spread in a portrait-shaped bitmap and mark its spine
    // item as page-spread-center. Aspect ratio alone must never pair it.
    auto centered = mixed;
    centered["book.opf"] = "<package xmlns='http://www.idpf.org/2007/opf' version='3.0'><metadata/><manifest><item id='p' href='c.xhtml' media-type='application/xhtml+xml'/><item id='s' href='center.xhtml' media-type='application/xhtml+xml'/><item id='i' href='art.png' media-type='image/png'/></manifest><spine><itemref idref='p'/><itemref idref='s' properties='rendition:page-spread-center'/></spine></package>";
    centered["c.xhtml"] = "<html xmlns='http://www.w3.org/1999/xhtml'><head/><body><div><img src='art.png'/></div></body></html>";
    centered["center.xhtml"] = centered["c.xhtml"];
    const auto openCentered = [&centered](EngineDocument &document, const QJsonObject &settings) {
        QMap<QString, qint64> lengths;
        for (auto it = centered.cbegin(); it != centered.cend(); ++it) lengths[it.key()] = it.value().size();
        auto resources = std::make_shared<ResourceCache>(centered.keys(),
            [centered](const QString &name, const ResourceCache::Sink &sink, QString *) {
                const auto bytes = centered.value(name);
                return sink(bytes.constData(), bytes.size());
            }, std::make_shared<std::atomic_bool>(false));
        QString error;
        require(document.openResources(resources, lengths, QSize(1680, 1264), settings, &error), qPrintable(error));
    };
    QTemporaryDir centeredCache;
    require(centeredCache.isValid(), "Create centered spread cache");
    EngineDocument center(test::readerFonts());
    center.setDocumentCache(centeredCache.path(), "centered-spread", "comics-center-test", 1024 * 1024);
    openCentered(center, {{appearance::comicsModeKey, true}});
    require(center.pages() == 2, "Centered portrait-shaped source paired with its neighbour");
    require(center.go(1), "Select standalone centered screen");
    const auto centerBounds = inkBounds(center.image(1));
    require(centerBounds.width() == 1680 && qAbs(centerBounds.height() - 1160) <= 1,
            "Portrait-shaped center spread was not rotated to full landscape width");
    require(center.imageAt(centerBounds.center()).size() == QSize(1160, 1680),
            "Rotated center hit did not retain the original image identity");
    require(center.imageAt(QPoint(0, 0)).isNull(), "Letterbox outside rotated center hit artwork");
    const auto centerAnchor = center.position();
    require(inkBounds(center.imageFromAnchor()) == centerBounds,
            "Anchored center preview lost landscape rotation");
    require(center.saveDocumentCache(), "Save centered spread cache");
    EngineDocument centerCached(test::readerFonts());
    centerCached.setDocumentCache(centeredCache.path(), "centered-spread", "comics-center-test", 1024 * 1024);
    openCentered(centerCached, center.state());
    // The engine may reject cached rendered pages on a style-hash change and
    // repaginate the saved DOM. Both routes must retain standalone semantics.
    // The existing single-spine test above independently requires a rendered hit.
    require(centerCached.pages() == 2, "Saved centered checkpoint lost standalone page mapping");
    std::printf("Centered checkpoint rendered-cache-hit=%s; standalone pages preserved\n",
                centerCached.openedFromCache() ? "true" : "false");
    require(center.applyAppearance({}, QSize(1264, 1680)) && center.pages() == 2,
            "Portrait center mapping changed");
    require(center.applyAppearance({}, QSize(1680, 1264)) && center.pages() == 2,
            "Center placement lost after landscape round trip");
    require(center.goTo(centerAnchor) && inkBounds(center.image(center.page())) == centerBounds,
            "Standalone anchor lost rotation after orientation round trip");
    EngineDocument centerToggle(test::readerFonts());
    openCentered(centerToggle, {{appearance::comicsModeKey, false}});
    require(centerToggle.applyAppearance({{appearance::comicsModeKey, true}}) && centerToggle.pages() == 2,
            "Opening with comics off lost standalone metadata on enable");
    // Both resources of the visible spread must be ready before the next
    // image-load frame. Neighbour resources remain pending, not preloaded.
    auto deferredData = data;
    QImage blue(1160, 1500, QImage::Format_RGB32);
    blue.fill(Qt::blue);
    QByteArray bluePng;
    QBuffer blueBuffer(&bluePng);
    require(blueBuffer.open(QIODevice::WriteOnly) && blue.save(&blueBuffer, "PNG"), "Encode second portrait");
    deferredData["blue.png"] = bluePng;
    deferredData["neighbor.png"] = bluePng;
    deferredData["book.opf"] = "<package xmlns='http://www.idpf.org/2007/opf' version='2.0'><metadata/><manifest><item id='c' href='c.xhtml' media-type='application/xhtml+xml'/><item id='i' href='art.png' media-type='image/png'/><item id='b' href='blue.png' media-type='image/png'/></manifest><spine><itemref idref='c'/></spine></package>";
    deferredData["c.xhtml"] = "<html xmlns='http://www.w3.org/1999/xhtml'><head/><body><div><img src='art.png'/></div><div><img src='blue.png'/></div></body></html>";
    QMap<QString, qint64> deferredLengths;
    QMap<QString, int> loads;
    for (auto it = deferredData.cbegin(); it != deferredData.cend(); ++it) deferredLengths[it.key()] = it.value().size();
    auto deferredResources = std::make_shared<ResourceCache>(deferredData.keys(),
        [&loads, deferredData](const QString &name, const ResourceCache::Sink &sink, QString *) {
            ++loads[name];
            const auto bytes = deferredData.value(name);
            return sink(bytes.constData(), bytes.size());
        }, std::make_shared<std::atomic_bool>(false), ResourceCache::Statistics(),
        [](const QString &name) {
            return name.endsWith(".png") ? QSize(1160, name == "art.png" ? 1680 : 1500) : QSize();
        });
    EngineDocument deferredSpread(test::readerFonts());
    require(deferredSpread.openResources(deferredResources, deferredLengths, QSize(1680, 1264),
                {{appearance::comicsModeKey, true}}, &error), qPrintable(error));
    require(deferredSpread.pages() == 1, "Deferred portrait resources did not form one spread");
    deferredSpread.image(0);
    require(deferredResources->statistics().value("pending_images").toInt() == 2,
            "Visible spread did not queue both deferred portraits");
    // A neighbour is intentionally queued before the visible requests are
    // loaded; batching must still select only resources of the visible screen.
    deferredResources->imageReady("neighbor.png");
    require(deferredResources->prioritizePendingImage("neighbor.png"), "Queue neighbour first");
    require(deferredSpread.loadNextImage(), "Visible spread load made no progress");
    require(loads.value("art.png") == 1 && loads.value("blue.png") == 1 && !loads.contains("neighbor.png"),
            "Spread load published one side or eagerly loaded a neighbour");
    require(deferredResources->statistics().value("pending_images").toInt() == 1,
            "Visible spread batch consumed the neighbour queue");
    const auto readySpread = deferredSpread.image(0);
    require(qRed(readySpread.pixel(420, 600)) > 200 && qBlue(readySpread.pixel(1260, 600)) > 200,
            "Completed spread frame lacks one of the visible portraits");
    const auto leftBounds = inkBounds(readySpread);
    QRect rightBounds;
    for (int y = 0; y < readySpread.height(); ++y)
        for (int x = readySpread.width() / 2; x < readySpread.width(); ++x)
            if (qBlue(readySpread.pixel(x, y)) > 200 && qRed(readySpread.pixel(x, y)) < 40)
                rightBounds = rightBounds.united(QRect(x, y, 1, 1));
    require(leftBounds.top() > 0 && rightBounds.top() > leftBounds.top()
                && qAbs(leftBounds.top() - (1264 - leftBounds.height()) / 2) <= 1
                && qAbs(rightBounds.top() - (1264 - rightBounds.height()) / 2) <= 1,
            "Different-height portraits are not independently vertically centered");
    require(deferredSpread.imageAt(leftBounds.center()).size() == QSize(1160, 1680)
                && deferredSpread.imageAt(rightBounds.center()).size() == QSize(1160, 1500),
            "Centered pair image-viewer hit lost its source");
    require(deferredSpread.imageAt(QPoint(420, 0)).isNull()
                && deferredSpread.imageAt(QPoint(1260, 0)).isNull(),
            "Pair letterbox incorrectly hits artwork");

    // Regression for the case missed by load-batching alone: the first
    // publishable frame has one ready bitmap and one deferred bitmap.
    deferredSpread.releaseImages();
    loads.clear();
    deferredResources->imageReady("art.png");
    require(deferredResources->loadNextImage(), "Prime one ready portrait");
    deferredResources->imageReady("neighbor.png");
    const auto mixedReadyFrame = deferredSpread.frameImage();
    require(qRed(mixedReadyFrame.pixel(420, 600)) > 200
                && qBlue(mixedReadyFrame.pixel(1260, 600)) > 200
                && loads.value("blue.png") == 1 && !loads.contains("neighbor.png"),
            "First published frame exposed a half-ready spread or loaded a neighbour");
    // A completely cold first frame and a cached frame must follow the same
    // publication contract; neighbours remain asynchronous in both cases.
    deferredSpread.releaseImages();
    loads.clear();
    deferredResources->imageReady("neighbor.png");
    const auto coldFrame = deferredSpread.frameImage();
    require(qRed(coldFrame.pixel(420, 600)) > 200 && qBlue(coldFrame.pixel(1260, 600)) > 200
                && loads.value("art.png") == 1 && loads.value("blue.png") == 1
                && !loads.contains("neighbor.png"), "Cold published frame is not a complete visible pair");
    deferredSpread.frameImage();
    require(!loads.contains("neighbor.png"), "Cached publication consumed neighbour work");

    // Preparing many neighbour candidates must not erase the identity of a
    // still-cached half-ready current frame (the old map-wide clear did this).
    auto manyData = deferredData;
    QByteArray manyBody = "<html xmlns='http://www.w3.org/1999/xhtml'><head/><body>";
    for (int i = 0; i < 11; ++i)
        manyBody += "<div><img src='art.png'/></div><div><img src='blue.png'/></div>";
    manyData["c.xhtml"] = manyBody + "</body></html>";
    QMap<QString, qint64> manyLengths;
    for (auto it = manyData.cbegin(); it != manyData.cend(); ++it) manyLengths[it.key()] = it.value().size();
    auto manyResources = std::make_shared<ResourceCache>(manyData.keys(),
        [manyData](const QString &name, const ResourceCache::Sink &sink, QString *) {
            const auto bytes = manyData.value(name);
            return sink(bytes.constData(), bytes.size());
        }, std::make_shared<std::atomic_bool>(false), ResourceCache::Statistics(),
        [](const QString &name) {
            return name.endsWith(".png") ? QSize(1160, name == "art.png" ? 1680 : 1500) : QSize();
        });
    EngineDocument many(test::readerFonts());
    require(many.openResources(manyResources, manyLengths, QSize(1680, 1264),
                {{appearance::comicsModeKey, true}}, &error) && many.pages() == 11, qPrintable(error));
    many.image(0);
    for (int i = 1; i < many.pages(); ++i) many.image(i);
    const auto retainedFrame = many.frameImage();
    require(qRed(retainedFrame.pixel(420, 600)) > 200 && qBlue(retainedFrame.pixel(1260, 600)) > 200,
            "Neighbour request-map pressure exposed a cached half-ready current frame");

    // Center placement never requests a quarter-turn for an already-wide bitmap.
    auto wideCenteredData = centered;
    wideCenteredData["art.png"] = widePng;
    QMap<QString, qint64> wideCenteredLengths;
    for (auto it = wideCenteredData.cbegin(); it != wideCenteredData.cend(); ++it)
        wideCenteredLengths[it.key()] = it.value().size();
    auto wideCenteredResources = std::make_shared<ResourceCache>(wideCenteredData.keys(),
        [wideCenteredData](const QString &name, const ResourceCache::Sink &sink, QString *) {
            const auto bytes = wideCenteredData.value(name);
            return sink(bytes.constData(), bytes.size());
        }, std::make_shared<std::atomic_bool>(false));
    EngineDocument wideCentered(test::readerFonts());
    require(wideCentered.openResources(wideCenteredResources, wideCenteredLengths, QSize(1680, 1264),
                {{appearance::comicsModeKey, true}}, &error) && wideCentered.go(1), qPrintable(error));
    const auto landscapeBounds = inkBounds(wideCentered.frameImage());
    require(landscapeBounds == QRect(0, 132, 1680, 1000),
            "Already-landscape center bitmap was rotated or not vertically centered");
    require(wideCentered.imageAt(landscapeBounds.center()).size() == QSize(1680, 1000),
            "Landscape center viewer lost original dimensions");
    require(wideCentered.applyAppearance({}, QSize(1264, 1680)), "Rotate wide center to portrait view");
    const auto portraitBounds = inkBounds(wideCentered.image(wideCentered.page()));
    require(portraitBounds.width() > portraitBounds.height(), "Portrait viewport rotated an already-wide source");
    std::puts("PASS: comics fit, zero effective margins, orientation and editable base persistence");
}
