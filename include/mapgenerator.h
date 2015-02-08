#ifndef MAPGENERATOR_H
#define MAPGENERATOR_H


#include <cstdlib>
#include <ctime>
#include <QFile>
#include <QTextStream>
//#include "hexagonalgrid.h"
#include "include/arraygrid.h"

enum Territori :unsigned {Land, Sea};

class MapGenerator
{
private:
    MapGenerator();
    ~MapGenerator();

public:
    static ArrayGrid *generate(int width, int height);

private:
    static void makeFile(Territori ** map, int width, int height, QString filename= "map_test.txt");
};

#endif // MAPGENERATOR_H
