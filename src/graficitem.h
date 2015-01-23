#ifndef GRAFICITEM_H
#define GRAFICITEM_H

#include <QPainter>
#include <QPointF>
#include <QList>
#include "levelofdetalisation.h"

class GraficItem
{
    public:
        GraficItem(typename QList<LevelOfDetalisation *>::iterator from, typename QList<LevelOfDetalisation *>::iterator to);

        //void draw(QPainter &painter, QPointF center, double characteristicSize) const;
    private:
        QList<LevelOfDetalisation *> lod;

};

#endif // GRAFICITEM_H
