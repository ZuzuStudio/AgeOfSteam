#include "../include/worldview.h"
#include <algorithm>

using namespace std;


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
