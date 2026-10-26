#include "reader_inputs.h"
#include "check.h"
#include "engine/crengine/reader_model.h"
#include "publication/publication.h"
#include "support/qt_compat.h"

#include <QGuiApplication>
#include <QString>
#include <algorithm>

using namespace kobalt;

namespace {
constexpr auto fb2Namespace = "http://www.gribuser.ru/xml/fictionbook/2.0";
// A well-formed 1x1 PNG, small enough to stay below the resource budget.
constexpr auto coverPngBase64 =
    "iVBORw0KGgoAAAANSUhEUgAAAAEAAAABCAYAAAAfFcSJAAAAC0lEQVR42mNk+M9QDwADhgGAWjR9awAAAABJRU5ErkJggg==";

QByteArray utf8FictionBook() {
    QByteArray book;
    book += "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
    book += "<FictionBook xmlns=\"" + QByteArray(fb2Namespace) + "\">\n";
    book += "<description><title-info>";
    book += "<genre>science_fiction</genre>";
    book += "<book-title>Russian Test Book</book-title>";
    book += "<author><first-name>Test</first-name><last-name>Author</last-name></author>";
    book += "<lang>ru</lang>";
    book += "<coverpage><image href=\"#cover.png\"/></coverpage>";
    book += "</title-info>";
    book += "<document-info><id>test-document-id</id></document-info>";
    book += "</description>\n<body>\n";
    book += "<section id=\"ch1\"><title><p>Chapter One</p></title>";
    book += "<epigraph><p>Epigraph text</p></epigraph>";
    book += "<p>Regular paragraph with a note reference <a l:href=\"#n1\" type=\"note\">[1]</a> and following words.</p>";
    book += "<poem><stanza><v>Line one</v><v>Line two</v></stanza></poem>";
    book += "<table><tr><td><p>Table cell</p></td></tr></table>";
    for (int i = 0; i < 12; ++i)
        book += "<p>Filler paragraph " + QByteArray::number(i) + " with enough words to fill a screen.</p>";
    book += "</section>\n";
    book += "<section id=\"ch2\"><title><p>Chapter Two</p></title><p>Second chapter text.</p></section>";
    book += "</body>\n<body name=\"notes\">";
    book += "<section id=\"n1\"><title><p>1</p></title>";
    book += "<p>First <strong>note</strong> paragraph.</p><p>Second note paragraph.</p>";
    book += "</section></body>\n";
    book += "<binary id=\"cover.png\" content-type=\"image/png\">" + QByteArray(coverPngBase64) + "</binary>";
    book += "</FictionBook>";
    return book;
}

QByteArray windows1251FictionBook() {
    QByteArray book;
    book += "<?xml version=\"1.0\" encoding=\"windows-1251\"?>\r\n";
    book += "<FictionBook xmlns=\"" + QByteArray(fb2Namespace) + "\">";
    book += "<description><title-info><book-title>";
    book += QByteArray::fromHex("d0e5f1f2"); // Тест
    book += "</book-title><author><last-name>";
    book += QByteArray::fromHex("d0e5f1f2"); // Тест
    book += "</last-name></author><lang>ru</lang></title-info></description>";
    book += "<body><section><p>";
    book += QByteArray::fromHex("cff0e8e2e5f22c20ece8f021"); // Привет, мир!
    book += "</p></section></body></FictionBook>";
    return book;
}

void inspectFictionBook() {
    const auto bytes = utf8FictionBook();
    Publication publication;
    QString error;
    require(publication.inspect(bytes, &error), qPrintable(error));
    require(publication.format() == Publication::PublicationFormat::Fb2, "FB2 format classification");
    require(!publication.fingerprint().isEmpty(), "FB2 fingerprint is empty");
    require(publication.names() ==
                QStringList{QString::fromLatin1(Publication::singleFb2ResourceName)},
            "FB2 exposes one virtual resource name");
    require(publication.sizes().value(QString::fromLatin1(Publication::singleFb2ResourceName)) == bytes.size(),
            "FB2 virtual resource size");
    require(!publication.protectedBook(), "FB2 cannot be a protected publication");
    auto changed = bytes;
    changed.replace("Filler paragraph 0", "Changed paragraph 0");
    Publication other;
    require(other.inspect(changed, &error) && other.fingerprint() != publication.fingerprint(),
            "FB2 fingerprint must follow content edits");
}

void rejectNonFictionBook() {
    Publication publication;
    QString error;
    const QByteArray entity = "<?xml version=\"1.0\"?>\n<!DOCTYPE x [<!ENTITY a \"b\">]>\n"
                              "<FictionBook xmlns=\"x\"><body/></FictionBook>";
    require(!publication.inspect(entity, &error), "XML entity expansion was accepted");
    const QByteArray feed = "<?xml version=\"1.0\"?><rss version=\"2.0\"><channel>"
                            "<title>Feed title with enough length</title></channel></rss>";
    require(!publication.inspect(feed, &error), "An RSS feed was accepted as FB2");
    const QByteArray archive = QByteArray::fromHex("504b0304") + QByteArray(40, '\0');
    require(!publication.inspect(archive, &error), "A ZIP archive was accepted as FB2");
}

void renderFictionBook() {
    const auto bytes = utf8FictionBook();
    ReaderModel model(test::readerFonts());
    QString error;
    require(model.open(bytes, QSize(1072, 1300), qt::object({{"font_pixels", 32}, {"hyphenation", "on"}}),
                       &error),
            qPrintable(error));
    require(model.pages() > 0, "FB2 rendered no pages");
    require(model.title().contains("Russian Test Book"), "FB2 title metadata");
    require(!model.author().isEmpty(), "FB2 author metadata");
    const auto chapters = model.chapters();
    require(chapters.size() >= 2, "FB2 nested sections produced no table of contents");
    require(std::any_of(chapters.begin(), chapters.end(),
                        [](const ChapterLink &chapter) { return chapter.title.contains("Chapter One"); }),
            "FB2 chapter title missing from contents");
    const auto firstPage = model.pageExcerpt(0);
    require(!firstPage.contains("science_fiction"), "Raw FB2 genre metadata leaked onto the page");
    require(!firstPage.contains("test-document-id"), "document-info leaked onto the page");
    require(!model.coverImage(QSize(120, 180)).isNull(), "FB2 binary cover was not decoded");
    bool foundNote = false;
    for (const auto &link : model.pageLinks()) {
        const auto details = model.linkDetails(link.source);
        if (details.kind == LinkKind::Footnote) {
            foundNote = true;
            require(details.html.contains("First") && details.html.contains("Second note paragraph"),
                    "FB2 footnote lost multi-paragraph content");
            require(!details.html.contains("href="), "Footnote preview kept an active link");
            break;
        }
    }
    require(foundNote, "FB2 note reference did not resolve as a footnote");
    ReaderModel preview(test::readerFonts());
    require(preview.openPreview(model, QSize(800, 600), &error), qPrintable(error));
    require(preview.pages() > 0, "FB2 font preview rendered no pages");
    require(model.setHyphenation("on"), "Russian hyphenation mode was rejected");
    require(model.pages() > 0, "Hyphenation change removed the FB2 pages");
}

void renderWindows1251() {
    const auto bytes = windows1251FictionBook();
    Publication publication;
    QString error;
    require(publication.inspect(bytes, &error) &&
                publication.format() == Publication::PublicationFormat::Fb2,
            "windows-1251 FB2 inspection");
    ReaderModel model(test::readerFonts());
    require(model.open(bytes, QSize(800, 600), {}, &error), qPrintable(error));
    require(model.title() == QString::fromUtf8("Тест"),
            qPrintable(QString("windows-1251 title decoded as: %1").arg(model.title())));
    require(!model.pageExcerpt(0).contains(QChar::ReplacementCharacter),
            "windows-1251 text produced replacement characters");
}
}

int main(int argc, char **argv) {
    QGuiApplication app(argc, argv);
    inspectFictionBook();
    rejectNonFictionBook();
    renderFictionBook();
    renderWindows1251();
    std::puts("PASS: FB2 inspection, cp1251 decoding, rendering, contents, cover, footnotes and preview");
}
