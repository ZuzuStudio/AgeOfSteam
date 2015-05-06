#ifndef ARRAYGRID_H
#define ARRAYGRID_H

enum TerrainType{HILL};

// TODO rename the class

class ArrayGrid
{
public:
    ArrayGrid(TerrainType **array, unsigned columns, unsigned rows);
    ArrayGrid(unsigned columns, unsigned rows);//temporary candidate for deletetion
    TerrainType cell(unsigned column, unsigned row);
    void setCell(TerrainType value, unsigned row, unsigned column);
    unsigned rows() const noexcept {return mRows;}
    unsigned columns() const noexcept {return mColumns;}

    ~ArrayGrid() noexcept;
private:
    TerrainType **array;
    unsigned mColumns, mRows;

    void allocateMemory();
};

#endif // ARRAYGRID_H
