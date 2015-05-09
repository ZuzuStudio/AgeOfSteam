#ifndef LOGICALMAP_H
#define LOGICALMAP_H

enum TerrainType {SEA, LAND, HILL};

class LogicalMap
{
public:
    LogicalMap(TerrainType **array, int columns, int rows);
    LogicalMap(int columns, int rows);//temporary candidate for deletetion
    TerrainType cell(int column, int row);
    void setCell(TerrainType value, int row, int column);

    int rows() const noexcept
    {
        return mRows;
    }

    int columns() const noexcept
    {
        return mColumns;
    }

    ~LogicalMap() noexcept;
private:
    TerrainType **array;
    int mColumns, mRows;

    void allocateMemory();
};

#endif // LOGICALMAP_H
