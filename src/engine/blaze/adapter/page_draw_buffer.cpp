#include "page_draw_buffer.h"
#include "render/page_refresh_regions.h"
#include <vector>

namespace kobalt {
namespace {
bool equationImage(const LVImageSourceRef &image)
{
    for (auto *node = image->GetSourceNode(); node; node = node->getParentNode()) {
        if (!node->isElement())
            continue;
        if (!node->getAttributeValue("data-tex").empty()
            || node->getAttributeValue("role") == U"math")
            return true;
    }
    return false;
}

// Transform neutral ink before alpha compositing, so transparent pixels keep
// the actual page background. Delegate vector sizing to avoid an extra raster scale.
class EquationImage : public LVImageSource, private LVImageDecoderCallback
{
public:
    explicit EquationImage(LVImageSourceRef source) : source(source) { }

    ldomDocument *GetSourceDocument() override { return source->GetSourceDocument(); }

    ldomNode *GetSourceNode() override { return source->GetSourceNode(); }

    LVStream *GetSourceStream() override { return source->GetSourceStream(); }

    void Compact() override { source->Compact(); }

    int GetWidth() const override { return source->GetWidth(); }

    int GetHeight() const override { return source->GetHeight(); }

    bool IsScalable() const override { return source->IsScalable(); }

    bool Decode(LVImageDecoderCallback *target) override
    {
        if (!target)
            return source->Decode(nullptr);
        callback = target;
        int width = GetWidth(), height = GetHeight();
        if (IsScalable())
            target->GetTargetSize(width, height);
        row.resize(width);
        return source->Decode(this);
    }

private:
    bool GetTargetSize(int &width, int &height) const override
    {
        return callback->GetTargetSize(width, height);
    }

    void OnStartDecode(LVImageSource *) override { callback->OnStartDecode(this); }

    bool OnLineDecoded(LVImageSource *, int y, lUInt32 *pixels) override
    {
        for (size_t x = 0; x < row.size(); ++x) {
            const auto pixel = pixels[x];
            const bool neutral = qRed(pixel) == qGreen(pixel) && qRed(pixel) == qBlue(pixel);
            row[x] = neutral ? pixel ^ 0x00ffffffU : pixel;
        }
        return callback->OnLineDecoded(this, y, row.data());
    }

    void OnEndDecode(LVImageSource *, bool errors) override { callback->OnEndDecode(this, errors); }

    LVImageSourceRef source;
    LVImageDecoderCallback *callback = nullptr;
    std::vector<lUInt32> row;
};

// A white border and mostly white paper distinguish illustrations from dark artwork.
// Check every visible source pixel so even a small colour accent prevents inversion.
class IllustrationProbe : public LVImageDecoderCallback
{
public:
    IllustrationProbe(int width, int height) : width(width), height(height) { }

    bool eligible(LVImageSourceRef source)
    {
        return source->Decode(this) && !failed && lines == height && neutral && total
                && white * 2 >= total && border && whiteBorder * 10 >= border * 8;
    }

private:
    bool GetTargetSize(int &w, int &h) const override
    {
        w = width;
        h = height;
        return true;
    }

    void OnStartDecode(LVImageSource *) override { }

    bool OnLineDecoded(LVImageSource *, int y, lUInt32 *pixels) override
    {
        ++lines;
        for (int x = 0; x < width; ++x) {
            const auto pixel = pixels[x];
            // CREngine uses an inverted alpha channel. Invisible RGB is irrelevant.
            const int transparency = pixel >> 24;
            if (transparency != 255
                && (qRed(pixel) != qGreen(pixel) || qRed(pixel) != qBlue(pixel)))
                neutral = false;
            const bool paper = transparency == 255
                    || (qRed(pixel) >= 245 && qGreen(pixel) >= 245 && qBlue(pixel) >= 245);
            ++total;
            white += paper;
            if (x == 0 || y == 0 || x == width - 1 || y == height - 1) {
                ++border;
                whiteBorder += paper;
            }
        }
        return true;
    }

    void OnEndDecode(LVImageSource *, bool errors) override { failed = errors; }

    int width, height, lines = 0;
    quint64 total = 0, white = 0, border = 0, whiteBorder = 0;
    bool neutral = true, failed = false;
};

} // namespace

PageDrawBuffer::PageDrawBuffer(QImage &image, bool dark, bool smooth, bool normalizeTextColors,
                               QMap<std::tuple<quintptr, int, int>, bool> *inversion)
    : LVColorDrawBuf(image.width(), image.height(), image.bits(), 32),
      pageSize(image.size()),
      dark(dark),
      normalizeTextColors(normalizeTextColors),
      inversion(inversion)
{
    setInvertColors(dark);
    setSmoothScalingImages(smooth);
}

void PageDrawBuffer::SetTextColor(lUInt32 color)
{
    LVColorDrawBuf::SetTextColor(normalizeTextColors ? 0 : color);
}

void PageDrawBuffer::Draw(int x, int y, const lUInt8 *bitmap, int width, int height,
                          const lUInt32 *palette)
{
    lvRect clip;
    GetClipRect(&clip);
    if (width > 0 && height > 0 && qint64(x) + width > clip.left && x < clip.right
        && qint64(y) + height > clip.top && y < clip.bottom) {
        hasText = true;
        hasUnsupportedFormatting |= unsupportedSource;
    }
    const auto color = palette ? palette[0] : GetTextColor();
    const auto background = GetBackgroundColor();
    const bool neutral = qRed(color) == qGreen(color) && qRed(color) == qBlue(color);
    const bool coloredPaper =
            qRed(background) != qGreen(background) || qRed(background) != qBlue(background);
    const bool preserveContrast = coloredPaper && (background >> 24) != 255;
    // CREngine pre-inverts every non-gray text color. Undo that for faint
    // tints on neutral paper so near-black text becomes light in dark mode.
    // On preserved colored paper, keep the publisher's original contrast.
    const bool invertInk =
            dark && (preserveContrast ? neutral : !neutral && !hasVisibleColor(color));
    const lUInt32 ink = normalizeTextColors ? 0 : invertInk ? color ^ 0x00ffffffU : color;
    LVColorDrawBuf::Draw(x, y, bitmap, width, height, &ink);
}

void PageDrawBuffer::Draw(LVImageSourceRef image, int x, int y, int width, int height, bool dither)
{
    hasImages = true;
    lvRect clip;
    GetClipRect(&clip);
    const qint64 left = qMax<qint64>(0, qMax<qint64>(x, clip.left));
    const qint64 top = qMax<qint64>(0, qMax<qint64>(y, clip.top));
    const qint64 right =
            qMin<qint64>(pageSize.width(), qMin<qint64>(qint64(x) + width, clip.right));
    const qint64 bottom =
            qMin<qint64>(pageSize.height(), qMin<qint64>(qint64(y) + height, clip.bottom));
    auto *node = image->GetSourceNode();
    const auto style = node ? node->getStyle() : css_style_ref_t();
    const bool unsupportedImage =
            !style.isNull() && style->writing_mode_warning == css_wmw_unsupported;
    if (!imageRegionsFull && right > left && bottom > top) {
        if (imageRegions.size() < 128)
            imageRegions.append(QRect(int(left), int(top), int(right - left), int(bottom - top)));
        else {
            imageRegions.clear();
            imageRegions.append(QRect(QPoint(0, 0), pageSize));
            imageRegionsFull = true;
        }
    }
    bool invertIllustration = false;
    if (dark && inversion && !equationImage(image)) {
        const auto node = reinterpret_cast<quintptr>(image->GetSourceNode());
        // Vector details can appear at a larger size. Do not reuse a smaller raster verdict.
        const auto key = std::make_tuple(node, image->IsScalable() ? width : 0,
                                         image->IsScalable() ? height : 0);
        auto found = inversion->constFind(key);
        if (node && found != inversion->constEnd()) {
            invertIllustration = found.value();
        } else {
            IllustrationProbe probe(image->IsScalable() ? width : image->GetWidth(),
                                    image->IsScalable() ? height : image->GetHeight());
            invertIllustration = probe.eligible(image);
            if (inversion->size() >= 1024)
                inversion->clear();
            if (node)
                inversion->insert(key, invertIllustration);
        }
    }
    if (dark && (equationImage(image) || invertIllustration))
        image = LVImageSourceRef(new EquationImage(image));
    // Composite images in the final page colors. CREngine's invert-images
    // option instead blends transparency against a white background.
    if (dark)
        InvertRect(x, y, x + width, y + height);
    LVColorDrawBuf::Draw(image, x, y, width, height, dither);
    hasUnsupportedFormatting |= unsupportedImage && imageHasVisiblePixels;
    if (dark)
        InvertRect(x, y, x + width, y + height);
}
} // namespace kobalt
