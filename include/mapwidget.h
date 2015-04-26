#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QWidget>
#include <QtSvg/QSvgRenderer>
#include <QImage>
#include <QPainter>
#include <QKeyEvent>
#include <include/hexagonalgrid.h>

class MapWidget : public QWidget
{
        Q_OBJECT
    public:
        explicit MapWidget(QWidget *parent = nullptr);
        void setScale(double scale);
        ~MapWidget();


    protected:
        void paintEvent(QPaintEvent *event);
        void keyPressEvent(QKeyEvent *event);
        bool event(QEvent *event);


    private:
        HexagonalGrid * HG;
        QPoint leftTop, rightTop, rightBottom, leftBottom, center;
        void calculateCenter();

        //---translating around point
        QPoint origin;
        QPoint d_origin;
        QPoint clickPos;

        QSvgRenderer *renderer;
        QImage *image;
        double scale;
};

#endif // MAPWIDGET_H
