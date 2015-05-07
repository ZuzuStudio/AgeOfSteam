#include "../include/worldview.h"
#include <algorithm>
#include <QDebug>

using namespace std;

enum Direction {neg, pos};

template<Direction dir>
qreal oneWay(qreal value);

template<>
qreal oneWay<neg>(qreal value)
{
    return value < 0 ? value : 0;
}

template<>
qreal oneWay<pos>(qreal value)
{
    return value > 0 ? value : 0;
}

WorldView::WorldView(qreal mapLeft, qreal mapRight, qreal mapTop, qreal mapBottom,
                     qreal screenLeft, qreal screenRight, qreal screenTop, qreal screenBottom,
                     qreal scale):
    mapLeft(mapLeft),
    mapRight(mapRight),
    mapTop(mapTop),
    mapBottom(mapBottom),
    screenLeft(screenLeft),
    screenRight(screenRight),
    screenTop(screenTop),
    screenBottom(screenBottom),
    currentCenter((mapLeft + mapRight) / 2.0, (mapTop + mapBottom) / 2.0),
    scale(scale),
    maximalScale(10.0)// TODO without magic number
{
    auto horizontalMinimalScale = (this->screenRight - this->screenLeft)
                                  / (this->mapRight - this->mapLeft);

    auto verticalMinimalScale = (this->screenTop - this->screenBottom)
                                / (this->mapTop - this->mapBottom);
    minimalScale = max(horizontalMinimalScale, verticalMinimalScale);
    qDebug() << "minimal scale" << minimalScale;
}

void WorldView::moveScreen(QPointF screenShift)
{
    //qDebug() << "screenShift" << screenShift;
    QPointF mapShift = screenShift / scale;
    //qDebug() << "mapShift" << mapShift;
    qreal boxLeft = mapLeft - (currentCenter.x() + screenLeft / scale);
    qreal boxRight = mapRight - (currentCenter.x() + screenRight / scale);
    qreal boxTop = mapTop - (currentCenter.y() + screenTop / scale);
    qreal boxBottom = mapBottom - (currentCenter.y() + screenBottom / scale);
    //qDebug() << "move box" << boxLeft << boxRight << boxTop << boxBottom;
    int sector = 0;
    sector += mapShift.x() < boxLeft ? -1 : (mapShift.x() > boxRight ? 1 : 0);
    sector += mapShift.y() < boxBottom ? -3 : (mapShift.y() > boxTop ? 3 : 0);
    QPointF boxedShift(0.0, 0.0);


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
            max(max(screenLeft / (mapLeft - currentCenter.x()),
                    screenRight / (mapRight - currentCenter.x())),
                max(screenTop / (mapTop - currentCenter.y()),
                    screenBottom / (mapBottom - currentCenter.y())));

        qDebug() << "\nin decreaseScale";
        qDebug() << "scale:\t" << scale;
        qDebug() << "fu scale:\t"<<futureScale;
        qDebug() << "minSmplScl:\t"<< minimalSimpleScale;

        if(futureScale < minimalSimpleScale)
        {
            auto cornerNW(getNW());
            auto cornerSE(getSE());
            qDebug() << "have to shift";
            qDebug() << "l:\t"<<cornerNW.x() <<   mapLeft;
            qDebug() << "r:\t"<<cornerSE.x() << mapRight;
            qDebug() << "t:\t"<<cornerNW.y() << mapTop;
            qDebug() << "b:\t"<<cornerSE.y() << mapBottom;
            auto shift = QPointF(oneWay<neg>(cornerNW.x() - mapLeft)
                                + oneWay<pos>(cornerSE.x() - mapRight),
                                oneWay<pos>(cornerNW.y() - mapTop)
                                + oneWay<neg>(cornerSE.y() - mapBottom));

            qDebug()<<"shift:\t"<< shift;
            currentCenter -= shift;
        }

        scale = futureScale;
        qDebug()<<"\n";
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
    qreal adjust = 1.0;
    QPointF center = rect.center();
    QSizeF size(rect.size() * scale);
    size.rwidth() += adjust;
    size.rheight() += adjust;
    rect.setSize(size);
    rect.moveCenter(transformToScreenCordinates(center));
    return rect;
}

QRectF WorldView::transformToMapCordinates(QRectF rect) const
{
    QPointF center = rect.center();
    rect.setSize(rect.size() / scale);
    rect.moveCenter(transformToMapCordinates(center));
    return rect;
}
