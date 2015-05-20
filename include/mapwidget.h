#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QWidget>
#include <QImage>
#include <QPainter>
#include <QKeyEvent>
#include <vector>
#include "include/graphicalmap.h"
#include "include/worldview.h"



class MapWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MapWidget(GraphicalMap &model, QWidget *parent = nullptr);
    ~MapWidget();


protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    //bool event(QEvent *event);
private:
    GraphicalMap &model;
    WorldView *worldView;
    QImage *imageBufer;
    int fringe;
    QRectF fringedArea;
};

#endif // MAPWIDGET_H
