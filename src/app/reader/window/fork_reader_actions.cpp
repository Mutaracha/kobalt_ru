#include "app/reader/views/fork_navigation.h"
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


} // namespace kobalt::reader
