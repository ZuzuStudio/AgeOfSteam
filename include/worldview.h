#ifndef WORLDVIEW_H
#define WORLDVIEW_H

#include <QPointF>
#include <QRectF>
#include <QDebug>

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
        return transformToMapCordinates(screenNW);
    }

    QPointF getSE() const
    {
        return transformToMapCordinates(screenSE);
    }

    QPointF transformToScreenCordinates(const QPointF &point)const;
    QPointF transformToMapCordinates(const QPointF &point)const;
    QRectF transformToScreenCordinates(QRectF rect, qreal adjust = 1.0)const; //by value semantic is essential
    QRectF transformToMapCordinates(QRectF rect)const; //by value semantic is essential
private:
    QPointF mapNW, mapSE;
    QPointF screenNW, screenSE;
    QPointF mapViewCenter, screenViewCenter; //in map coordinates
    qreal scale;
    qreal minimalScale, maximalScale;

    void restoreCorrectness();
};

#endif // WORLDVIEW_H

