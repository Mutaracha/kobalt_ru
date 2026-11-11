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
        require(ReaderSourceIndexProbe::comicsGeometry(model), "Landscape enabled a spread");
        bounds = inkBounds(model.image(0));
        require(bounds.height() == 1264 && qAbs(bounds.width() - qRound(1264.0 * 1160 / 1680)) <= 1,
                "Landscape cropped or stretched the graphic");
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
    std::puts("PASS: comics fit, zero effective margins, orientation and editable base persistence");
}
