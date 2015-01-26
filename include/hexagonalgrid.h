#ifndef HEXAGONALGRID_H
#define HEXAGONALGRID_H

#endif // HEXAGONALGRID_H

#include "include/hexagon.h"
#include <QSvgRenderer>
#include <QImage>
#include <QPainter>
#include <QDebug>

class HexagonalGrid
{

public:

    explicit HexagonalGrid(qreal scale);
    ~HexagonalGrid();
    void drawRastr(QSvgRenderer * renderer, QPainter * painter);
    QImage drawSVG(QSvgRenderer, QPainter * painter);

private:

    class ArrayGrid
    {
    private:
        int **array;
        unsigned rows, colomns;
        const int number_of_hex_corners = 6;
        const int cell_radius = 40;

    public:
        ArrayGrid(unsigned rows, unsigned colomns);
        ~ArrayGrid();
        int ** createGrid();

    }* grid;

    Hexagon *hexagon;
    qreal scale;
    std::vector<QImage> painters;
    int **matrix;

    const qreal scaleFactor = 120.0;
    const int numberOfClasters = 5;
    const size_t sizeOfClaster = 10;
};
