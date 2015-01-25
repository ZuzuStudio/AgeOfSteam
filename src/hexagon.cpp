#include "include/hexagon.h"

Hexagon::Hexagon():size(0), height(0), width(0)
{
}

Hexagon::Hexagon(qreal size, QPoint center):center(center), size(size)
{
    height = 2 * size;
    width = sqrt(3) * height / 2;
    for(int i = 0; i < 6; ++i)
    {
        qreal angle = 2 * M_PI / 6 * (i + 0.5);
        draw_points[i] = QPoint(center.rx() + size * cos(angle),
                                center.ry() + size * sin(angle));
    }
}

Hexagon::Hexagon(const Hexagon & other):draw_points(other.draw_points), center(other.center),
                                        size(other.size), height(other.height), width(other.width)
{
}

Hexagon & Hexagon::operator = (const Hexagon & other)
{
    if(this != &other)
    {
        std::copy(other.draw_points, other.draw_points + 6, draw_points);
        center = other.center;
        size = other.size;
        height = other.height;
        width = other.width;
    }
    return *this;
}

Hexagon::~Hexagon()
{

}

qreal Hexagon::getSize()
{
    return size;
}

qreal Hexagon::getHeight()
{
    return height;
}

qreal Hexagon::getWidth()
{
    return width;
}

QPoint Hexagon::getCenter()
{
    return center;
}

QPoint * Hexagon::getCoordinates()
{
    return draw_points;
}
