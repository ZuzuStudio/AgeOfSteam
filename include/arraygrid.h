#ifndef ARRAYGRID_H
#define ARRAYGRID_H

//enum TerrainType{SEA, HILL};

// TODO rename the class

class ArrayGrid
{
public:
    ArrayGrid(TerrainType **array, int columns, int rows);
    ArrayGrid(int columns, int rows);//temporary candidate for deletetion
    TerrainType cell(int column, int row);
    void setCell(TerrainType value, int row, int column);
    int rows() const noexcept {return mRows;}
    int columns() const noexcept {return mColumns;}

    ~ArrayGrid() noexcept;
private:
    TerrainType **array;
    int mColumns, mRows;

    void allocateMemory();
};

#endif // ARRAYGRID_H
