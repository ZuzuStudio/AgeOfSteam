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
        explicit MapWidget(QWidget *parent = 0);
        void setScale(double scale)
        {
            this->scale = scale;
        }
        ~MapWidget();


    protected:
        void paintEvent(QPaintEvent *event);
        void keyPressEvent(QKeyEvent *event);
        virtual bool event(QEvent *event);


    private:
        HexagonalGrid * HG;

        //---translating around point
        QPoint origin;
        QPoint d_origin;
        QSize pivotPoint;

        QPoint clickPos;

        QSvgRenderer *renderer;
        QImage *image;
        double scale;
};

#endif // MAPWIDGET_H
