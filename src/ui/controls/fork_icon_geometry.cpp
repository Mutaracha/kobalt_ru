#include "fork_icon_geometry.h"
#include <QPainterPath>

namespace kobalt::ui::forkicons {
void comicsGeometry(QPainterPath &path)
{
    // A flattened oval and swept, curved tail follow the user's reference.
    path.moveTo(112, 184);
    path.cubicTo(61, 184, 24, 157, 24, 118);
    path.cubicTo(24, 79, 70, 48, 128, 48);
    path.cubicTo(186, 48, 232, 79, 232, 118);
    path.cubicTo(232, 153, 201, 178, 158, 184);
    path.cubicTo(142, 208, 108, 222, 70, 228);
    path.cubicTo(96, 213, 112, 200, 112, 184);
    path.closeSubpath();
}

// Microsoft Fluent UI "ic_fluent_dark_theme_24_regular" (MIT), re-scaled from
// its 24 unit viewBox to the shared 256 unit toolbar canvas. The two subpaths
// wind in opposite directions, so the overlapping right half stays hollow
// under either fill rule and the glyph reads as a half-filled contrast circle.
void darkThemeGeometry(QPainterPath &path)
{
    path.moveTo(128, 234.6667);
    path.cubicTo(186.9104, 234.6667, 234.6667, 186.9104, 234.6667, 128);
    path.cubicTo(234.6667, 69.0896, 186.9104, 21.3333, 128, 21.3333);
    path.cubicTo(69.0896, 21.3333, 21.3333, 69.0896, 21.3333, 128);
    path.cubicTo(21.3333, 186.9104, 69.0896, 234.6667, 128, 234.6667);
    path.closeSubpath();
    path.moveTo(128, 218.6667);
    path.lineTo(128, 37.3333);
    path.cubicTo(178.0738, 37.3333, 218.6667, 77.9262, 218.6667, 128);
    path.cubicTo(218.6667, 178.0738, 178.0738, 218.6667, 128, 218.6667);
    path.closeSubpath();
}
} // namespace kobalt::ui::forkicons
