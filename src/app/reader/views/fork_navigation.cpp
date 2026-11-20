#include "ui/controls/rows.h"
#include <QEvent>
#include "fork_navigation.h"
#include "navigation.h"
#include "support/strings.h"
#include "ui/style/fonts.h"
#include "ui/style/styles.h"
#include "ui/controls/text.h"
#include <QPushButton>

namespace kobalt::reader {
namespace chrome = ui::metrics::chrome;

ui::ListHeader *forkui::makeMenuPageStrip(QWidget *parent)
{
    auto *strip = new ui::ListHeader(parent);
    strip->setObjectName("rrMenuPageStrip");
    strip->setShaded(true);
    strip->hide(); // Never expose default geometry before the first menu opening.
    strip->leftLabel()->setObjectName("rrMenuTimeLeft");
    strip->rightLabel()->setObjectName("rrMenuPage");
    for (auto *caption : { strip->leftLabel(), strip->rightLabel() }) {
        auto font = ui::interfaceFont(ui::metrics::panel::sessionText);
        font.setItalic(true);
        caption->setFont(font);
        ui::readerStyle(caption,
                        QString("font-family:'@ui-font-family@';font-size:%1px;"
                                "font-style:italic;font-weight:400;background:transparent;"
                                "border:none;padding:0;color:palette(window-text);")
                                .arg(int(ui::metrics::panel::sessionText)));
    }
    return strip;
}

void forkui::updateMenuPageStrip(ui::ListHeader *strip, const headerfooter::Values &values,
                                 bool shown)
{
    QString time;
    // Prepare before toolbar show; no visibility-dependent estimate gate.
    if (shown && values.bookMilliseconds >= 0) {
        time = headerfooter::content(headerfooter::Field::BookTimeLeft, values, false).text;
        if (!time.isEmpty())
            time[0] = time.at(0).toUpper();
    }
    const auto pages = values.percentTenths >= 0
            ? strings::get("header_footer.calculating")
            : strings::get("navigation.page_count")
                      .arg(values.page)
                      .arg(headerfooter::pageTotal(values.pages, values.pagesEstimated));
    strip->setCaptions(time, shown ? pages : QString());
    strip->rightLabel()->setVisible(shown);
}

bool Navigation::eventFilter(QObject *watched, QEvent *event)
{
    // The strip is a sibling overlay, not a child of the toolbar. Several
    // dismissal paths hide the toolbar without replaying Navigation::update().
    if (watched == toolbar && event->type() == QEvent::Hide && pageStrip)
        pageStrip->hide();
    return QObject::eventFilter(watched, event);
}

bool forkui::menuPageStripVisible(const NavigationState &state, bool countAvailable)
{
    const auto visible = [](const QWidget *panel) { return panel && panel->isVisible(); };
    // Beta.9 builds Settings and Appearance lazily. Checking visibility must
    // neither dereference an absent panel nor create it just for the strip.
    return state.visible && !state.zen && !state.chaptersOpen && !state.searchOpen
            && !visible(state.settings) && !visible(state.appearance) && countAvailable;
}

void Navigation::updateForkPageStrip(QSize size, const NavigationState &state)
{
    if (pageStrip) {
        pageStrip->setGeometry(0, toolbarHeight(), size.width(), pageStrip->sizeHint().height());
        pageStrip->setVisible(forkui::menuPageStripVisible(state, page && !page->text().isEmpty()));
        pageStrip->stackUnder(state.appearance ? state.appearance : toolbar);
    }
}

// Use one shared ink height for all translated labels. Latin H alone is
// shorter than Cyrillic Д (in НАВИГАЦИЯ/РАСКЛАДКА); the private text painter
// clips to this area after centering, otherwise cutting both edges of Д.
QRect forkui::tabInkBounds(int pixels)
{
    QString sample = QStringLiteral("H");
    for (const auto *key : { "navigation.chapters", "navigation.seek", "navigation.text",
                             "navigation.layout", "navigation.light", "navigation.stats" })
        sample += QLatin1Char(' ') + strings::get(key);
    return ui::interfaceInkBounds(sample, ui::interfaceFont(pixels));
}

void Navigation::styleForkToolbarActions()
{
    if (darkTheme) {
        darkThemeStyle = darkTheme->styleSheet();
        ui::readerStyle(darkTheme, "QPushButton{border:none;padding:0;}");
        darkTheme->setFixedSize(chrome::cellWidth, chrome::actionHeight);
        if (auto *button = dynamic_cast<ui::Button *>(darkTheme)) {
            darkThemePaint = button->paintMode();
            button->setPaintMode(ui::ButtonPaintMode::ToolbarCell);
        }
    }
    if (comics) {
        comicsStyle = comics->styleSheet();
        ui::readerStyle(comics, "QPushButton{border:none;padding:0;}");
        comics->setFixedSize(chrome::cellWidth, chrome::actionHeight);
        if (auto *button = dynamic_cast<ui::Button *>(comics)) {
            comicsPaint = button->paintMode();
            button->setPaintMode(ui::ButtonPaintMode::ToolbarCell);
        }
    }
}

void Navigation::restoreForkToolbarActions()
{
    if (darkTheme) {
        darkTheme->setStyleSheet(darkThemeStyle);
        darkTheme->setFixedSize(ui::metrics::control::toolbarButtonSize,
                                ui::metrics::control::toolbarButtonSize);
        if (auto *button = dynamic_cast<ui::Button *>(darkTheme)) {
            button->setPaintMode(darkThemePaint);
        }
    }
    if (comics) {
        comics->setStyleSheet(comicsStyle);
        comics->setFixedSize(ui::metrics::control::toolbarButtonSize,
                             ui::metrics::control::toolbarButtonSize);
        if (auto *button = dynamic_cast<ui::Button *>(comics)) {
            button->setPaintMode(comicsPaint);
        }
    }
}
} // namespace kobalt::reader
