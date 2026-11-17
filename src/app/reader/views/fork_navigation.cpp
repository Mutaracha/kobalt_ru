#include "fork_navigation.h"
#include "navigation.h"
#include "support/strings.h"
#include "ui/style/fonts.h"
#include "ui/style/styles.h"
#include "ui/controls/text.h"
#include <QPushButton>

namespace kobalt::reader {
namespace chrome = ui::metrics::chrome;

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
