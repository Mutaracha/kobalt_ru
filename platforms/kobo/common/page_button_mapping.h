#pragma once
#include <Qt>

namespace kobalt::nickel {
// Qt5 fallback only. Libra 2 reports the opposite F23/F24 pair to the
// firmware-4.46 baseline. Keep model polarity separate from user inversion
// and the half-turn compensation; arrow keys already express directions.
inline int qt5PageButtonDirection(int key, Qt::ScreenOrientation orientation,
                                 bool inverted, bool libra2)
{
    if (key == Qt::Key_Left || key == Qt::Key_Up) return -1;
    if (key == Qt::Key_Right || key == Qt::Key_Down) return 1;
    if (key != Qt::Key_F23 && key != Qt::Key_F24) return 0;
    if (libra2) inverted = !inverted;
    if (orientation == Qt::InvertedPortraitOrientation
        || orientation == Qt::InvertedLandscapeOrientation)
        inverted = !inverted;
    return (key == Qt::Key_F24) != inverted ? 1 : -1;
}
} // namespace kobalt::nickel
