#ifndef AREA_H
#define AREA_H

#include <QPointF>

// it can be stuct, but for possible future changes
class Area
{
public:
    Area(QPointF nw, QPointF se):
        nwPrivate(nw),
        sePrivate(se)
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
