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
    QPoint mapShift = screenShift / scale;
    int boxLeft = mapLeft - (currentCenter.x() + screenLeft / scale);
    int boxRight = mapRight - (currentCenter.x() + screenRight / scale);
    int boxTop = mapTop - (currentCenter.y() + screenTop / scale);
    int boxBottom = mapLeft - (currentCenter.y() + screenBottom / scale);
    qreal quotient =
        min(min(boxRight, max(boxLeft, mapShift.x())) / (qreal)mapShift.x(),
            min(boxTop, max(boxBottom, mapShift.y())) / (qreal)mapShift.y());
    currentCenter += quotient * mapShift;
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
    return QPointF(currentCenter.x() + screenLeft / scale,
                   currentCenter.y() + screenTop / scale);
}

QPointF WorldView::getNE() const
{
    return QPointF(currentCenter.x() + screenRight / scale,
                   currentCenter.y() + screenTop / scale);
}

QPointF WorldView::getSW() const
{
    return QPointF(currentCenter.x() + screenLeft / scale,
                   currentCenter.y() + screenBottom / scale);
}

QPointF WorldView::getSE() const
{
    return QPointF(currentCenter.x() + screenRight / scale,
                   currentCenter.y() + screenBottom / scale);
}
