#include "include/hexagonalgrid.h"

#include <math.h>
#include <assert.h>

HexagonalGrid::HexagonalGrid(int cols, int rows, int hexagonBigRadius):
    cols(cols),
    rows(rows),
    hexagonBigRadius(hexagonBigRadius),
    floatHexagonBigRadius((qreal)hexagonBigRadius),
    floatHexagonSmallRadius(sqrt(3.0) * floatHexagonBigRadius / 2.0),
    horizontalStep(2.0 * floatHexagonSmallRadius),
    verticalStep(1.5 * floatHexagonBigRadius),
    oddShift(floatHexagonSmallRadius)
{
    assert(cols > 0);
    assert(rows > 0);
    assert(rows & 1);
    assert(hexagonBigRadius > 0);
    auto width = (this->cols - 1) * horizontalStep;
    auto height = (this->rows - 1) * verticalStep;
    int halfWidth = (int)floor(width / 2.0);
    int halfHeight = (int)floor(height / 2.0);
    left = -halfWidth;
    right = halfWidth;
    top = halfHeight;
    bottom = -halfHeight;
}

QPointF HexagonalGrid::cellCenter(int columnIndex, int rowIndex) const
{
    return QPointF(left + columnIndex * horizontalStep + (rowIndex & 1) * oddShift,
                   top - rowIndex * verticalStep);
}

QPoint HexagonalGrid::indices(QPointF point, QPointF directionToView) const
{
    int approximateColumnIndex = (int)((point.x() - left) / horizontalStep);
    int approximateRowIndex = (int)((top - point.y()) / verticalStep);
    auto squareSearchRadius = (1.125 * floatHexagonBigRadius) * (1.125 * floatHexagonBigRadius);
    QPointF candidateRadiusvector[3];
    QPoint candidateIndices[3];
    int candidateCount = 0;

    for(int collumn = approximateColumnIndex - 2; collumn <= approximateColumnIndex + 2; ++collumn)
        for(int row = approximateRowIndex - 2; row <= approximateRowIndex + 2; ++row)
        {
            auto radiusVector = cellCenter(collumn, row) - point;

            if(QPointF::dotProduct(radiusVector, radiusVector) < squareSearchRadius)
            {
                candidateRadiusvector[candidateCount] = radiusVector;
                candidateIndices[candidateCount] = QPoint(collumn, row);
                ++candidateCount;
                assert(candidateCount <= 3);
            }
        }

    assert(candidateCount > 0);

    if(candidateCount == 1)
        return candidateIndices[0];

    int maxIndex = 0;
    qreal maxValue = QPointF::dotProduct(candidateRadiusvector[maxIndex], directionToView);

    for(int i = 1; i < candidateCount; ++i)
    {
        qreal currrentValue = QPointF::dotProduct(candidateRadiusvector[i], directionToView);
        if(currrentValue > maxValue)
        {
            maxValue = currrentValue;
            maxIndex = i;
        }
    }

    return candidateIndices[maxIndex];
}
