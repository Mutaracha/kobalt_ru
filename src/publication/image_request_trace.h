#pragma once
#include <QStringList>

namespace kobalt {
// Worker-only requests for one rendered screen, in first-request order.
class ImageRequestTrace
{
public:
    void begin()
    {
        names_.clear();
        active_ = true;
    }

    QStringList end()
    {
        active_ = false;
        return names_;
    }

    void record(const QString &name)
    {
        if (active_ && !names_.contains(name))
            names_.append(name);
    }

    void clear()
    {
        names_.clear();
        active_ = false;
    }

private:
    bool active_ = false;
    QStringList names_;
};
} // namespace kobalt
