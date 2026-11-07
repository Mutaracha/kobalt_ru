#pragma once
class QWidget;
class QBoxLayout;
namespace kobalt::ui { class DescribedOption; }
namespace kobalt::reader {
struct SettingsPanelInputs;
// Shared row contract, including rows added by later upstream settings.
void styleSettingsOption(ui::DescribedOption *option);
// Only appends the fork label; stock About labels/rendering are untouched.
void appendForkBuildRow(const SettingsPanelInputs &inputs, QWidget *page, QBoxLayout *rows, QWidget *host);
}
