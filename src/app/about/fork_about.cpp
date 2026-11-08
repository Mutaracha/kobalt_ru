#include "fork_about.h"
#include "fork_debug.h"
#include "build_info.h"
#include "support/debug_language.h"
#include "support/strings.h"
#include "ui/controls/text.h"
#include "ui/style/metrics.h"
#include "ui/style/styles.h"
#include "ui/style/theme.h"
#include <QBoxLayout>

namespace kobalt::reader {
void appendForkBuildRow(QWidget *page, QBoxLayout *rows, QWidget *host)
{
    if (kobaltForkName[0] == '\0')
        return;
    const auto name = QString::fromLatin1(kobaltForkName);
    const auto stamp = QString::fromLatin1(kobaltBuildTime);
    const auto text = stamp.isEmpty() ? strings::get("about.fork").arg(name)
                                      : strings::get("about.fork_build").arg(name, stamp);
    auto *label = new ui::InterfaceLabel(text, page);
    label->setObjectName("rrAboutFork");
    label->setAlignment(Qt::AlignCenter);
    ui::readerStyle(label, ui::styles::font(ui::metrics::about::commitText));
    rows->addWidget(label);
    installForkLanguageDebug(label, host, QString::fromLatin1(debugLanguageDirectory));
}
} // namespace kobalt::reader
