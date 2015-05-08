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

WorldView::WorldView(qreal mapLeft, qreal mapRight, qreal mapTop, qreal mapBottom,
                     qreal screenLeft, qreal screenRight, qreal screenTop, qreal screenBottom,
                     qreal scale):
    mapNW(mapLeft, mapTop),
    mapSE(mapRight, mapBottom),
    mapViewCenter((mapNW + mapSE) / 2.0),
    scale(scale),
    maximalScale(10.0)// TODO without magic number
{
    setScreenParameter(screenLeft, screenRight, screenTop, screenBottom);
}

void WorldView::setScreenParameter(qreal screenLeft, qreal screenRight, qreal screenTop, qreal screenBottom)
{
    screenNW = QPointF(screenLeft, screenTop);
    screenSE = QPointF(screenRight, screenBottom);
    screenViewCenter = (screenNW + screenSE) / 2.0;
    auto screenDiagonal = screenNW - screenSE;
    auto mapDiagonal = mapNW - mapSE;
    auto diagonalFactor = directDiv(screenDiagonal,  mapDiagonal);
    minimalScale = max(diagonalFactor.x(), -diagonalFactor.y());
    restoreCorrectness();
}

void WorldView::moveScreen(QPointF screenShift)
{
    auto mapShift = screenShift / scale;
    mapShift.ry() *= -1.0;
    mapViewCenter += mapShift;
    restoreCorrectness();
}

void WorldView::decreaseScale()
{
    scale /= 1.01;
    restoreCorrectness();
}

void WorldView::increaseScale()
{
    scale *= 1.01;
    restoreCorrectness();
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

void WorldView::restoreCorrectness()
{
    // N.B! order between scale and move adjustment is essential
    if(scale < minimalScale)
        scale = minimalScale;

    if(scale > maximalScale)
        scale = maximalScale;

    auto overhead = getNW().x() - mapNW.x();

    if(overhead < 0.0)
        mapViewCenter -= QPointF(overhead, 0.0);

    overhead = getNW().y() - mapNW.y();

    if(overhead > 0.0)
        mapViewCenter -= QPointF(0.0, overhead);

    overhead = getSE().x() - mapSE.x();

    if(overhead > 0.0)
        mapViewCenter -= QPointF(overhead, 0.0);

    overhead = getSE().y() - mapSE.y();

    if(overhead < 0.0)
        mapViewCenter -= QPointF(0.0, overhead);
}
