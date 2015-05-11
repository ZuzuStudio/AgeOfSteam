#include "include/mapgenerator.h"

namespace generate_tile
{
    /*void average_balancying(int ** map, int width, int height)
    {
        int sum = 0;
        int neighbors = 0;

        for(int i = 0; i < height; ++i)
        {
            for(int j = 0; j < width; ++j)
            {

                for(int x = std::max(0, i - 1); x <= std::min(i + 1, height - 1); ++x)
                {
                    for(int y = std::max(0, j - 1); y <= std::min(j + 1, width - 1); ++y)
                    {
                        if(x != i || y != j)
                        {
                            sum += map[x][y];
                            ++neighbors;
                        }
                    }
                }
                map[i][j] = (map[i][j] + sum) / (neighbors + 1);

            }
        }

    }*/

    int **height_map(int width, int height)
    {
        srand(time(NULL));
        int **map = new int*[height];
        for(int i = 0; i < width; ++i)
        {
            map[i] = new int[width];
        }

        for(int i = 0; i < height; ++i)
        {
            for(int j = 0; j < width; ++j)
            {
                //map[i][j] = std::sin(i * 0.3) + std::cos(j * 0.3)
                //            + std::sin(std::sqrt(i * i + j * j) * 0.7) * 2;
                //map[i][j] = -255 + (rand() % 511);

                PerlinNoise pn(1, 0.1, 0.33, 1, 2);
                //qDebug() << pn.GetHeight(i, j);
                map[i][j] = 1000 * pn.getHeight(i, j);
            }
        }


        for(int i = 0; i < height; ++i)
        {
            for(int j = 0; j < width; ++j)
            {
                qDebug() << map[i][j] << " ";
            }
        }


        //average_balancying(map, width, height);

        return map;
    }
}


LogicalMap *MapGenerator::generate(int width, int height)
{

    int ** heightmap = generate_tile::height_map(width, height);


    TerrainType **map = new TerrainType*[height];

    for(int i = 0; i < height; ++i)
    {
        map[i] = new TerrainType[width];
        for(int n = 0; n < width; ++n)
        {
            map[i][n] = TerrainType::SEA;
        }
    }

    for(int i = 0; i < height; ++i)
    {
        for(int j = 0; j < width; ++j)
        {
            if(heightmap[i][j] > 100)
            {
                map[i][j] = TerrainType::HILL;
            }
            else if(heightmap[i][j] <= 100 && heightmap[i][j] > 0)
            {
                map[i][j] = TerrainType::LAND;
            }
            else
            {
                map[i][j] = TerrainType::SEA;
            }
        }
    }

    /*int x = 0, y = 0;

    for(int k = 0; k < 40; ++k)
    {
        x = rand() % (height - height / 5);
        y = rand() % (width - width / 10);

        for(int i = 0; i < height / 10; ++i)
        {
            for(int j = 0; j < width / 10; ++j)
            {
                map[i + x][j + y] = heightmap[i + x][j + y] > 100 ? TerrainType::HILL
                                                                  : TerrainType::HILL;
            }
        }
    }

    int step = (width + height) / 2;

    do
    {
        step /= 2;

        for(int y = 0; y < width; y += step)
        {
            for(int x = 0; x < height; x += step)
            {

                int cx = x + ((rand() % 2) ? 0 : step);
                int cy = y + ((rand() % 2) ? 0 : step);

                cx = (cx / (step * 2)) * step * 2;
                cy = (cy / (step * 2)) * step * 2;

                cx = cx % height;
                cy = cy % width;

                map[x][y] = map[cx][cy];
            }
        }
    } while (step > 1);*/

    delete heightmap;

    return new LogicalMap(map, width, height);
}

void MapGenerator::makeFile(int **map, int width, int height, QString filename)
{
    QFile file(filename);

    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);

    for(int i = 0; i < height; ++i)
    {
        for(int n = 0; n < width; ++n)
            out << map[i][n];

        out << '\n';
    }
}

