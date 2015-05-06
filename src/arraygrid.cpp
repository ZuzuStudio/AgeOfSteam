#include "include/arraygrid.h"
#include <assert.h>

ArrayGrid::ArrayGrid(TerrainType **array, unsigned rows, unsigned columns):
    array(nullptr), mColumns(columns), mRows(rows)
{
    assert(columns > 0);
    assert(rows > 0);
    allocateMemory();
    for(unsigned column = 0; column < mColumns; ++column)
        for(unsigned row = 0; row < mRows; ++row)
            this->array[column][row] = array[column][row];
}

ArrayGrid::ArrayGrid(unsigned columns, unsigned rows):
    array(nullptr), mColumns(columns), mRows(rows)
{
    assert(columns > 0);
    assert(rows > 0);
    allocateMemory();
    for(unsigned column = 0; column < mColumns; ++column)
        for(unsigned row = 0; row < mRows; ++row)
            this->array[column][row] = HILL;
}

TerrainType ArrayGrid::cell(unsigned column, unsigned row)
{
    return array[column][row];
}

void ArrayGrid::setCell(TerrainType value, unsigned row, unsigned column)
{
    array[column][row] = value;
}

ArrayGrid::~ArrayGrid()
{
    for(unsigned column = 0; column < mColumns; ++column)
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
    for(unsigned column = 0; column < mColumns; ++column)
    {
        array[column] = new TerrainType[mRows];
    }
}

