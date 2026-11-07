#pragma once
#include "ui/controls/text.h"
#include "ui/style/fonts.h"
#include "ui/style/private_text.h"
#include "ui/style/metrics.h"
#include <QPainter>
#include <QVector>
#include <QPair>

namespace kobalt::ui {
// Shared measurement/painting of an interface caption with a serif brand run.
inline QVector<QPair<QString, QFont>> returnTitleRuns(const InterfaceLabel &label) {
    QVector<QPair<QString, QFont>> runs;
    const int at = label.text().indexOf(QStringLiteral("Kobalt"), 0, Qt::CaseInsensitive);
    if (at < 0) {
        runs.append({label.text(), label.font()});
    } else {
        if (at) runs.append({label.text().left(at), label.font()});
        runs.append({label.text().mid(at, 6), brandFont(label.font().pixelSize())});
        if (at + 6 < label.text().size()) runs.append({label.text().mid(at + 6), label.font()});
    }
    return runs;
}

inline QSize returnTitleSizeHint(const InterfaceLabel &label) {
    label.ensurePolished();
    int width = 0, height = 0;
    for (const auto &run : returnTitleRuns(label)) {
        width += interfaceTextWidth(run.first, run.second);
        height = qMax(height, interfaceLineHeight(run.second));
    }
    const auto margins = label.contentsMargins();
    return {width + margins.left() + margins.right() + 2 * label.margin(),
            height + margins.top() + margins.bottom() + 2 * label.margin()};
}

inline void paintReturnTitle(QPainter &painter, const QRect &bounds, const InterfaceLabel &label) {
    // Preserve the brand word, but draw the surrounding interface text
    // in Sans. Measure and paint the same runs on a common baseline.
    const auto runs = returnTitleRuns(label);
    QRect ink;
    int advance = 0;
    for (const auto &run : runs) {
        ink |= interfaceInkBounds(run.first, run.second).translated(advance, 0);
        advance += interfaceTextWidth(run.first, run.second);
    }
    const int x = bounds.center().x() - ink.center().x();
    const int baseline = bounds.center().y() - ink.center().y() + int(metrics::chrome::returnTitleOffset);
    painter.setClipRect(bounds);
    advance = 0;
    for (const auto &run : runs) {
        painter.setFont(run.second);
        if (needsPrivateText(run.first, run.second)) {
            auto rendered = renderPrivateText(run.first, run.second);
            if (!rendered.image.isNull()) {
                QPainter tint(&rendered.image);
                tint.setCompositionMode(QPainter::CompositionMode_SourceIn);
                tint.fillRect(rendered.image.rect(), painter.pen().color());
                tint.end();
                painter.drawImage(QPoint(x + advance, baseline) + rendered.origin, rendered.image);
            }
        } else {
            painter.drawText(QPoint(x + advance, baseline), run.first);
        }
        advance += interfaceTextWidth(run.first, run.second);
    }
}
} // namespace kobalt::ui
