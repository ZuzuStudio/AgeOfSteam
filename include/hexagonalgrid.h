#ifndef HEXAGONALGRID_H
#define HEXAGONALGRID_H

#include <QPointF>
#include <QPoint>
#include <QRectF>
#include "include/area.h"

class HexagonalGrid
{
public:
    HexagonalGrid(int cols, int rows, int hexagonBigRadius);
    HexagonalGrid(const HexagonalGrid &original) = default;
    HexagonalGrid &operator=(const HexagonalGrid &rhs) = default;
    HexagonalGrid(HexagonalGrid &&victim) = default;
    HexagonalGrid &operator=(HexagonalGrid &&rhs) = default;
    ~HexagonalGrid() = default;

    QPointF cellCenter(int columnIndex, int rowIndex)const;

    QPointF cellNW(int columnIndex, int rowIndex)const
    {
        return cellCenter(columnIndex, rowIndex)
               - QPointF(floatHexagonSmallRadius, -floatHexagonBigRadius);
    }

    QPointF cellSE(int columnIndex, int rowIndex)const
    {
        return cellCenter(columnIndex, rowIndex)
               + QPointF(floatHexagonSmallRadius, -floatHexagonBigRadius);
    }

    Area cellArea(int columnIndex, int rowIndex)const
    {
        return Area(cellNW(columnIndex, rowIndex), cellSE(columnIndex, rowIndex));
    }

    QPoint indices(QPointF point, QPointF directionToView)const;

    Area mapBorder()const
    {
        return Area(left, top, right, bottom);
    }

private:
    int cols, rows;
    int hexagonBigRadius;
    qreal floatHexagonBigRadius, floatHexagonSmallRadius;
    qreal horizontalStep, verticalStep, oddShift;
    int left, right, top, bottom;
};

#endif // HEXAGONALGRID_H
