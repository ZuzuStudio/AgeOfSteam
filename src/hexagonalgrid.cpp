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
    for(size_t i = 0 ; i < rows; ++i)
    {
        delete array[i];
        array[i] = nullptr;
    }
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
    image_size = QSize( (sizeOfClaster + 0.5) * (cornersX[1] - cornersX[5]),
                        (sizeOfClaster / 2 + sizeOfClaster % 2) * (cornersY[3] - cornersY[0]) +
                         sizeOfClaster / 2 * (cornersY[2] - cornersY[1]));

    for(size_t k = 0; k < numberOfClasters; ++k)
    {
        QImage image(image_size, QImage::Format_ARGB32_Premultiplied);
        QPainter *painter = new QPainter(&image);
        //painter->fillRect(0, 0, image_size.width(), image_size.height(), Qt::white);

        for (size_t j = 0; j < sizeOfClaster; ++j)
        {
            for (size_t i = 0; i < sizeOfClaster; ++i)
            {
                hexagon->setCellIndex(i, j);
                hexagon->computeCorners(cornersX, cornersY);
                renderer->render(painter, QRectF(cornersX[5], cornersY[0], cornersX[1] - cornersX[5], cornersY[3] - cornersY[0]));
            }
        }
        delete painter;
        painters.push_back(image);
    }
    return painters;
}
//44 текущее знач регистра интсрукции помещается в стек и управление переходит по указанному адресу
//45 управление передаётся по адресу в стеке. из стека выталкивается некоторое количество машинных слов
/*
1. if a function has a return statement then make a push()
2. function params are placed in stack inverse
3. call() is called
4. registr bios is placed in stack
5. in bios register top register statement is placed
*/

/*
1. pop destructs local variables
2. get from stack old bios and place it at top
3. return from function adress is the number of function params
4. function result is placed at stack's top, then to get it use pop or
*/

void HexagonalGrid::gluingTogetherClasters(QPainter *p)
{
    size_t counter = 1;
    int width = shift.x();
    int height = shift.y();
    p->drawImage(shift.x(), shift.y(), painters.at(0));

    int mod = (int) sqrt(numberOfClasters);

    while(counter < numberOfClasters)
    {
        if(counter % mod != 0)
        {
            width += painters.at(0).size().width() - hexagon->getWidth() / 2 - (sizeOfClaster - 1);
            p->drawImage(width,
                         height,
                         painters.at(counter));
        }
        else
        {
            height += painters.at(0).size().height() - hexagon->getHeight();
            width = shift.x();
            p->drawImage(width,
                         height,
                         painters.at(counter));
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
    shift += QPoint(x,y) / scale;
}

void HexagonalGrid::addShift(QPoint pos)
{
    shift += pos / scale;
}

void HexagonalGrid::drawSVG(QSvgRenderer *renderer, QPainter *painter)
{
    int cornersX[6];
    int cornersY[6];
    Hexagon currentHexagon(100 * scale);
    currentHexagon.setCellIndex(0, 0);
    currentHexagon.computeCorners(cornersX, cornersY);
    image_size = QSize( (sizeOfClaster / 2 + sizeOfClaster % 2) * (cornersX[2] - cornersX[5]) +
                        (sizeOfClaster / 2  + (sizeOfClaster % 2 == 0 ? 0.5 : 0)) * (cornersX[1] - cornersX[0]),
                         (sizeOfClaster + 0.5) * (cornersY[4] - cornersY[0]) );

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
                    (cornersX[1] - cornersX[5]) + 1, (cornersY[3] - cornersY[0]) + 1));
        }
    }

    currentHexagon.setCellByPoint( - shift.y() * scale + painter->window().bottomRight().y(),
                                   - shift.x() * scale + painter->window().bottomRight().x());
    //qDebug() << "Mi = " << currentHexagon.getIndexI() << "\tMj = " << currentHexagon.getIndexJ();
    painter->end();

    /**
     * Now on we gonna get all four points of current screen metrics, and they ought to reflect
     * which clusters should be loaded. I suppose that we need to load each cluster corresponding
     * for it's point. Hope so, the worst case is 4(8) clusters to load won't be a great ploblem
     * to solve.
    */
}


