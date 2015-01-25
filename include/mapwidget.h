#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QWidget>
#include <QtSvg/QSvgRenderer>
#include <QImage>
#include <QPainter>
#include <QKeyEvent>
#include <QScrollArea>

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
        //---scale factor nessesor for map clasters
        const qreal scaleFactor = 120.0;
        const int numberOfClasters = 30;

        //---translating around point
        QPoint origin;
        QPoint d_origin;
        QSize pivotPoint;

        QSvgRenderer *renderer;
        QImage *image;
        double scale;
        std::vector<QImage> painters;
};

#endif // MAPWIDGET_H
