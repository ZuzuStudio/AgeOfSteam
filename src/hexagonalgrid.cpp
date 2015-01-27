#include "include/hexagonalgrid.h"

HexagonalGrid::ArrayGrid::ArrayGrid(unsigned rows, unsigned colomns):rows(rows), colomns(colomns)
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

/**
 * Now it just creates a simple matrix of 1's, but in nearest time it should also become
 * the base of generation of true map
*/
int ** HexagonalGrid::ArrayGrid::createGrid()
{
    for(size_t i = 0; i < rows; ++i)
    {
        for(size_t j = 0; j < colomns; ++j)
        {
            array[i][j] = 1;
        }
    }
    return array;
}

HexagonalGrid::HexagonalGrid(qreal scale):grid(nullptr), hexagon(nullptr), scale(scale), shift(0,0)
{
    hexagon = new Hexagon(100);
    grid = new ArrayGrid(20, 20);

}

HexagonalGrid::~HexagonalGrid()
{
    delete grid;
    grid = nullptr;
    delete hexagon;
    hexagon = nullptr;
}

std::vector<QImage> HexagonalGrid::drawRastr(QSvgRenderer * renderer)
{
    int cornersX[6];
    int cornersY[6];
    hexagon->setCellIndex(0, 0);
    hexagon->computeCorners(cornersX, cornersY);
    image_size = QSize( (sizeOfClaster / 2 + sizeOfClaster % 2) * (cornersX[2] - cornersX[5]) + sizeOfClaster / 2 * (cornersX[1] - cornersX[0]) ,
                      sizeOfClaster * (cornersY[4] - cornersY[0]));
    for(size_t k = 0; k < numberOfClasters; ++k)
    {
        QImage image(image_size, QImage::Format_ARGB32_Premultiplied);
        QPainter *painter = new QPainter(&image);

        for (size_t j = 0; j < sizeOfClaster; ++j)
        {
            for (size_t i = 0; i < sizeOfClaster; ++i)
            {
                hexagon->setCellIndex(i, j);
                hexagon->computeCorners(cornersX, cornersY);
                renderer->render(painter, QRectF(cornersX[5], cornersY[0], cornersX[2] - cornersX[5], cornersY[4] - cornersY[0]));
            }
        }
        delete painter;
        painters.push_back(image);
    }
    return painters;
}

void HexagonalGrid::gluingTogetherClasters(QPainter *p)
{
    size_t counter = 1;
    int width = shift.x();
    int height = shift.y();
    p->drawImage(shift.x(), shift.y(), painters.at(0));

    int mod = (int)sqrt(numberOfClasters);


    qDebug() << hexagon->getHeight();
    while(counter < numberOfClasters)
    {
        if(counter % mod != 0)
        {
            width += painters.at(0).size().width();
            p->drawImage(width - hexagon->getWidth() * 3 / 2 * (counter % mod),
                         height ,
                         painters.at(counter));
            qDebug() << height;
        }
        else
        {
            height += painters.at(0).size().height()- hexagon->getHeight() ;
            width = shift.x();
            p->drawImage(width,
                         height,
                         painters.at(counter));
            qDebug() << height;
        }
        ++counter;
    }

}

void HexagonalGrid::setScale(qreal scale)
{
    this->scale = scale;
}

void HexagonalGrid::addShift(int x, int y)
{
    shift += QPoint(x,y) ;
}

void HexagonalGrid::addShift(QPoint pos)
{
    shift += pos;
}

void HexagonalGrid::drawSVG(QSvgRenderer *renderer, QPainter *painter)
{
    int cornersX[6];
    int cornersY[6];
    Hexagon currentHexagon(100 * scale);
    currentHexagon.setCellIndex(0, 0);
    currentHexagon.computeCorners(cornersX, cornersY);

    int totalWidth = image_size.width();
    unsigned width_cells = totalWidth / ( (cornersX[2] - cornersX[5]) * 0.75) + 2;

    int totalHeight = image_size.height();
    unsigned height_cells = totalHeight / (cornersY[4] - cornersY[0]) + 2;
    for (size_t j = 0; j < height_cells; ++j)
    {
        for (size_t i = 0; i < width_cells; ++i)
        {
            currentHexagon.setCellIndex(i, j);
            currentHexagon.computeCorners(cornersX, cornersY);
            renderer->render(painter, QRectF(cornersX[5], cornersY[0],
                    (cornersX[2] - cornersX[5]) + 1, (cornersY[4] - cornersY[0]) + 1));
        }
    }

    currentHexagon.setCellByPoint(100, 100);
    qDebug() << "Mj = " << currentHexagon.getIndexJ();
    painter->end();
}


