#include "../include/worldview.h"
#include <algorithm>
#include <QDebug>

using namespace std;

QPointF directDiv(const QPointF &a, const QPointF &b)
{
    return QPointF(a.x() / b.x(), a.y() / b.y());
}

QPointF directMul(const QPointF &a, const QPointF &b)
{
    return QPointF(a.x() * b.x(), a.y() * b.y());
}

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
    mapNW(mapLeft, mapTop),
    mapSE(mapRight, mapBottom),
    screenNW(screenLeft, screenTop),
    screenSE(screenRight, screenBottom),
    currentCenter((mapNW + mapSE) / 2.0),
    scale(scale),
    maximalScale(10.0)// TODO without magic number
{
    auto screenDiagonal = screenNW - screenSE;
    auto mapDiagonal = mapNW - mapSE;
    auto diagonalFactor = directDiv(screenDiagonal,  mapDiagonal);
    minimalScale = max(diagonalFactor.x(), diagonalFactor.y());
    qDebug() << "minimal scale" << minimalScale;
}

void WorldView::moveScreen(QPointF screenShift)
{
    //qDebug() << "screenShift" << screenShift;
    QPointF mapShift = screenShift / scale;
    //qDebug() << "mapShift" << mapShift;
    qreal boxLeft = mapNW.x() - (currentCenter.x() + screenNW.x() / scale);
    qreal boxRight = mapSE.x() - (currentCenter.x() + screenSE.x() / scale);
    qreal boxTop = mapNW.y() - (currentCenter.y() + screenNW.y() / scale);
    qreal boxBottom = mapSE.y() - (currentCenter.y() + screenSE.y() / scale);
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
        auto nwFactor = directDiv(screenNW, mapNW - currentCenter);
        auto seFactor = directDiv(screenSE, mapSE - currentCenter);
        auto minimalSimpleScale =
            max(max(nwFactor.x(), nwFactor.y()),
                max(seFactor.x(), seFactor.y()));

        qDebug() << "\nin decreaseScale";
        qDebug() << "scale:\t" << scale;
        qDebug() << "fu scale:\t" << futureScale;
        qDebug() << "minSmplScl:\t" << minimalSimpleScale;

        if(futureScale < minimalSimpleScale)
        {
            auto shift = QPointF(oneWay<neg>(getNW().x() - mapNW.x())
                                 + oneWay<pos>(getSE().x() - mapSE.x()),
                                 oneWay<pos>(getNW().y() - mapNW.y())
                                 + oneWay<neg>(getSE().y() - mapSE.y()));

            qDebug() << "shift:\t" << shift;
            currentCenter -= shift;
        }

        scale = futureScale;
        qDebug() << "\n";
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

QRectF WorldView::transformToScreenCordinates(QRectF rect, qreal adjust) const
{
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
