#include "include/graphicalmap.h"
#include <QtSvg/QSvgRenderer>

#include <QDebug>

using namespace std;

GraphicalMap::GraphicalMap(LogicalMap &model):
    model(model),
    grid(nullptr)
{
    grid = new HexagonalGrid(this->model.columns(), this->model.rows(), 128);//256 px full height of svg

    auto lodSea = new LevelOfDetalisation();
    lodSea->addRenderer(QString(":/res/seeFlatLod1_res.svg"), 0.0);
    terrainTypes.push_back(lodSea);

    auto lodLand = new LevelOfDetalisation();
    lodLand->addRenderer(QString(":/res/landFlatLod1_res.svg"), 0.0);
    terrainTypes.push_back(lodLand);

    auto lodHill = new LevelOfDetalisation();
    lodHill->addRenderer(QString(":/res/hillFlatLod3_res.svg"), 0.0);
    lodHill->addRenderer(QString(":/res/hillFlatLod2_res.svg"), 0.1);
    lodHill->addRenderer(QString(":/res/hillFlatLod1_res.svg"), 0.5);
    terrainTypes.push_back(lodHill);
}

GraphicalMap::~GraphicalMap()
{
    for(auto it : terrainTypes)
        delete it;

    delete grid;
    grid = nullptr;
}

void GraphicalMap::drawArea(QPainter &painter, const Area &inScreenArea,
                            const TransformatorInterface &trator) const
{
    auto inMapArea = trator.transformToMapCordinates(inScreenArea);
    qDebug() << "inMap:" << inMapArea;
    qDebug() << "mapBorder" << grid->mapBorder();
    qDebug() << "nwMapBorder cell area" << grid->cellArea(0, 0);
    qDebug() << "nwMapBorder cell area inScreen:" << trator.transformToScreenCordinates(grid->cellArea(0, 0));
    auto nwIndex = grid->indices(inMapArea.nw(), QPointF(-1.0, -1.0)) - QPoint(1, 1);
    auto seIndex = grid->indices(inMapArea.se(), QPointF(+1.0, +1.0)) + QPoint(1, 1);
    auto diff = seIndex - nwIndex;
    qDebug() << "nwi:" << nwIndex << "sei:" << seIndex;
    qDebug() << diff << diff.x() * diff.y();
    qDebug() << Area(grid->cellCenter(nwIndex.x(), nwIndex.y()), grid->cellCenter(seIndex.x(), seIndex.y()));


    for(auto column = nwIndex.x(); column <= seIndex.x(); ++column)
        for(auto row = nwIndex.y(); row <= seIndex.y(); ++row)
        {
            terrainTypes[model.cell(column, row)]
            ->renderer(trator.scale())
            ->render(&painter,
                     trator
                     .transformToScreenCordinates(grid->cellArea(column, row))
                     .adjust(0.5)
                     .toRectF());
        }
}
