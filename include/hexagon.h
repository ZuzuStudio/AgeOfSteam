#ifndef HEXAGON_H
#define HEXAGON_H

#include <QPoint>

class Hexagon
{

public :
    Hexagon();
    explicit Hexagon(qreal radius);
    Hexagon(const Hexagon & other);
    Hexagon(Hexagon && other) = default;

    Hexagon & operator = (const Hexagon & other);
    Hexagon & operator = (Hexagon && other) = default;

    ~Hexagon();

    int getLeft();
    int getTop();
    int getCenterX();
    int getCenterY();
    int getIndexI();
    int getIndexJ();
    int getNeighbourI(int neighbourIdx);// horizontal grid for given neighbours
    int getNeighbourJ(int neighbourJdx);// vertical grid coordinate for the given neighbour
    int getWidth();
    int getHeight();

    //--- starting from the top left corners, computing all the corners
    void computeCorners(int * cornersX, int * cornersY);
    void setCellIndex(int i, int j);

    //--- important for claster's position definition
    void setCellByPoint(int x, int y);


    enum corner {TOP = 0, TOPRIGHT = 1, BOTTOMRIGHT = 2, BOTTOM = 3, BOTTOMLEFT = 4, TOPLEFT = 5};

private:
    static const int number_of_neighbours = 6;

    const int neigbours_di[2][number_of_neighbours] = {{1, 0, -1, 0, 1, -1}, {-1, 0, 1, 0, -1, 1}};
    const int neigbours_dj[number_of_neighbours] = {-1, 1, 0, 1, -1, 0};

    int corners_dx[number_of_neighbours]; //horizontal offsets of cell's corners
    int corners_dy[number_of_neighbours];

    int mx = 0;
    int my = 0;// cell's top coordinate

    int mi = 0;
    int mj = 0;// cell's vertical gris coorinate

    //--- the fact is the hexagon may be defined by 2 values : a side size and the center point
    // or simply : the radius
    qreal radius;
    qreal size;

    //--- another two values : width and height are used in making grid proccess
    qreal height;
    qreal width;

};

#endif // HEXAGON_H
