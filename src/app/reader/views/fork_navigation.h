#pragma once
#include <QRect>
#include "reading/header_footer.h"
class QWidget;
namespace kobalt::ui { class ListHeader; }

namespace kobalt { struct NavigationState; }

namespace kobalt::reader::forkui {
ui::ListHeader *makeMenuPageStrip(QWidget *parent);
bool menuPageStripVisible(const NavigationState &state, bool countAvailable);
void updateMenuPageStrip(ui::ListHeader *strip, const headerfooter::Values &values, bool shown);
QRect tabInkBounds(int pixels);
} // namespace kobalt::reader::forkui
