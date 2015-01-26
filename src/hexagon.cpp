#include "include/hexagon.h"

Hexagon::Hexagon():radius(0), size(0), height(0), width(0)
{
}

Hexagon::Hexagon(qreal radius):radius(radius), size(radius * 3 / 2)
{
    width = 2 * radius;
    height = sqrt(3) * radius;
    int dx[6] = {radius / 2, size, width, size, radius / 2, 0};
    std::copy(dx, dx + 6, corners_dx);
    int dy[6] = {0, 0, height / 2, height, height, height / 2};
    std::copy(dy, dy + 6, corners_dy);
}

Hexagon::Hexagon(const Hexagon & other):radius(other.radius), size(other.size), height(other.height), width(other.width)
{
}

Hexagon & Hexagon::operator = (const Hexagon & other)
{
    if(this != &other)
    {
        std::copy(other.corners_dx, other.corners_dx + 6, corners_dx);
        std::copy(other.corners_dy, other.corners_dy + 6, corners_dy);
        radius = other.radius;
        size = other.size;
        height = other.height;
        width = other.width;
    }
    return *this;
}

Hexagon::~Hexagon()
{

}

int Hexagon::getLeft()
{
    return mx;
}

int Hexagon::getTop()
{
    return my;
}

int Hexagon::getCenterX()
{
    return mx + radius;
}

int Hexagon::getCenterY()
{
    return my + height / 2;
}

int Hexagon::getIndexI()
{
    return mi;
}

int Hexagon::getIndexJ()
{
    return mj;
}

int Hexagon::getNeighbourI(int neighbourIdx)
{
    return mi + neigbours_di[neighbourIdx];
}

int Hexagon::getNeighbourJ(int neighbourJdx)
{
    return mj + neigbours_dj[mi % 2][neighbourJdx];
}

void Hexagon::computeCorners(int *cornersX, int *cornersY)
{
    for(int i= 0; i < number_of_neighbours; ++i)
    {
        cornersX[i] = mx + corners_dx[i];
        cornersY[i] = my + corners_dy[i];
    }
}

void Hexagon::setCellIndex(int i, int j)
{
    mi = i;
    mj = j;
    mx = i * size;
    my = height * (2 * j + (i % 2)) / 2;
}

void Hexagon::setCellByPoint(int x, int y)
{
    int ci = (int)floor((double)x / size);
    int cx = x - size * ci;

    int ty = y - (ci % 2) * height / 2;
    int cj = (int)floor((double)ty / height);
    int cy = ty - height * cj;

    if(cx > abs((int)(radius / 2 - radius * cy / height)))
    {
        setCellIndex(ci, cj);
    }
    else
    {
        setCellIndex(ci - 1, cj + (ci % 2) - ((cy < height / 2) ? 1 : 0));
    }

}

