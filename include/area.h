#ifndef AREA_H
#define AREA_H

#include <QPointF>
#include <QRectF>
#include <QSizeF>

// it can be stuct, but for possible future changes
class Area
{
public:
    Area(QPointF nw, QPointF se):
        nwPrivate(nw),
        sePrivate(se)
    {}

    Area(qreal left, qreal top, qreal right, qreal bottom):
        Area(QPointF(left, top), QPointF(right, bottom))
    {}

    Area(QPointF nw, QSizeF size):
        Area(nw, QPointF(nw.x() + size.width(), nw.y() + size.height()))
    {}

    Area(qreal nwX, qreal nwY, QSizeF size):
        Area(QPointF(nwX, nwY), size)
    {}

    Area(QRectF rect):
        Area(rect.topLeft(), rect.bottomLeft())
    {}

    Area(const Area &) = default;
    Area &operator=(const Area &) = default;
    Area(Area &&) = default;
    Area &operator=(Area &&) = default;
    ~Area() = default;

    const QPointF &nw()const
    {
        return nwPrivate;
    }

    QPointF &nw()
    {
        return nwPrivate;
    }

    const QPointF &se()const
    {
        return sePrivate;
    }

    QPointF &se()
    {
        return sePrivate;
    }

private:
    QPointF nwPrivate, sePrivate;
};

#endif // AREA_H
