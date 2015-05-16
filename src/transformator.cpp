#include "../include/transformator.h"


TransformatorInterface::~TransformatorInterface() {}

AbstractTransformator::~AbstractTransformator() {}

QPointF AbstractTransformator::transformToScreenCordinates(const QPointF &point) const
{
    return QPointF(screenViewCenter.x() + (point.x() - mapViewCenter.x()) * scale,
                   screenViewCenter.y() + (mapViewCenter.y() - point.y()) * scale);
}

QPointF AbstractTransformator::transformToMapCordinates(const QPointF &point) const
{
    return QPointF(mapViewCenter.x() + (point.x() - screenViewCenter.x()) / scale,
                   mapViewCenter.y() + (screenViewCenter.y() - point.y()) / scale);
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



