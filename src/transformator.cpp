#include "include/transformator.h"
#include "include/area.h"


TransformatorInterface::~TransformatorInterface() {}

AbstractTransformator::~AbstractTransformator() {}

QPointF AbstractTransformator::transformToScreenCordinates(const QPointF &point) const
{
    return QPointF(screenViewCenter.x() + (point.x() - mapViewCenter.x()) * scalePrivate,
                   screenViewCenter.y() + (mapViewCenter.y() - point.y()) * scalePrivate);
}

QPointF AbstractTransformator::transformToMapCordinates(const QPointF &point) const
{
    return QPointF(mapViewCenter.x() + (point.x() - screenViewCenter.x()) / scalePrivate,
                   mapViewCenter.y() + (screenViewCenter.y() - point.y()) / scalePrivate);
}

Area AbstractTransformator::transformToScreenCordinates(const Area &area) const
{
    return Area(transformToScreenCordinates(area.nw()),
                transformToScreenCordinates(area.se()));
}

Area AbstractTransformator::transformToMapCordinates(const Area &area) const
{
    return Area(transformToMapCordinates(area.nw()),
                transformToMapCordinates(area.se()));
}



