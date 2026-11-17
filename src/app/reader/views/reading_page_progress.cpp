#include "reading_page.h"
#include <QPainter>

namespace kobalt {
namespace metrics = ui::metrics;

void ReadingPage::paintProgress(QPainter &painter, int baseline, bool down) const
{
    painter.save();
    // Mirror the entire bottom-style bar, including fill and chapter ticks.
    // The baseline retains its thickness; ticks point into the page on both sides.
    if (down) {
        painter.translate(0, 2 * baseline + int(metrics::control::border));
        painter.scale(1, -1);
    }
    {
        const auto ink = palette().color(QPalette::WindowText);
        painter.fillRect(QRect(0, baseline, width(), metrics::control::border), ink);
        painter.fillRect(QRect(0, baseline - metrics::reading::progressFillHeight,
                               qRound(width() * progressFraction),
                               metrics::reading::progressFillHeight),
                         ink);
        // Extend above the fill so already-read chapter marks stay visible.
        // Same scale as the fill. Collapse boundaries landing on one pixel.
        int lastX = -1;
        for (const auto fraction : chapterFractions) {
            const int x = qBound(0, qRound(width() * fraction), qMax(0, width() - 1));
            if (x == lastX)
                continue;
            lastX = x;
            painter.fillRect(
                    QRect(x, baseline - 2 * metrics::reading::progressFillHeight,
                          metrics::control::border,
                          2 * metrics::reading::progressFillHeight + metrics::control::border),
                    ink);
        }
    }
    painter.restore();
}

} // namespace kobalt
