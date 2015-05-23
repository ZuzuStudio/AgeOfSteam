#ifndef WORLDVIEW_H
#define WORLDVIEW_H

#include <QPointF>
#include <QRectF>
#include <QDebug>
#include "include/area.h"
#include "include/transformator.h"

class WorldView final
{
public:
    WorldView(Area mapArea, Area screenArea, qreal scale);
    WorldView(const WorldView &) = default;
    WorldView &operator=(const WorldView &) = default;
    WorldView(WorldView &&) = default;
    WorldView &operator =(WorldView &&) = default;
    ~WorldView() = default;

    void setScreenParameter(Area newScreenArea);

    Area screenParameter() const
    {
        return screenArea;
    }

    void moveScreen(QPointF screenShift);

    void moveScreen(qreal dx, qreal dy)
    {
        moveScreen(QPointF(dx, dy));
    }

    void modifyScale(qreal multiplayer);

    const qreal &scale() const
    {
        return scalePrivate;
    }

    FollowingTransformator *followingTransformator()const
    {
        return new FollowingTransformator(mapViewCenter, screenViewCenter, scalePrivate);
    }

    FixedScaleTransformator *fixedScaleTransformator(qreal scale)const
    {
        return new FixedScaleTransformator(mapViewCenter, screenViewCenter, scale);
    }

private:
    Area mapArea;
    Area screenArea;
    QPointF mapViewCenter, screenViewCenter;
    qreal scalePrivate;
    qreal minimalScale, maximalScale;

    void applyNewScreenParameter();
    void restoreCorrectness();

    friend QDebug operator<<(QDebug dbg, const WorldView &wv)
    {
        dbg.nospace() << "WV(" << wv.mapViewCenter << "; " << wv.scalePrivate << ")";

        return dbg.space();
    }
};

#endif // WORLDVIEW_H

