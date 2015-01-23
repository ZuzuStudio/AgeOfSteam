#include "graficitem.h"


GraficItem::GraficItem(typename QList<LevelOfDetalisation *>::iterator from, typename QList<LevelOfDetalisation *>::iterator to)
{
    for(QList<LevelOfDetalisation *>::iterator it = from; it != to; ++it)
        lod.append(*it);
}

/*void GraficItem::draw(QPainter &painter, QPointF center, double characteristicSize) const
{
    for(QList<LevelOfDetalisation *>::iterator it = lod.begin(), end = lod.end(); it != end; ++it)
    {

    }
}*/
