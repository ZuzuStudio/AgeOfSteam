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

    QPointF getNW() const;
    QPointF getNE() const;
    QPointF getSW() const;
    QPointF getSE() const;

    QPointF transformToScreenCordinates(const QPointF &point)const;
    QPointF transformToMapCordinates(const QPointF &point)const;
    QRectF transformToScreenCordinates(QRectF rect)const; //by value semantic is essential
    QRectF transformToMapCordinates(QRectF rect)const; //by value semantic is essential
private:
    qreal mapLeft, mapRight, mapTop, mapBottom;
    qreal screenLeft, screenRight, screenTop, screenBottom;
    QPointF currentCenter; //in map coordinates
    qreal scale;
    qreal minimalScale, maximalScale;
};

#endif // WORLDVIEW_H

