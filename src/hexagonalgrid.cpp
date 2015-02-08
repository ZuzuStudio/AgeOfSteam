#include "include/hexagonalgrid.h"

HexagonalGrid::HexagonalGrid(qreal scale):grid(nullptr), hexagon(nullptr), scale(scale), shift(0,0)
{
    hexagon = new Hexagon(100);
    grid = new ArrayGrid(20, 20);

}

HexagonalGrid::HexagonalGrid(ArrayGrid *grid, QSvgRenderer **rendererArray, qreal scale):
    grid(grid), rendererArray(rendererArray), scale(scale), shift(0,0)
{
    hexagon = new Hexagon(100);
    makeClasters();
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
    image_size = QSize( (sizeOfClaster / 2 + sizeOfClaster % 2) * (cornersX[2] - cornersX[5]) +
                        (sizeOfClaster / 2  + (sizeOfClaster % 2 == 0 ? 0.5 : 0)) * (cornersX[1] - cornersX[0]),
                         (sizeOfClaster + 0.5) * (cornersY[4] - cornersY[0]) );
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

    while(counter < numberOfClasters)
    {
        if(counter % mod != 0)
        {
            width += painters.at(0).size().width();
            p->drawImage(width - (sizeOfClaster % 2 == 0 ? hexagon->getWidth() * (counter % mod) / 4 : hexagon->getWidth() * (counter % mod)),
                         height ,
                         painters.at(counter));
        }
        else
        {
            height += painters.at(0).size().height() - hexagon->getHeight() / 2;
            width = shift.x();
            p->drawImage(width,
                         height,
                         painters.at(counter));
        }
        ++counter;
    }
}

void HexagonalGrid::draw(QPainter *painter)
{
    Claster *claster;
    for(size_t i = 0; i < clastersRows; i++){
        for(size_t j = 0; j < clastersColumns; j++){
            claster = &clasters[i][j];
            painter->drawImage(claster->position + shift, claster->image);
        }
    }
}

void HexagonalGrid::setScale(qreal scale)
{
    this->scale = scale;
}

void HexagonalGrid::addShift(int x, int y)
{
    shift += QPoint(x,y)/scale ;
}

void HexagonalGrid::addShift(QPoint pos)
{
    shift += pos/scale;
}

void HexagonalGrid::makeClasters()
{
    clastersRows = grid->getRows()/sizeOfClaster + (grid->getRows()%sizeOfClaster != 0);
    clastersColumns = grid->getColumns()/sizeOfClaster + (grid->getColumns()%sizeOfClaster != 0);

    clasters = new Claster*[clastersRows];
    for(size_t i = 0; i < clastersRows; i++){
        clasters[i] = new Claster[clastersColumns];
    }


    int cornersX[6];
    int cornersY[6];
    hexagon->setCellIndex(0, 0);
    hexagon->computeCorners(cornersX, cornersY);
    Claster *claster;
    QPoint position(0,0);
    int cur_x = 0, cur_y = 0;
    for(size_t y = 0, rows = grid->getRows(); y < clastersRows; y++){
        for(size_t x = 0, columns = grid->getColumns(); x < clastersColumns; x++){
            claster = &clasters[y][x];
            claster->rows = (rows > sizeOfClaster)?sizeOfClaster:rows;
            claster->columns = (columns > sizeOfClaster)?sizeOfClaster:columns;
            claster->position = position;
            image_size = QSize( (claster->columns / 2 + claster->columns % 2) * (cornersX[2] - cornersX[5]) +
                                (claster->columns / 2  + (claster->columns % 2 == 0 ? 0.5 : 0)) * (cornersX[1] - cornersX[0]),
                                 (claster->rows + 0.5) * (cornersY[4] - cornersY[0]) );

            QImage image(image_size, QImage::Format_ARGB32_Premultiplied);
            QPainter *painter = new QPainter(&image);
            for (size_t j = 0; j < claster->rows; ++j)
            {
                for (size_t i = 0; i < claster->columns; ++i)
                {
                    hexagon->setCellIndex(i, j);
                    hexagon->computeCorners(cornersX, cornersY);
                    rendererArray[grid->get(j+cur_y, i+cur_x)]->render(painter, QRectF(cornersX[5], cornersY[0], cornersX[2] - cornersX[5], cornersY[4] - cornersY[0]));
                }
            }
            delete painter;
            claster->image = image;
            columns -=sizeOfClaster;
            cur_x+=sizeOfClaster;
            position.setX(position.x()+image_size.width() - hexagon->getWidth()/4);
        }
        rows -= sizeOfClaster;
        cur_y+=sizeOfClaster;
        cur_x = 0;
        position.setX(0);
        position.setY(position.y()+image_size.height()-hexagon->getHeight() / 2);
    }

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
                    (cornersX[2] - cornersX[5]) + 1, (cornersY[4] - cornersY[0]) + 1));
        }
    }

    currentHexagon.setCellByPoint( - shift.x() * scale + painter->window().bottomRight().x() ,
                                   - shift.y() * scale + painter->window().bottomRight().y() );
    qDebug() << "Mi = " << currentHexagon.getIndexI() << "\tMj = " << currentHexagon.getIndexJ();
    painter->end();
}


