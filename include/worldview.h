#ifndef WORLDVIEW_H
#define WORLDVIEW_H

#include <QPoint>
#include <QPointF>
#include <QRectF>
#include <QDebug>

class WorldView final
{
public:
    WorldView(int mapLeft, int mapRight, int mapTop, int mapBottom,
              int screenLeft, int screenRight, int screenTop, int screenBottom,
              qreal scale);
    WorldView(const WorldView &) = default;
    WorldView &operator=(const WorldView &) = default;
    WorldView(WorldView &&) = default;
    WorldView &operator =(WorldView &&) = default;
    ~WorldView() = default;

    void moveScreen(QPoint screenShift);

    void moveScreen(int x, int y)
    {
        moveScreen(QPoint(x, y));
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
    int mapLeft, mapRight, mapTop, mapBottom;
    int screenLeft, screenRight, screenTop, screenBottom;
    QPoint currentCenter; //in map coordinate
    qreal scale;
    qreal minimalScale, maximalScale;
};

#endif // WORLDVIEW_H

