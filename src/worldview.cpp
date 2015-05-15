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

WorldView::WorldView(Area mapArea, Area screenArea, qreal scale):
    mapArea(mapArea),
    screenArea(screenArea),
    mapViewCenter((mapArea.nw() + mapArea.se()) / 2.0),
    scale(scale),
    maximalScale(10.0)// TODO without magic number
{
    applyNewScreenParameter();
}

void WorldView::setScreenParameter(Area newScreenArea)
{
    screenArea = newScreenArea;
    applyNewScreenParameter();
}

void WorldView::applyNewScreenParameter()
{
    screenViewCenter = (screenArea.nw() + screenArea.se()) / 2.0;
    auto screenDiagonal = screenArea.nw() - screenArea.se();
    auto mapDiagonal = mapArea.nw() - mapArea.se();
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

    auto overhead = getNW().x() - mapArea.nw().x();

    if(overhead < 0.0)
        mapViewCenter -= QPointF(overhead, 0.0);

    overhead = getNW().y() - mapArea.nw().y();

    if(overhead > 0.0)
        mapViewCenter -= QPointF(0.0, overhead);

    overhead = getSE().x() - mapArea.se().x();

    if(overhead > 0.0)
        mapViewCenter -= QPointF(overhead, 0.0);

    overhead = getSE().y() - mapArea.se().y();

    if(overhead < 0.0)
        mapViewCenter -= QPointF(0.0, overhead);
}
