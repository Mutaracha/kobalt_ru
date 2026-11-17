#include "reader_p.h"
#include "ui/controls/icons.h"
#include "platform/api/kobo_refresh.h"
#include "support/qt_compat.h"
#include "support/diagnostics.h"
#include <QScopedValueRollback>

namespace kobalt::reader {
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
