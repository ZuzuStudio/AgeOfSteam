#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QWidget>
#include <QtSvg/QSvgRenderer>
#include <QImage>
#include <QPainter>
#include <QKeyEvent>
#include <vector>
#include "include/logicalmap.h"
#include "include/hexagonalgrid.h"
#include "include/worldview.h"
#include "include/levelofdetalisation.h"


class MapWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MapWidget(LogicalMap &model, QWidget *parent = nullptr);
    ~MapWidget();


protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    //bool event(QEvent *event);
private:
    LogicalMap &model;
    HexagonalGrid *grid;
    WorldView *worldView;
    std::vector<LevelOfDetalisation *> terrainTypes; // NOTE maybe place in other class
    QImage *imageBufer;
};

#endif // MAPWIDGET_H
