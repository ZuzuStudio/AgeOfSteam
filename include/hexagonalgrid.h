#ifndef HEXAGONALGRID_H
#define HEXAGONALGRID_H

#include <QPointF>
#include <QPoint>
#include <QRectF>

class HexagonalGrid
{
public:
    HexagonalGrid(int cols, int rows, int hexagonBigRadius);
    //TODO GVI

    QPointF cellCenter(int columnIndex, int rowIndex)const;

    QPointF northWestCorner(int columnIndex, int rowIndex)const
    {
        return cellCenter(columnIndex, rowIndex)
               - QPointF(floatHexagonSmallRadius, floatHexagonBigRadius);
    }

    QRectF tilingBox(int columnIndex, int rowIndex)const
    {
        QRectF result(0.0, 0.0,
                      2.0 * floatHexagonSmallRadius,
                      2.0 * floatHexagonBigRadius);
        result.moveCenter(cellCenter(columnIndex, rowIndex));
        return result;
    }

    QPoint indices(QPointF point, QPointF directionToView)const;

    int leftMapBorder()const
    {
        return left;
    }

    int rightMapBorder()const
    {
        return right;
    }

    int topMapBorder()const
    {
        return top;
    }

    int bottomMapBorder()const
    {
        return bottom;
    }

private:
    int cols, rows;
    int hexagonBigRadius;
    qreal floatHexagonBigRadius, floatHexagonSmallRadius;
    qreal horizontalStep, verticalStep, oddShift;
    int left, right, top, bottom;
};

#endif // HEXAGONALGRID_H
