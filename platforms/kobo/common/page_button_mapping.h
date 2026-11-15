#pragma once
#include <Qt>
namespace kobalt::nickel {
inline bool pageButtonsRotated(Qt::ScreenOrientation orientation, bool libra2)
{
    // Libra 2 portrait is unchanged; both landscape poses need opposite polarity.
    return orientation == Qt::InvertedPortraitOrientation
            || orientation == (libra2 ? Qt::LandscapeOrientation : Qt::InvertedLandscapeOrientation);
}
}
