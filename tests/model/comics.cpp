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
                "Comics mode collapsed prose lines or overlaid paragraphs");
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
    std::puts("PASS: comics fit, zero effective margins, orientation and editable base persistence");
}
