#ifndef HEXAGONALGRID_H
#define HEXAGONALGRID_H

#endif // HEXAGONALGRID_H

#include "include/hexagon.h"
#include <QSvgRenderer>
#include <QImage>
#include <QPainter>

class HexagonalGrid
{

public:
    HexagonalGrid();
    ~HexagonalGrid();
    void drawRastr(QPainter * painter);
    QImage drawSVG(QPainter * painter);

    int **matrix;

private:
    class ArrayGrid
    {
    private:
        int **array;
        int rows, colomns;

    public:
        ArrayGrid(int rows, int colomns);
        int ** createGrid(std::vector<Hexagon> hexagons);
        ~ArrayGrid();
    }* grid;
    Hexagon hexagon;
    QSvgRenderer *renderer;
    QImage *image;
    double scale;
    std::vector<QImage> painters;

    const qreal scaleFactor = 120.0;
    const int numberOfClasters = 5;
    const size_t sizeOfClaster = 10;
};
