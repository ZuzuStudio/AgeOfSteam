#include "include/arraygrid.h"

ArrayGrid::ArrayGrid(unsigned **array, unsigned rows, unsigned columns):
    array(array), rows(rows), columns(columns)
{}

ArrayGrid::ArrayGrid(unsigned rows, unsigned columns):
    rows(rows), columns(columns)
{
    array = new unsigned*[rows];
    for(unsigned i = 0; i < rows; ++i)
    {
        array[i] = new unsigned[columns];
    }
}

int ArrayGrid::get(unsigned row, unsigned column)
{
    return array[row][column];
}

void ArrayGrid::set(int value, unsigned row, unsigned column)
{
    array[row][column] = value;
}

int ArrayGrid::getRows()
{
    return rows;
}

int ArrayGrid::getColumns()
{
    return columns;
}

ArrayGrid::~ArrayGrid()
{
    for(unsigned i = 0; i < rows; ++i)
    {
        delete [] array[i];
    }
    delete [] array;
    array = nullptr;
}

