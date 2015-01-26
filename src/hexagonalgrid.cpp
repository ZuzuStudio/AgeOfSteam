#include "include/hexagonalgrid.h"

HexagonalGrid::ArrayGrid::ArrayGrid(unsigned rows, unsigned colomns):rows(rows), colomns(colomns)
{
    array = new int*[rows];
    for(size_t i = 0; i < rows; ++i)
    {
        array[i] = new int[colomns];
    }
}

HexagonalGrid::ArrayGrid::~ArrayGrid()
{
    delete array;
    array = nullptr;
}

/**
 * Now it just creates a simple matrix of 1's, but in nearest time it should also become
 * the base of generation of true map
*/
int ** HexagonalGrid::ArrayGrid::createGrid()
{
    for(size_t i = 0; i < rows; ++i)
    {
        for(size_t j = 0; j < colomns; ++j)
        {
            array[i][j] = 1;
        }
    }
    return array;
}

HexagonalGrid::HexagonalGrid(qreal scale):grid(nullptr), hexagon(nullptr), scale(scale)
{
    hexagon = new Hexagon(scale);
    grid = new ArrayGrid(20, 20);

}

HexagonalGrid::~HexagonalGrid()
{
    delete grid;
    grid = nullptr;
    delete hexagon;
    hexagon = nullptr;
}

void HexagonalGrid::drawRastr(QSvgRenderer * renderer, QPainter *painter)
{
    int cornersX[6];
    int cornersY[6];
    for (size_t j = 0; j < sizeOfClaster; ++j)
    {
        for (size_t i = 0; i < sizeOfClaster; ++i)
        {
            hexagon->setCellIndex(i, j);
            hexagon->computeCorners(cornersX, cornersY);
            qDebug() << "( " << cornersX[5] << " - > " << cornersX[2] << " )";
            renderer->render(painter, QRectF(cornersX[5], cornersY[0], cornersX[2] - cornersX[5], cornersY[4] - cornersY[0]));
        }
    }
}


