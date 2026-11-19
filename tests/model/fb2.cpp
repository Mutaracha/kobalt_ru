#include "reader_inputs.h"
#include "anchor_preview.h"
#include "reader_source_index_probe.h"
#include "check.h"
#include "engine/blaze/adapter/engine_document.h"
#include "engine/blaze/adapter/reader_fb2.h"
#include "engine/blaze/adapter/reader_fork_styles.h"
#include <QCryptographicHash>
#include <QtEndian>
#include "publication/fb2.h"
#include "render/screen_refresh.h"
#include "publication/publication.h"
#include "support/qt_compat.h"

#include <QGuiApplication>
#include <QFile>
#include <QRegularExpression>
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
    require(fb2::matchesHead("<!-- before root --><?book test?><FictionBook >"),
            "FB2 comments and processing instructions before root");
    require(fb2::matchesHead("<?xml version='1.0'?><!DOCTYPE FictionBook SYSTEM 'unused>name'><FictionBook>"),
            "FB2 doctype quoted delimiter");
    for (const auto &invalid : {QByteArray("<?xml?><rss><!-- <FictionBook> --></rss>"),
                               QByteArray("<?xml?><!-- <FictionBook> -->"),
                               QByteArray("<?book <FictionBook> ?>"),
                               QByteArray("<!-- unfinished <FictionBook>"),
                               QByteArray("<!DOCTYPE FictionBook [<!ENTITY x 'test'>]><FictionBook>"),
                               QByteArray("<FictionBookish>"), QByteArray("<FictionBook")})
        require(!fb2::matchesHead(invalid), "FB2 probe accepted a decoy/incomplete root");
    require(fb2::cachePolicy(false).isEmpty(), "EPUB cache policy must not acquire the FB2 marker");
    require(fb2::cachePolicy(true) == QByteArray(":fb2-v9:"), "FB2 migration cache marker");

}

void languageBinding()
{
    for (const auto &explicitLanguage : {QByteArray(), QByteArray("de")}) {
        const auto bytes = QByteArray("<FictionBook id='book' lang='") + explicitLanguage
                + "' xmlns='http://www.gribuser.ru/xml/fictionbook/2.0'>"
                  "<description><title-info><lang>ru</lang></title-info></description>"
                  "<body><section><p id='local' lang='fr'>Texte.</p></section></body></FictionBook>";
        EngineDocument model(test::readerFonts());
        QString error;
        require(model.open(bytes, QSize(800, 600), {}, &error), qPrintable(error));
        require(ReaderSourceIndexProbe::nodeAttribute(model, "book", "lang")
                        == (explicitLanguage.isEmpty() ? QString("ru") : QString("de")),
                "FB2 metadata language binding replaced explicit root language");
        require(ReaderSourceIndexProbe::nodeAttribute(model, "local", "lang") == "fr",
                "FB2 metadata language binding replaced local language");
    }
}

void previewPosition()
{
    QByteArray bytes("<FictionBook xmlns='http://www.gribuser.ru/xml/fictionbook/2.0'>"
                     "<description><title-info><lang>ru</lang></title-info></description>"
                     "<body><section><title><p>Chapter</p></title>");
    for (int i = 0; i < 40; ++i)
        bytes += "<p>Earlier paragraph that must not replace the preview passage.</p>";
    bytes += "<p>Prefix before inline <emphasis id='target'>abcdef Bookmark passage inside inline.</emphasis>"
             " Following text after inline.</p>";
    for (int i = 0; i < 40; ++i)
        bytes += "<p>Following paragraph with enough text to fill the preview.</p>";
    bytes += "</section></body></FictionBook>";
    EngineDocument source(test::readerFonts());
    QString error;
    require(source.open(bytes, QSize(800, 600), {}, &error), qPrintable(error));
    const auto position = ReaderSourceIndexProbe::textAnchor(source, "target", 7);
    require(!position.isEmpty() && source.goTo(position), "Cannot set inline source bookmark");
    const auto state = source.state();
    const auto image = source.image(source.page());
    EngineDocument preview(test::readerFonts());
    require(preview.openPreview(source, QSize(800, 600), &error), qPrintable(error));
    // Flattened preview text includes the rest of the paragraph after inline,
    // so compare the source tail as a prefix, not as the complete text node.
    const auto tail = ReaderSourceIndexProbe::anchorTail(source);
    require(tail.startsWith("Bookmark"), "Regression bookmark offset is not nonzero");
    const auto anchor = preview.position();
    for (int thickness : {0, 1, 2, 0}) {
        require(preview.setThickness(thickness), "Preview thickness edit failed");
        require(ReaderSourceIndexProbe::anchorTail(preview).startsWith(tail),
                "FB2 preview lost source passage or inline text offset");
        require(preview.position() == anchor && !preview.imageFromAnchor(false).isNull(),
                "FB2 preview thickness edit moved its bookmark");
        require(source.position() == position && source.state() == state
                        && source.image(source.page()) == image,
                "FB2 preview changed main reading state");
    }
}

void previewReliability()
{
    for (const auto &wrapper : {QPair<QByteArray, QByteArray>{"<p>", "</p>"},
                                {"<section><cite><p>", "</p></cite></section>"},
                                {"<poem><stanza><v>", "</v></stanza></poem>"},
                                {"<subtitle>", "</subtitle>"}}) {
        const auto bytes = QByteArray("<FictionBook xmlns='http://www.gribuser.ru/xml/fictionbook/2.0'>"
                          "<description><title-info><lang>ru-RU</lang></title-info></description>"
                          "<body><section><p>Earlier source paragraph.</p>")
                + wrapper.first
                + "До вставки 🦉 <emphasis id='target'>абвгде Контроль позиции.</emphasis> После вставки."
                + wrapper.second + "<p>Following paragraph.</p></section></body></FictionBook>";
        EngineDocument source(test::readerFonts()), preview(test::readerFonts());
        QString error;
        require(source.open(bytes, QSize(800, 600), {}, &error), qPrintable(error));
        const auto position = ReaderSourceIndexProbe::textAnchor(source, "target", 7);
        require(source.goTo(position), "Cannot set Russian FB2 preview bookmark");
        const auto state = source.state();
        require(preview.openPreview(source, QSize(800, 600), &error), qPrintable(error));
        const auto tail = ReaderSourceIndexProbe::anchorTail(source);
        const auto bookmark = preview.position();
        require(tail.startsWith(QString::fromUtf8("Контроль")), "Russian preview fixture offset");
        require(ReaderSourceIndexProbe::documentLanguage(preview) == "ru-RU",
                "FB2 preview truncated a language subtag");
        for (int size : {40, 56, 40}) {
            require(preview.style(size, 130), "FB2 preview reflow failed");
            require(preview.position() == bookmark
                            && ReaderSourceIndexProbe::anchorTail(preview).startsWith(tail)
                            && !preview.imageFromAnchor(false).isNull(),
                    "Russian FB2 preview reflow lost a codepoint bookmark");
            require(source.state() == state && source.position() == position,
                    "Russian FB2 preview reflow changed reading state");
        }
    }
    EngineDocument source(test::readerFonts()), preview(test::readerFonts());
    QString error;
    require(source.open(QByteArray("<FictionBook lang='ru-RU' xmlns='http://www.gribuser.ru/xml/fictionbook/2.0'>"
                        "<description><title-info><lang>en-US</lang></title-info></description>"
                        "<body><section><p>Текст с явным языком корня.</p></section></body></FictionBook>"),
                        QSize(800, 600), {}, &error), qPrintable(error));
    require(preview.openPreview(source, QSize(800, 600), &error), qPrintable(error));
    require(ReaderSourceIndexProbe::anchorLanguage(preview) == "ru-RU",
            "FB2 preview discarded explicit root language");
}

void previewHeading(EngineDocument &source, const QString &title)
{
    const auto bookmark = ReaderSourceIndexProbe::titleBookmark(source, title);
    require(!bookmark.isEmpty() && source.goTo(bookmark), "Cannot locate FB2 title bookmark");
    const auto signature = ReaderSourceIndexProbe::headingSignature(source);
    const auto tail = ReaderSourceIndexProbe::anchorTail(source);
    const auto state = source.state();
    const auto image = source.image(source.page());
    EngineDocument preview(test::readerFonts());
    QString error;
    require(preview.openPreview(source, source.image(source.page()).size(), &error), qPrintable(error));
    const auto anchor = preview.position();
    for (int thickness : {0, 1, 2, 0}) {
        require(preview.setThickness(thickness), "Cannot change FB2 title preview thickness");
        require(!signature.isEmpty() && ReaderSourceIndexProbe::headingSignature(preview) == signature,
                qPrintable(QString("FB2 title preview lost heading weight/margins: source=%1 preview=%2")
                           .arg(signature, ReaderSourceIndexProbe::headingSignature(preview))));
        require(ReaderSourceIndexProbe::anchorTail(preview) == tail && preview.position() == anchor
                        && !preview.imageFromAnchor(false).isNull(),
                "FB2 title preview lost its original text/position");
        require(source.state() == state && source.image(source.page()) == image,
                "FB2 title preview mutated the reading document");
    }
}

void semanticPreview()
{
    EngineDocument source(test::readerFonts());
    QString error;
    const auto bytes = QByteArray("<FictionBook xmlns='http://www.gribuser.ru/xml/fictionbook/2.0'>"
                      "<description><title-info><lang>ru</lang></title-info></description>"
                      "<body><section><title style='margin-top:2em;margin-bottom:1em'>"
                      "<p><strong>Проверка заголовка</strong></p></title>"
                      "<p>Обычный текст главы.</p></section></body></FictionBook>");
    require(source.open(bytes, QSize(800, 1000), {}, &error), qPrintable(error));
    previewHeading(source, QString::fromUtf8("Проверка заголовка"));
}

void forkStyleFragments()
{
    // Frozen pre-extraction bytes: no cascade, spacing or cache-policy change.
    const auto digest = [](const char *css) {
        return QCryptographicHash::hash(QByteArray(css), QCryptographicHash::Sha256).toHex();
    };
    require(digest(forkstyles::fb2LineHeightSelectors()) == QByteArray("d0069a24d19af3558e8d0eac11702b9ef7f33a8b3ad4ca35c7c3be590e6946b5"),
            "Extracted fb2LineHeightSelectors changed pre-refactor CSS bytes");
    require(digest(forkstyles::fb2NoteSelectors()) == QByteArray("1330b018264ccfc9625baba11c596f2a407c8dc8e49c00701ac9dd5e77332983"),
            "Extracted fb2NoteSelectors changed pre-refactor CSS bytes");
    require(digest(forkstyles::noteLabelsCss()) == QByteArray("eb821c290b2754a0fef5fda591964afbd56c001b4fee3e2ce06c754006932838"),
            "Extracted noteLabelsCss changed pre-refactor CSS bytes");
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
    book += QByteArray::fromHex("d2e5f1f2"); // Тест
    book += "</book-title><author><last-name>";
    book += QByteArray::fromHex("d2e5f1f2"); // Тест
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
    const quint64 length = qToLittleEndian(quint64(bytes.size()));
    QByteArray originalKey(reinterpret_cast<const char *>(&length), 8);
    originalKey += bytes;
    require(publication.fingerprint()
                    == QCryptographicHash::hash(originalKey, QCryptographicHash::Sha256).toHex(),
            "FB2 inspection changed the existing length-prefix/content cache fingerprint");
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

// A cover/title/epigraph may precede the first reference; do not assume page 0.
bool goToNoteReference(EngineDocument &model) {
    for (int page = 0; page < model.pages(); ++page) {
        if (!model.go(page))
            return false;
        for (const auto &link : model.pageLinks()) {
            const auto details = model.linkDetails(link.source);
            if (details.kind == LinkKind::Footnote && details.label == "[1]")
                return true;
        }
    }
    return false;
}

void renderFictionBook() {
    const auto bytes = utf8FictionBook();
    EngineDocument model(test::readerFonts());
    QString error;
    require(model.open(bytes, QSize(1072, 1300), qt::object({{"font_pixels", 32}, {"hyphenation", "on"}, {"inpage_footnotes", true}}),
                       &error),
            qPrintable(error));
    require(model.pages() > 0, "FB2 rendered no pages");
    require(model.embeddedFontCount() == 0, "FB2 must never enumerate embedded fonts");
    require(model.title().contains("Russian Test Book"), "FB2 title metadata");
    require(!model.author().isEmpty(), "FB2 author metadata");
    const auto chapters = model.chapters();
    require(chapters.size() >= 2, "FB2 nested sections produced no table of contents");
    require(std::any_of(chapters.begin(), chapters.end(),
                        [](const ChapterLink &chapter) { return chapter.title.contains("Chapter One"); }),
            "FB2 chapter title missing from contents");
    for (const auto &chapter : chapters) {
        if (chapter.page < 0 || !chapter.title.contains("Chapter"))
            continue;
        model.go(chapter.page);
        require(model.chapterProgress().page == 1,
                "FB2 chapter start does not reach the shared refresh/progress contract");
        require(refreshOnTurn(ScreenRefresh::Smart, smartRefreshTurns,
                              model.chapterProgress().page == 1),
                "Smart refresh missed an eligible FB2 chapter boundary");
    }
    model.go(0);
    const auto firstPage = model.pageExcerpt(0);
    require(!firstPage.contains("science_fiction"), "Raw FB2 genre metadata leaked onto the page");
    require(!firstPage.contains("test-document-id"), "document-info leaked onto the page");
    require(!model.coverImage(QSize(120, 180)).isNull(), "FB2 binary cover was not decoded");
    require(goToNoteReference(model), "FB2 bracketed reference disappeared or was changed by title cleanup");
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
    test::requireAnchoredPreviewRestores(model);
    const auto anchor = model.position();
    require(model.applyAppearance(appearance::Patch(qt::object({{"inpage_footnotes", false}}))), "Page notes toggle off");
    require(!model.state().value("inpage_footnotes").toBool(true) &&
                ReaderSourceIndexProbe::inPageFootnoteCount(model) == 0,
            "Disabled page notes still reserve a bottom block");
    bool popupWhileOff = false;
    for (const auto &link : model.pageLinks()) {
        const auto details = model.linkDetails(link.source);
        if (details.kind == LinkKind::Footnote)
            popupWhileOff = details.html.contains("Second note paragraph");
    }
    require(popupWhileOff, "Turning off page notes also disabled the multi-paragraph popup");
    require(model.applyAppearance(appearance::Patch(qt::object({{"inpage_footnotes", true}}))) &&
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
    require(endnotes.open(comments, QSize(1072, 1300), qt::object({{"font_pixels", 32}, {"inpage_footnotes", true}}), &error),
            qPrintable(error));
    require(goToNoteReference(endnotes), "FB2 comments lost the bracketed source reference");
    require(ReaderSourceIndexProbe::inPageFootnoteCount(endnotes) == 1,
            "FB2 comments must show the referenced note only");
    // Match KOReader's descendant section rule, not just direct body children.
    auto nestedBytes = bytes;
    nestedBytes.replace("<section id=\"n1\">", "<section id=\"group\"><section id=\"n1\">");
    nestedBytes.replace("</section></body>\n<binary", "</section></section></body>\n<binary");
    EngineDocument nestedNotes(test::readerFonts());
    require(nestedNotes.open(nestedBytes, QSize(1072, 1300), qt::object({{"font_pixels", 32}, {"inpage_footnotes", true}}), &error),
            qPrintable(error));
    require(goToNoteReference(nestedNotes) && ReaderSourceIndexProbe::inPageHint(nestedNotes, "n1") &&
                ReaderSourceIndexProbe::inPageFootnoteCount(nestedNotes) > 0,
            "Nested FB2 sections did not receive KOReader's inpage hint");
    const auto nestedPixels = ReaderSourceIndexProbe::nodeFontPixels(nestedNotes, "n1");
    require(nestedPixels >= 25 && nestedPixels <= 26,
            "Nested FB2 footnote must use upstream 0.8rem without compounding");
    auto runInBytes = bytes;
    runInBytes.replace("<title><p>1</p></title>", "<title id='note-title'><p id='note-label'>1</p></title>");
    runInBytes.replace("<p>First <strong>note</strong>", "<p><strong id='note-first-word'>First</strong> <strong>note</strong>");
    EngineDocument runIn(test::readerFonts());
    require(runIn.open(runInBytes, QSize(1072, 1300),
            qt::object({{"font_pixels", 32}, {"inpage_footnotes", true}}), &error), qPrintable(error));
    require(goToNoteReference(runIn), "Cannot locate run-in FB2 note reference");
    const auto labelRect = ReaderSourceIndexProbe::firstTextBounds(runIn, "note-label");
    const auto textRect = ReaderSourceIndexProbe::firstTextBounds(runIn, "note-first-word");
    require(!labelRect.isEmpty() && !textRect.isEmpty()
                && labelRect.top() < textRect.bottom() && textRect.top() < labelRect.bottom(),
            "Numeric FB2 note title and its first word occupy separate lines");
    require(ReaderSourceIndexProbe::nodeFontPixels(runIn, "note-label")
                < ReaderSourceIndexProbe::nodeFontPixels(runIn, "note-first-word"),
            "FB2 note number is not reduced for superscript");
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
            qPrintable(QString("FB2 ordinary prose bypassed shared normalization: %1")
                       .arg(ReaderSourceIndexProbe::normalizationDiagnostic(model, "prose"))));
    for (const auto *id : {"epigraph", "note-prose"})
        require(ReaderSourceIndexProbe::nodeAttribute(model, id, "data-kobalt-beautify").isEmpty(),
                "FB2 normalization flattened a semantic block");
}

// Optional user-supplied fixture; CI passes its canonical diagnostic path.
void reportedFictionBook(const QString &path)
{
    QFile input(path);
    require(input.open(QIODevice::ReadOnly), "Cannot read the reported FB2 fixture");
    const auto bytes = input.readAll();
    QString error;
    auto fonts = test::readerFonts();
    fonts.sources.sideloadDirectory = QStringLiteral(KOBALT_TEST_FIRA_DIRECTORY);
    {
        auto configured = QJsonObject{ { "font_face", "Fira Sans" }, { "font_pixels", 56 },
                { "weight_steps", 1 }, { "line_spacing", 130 }, { "word_spacing", 80 },
                { "text_alignment", "justify" }, { "widows_orphans", "allow" },
                { "vertical_justification", "off" }, { "inpage_footnotes", true },
                { "hide_nonlinear_fragments", true }, { "normalize_page_margins", true },
                { "reduce_chapter_spacing", true }, { "margin_pixels", 16 },
                { "top_margin_pixels", 0 }, { "bottom_margin_pixels", 0 } };
        for (const auto &language : {QByteArray("ru"), QByteArray("en")}) {
        auto localized = bytes;
        localized.replace("<lang>ru</lang>", QByteArray("<lang>") + language + "</lang>");
        const auto previewLanguage = language == "ru" ? QByteArray("en") : QByteArray("ru");
        for (const bool normalize : {false, true}) {
            configured["beautifier_spacing"] = normalize;
            EngineDocument replay(fonts);
            require(replay.open(localized, QSize(1264, 1608), configured, &error), qPrintable(error));
            require(replay.state().value("font_face") == "Fira Sans", "Fira Sans replay fell back");
            require(!replay.image(replay.page()).isNull(), "Reported FB2 first paint failed");
            auto drift = ReaderSourceIndexProbe::paragraphFormatDrift(replay);
            require(drift.isEmpty(), qPrintable(QString("FB2 pagination/paint mismatch after thickness (spacing=%1):\n%2")
                    .arg(normalize).arg(drift.join('\n'))));
            // Appearance samples are English and share CREngine's process-wide
            // language manager with the Russian book. They must not change the
            // line breaks of paragraphs reformatted later for paint or hits.
            EngineDocument preview(fonts);
            require(preview.open(
                    QByteArray("<FictionBook xmlns='http://www.gribuser.ru/xml/fictionbook/2.0'>"
                               "<description><title-info><lang>") + previewLanguage + "</lang></title-info></description>"
                               "<body><section><p>Appearance sample text.</p></section></body></FictionBook>",
                    QSize(800, 600), configured, &error), qPrintable(error));
            drift = ReaderSourceIndexProbe::paragraphFormatDrift(replay);
            require(drift.isEmpty(), qPrintable(QString("FB2 geometry changed before repaint after English preview:\n%1")
                    .arg(drift.join('\n'))));
            require(!replay.image(replay.page()).isNull(), "Reported FB2 repaint failed");
            drift = ReaderSourceIndexProbe::paragraphFormatDrift(replay);
            require(drift.isEmpty(), qPrintable(QString("FB2 pagination/paint mismatch after English preview (spacing=%1):\n%2")
                    .arg(normalize).arg(drift.join('\n'))));
        }
    }
    }
    EngineDocument book(test::readerFonts());
    const QJsonObject saved{ { "font_pixels", 48 }, { "line_spacing", 130 },
                             { "normalize_page_margins", true },
                             { "reduce_chapter_spacing", true } };
    require(book.open(bytes, QSize(1180, 1490), saved, &error), qPrintable(error));
    previewHeading(book, QString::fromUtf8("На другое утро"));
    const auto chapters = book.chapters();
    int checked = 0;
    const QRegularExpression numbered(QStringLiteral("^[0-9]+[.]"));
    for (const auto &chapter : chapters) {
        if (chapter.page < 0 || !numbered.match(chapter.title).hasMatch())
            continue;
        book.go(chapter.page);
        require(book.chapterProgress().page == 1,
                qPrintable(QString("Reported FB2 chapter start is not page one: %1 -> %2")
                                   .arg(chapter.title).arg(book.chapterProgress().page)));
        ++checked;
    }
    require(checked > 0, "No numbered chapters tested in the reported FB2");
    // Reproduce the screenshot's ordinary paragraphs, without storing a second
    // copy of the user's prose in this test. The source has no publisher CSS.
    const auto target = bytes.indexOf(QString::fromUtf8("Молчать дальше было").toUtf8());
    require(target > 0, "Reported FB2 lost the screenshot passage");
    const int previous = bytes.lastIndexOf("<p>", bytes.lastIndexOf("<p>", target) - 1);
    const int end = bytes.indexOf("</p>", target);
    auto passage = bytes.mid(previous, end + 4 - previous);
    passage.replace("<p>", "<p id='previous'>");
    const int last = passage.lastIndexOf("id='previous'");
    passage.replace(last, int(sizeof("id='previous'") - 1), "id='following'");
    const auto excerpt = QByteArray("<FictionBook xmlns='http://www.gribuser.ru/xml/fictionbook/2.0'><body><section>")
            + passage + "</section></body></FictionBook>";
    for (int height : { 600, 1000, 1490 }) {
        EngineDocument page(test::readerFonts());
        require(page.open(excerpt, QSize(1180, height), saved, &error), qPrintable(error));
        const auto beforePages = page.pages();
        const auto before = page.image(0);
        require(page.applyAppearance(appearance::Patch({ { "beautifier_spacing", true } })),
                "Reported FB2 spacing toggle failed");
        const auto first = ReaderSourceIndexProbe::nodeBounds(page, "previous");
        const auto following = ReaderSourceIndexProbe::nodeBounds(page, "following");
        require(!first.isEmpty() && !following.isEmpty()
                        && following.top() >= first.bottom(),
                "Reported FB2 spacing normalization overlaps adjacent paragraph boxes");
        require(page.pages() == beforePages && page.image(0) == before,
                "Zero-margin FB2 paragraphs changed layout/paint when removing zero gaps");
    }
    std::puts("PASS: reported FB2 numbered chapter starts and screenshot paragraph geometry");
}

// Exercise the real beta9 Engine API without inventing a chapter/spine for FB2.
void engineBoundary()
{
    setChapterModeOverride(1);
    auto model = createEngine(test::readerFonts());
    QString error;
    const auto bytes = utf8FictionBook();
    require(model->open(bytes, {600, 800}, {{"koblaze_first_spine_item", 2},
                                         {"koblaze_last_spine_item", 3}}, &error), qPrintable(error));
    const auto whole = [&] {
        const auto info = model->pageInfo();
        require(info.bookPages.exact() && info.bookPages.value() == model->pages()
                        && info.bookPage.exact() && info.bookPage.value() == model->page(),
                "FB2 did not expose exact whole-book page values");
        require(!model->continuesBefore() && !model->continuesAfter()
                        && model->roughPages().pages == 0 && model->bookFraction() < 0,
                "FB2 acquired a chapter window or a rough book estimate");
        require(!model->refinePageEstimate()
                        && model->prepareFullDocument([] { return true; }) == Engine::FullDocument::None
                        && !model->fullDocumentReady() && !model->fullDocumentStarted(),
                "Whole-book FB2 scheduled a redundant background document");
        require(model->bookPages({model->position()}).value(0) == model->page() + 1,
                "FB2 bookPages lost its one-based contract");
    };
    whole();
    require(model->go(qMin(1, model->pages() - 1)), "Cannot select FB2 page");
    const auto position = model->position();
    require(model->setChapterMode(true, appearance::Patch({{"font_pixels", 34}}), {800, 600}),
            "FB2 chapter-mode request did not apply whole-book reflow");
    require(model->fontSize() == 34 && model->viewport() == QSize(800, 600)
                    && model->position() == position, "FB2 reflow lost settings or source anchor");
    whole();
    auto preview = createEngine(test::readerFonts());
    require(preview->openPreview(*model, {600, 800}, &error), qPrintable(error));
    const auto previewPosition = preview->position();
    model.reset();
    require(!preview->image(preview->page()).isNull() && preview->goTo(previewPosition),
            "FB2 preview lost its source lifetime at the Engine boundary");
    setChapterModeOverride(-1);
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
    if (argc == 3 && QString::fromLocal8Bit(argv[1]) == "--reported-fb2") {
        reportedFictionBook(QString::fromLocal8Bit(argv[2]));
        return 0;
    }
    formatBoundaries();
    forkStyleFragments();
    languageBinding();
    previewPosition();
    previewReliability();
    semanticPreview();
    inspectFictionBook();
    rejectNonFictionBook();
    renderFictionBook();
    renderWindows1251();
    sharedNormalization();
    engineBoundary();
    std::puts("PASS: FB2 inspection, cp1251 decoding, rendering, contents, cover, footnotes and preview");
}
