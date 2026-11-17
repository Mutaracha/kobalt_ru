#pragma once
#include <lvdocview.h>
#include <QImage>
#include <QMap>
#include <QRect>
#include <QVector>
#include <tuple>

namespace kobalt {
// Common upstream drawing primitive, shared by normal pages, covers and comics.
// There is only one implementation; no fork-specific inversion/scaling policy.
class PageDrawBuffer : public LVColorDrawBuf
{
public:
    PageDrawBuffer(QImage &image, bool dark, bool smooth, bool normalizeTextColors,
                   QMap<std::tuple<quintptr, int, int>, bool> *inversion = nullptr);
    void SetTextColor(lUInt32 color) override;
    bool hasImages = false, hasText = false;
    QVector<QRect> imageRegions;
    using LVColorDrawBuf::Draw;
    void Draw(int x, int y, const lUInt8 *bitmap, int width, int height,
              const lUInt32 *palette) override;
    void Draw(LVImageSourceRef image, int x, int y, int width, int height, bool dither) override;

private:
    QSize pageSize;
    bool imageRegionsFull = false;
    bool dark;
    bool normalizeTextColors;
    QMap<std::tuple<quintptr, int, int>, bool> *inversion;
};
} // namespace kobalt
