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
        unsigned rows, colomns;
        const int number_of_hex_corners = 6;
        const int cell_radius = 40;

    public:
        ArrayGrid(unsigned rows, unsigned colomns);
        ~ArrayGrid();
        int ** createGrid();

    }* grid;

    QSize image_size;
    Hexagon *hexagon;
    qreal scale;
    int **matrix;
    std::vector<QImage> painters;

    QPoint shift;

    const qreal scaleFactor = 120.0;\
    const size_t numberOfClasters = 25;
    const size_t sizeOfClaster = 10;
};