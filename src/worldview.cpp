#include "../include/worldview.h"
#include <algorithm>
#include <QDebug>

using namespace std;

enum Direction {neg, pos};

template<Direction dir>
int oneWay(int value);

template<>
int oneWay<neg>(int value)
{
    return value < 0 ? value : 0;
}

template<>
int oneWay<pos>(int value)
{
    return value > 0 ? value : 0;
}

WorldView::WorldView(int mapLeft, int mapRight, int mapTop, int mapBottom,
                     int screenLeft, int screenRight, int screenTop, int screenBottom,
                     qreal scale):
    mapLeft(mapLeft),
    mapRight(mapRight),
    mapTop(mapTop),
    mapBottom(mapBottom),
    screenLeft(screenLeft),
    screenRight(screenRight),
    screenTop(screenTop),
    screenBottom(screenBottom),
    currentCenter((mapLeft + mapRight) / 2, (mapTop + mapBottom) / 2),
    scale(scale),
    maximalScale(10.0)// TODO without magic number
{
    auto horizontalMinimalScale = (qreal)(this->screenRight - this->screenLeft)
                                  / (qreal)(this->mapRight - this->mapLeft);

    auto verticalMinimalScale = (qreal)(this->screenTop - this->screenBottom)
                                / (qreal)(this->mapTop - this->mapBottom);
    minimalScale = max(horizontalMinimalScale, verticalMinimalScale);
    qDebug() << "minimal scale" << minimalScale;
}

void WorldView::moveScreen(QPoint screenShift)
{
    qDebug() << "screenShift" << screenShift;
    QPoint mapShift = screenShift / scale;
    qDebug() << "mapShift" << mapShift;
    int boxLeft = mapLeft - (currentCenter.x() + screenLeft / scale);
    int boxRight = mapRight - (currentCenter.x() + screenRight / scale);
    int boxTop = mapTop - (currentCenter.y() + screenTop / scale);
    int boxBottom = mapBottom - (currentCenter.y() + screenBottom / scale);
    qDebug() << "move box" << boxLeft << boxRight << boxTop << boxBottom;
    int sector = 0;
    sector += mapShift.x() < boxLeft ? -1 : (mapShift.x() > boxRight ? 1 : 0);
    sector += mapShift.y() < boxBottom ? -3 : (mapShift.y() > boxTop ? 3 : 0);
    QPoint boxedShift(0, 0);


    switch(sector)
    {
    case -4:// SW
        break;

    case -3:// S

        break;

    case -2:// SE
        break;

    case -1:// W
        break;

    case 0:// center
        boxedShift = mapShift;
        break;

    case 1:// E
        break;

    case 2:// NW
        break;

    case 3:// N
        break;

    case 4:// NE
        break;
    }

    currentCenter += boxedShift;
}

void WorldView::decreaseScale()
{
    auto futureScale = scale / 1.01;

    if(futureScale > minimalScale)
    {
        auto minimalSimpleScale =
            max(max((qreal)screenLeft / (mapLeft - currentCenter.x()),
                    (qreal)screenRight / (mapRight - currentCenter.x())),
                max((qreal)screenTop / (mapTop - currentCenter.y()),
                    (qreal)screenBottom / (mapBottom - currentCenter.y())));

        if(futureScale < minimalSimpleScale)
        {
            auto shift = QPoint(oneWay<pos>(screenLeft - mapLeft)
                                + oneWay<pos>(screenRight - mapRight),
                                oneWay<pos>(screenTop - mapTop)
                                + oneWay<pos>(screenBottom - mapBottom));
            currentCenter += shift;
        }

        scale = futureScale;
    }
}

void WorldView::increaseScale()
{
    auto futureScale = scale * 1.01;

    if(futureScale > maximalScale)
        scale = maximalScale;
    else
        scale = futureScale;
}

QPointF WorldView::getNW() const
{
    return transformToMapCordinates(QPointF(screenLeft, screenTop));
}

QPointF WorldView::getNE() const
{
    return transformToMapCordinates(QPointF(screenRight, screenTop));
}

QPointF WorldView::getSW() const
{
    return transformToMapCordinates(QPointF(screenLeft, screenBottom));
}

QPointF WorldView::getSE() const
{
    return transformToMapCordinates(QPointF(screenRight, screenBottom));
}

QPointF WorldView::transformToScreenCordinates(const QPointF &point) const
{
    return QPointF((currentCenter.x() - point.x()) * scale,
                   (currentCenter.y() - point.y()) * scale);
}

QPointF WorldView::transformToMapCordinates(const QPointF &point) const
{
    return QPointF(currentCenter.x() + point.x() / scale,
                   currentCenter.y() + point.y() / scale);
}

QRectF WorldView::transformToScreenCordinates(QRectF rect) const
{
    rect.moveCenter(transformToScreenCordinates(rect.center()));
    rect.setSize(rect.size() * scale);
    return rect;
}

QRectF WorldView::transformToMapCordinates(QRectF rect) const
{
    rect.moveCenter(transformToMapCordinates(rect.center()));
    rect.setSize(rect.size() / scale);
    return rect;
}
