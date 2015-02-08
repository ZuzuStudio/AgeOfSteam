#ifndef HEXAGONALGRID_H
#define HEXAGONALGRID_H

#endif // HEXAGONALGRID_H

#include "include/arraygrid.h"
#include "include/hexagon.h"
#include <QSvgRenderer>
#include <QImage>
#include <QPainter>
#include <QDebug>

struct Claster{
    QImage image;
    QPoint position;
    unsigned rows;
    unsigned columns;
};

class HexagonalGrid
{

public:

    explicit HexagonalGrid(qreal scale);
    HexagonalGrid(ArrayGrid *grid, QSvgRenderer **rendererArray, qreal scale);

    ~HexagonalGrid();
    std::vector<QImage> drawRastr(QSvgRenderer * renderer);
    void drawSVG(QSvgRenderer * renderer, QPainter * painter);
    void gluingTogetherClasters(QPainter * painter);
    void draw(QPainter * painter);

    void setScale(qreal scale);
    void addShift(int x, int y);
    void addShift(QPoint pos);

private:
    void makeClasters();

    ArrayGrid *grid;
    QSvgRenderer **rendererArray;
    std::vector<Claster> clastersVector;
    Claster **clasters;
    size_t clastersRows;
    size_t clastersColumns;

    QSize image_size;
    Hexagon *hexagon;
    qreal scale;
    int **matrix;
    std::vector<QImage> painters;

    QPoint shift;
    QPoint total_size;

    const qreal scaleFactor = 120.0;
    size_t numberOfClasters = 16;
    const size_t sizeOfClaster = 10; // from now on it is important
                                     // to use only clusters which size is odd integer
};
