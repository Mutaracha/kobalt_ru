#include "fork_settings.h"
#include "ui/controls/options.h"
#include "ui/controls/text.h"
#include "ui/style/metrics.h"
#include "ui/style/styles.h"
#include "support/strings.h"
#include <QBoxLayout>

namespace kobalt::reader {
void styleSettingsOption(ui::DescribedOption *option)
{
    namespace c = ui::metrics::chrome;
    auto presentation = option->presentation();
    presentation.indicator = ui::DescribedOption::Indicator::Square;
    presentation.pressedFill = true;
    presentation.dividers = Qt::BottomEdge;
    presentation.horizontalInset = c::inset;
    presentation.labelPixels = c::labelText;
    presentation.descriptionPixels = c::descriptionText;
    presentation.indicatorSize = { c::checkSize, c::checkSize };
    presentation.minimumRowHeight = c::optionHeight;
    option->setPresentation(presentation);
}
} // namespace kobalt::reader
