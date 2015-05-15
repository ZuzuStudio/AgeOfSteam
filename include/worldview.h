#ifndef WORLDVIEW_H
#define WORLDVIEW_H

#include <QPointF>
#include <QRectF>
#include <QDebug>
#include "../include/area.h"

class WorldView final
{
public:
    WorldView(qreal mapLeft, qreal mapRight, qreal mapTop, qreal mapBottom,
              qreal screenLeft, qreal screenRight, qreal screenTop, qreal screenBottom,
              qreal scale);
    WorldView(const WorldView &) = default;
    WorldView &operator=(const WorldView &) = default;
    WorldView(WorldView &&) = default;
    WorldView &operator =(WorldView &&) = default;
    ~WorldView() = default;

    void setScreenParameter(qreal screenLeft, qreal screenRight, qreal screenTop, qreal screenBottom);

    void moveScreen(QPointF screenShift);

    void moveScreen(qreal x, qreal y)
    {
        moveScreen(QPointF(x, y));
    }

    void decreaseScale();
    void increaseScale();

    const qreal &getScale() const
    {
        return scale;
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
    QRectF transformToScreenCordinates(QRectF rect, qreal adjust = 1.0)const; //by value semantic is essential
    QRectF transformToMapCordinates(QRectF rect)const; //by value semantic is essential
private:
    Area mapArea;
    //QPointF mapNW, mapSE;
    Area screenArea;
    //QPointF screenNW, screenSE;
    QPointF mapViewCenter, screenViewCenter;
    qreal scale;
    qreal minimalScale, maximalScale;

    void restoreCorrectness();

    friend QDebug operator<<(QDebug dbg, const WorldView &wv)
    {
        dbg.nospace() << "WV(" << wv.mapViewCenter << "; " << wv.scale << ")";

        return dbg.space();
    }
};

#endif // WORLDVIEW_H

