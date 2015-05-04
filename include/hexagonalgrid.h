#ifndef HEXAGONALGRID_H
#define HEXAGONALGRID_H

#include "include/hexagon.h"
#include "include/worldview.h"
#include <QSvgRenderer>
#include <QImage>
#include <QPainter>
#include <QDebug>

class HexagonalGrid
{

public:

    HexagonalGrid(qreal scale, QPoint topLeft, QPoint topRight, QPoint bottomRight, QPoint bottomLeft);
    ~HexagonalGrid();
    std::vector<QImage> drawRastr(QSvgRenderer * renderer);
    void drawSVG(QSvgRenderer * renderer, QPainter * painter);
    void gluingTogetherClasters(QPainter * painter);
    void setScale(qreal scale);
    void addShift(int x, int y);
    void addShift(QPoint pos);

private:

    class ArrayGrid
    {
    private:
        int **array;
        size_t rows, colomns;

    public:
        ArrayGrid(size_t rows, size_t colomns);
        ~ArrayGrid();
        int ** createGrid();

    }* grid;

    void calculateScreenCoordinates();

    QSize image_size;
    Hexagon *hexagon;
    qreal scale;
    int **matrix;
    std::vector<QImage> painters;

    QPoint shift;
    QPoint mapLeftTop, mapRightTop, mapRightBottom, mapLeftBottom;
    QPoint screenLeftTop, screenRightTop, screenRightBottom, screenLeftBottom;
    const size_t numberOfClasters = 25;
    const size_t sizeOfClaster = 20;  // from now on it is important
                                     // to use only clusters which size is odd integer
};

#endif // HEXAGONALGRID_H
