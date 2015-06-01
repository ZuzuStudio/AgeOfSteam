#include "include/transformator.h"
#include "include/area.h"


TransformatorInterface::~TransformatorInterface() {}

AbstractTransformator::~AbstractTransformator() {}

QPointF AbstractTransformator::transformToScreenCoordinates(const QPointF &point) const
{
    return QPointF(screenViewCenter.x() + (point.x() - mapViewCenter.x()) * scalePrivate,
                   screenViewCenter.y() + (mapViewCenter.y() - point.y()) * scalePrivate);
}

QPointF AbstractTransformator::transformToMapCoordinates(const QPointF &point) const
{
    return QPointF(mapViewCenter.x() + (point.x() - screenViewCenter.x()) / scalePrivate,
                   mapViewCenter.y() + (screenViewCenter.y() - point.y()) / scalePrivate);
}

Area AbstractTransformator::transformToScreenCoordinates(const Area &area) const
{
    return Area(transformToScreenCoordinates(area.nw()),
                transformToScreenCoordinates(area.se()));
}

Area AbstractTransformator::transformToMapCoordinates(const Area &area) const
{
    return Area(transformToMapCoordinates(area.nw()),
                transformToMapCoordinates(area.se()));
}



