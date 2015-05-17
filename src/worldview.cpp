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
    scalePrivate(scale),
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
    auto mapShift = screenShift / scalePrivate;
    mapShift.ry() *= -1.0;
    mapViewCenter += mapShift;
    restoreCorrectness();
}

void WorldView::modifyScale(qreal multiplayer)
{
    scalePrivate *= multiplayer;
    restoreCorrectness();
}

void WorldView::restoreCorrectness()
{
    // N.B! order between scale and move adjustment is essential
    if(scalePrivate < minimalScale)
        scalePrivate = minimalScale;

    if(scalePrivate > maximalScale)
        scalePrivate = maximalScale;

    auto tr = followingTransformator();
    auto inMapScreenArea = tr.transformToMapCordinates(screenArea);

    auto overhead = inMapScreenArea.nw().x() - mapArea.nw().x();

    if(overhead < 0.0)
        mapViewCenter -= QPointF(overhead, 0.0);

    overhead = inMapScreenArea.nw().y() - mapArea.nw().y();

    if(overhead > 0.0)
        mapViewCenter -= QPointF(0.0, overhead);

    overhead = inMapScreenArea.se().x() - mapArea.se().x();

    if(overhead > 0.0)
        mapViewCenter -= QPointF(overhead, 0.0);

    overhead = inMapScreenArea.se().y() - mapArea.se().y();

    if(overhead < 0.0)
        mapViewCenter -= QPointF(0.0, overhead);
}
