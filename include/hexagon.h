#ifndef HEXAGON_H
#define HEXAGON_H

#endif // HEXAGON_H

#include <QPoint>

class Hexagon
{

public :
    Hexagon();
    explicit Hexagon(qreal size, QPoint center);
    Hexagon(const Hexagon & other);
    Hexagon(Hexagon && other) = default;

    Hexagon & operator = (const Hexagon & other);
    Hexagon & operator = (Hexagon && other) = default;

    ~Hexagon();

    qreal getWidth();
    qreal getHeight();
    QPoint getCenter();
    qreal getSize();
    QPoint* getCoordinates();

    enum corner {TOP = 0, TOPRIGHT = 1, BOTTOMRIGHT = 2, BOTTOM = 3, BOTTOMLEFT = 4, TOPLEFT = 5};

private:
    QPoint draw_points[6]; // coordinates for each corner

    //--- the fact is the hexagon may be defined by 2 values : a side size and the center point
    QPoint center;
    qreal size;

    //--- another two values : width and height are used in making grid proccess
    qreal height;
    qreal width;

};
