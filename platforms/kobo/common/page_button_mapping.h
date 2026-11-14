#pragma once
#include <Qt>

namespace kobalt::nickel {
// Relative to the portrait button assignments. Only Libra 2 opts in.
inline int libra2ButtonRotation(Qt::ScreenOrientation orientation)
{
    // Preserve 3e810b7 portrait/half-turn behavior; reverse both landscape poses.
    return orientation == Qt::InvertedPortraitOrientation
            || orientation == Qt::LandscapeOrientation ? -1 : 1;
}
inline int qt5PagingDirection(int key, Qt::ScreenOrientation orientation,
                              bool inverted, bool libra2)
{
    if (key == Qt::Key_Left || key == Qt::Key_Up) return -1;
    if (key == Qt::Key_Right || key == Qt::Key_Down) return 1;
    if (key != Qt::Key_F23 && key != Qt::Key_F24) return 0;
    const int rotation = libra2 ? libra2ButtonRotation(orientation)
            : (orientation == Qt::InvertedPortraitOrientation
               || orientation == Qt::InvertedLandscapeOrientation ? -1 : 1);
    return ((key == Qt::Key_F24) != inverted ? 1 : -1) * rotation;
}
} // namespace kobalt::nickel
