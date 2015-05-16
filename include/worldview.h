#ifndef WORLDVIEW_H
#define WORLDVIEW_H

#include <QPointF>
#include <QRectF>
#include <QDebug>
#include "../include/area.h"

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

    void moveScreen(QPointF screenShift);

    void moveScreen(qreal x, qreal y)
    {
        moveScreen(QPointF(x, y));
    }

    void modifyScale(qreal multiplayer);

    const qreal &scale() const
    {
        return scalePrivate;
    }

    QPointF getNW() const
    {
        return transformToMapCordinates(screenArea.nw());
    }

    QPointF getSE() const
    {
        return transformToMapCordinates(screenArea.se());
    }

    QPointF transformToScreenCordinates(const QPointF &point)const;
    QPointF transformToMapCordinates(const QPointF &point)const;
    Area transformToScreenCordinates(const Area &area)const;
    Area transformToMapCordinates(const Area &area)const;
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

