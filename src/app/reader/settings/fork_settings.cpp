#include "fork_settings.h"
#include "fork_debug.h"
#include "settings_panel.h"
#include "ui/controls/options.h"
#include "ui/controls/text.h"
#include "ui/style/metrics.h"
#include "ui/style/styles.h"
#include "support/strings.h"
#include <QBoxLayout>
namespace kobalt::reader {
void styleSettingsOption(ui::DescribedOption *option) {
    namespace c = ui::metrics::chrome;
    auto presentation = option->presentation();
    presentation.indicator = ui::DescribedOption::Indicator::Square;
    presentation.pressedFill = true;
    presentation.dividers = Qt::BottomEdge;
    presentation.horizontalInset = c::inset;
    presentation.labelPixels = c::labelText;
    presentation.descriptionPixels = c::descriptionText;
    presentation.indicatorSize = {c::checkSize, c::checkSize};
    presentation.minimumRowHeight = c::optionHeight;
    option->setPresentation(presentation);
}
void appendForkBuildRow(const SettingsPanelInputs &inputs, QWidget *page, QBoxLayout *rows, QWidget *host) {
    if (!inputs.forkVisible) return;
    const QString text = inputs.forkBuildTime.isEmpty()
        ? strings::get("about.fork").arg(inputs.forkName)
        : strings::get("about.fork_build").arg(inputs.forkName, inputs.forkBuildTime);
    auto *label = new ui::InterfaceLabel(text, page);
    label->setObjectName("rrAboutFork");
    label->setAlignment(Qt::AlignCenter);
    ui::readerStyle(label, ui::styles::font(ui::metrics::about::commitText));
    rows->addWidget(label);
    installForkLanguageDebug(label, host, inputs.debugLanguageDirectory);
}
}
