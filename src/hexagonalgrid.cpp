#include "include/hexagonalgrid.h"

HexagonalGrid::ArrayGrid::ArrayGrid(int rows, int colomns):rows(rows), colomns(colomns)
{
    array = new int*[rows];
    for(size_t i = 0; i < rows; ++i)
    {
        array[i] = new int[colomns];
    }
}

HexagonalGrid::ArrayGrid::~ArrayGrid()
{
    delete array;
    array = nullptr;
}

int ** HexagonalGrid::ArrayGrid::createGrid(std::vector<Hexagon> hexagons)
{
    qreal q;
    qreal r;

    for(size_t i = 0; i < hexagons.size(); ++i)
    {
        Hexagon current = hexagons.at(i);
        q = (current.getCenter().rx() * sqrt(3) / 3 - current.getCenter().ry() / 3) / current.getSize();
        r = (current.getCenter().ry() * 2 / 3) / current.getSize();
        if(r < 20 && q < 20 && (q + r / 2) < 20)
        array[(int)r][(int)(q + r / 2)] = 1;
    }

    for(size_t i = 0; i < rows; ++i)
    {
        for(size_t j = 0; j < colomns; ++j)
        {
            if(array[i][j] != 1)
            {
                array[i][j] = 0;
            }
        }
    }
    return array;
}

HexagonalGrid::HexagonalGrid():grid(nullptr), renderer(nullptr), image(nullptr), scale(30)
{
    QSize currentSize(sqrt(3.0) * scale / 2.0 + sqrt(3.0) * scale / 4.0, scale);
    image = new QImage(currentSize, QImage::Format_ARGB32_Premultiplied);
    renderer = new QSvgRenderer(QString(":/res/hillFlatLod1_res.svg"));
    //---------------//
    grid = new ArrayGrid(20, 20);

}

HexagonalGrid::~HexagonalGrid()
{
    delete grid;
    grid = nullptr;
    delete image;
    image = nullptr;
    delete renderer;
    renderer = nullptr;
}

void HexagonalGrid::drawRastr(QPainter *painter)
{
    QSize currentSize(sqrt(3.0) * scale / 2.0 + sqrt(3.0) * scale / 4.0, scale);
    std::vector<Hexagon> list;
    for (int k = 0; k < numberOfClasters; ++k)
    {
        QImage currentImage(currentSize, QImage::Format_ARGB32_Premultiplied);
        QPainter * imagePainter = new QPainter(&currentImage);

        for (int j = 0; j < sizeOfClaster; ++j)
        {
            for (int i = 0; i < sizeOfClaster; ++i)
            {
                qreal x = sqrt(3.0) * scale * i / 2.0 + (j & 1) * sqrt(3.0) * scale / 4.0 - 1;
                qreal y = scale * 0.75 * j - 1;
                renderer->render(imagePainter, QRectF(x, y, 2 + scale, 2 + scale));
                list.push_back(Hexagon(0.75, QPoint( (2 + scale + x) / 2, (2 + scale + y) / 2 )));
            }
        }
        painters.push_back(currentImage);
        delete imagePainter;
    }
    matrix = grid->createGrid(list);
}


