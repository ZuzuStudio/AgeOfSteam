#ifndef MAPGENERATOR_H
#define MAPGENERATOR_H


#include <cstdlib>
#include <ctime>
#include <QFile>
#include <QTextStream>
//#include "hexagonalgrid.h"
#include "include/logicalmap.h"

class MapGenerator
{
private:
    MapGenerator(){}
    ~MapGenerator(){}

public:
    static LogicalMap *generate(int width, int height);

private:
    static void makeFile(TerrainType ** map, int width, int height, QString filename= "map_test.txt");
};

#endif // MAPGENERATOR_H
