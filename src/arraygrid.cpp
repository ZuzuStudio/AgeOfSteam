#include "include/arraygrid.h"
#include <assert.h>

ArrayGrid::ArrayGrid(TerrainType **array, int rows, int columns):
    array(nullptr), mColumns(columns), mRows(rows)
{
    assert(columns > 0);
    assert(rows > 0);
    allocateMemory();

    for(int column = 0; column < mColumns; ++column)
        for(int row = 0; row < mRows; ++row)
            this->array[column][row] = array[column][row];
}

ArrayGrid::ArrayGrid(int columns, int rows):
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
}

TerrainType ArrayGrid::cell(int column, int row)
{
    if(column < 0 || row < 0 || column >= mColumns || row >= mRows)
        return SEA;

    return array[column][row];
}

void ArrayGrid::setCell(TerrainType value, int row, int column)
{
    if(column >= 0 && row >= 0 && column < mColumns && row < mRows)
        array[column][row] = value;
}

ArrayGrid::~ArrayGrid()
{
    for(int column = 0; column < mColumns; ++column)
    {
        delete [] array[column];
        array[column] = nullptr;
    }

    delete [] array;
    array = nullptr;
}

void ArrayGrid::allocateMemory()
{
    array = new TerrainType*[mColumns]();

    for(int column = 0; column < mColumns; ++column)
    {
        array[column] = new TerrainType[mRows];
    }
}

