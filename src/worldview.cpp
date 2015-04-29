#include "../include/worldview.h"
#include <algorithm>

using namespace std;


WorldView::WorldView(int mapLeft, int mapRight, int mapTop, int mapBottom,
                     int screenLeft, int screenRight, int screenTop, int screenBottom,
                     qreal scale):
    mapLeft(mapLeft),
    mapRight(mapRight),
    mapTop(mapTop),
    mapBottom(mapBottom),
    currentCenter((mapLeft + mapRight) / 2, (mapTop + mapBottom) / 2),
    scale(scale),
    maximalScale(10.0)// TODO without magic number
{
    auto horizontalMinimalScale = (qreal)(mapRight - mapLeft)
                                  / (qreal)(screenRight - screenLeft);
    auto verticalMinimalScale = (qreal)(mapTop - mapBottom)
                                / (qreal)(screenTop - screenBottom);
    minimalScale = max(horizontalMinimalScale, verticalMinimalScale);
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
    //TODO
}

void WorldView::increaseScale()
{
    auto futureScale = scale * 1.01;
    if(futureScale > maximalScale)
        scale = maximalScale;
    else
        scale = futureScale;
}
