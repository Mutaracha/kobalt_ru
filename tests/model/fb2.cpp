#include "reader_inputs.h"
#include "reader_source_index_probe.h"
#include "check.h"
#include "engine/crengine/engine_document.h"
#include "engine/crengine/reader_fb2.h"
#include "publication/fb2.h"
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

void formatBoundaries() {
    require(fb2::matchesHead("<FictionBook >"), "FB2 root with whitespace");
    require(fb2::matchesHead(QByteArray("\xEF\xBB\xBF") + " \n<?xml version=\"1.0\"?><FictionBook>"),
            "FB2 BOM and XML declaration");
    require(!fb2::matchesHead("PK\003\004"), "ZIP must stay on the original archive path");
    require(!fb2::matchesHead("<?xml version=\"1.0\"?><rss>"), "Other XML is not FB2");
    require(!fb2::matchesHead("<FictionBookish>"), "FB2 root delimiter");
    require(!fb2::matchesHead("<?xml?><!ENTITY x 'text'><FictionBook>"), "FB2 entity guard");
    require(fb2::cachePolicy(false).isEmpty(), "EPUB cache policy must not acquire the FB2 marker");
    require(fb2::cachePolicy(true, false) != fb2::cachePolicy(true, true),
            "Toggling page notes must not reuse the other mode's parsed cache");
    require(fb2::cachePolicy(false, false).isEmpty(), "EPUB mode must not affect its cache key");
    require(fb2::noteNumber("[12]") == "12" && fb2::noteNumber(" (3) ") == "3" &&
                fb2::noteNumber("4") == "4" && fb2::noteNumber("[See 12]") == "[See 12]" &&
                fb2::noteNumber("IV") == "IV" && fb2::noteNumber("[]") == "[]",
            "FB2 number cleanup changed a non-numeric label or left brackets");
    require(fb2::cachePolicy(true) == QByteArray(":fb2-v4:inpage-on:"), "In-page FB2 CSS must invalidate the old parsed-book cache");
}

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
    EngineDocument model(test::readerFonts());
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
    require(firstPage.contains("[1]"), "Note-title cleanup changed the bracketed reference in main text");
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
    require(ReaderSourceIndexProbe::inPageFootnoteCount(model) > 0,
            "FB2 note is popup-only: no bottom-of-page footnote fragment");
    require(!model.image(model.page()).isNull(), "FB2 page with an in-page footnote did not paint");
    const auto anchor = model.position();
    require(model.applyAppearance(qt::object({{"fb2_inpage_footnotes", false}})), "Page notes toggle off");
    require(!model.state().value("fb2_inpage_footnotes").toBool(true) &&
                ReaderSourceIndexProbe::inPageFootnoteCount(model) == 0,
            "Disabled page notes still reserve a bottom block");
    bool popupWhileOff = false;
    for (const auto &link : model.pageLinks()) {
        const auto details = model.linkDetails(link.source);
        if (details.kind == LinkKind::Footnote)
            popupWhileOff = details.html.contains("Second note paragraph");
    }
    require(popupWhileOff, "Turning off page notes also disabled the multi-paragraph popup");
    require(model.applyAppearance(qt::object({{"fb2_inpage_footnotes", true}})) &&
                ReaderSourceIndexProbe::inPageFootnoteCount(model) > 0,
            "Enabling page notes did not repaginate the current book");
    require(model.position() == anchor, "Page-note toggle lost the source anchor");


    // Both conventional FB2 note bodies use the ready-made engine mechanism.
    // An unreferenced neighbouring section must not become another page note.
    auto comments = bytes;
    comments.replace("name=\"notes\"", "name=\"comments\"");
    comments.replace("</section></body>\n<binary",
                     "</section><section id=\"unused\"><title><p>Additional remarks</p></title><p>Unreferenced endnote.</p></section></body>\n<binary");
    comments.replace("<title><p>1</p></title>", "<title><p><strong>[1]</strong></p></title>");
    EngineDocument endnotes(test::readerFonts());
    require(endnotes.open(comments, QSize(1072, 1300), qt::object({{"font_pixels", 32}}), &error),
            qPrintable(error));
    require(ReaderSourceIndexProbe::noteTitle(endnotes, "n1") == "1",
            "FB2 note-number cleanup lost markup/IDs or kept brackets");
    require(ReaderSourceIndexProbe::noteTitle(endnotes, "unused") == "Additional remarks" &&
                !ReaderSourceIndexProbe::numericNoteTitle(endnotes, "unused"),
            "FB2 prose heading was converted to a numeric label");
    require(ReaderSourceIndexProbe::numericNoteTitle(endnotes, "n1"),
            "FB2 number was not marked for superscript styling");
    require(ReaderSourceIndexProbe::inPageFootnoteCount(endnotes) == 1,
            "FB2 comments must show the referenced note only");
    EngineDocument preview(test::readerFonts());
    require(preview.openPreview(model, QSize(800, 600), &error), qPrintable(error));
    require(preview.pages() > 0, "FB2 font preview rendered no pages");
    require(model.setHyphenation("on"), "Russian hyphenation mode was rejected");
    require(model.pages() > 0, "Hyphenation change removed the FB2 pages");
}

// Both formats must use the shared normalization markers; semantic blocks stay protected.
void sharedNormalization() {
    const QByteArray prose = "A regular prose paragraph with enough ordinary words to exceed the normalization threshold and exercise the same marker pipeline used for EPUB books.";
    const auto bytes = QByteArray("<FictionBook xmlns=\"http://www.gribuser.ru/xml/fictionbook/2.0\"><body><section>")
        + "<p id=\"prose\">" + prose + "</p><epigraph><p id=\"epigraph\">" + prose
        + "</p></epigraph></section></body><body name=\"notes\"><section><title><p>1</p></title><p id=\"note-prose\">"
        + prose + "</p></section></body></FictionBook>";
    EngineDocument model(test::readerFonts());
    QString error;
    require(model.open(bytes, QSize(800, 1000), {}, &error), qPrintable(error));
    require(ReaderSourceIndexProbe::nodeAttribute(model, "prose", "data-kobalt-beautify") == "body",
            "FB2 ordinary prose bypassed shared normalization");
    for (const auto *id : {"epigraph", "note-prose"})
        require(ReaderSourceIndexProbe::nodeAttribute(model, id, "data-kobalt-beautify").isEmpty(),
                "FB2 normalization flattened a semantic block");
}

void renderWindows1251() {
    const auto bytes = windows1251FictionBook();
    Publication publication;
    QString error;
    require(publication.inspect(bytes, &error) &&
                publication.format() == Publication::PublicationFormat::Fb2,
            "windows-1251 FB2 inspection");
    EngineDocument model(test::readerFonts());
    require(model.open(bytes, QSize(800, 600), {}, &error), qPrintable(error));
    require(model.title() == QString::fromUtf8("Тест"),
            qPrintable(QString("windows-1251 title decoded as: %1").arg(model.title())));
    require(!model.pageExcerpt(0).contains(QChar::ReplacementCharacter),
            "windows-1251 text produced replacement characters");
}
}

int main(int argc, char **argv) {
    QGuiApplication app(argc, argv);
    formatBoundaries();
    inspectFictionBook();
    rejectNonFictionBook();
    renderFictionBook();
    renderWindows1251();
    sharedNormalization();
    std::puts("PASS: FB2 inspection, cp1251 decoding, rendering, contents, cover, footnotes and preview");
}
