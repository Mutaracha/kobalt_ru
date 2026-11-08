#pragma once
class QWidget;
class QBoxLayout;

namespace kobalt::ui {
class DescribedOption;
}

namespace kobalt::reader {
// Shared row contract, including rows added by later upstream settings.
void styleSettingsOption(ui::DescribedOption *option);
} // namespace kobalt::reader
