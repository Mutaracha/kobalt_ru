#include "ui/controls/rows.h"
#include "reader_p.h"
#include "support/strings.h"
#include "ui/controls/icons.h"
#include "platform/api/kobo_refresh.h"
#include "support/qt_compat.h"
#include "support/diagnostics.h"
#include <QScopedValueRollback>

namespace kobalt::reader {
void Reader::styleForkToolbarActions()
{
    toolbarComposition.darkTheme->setCheckable(true);
    static_cast<kobalt::ui::Button *>(toolbarComposition.darkTheme)->setHighlightChecked(false);
    toolbarComposition.comics->setCheckable(true);
    auto *comicsButton = static_cast<ui::Button *>(toolbarComposition.comics);
    comicsButton->setHighlightChecked(false);
    ui::ButtonIcon comicsIcon{ ui::ControlIcon::Comics, ui::IconRendition::Action };
    comicsIcon.stateInkOnly = true;
    comicsButton->setIconConfiguration(comicsIcon);
}

void Reader::refreshForkToolbarActions(bool settingsOpen, bool zen)
{
    if (toolbarComposition.darkTheme) {
        toolbarComposition.darkTheme->setChecked(session->state().value("dark_mode").toBool(false));
        toolbarComposition.darkTheme->setVisible(
                !settingsOpen && !zen && features::enabled(preferences, features::DarkMode));
    }
    if (toolbarComposition.comics) {
        auto *source = appearanceEdit.edited() && appearanceEdit.ready() ? appearanceEdit.preview()
                                                                         : session;
        // Keep mode ink stable while a preview session is being prepared/reflowed.
        if (!appearanceEdit.edited() || appearanceEdit.ready())
            toolbarComposition.comics->setChecked(source->requestedAppearance()
                                                          .fields()
                                                          .value(appearance::comicsModeKey)
                                                          .toBool(false));
        toolbarComposition.comics->setVisible(!settingsOpen && !zen
                                              && features::enabled(preferences, features::Comics));
    }
}

void Reader::updateForkMenuPage()
{
    const bool showPage = !zenMode() && session->pages() > 0;
    static_cast<ui::ListHeader *>(menuPage->parentWidget())->setCaptions({}, showPage ? strings::get("navigation.page_count")
                                         .arg(session->page() + 1)
                                         .arg(session->pages())
                               : QString());
    menuPage->setVisible(showPage);
}

void Reader::toggleComicsMode()
{
    auto *source = appearanceEdit.preview() ? appearanceEdit.preview() : session;
    if (source->busy()) {
        refresh();
        return;
    }
    if (appearanceEdit.panelOpen()) {
        if (auto *option = findChild<QCheckBox *>("rrComicsReadingMode"))
            option->click();
        return;
    }
    const bool enabled = !session->requestedAppearance()
                                  .fields()
                                  .value(kobalt::appearance::comicsModeKey)
                                  .toBool(false);
    QSize viewport;
    {
        QScopedValueRollback<bool> mode(page->comicsMode);
        page->comicsMode = enabled;
        viewport = bodySize();
    }
    session->editAppearance(kobalt::appearance::Patch(kobalt::qt::object(
                                    { { kobalt::appearance::comicsModeKey, enabled } })),
                            viewport);
    refresh();
}

void Reader::ensureAppearance()
{
    if (appearanceBuilt)
        return;
    QElapsedTimer elapsed;
    elapsed.start();
    const auto previousWidgets = findChildren<QWidget *>();
    QSet<QWidget *> previous;
    for (auto *widget : previousWidgets)
        previous.insert(widget);
    buildAppearance(appearanceLayout);
    // Late children need the same palette/refresh policy as children built at
    // startup, including when the user changed theme or interface fonts first.
    const auto colors = ui::readerPalette(palette(), darkTheme);
    for (auto *widget : findChildren<QWidget *>()) {
        if (previous.contains(widget))
            continue;
        ui::applyReaderPalette(widget, colors);
        platform::textRefresh(widget);
        if (auto *button = qobject_cast<QPushButton *>(widget))
            ui::refreshButtonIcon(button);
    }
    appearanceBuilt = true;
    syncChromeControls();
    syncAppearanceControls(false);
    kobalt::diagnostic(
            "deferred-panel-build",
            { { "panel", "appearance" }, { "elapsed_ms", elapsed.nsecsElapsed() / 1e6 } }, false);
}

} // namespace kobalt::reader
