#ifndef GRAPHICALMAP_H
#define GRAPHICALMAP_H

#include "include/area.h"
#include "include/hexagonalgrid.h"

class QPainter;
class Area;
class TransformatorInterface;
class LogicalMap;
class HexagonalGrid;
class LevelOfDetalisation;

class GraphicalMap final
{
public:
    GraphicalMap(LogicalMap &model);
    GraphicalMap(const GraphicalMap &original) = delete;
    GraphicalMap &operator=(const GraphicalMap &rhs) = delete;
    GraphicalMap(GraphicalMap &&victim) = delete;
    GraphicalMap &operator=(GraphicalMap &&rhs) = delete;
    ~GraphicalMap();

    Area mapBorder()const
    {
        return grid->mapBorder();
    }

    void drawArea(QPainter &painter, const Area &inScreenArea,
                  const TransformatorInterface *trator) const;

private:
    LogicalMap &model;
    HexagonalGrid *grid;
    std::vector<LevelOfDetalisation *> terrainTypes;
};

#endif // GRAPHICALMAP_H

