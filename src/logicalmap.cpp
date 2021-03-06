#include "include/logicalmap.h"
#include <assert.h>

LogicalMap::LogicalMap(TerrainType **array, int rows, int columns):
    array(nullptr), mColumns(columns), mRows(rows)
{
    assert(columns > 0);
    assert(rows > 0);
    allocateMemory();

    for(int column = 0; column < mColumns; ++column)
        for(int row = 0; row < mRows; ++row)
            this->array[column][row] = array[column][row];
}

LogicalMap::LogicalMap(int columns, int rows):
    array(nullptr), mColumns(columns), mRows(rows)
{
    assert(columns > 0);
    assert(rows > 0);
    allocateMemory();

    for(int column = 0; column < mColumns; ++column)
        for(int row = 0; row < mRows; ++row)
            array[column][row] = HILL;

    array[0][0] = SEA;
    array[1][0] = SEA;
    array[1][1] = SEA;
    array[1][2] = SEA;
    array[mColumns / 2 - 1][mRows / 2 - 2] = LAND;
    array[mColumns / 2 + 1][mRows / 2 - 2] = LAND;
    array[mColumns / 2 - 3][mRows / 2 + 1] = LAND;
    array[mColumns / 2 - 2][mRows / 2 + 2] = LAND;
    array[mColumns / 2 - 2][mRows / 2 + 3] = LAND;
    array[mColumns / 2 - 1][mRows / 2 + 3] = LAND;
    array[mColumns / 2][mRows / 2 + 3] = LAND;
    array[mColumns / 2 + 1][mRows / 2 + 3] = LAND;
    array[mColumns / 2 + 2][mRows / 2 + 2] = LAND;
    array[mColumns / 2 + 2][mRows / 2 + 1] = LAND;
}

TerrainType LogicalMap::cell(int column, int row)
{
    if(column < 0 || row < 0 || column >= mColumns || row >= mRows)
        return SEA;

    return array[column][row];
}

void LogicalMap::setCell(TerrainType value, int row, int column)
{
    if(column >= 0 && row >= 0 && column < mColumns && row < mRows)
        array[column][row] = value;
}

LogicalMap::~LogicalMap()
{
    for(int column = 0; column < mColumns; ++column)
    {
        delete [] array[column];
        array[column] = nullptr;
    }

    delete [] array;
    array = nullptr;
}

void LogicalMap::allocateMemory()
{
    array = new TerrainType*[mColumns]();

    for(int column = 0; column < mColumns; ++column)
    {
        array[column] = new TerrainType[mRows];
    }
}

