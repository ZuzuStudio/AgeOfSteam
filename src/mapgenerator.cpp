#include "include/mapgenerator.h"

MapGenerator::MapGenerator()
{

}

MapGenerator::~MapGenerator()
{

}

ArrayGrid *MapGenerator::generate(int width, int height)
{
    //init map
    Territori **map = new Territori*[height];
    for(int i = 0; i < height; ++i)
    {
        map[i] = new Territori[width];
        for(int n = 0; n < width; ++n)
            map[i][n] = Sea;
    }

    //generate default land
    srand(time(NULL));
    int x,y;
    for(int k = 0; k <40; ++k){
        x = rand()%(height - height/5);
        y= rand()%(width - width/10);

        for(int i = 0; i < height/10; ++i){
            for(int n = 0; n < width/10; ++n)
            map[i+x][n+y]= Land;
        }
    }

    //fractal
    int step = (width+height)/2;
    do {
        step/=2;
        for (int y=0; y<width; y+=step) {
          for (int x=0; x<height; x+=step) {

            int cx=x+ ((rand()%2) ? 0 : step);
            int cy=y+ ((rand()%2) ? 0 : step);

            cx=(cx/(step*2))*step*2;
            cy=(cy/(step*2))*step*2;

            cx=cx%height;
            cy=cy%width;

            map[x][y] = map[cx][cy];
            //changeType(getElement(cx,cy).type, x, y);
          }
        }
    } while (step>1);


    //makeFile(map, width, height);

    //ArrayGrid *m = new ArrayGrid((unsigned**)map, height, width);

    //makeFile(m);
    return nullptr;
}

void MapGenerator::makeFile(Territori **map, int width, int height, QString filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    QTextStream out(&file);
    for(int i = 0; i < height; ++i){
        for(int n = 0; n < width; ++n)
            out << map[i][n];
        out << '\n';
    }
}

