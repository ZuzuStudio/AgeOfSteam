#include "include/hexagon.h"

/*Hexagon::Hexagon():radius(0), size(0), height(0), width(0)
{
}

Hexagon::Hexagon(qreal radius):radius(radius), size(radius)
{
    //width = 2 * radius;
    //height = sqrt(3) * radius;
    height = 2 * size;
    width = sqrt(3) * size;
    int dy[6] = {0, (int)(height / 4), (int)(height * 3 / 4), (int)height, (int)(height * 3 / 4), (int)(height / 4)};
    int dx[6] = {(int)(width / 2) , (int) width, (int) width, (int) (width / 2), 0, 0};
    std::copy(dx, dx + 6, corners_dx);
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
    return mi + neigbours_di[mi % 2][neighbourIdx];
}

int Hexagon::getNeighbourJ(int neighbourJdx)
{
    return mj + neigbours_dj[neighbourJdx];
}

int Hexagon::getWidth()
{
    return (int)width;
}

int Hexagon::getHeight()
{
    return (int)height;
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
    my = j * height * 0.75 - j;
    mx = width * (i + (j % 2) * 0.5) - i;
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

*/
