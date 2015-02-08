#ifndef ARRAYGRID_H
#define ARRAYGRID_H


class ArrayGrid
{
private:
    unsigned **array;
    unsigned rows, columns;
    //const int number_of_hex_corners = 6;
    //const int cell_radius = 40;

public:
    ArrayGrid(unsigned **array, unsigned rows, unsigned columns);
    ArrayGrid(unsigned rows, unsigned columns);
    int get(unsigned row, unsigned column);
    void set(int value, unsigned row, unsigned column);
    int getRows();
    int getColumns();

    ~ArrayGrid();
    //int ** createGrid();

};

#endif // ARRAYGRID_H
