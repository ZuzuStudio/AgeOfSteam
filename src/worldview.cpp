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
    mapViewCenter((mapNW + mapSE) / 2.0),
    screenViewCenter((screenNW + screenSE) / 2.0),
    scale(scale),
    maximalScale(10.0)// TODO without magic number
{
    qDebug() << "screenNW" << screenNW;
    qDebug() << "screenSE" << screenSE;
    qDebug() << "screenVC" << screenViewCenter;
    auto screenDiagonal = screenNW - screenSE;
    qDebug() << "screenDiagonal" << screenDiagonal;
    auto mapDiagonal = mapNW - mapSE;
    qDebug() << "mapDiagonal" << mapDiagonal;
    auto diagonalFactor = directDiv(screenDiagonal,  mapDiagonal);
    qDebug() << "diagonal factor" << diagonalFactor;
    minimalScale = max(diagonalFactor.x(), -diagonalFactor.y());
    qDebug() << "minimal scale" << minimalScale;
}

void WorldView::moveScreen(QPointF screenShift)
{
    //qDebug() << "screenShift" << screenShift;
    QPointF mapShift = screenShift / scale;
    //qDebug() << "mapShift" << mapShift;
    QPointF boxNW = mapNW - getNW();
    QPointF boxSE = mapSE - getSE();
    //qDebug() << "move box" << boxLeft << boxRight << boxTop << boxBottom;
    int sector = 0;
    sector += mapShift.x() < boxNW.x() ? -1 : (mapShift.x() > boxSE.x() ? 1 : 0);
    sector += mapShift.y() < boxSE.y() ? -3 : (mapShift.y() > boxNW.y() ? 3 : 0);
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

    mapViewCenter += boxedShift;
}

void WorldView::decreaseScale()
{
    auto futureScale = scale / 1.01;

    if(futureScale > minimalScale)
    {
        qDebug() << "\nin decreaseScale";
        auto nwFactor = directDiv(screenNW - screenViewCenter, mapNW - mapViewCenter);
        auto seFactor = directDiv(screenSE - screenViewCenter, mapSE - mapViewCenter);
        qDebug() << "nwFactor" << nwFactor;
        qDebug() << "seFactor" << seFactor;
        auto minimalSimpleScale =
            max(max(nwFactor.x(), -nwFactor.y()),
                max(seFactor.x(), -seFactor.y()));

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
            mapViewCenter -= shift;
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
    return QPointF(screenViewCenter.x() + (point.x() - mapViewCenter.x()) * scale,
                   screenViewCenter.y() + (mapViewCenter.y() - point.y()) * scale);
}

QPointF WorldView::transformToMapCordinates(const QPointF &point) const
{
    return QPointF(mapViewCenter.x() + (point.x() - screenViewCenter.x()) / scale,
                   mapViewCenter.y() + (screenViewCenter.y() - point.y()) / scale);
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
