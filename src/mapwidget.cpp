#include "../include/mapwidget.h"

#include <QDebug>

#define CONTROL
#include <algorithm>


MapWidget::MapWidget(GraphicalMap &model, QWidget *parent) :
    QWidget(parent),
    model(model),
    worldView(nullptr),
    imageBufer(nullptr),
    fringe(200)
{
    fringedArea.setSize(size() + 2.0 * QSizeF(fringe, fringe));
    fringedArea.moveCenter(QPointF(width() / 2.0, height() / 2.0));

    worldView = new WorldView(model.mapBorder(), Area(0, 0, this->size()), 1.0);

    imageBufer  = new QImage(fringedArea.size().toSize(), QImage::Format_ARGB32_Premultiplied);

    // NOTE maybe all controll and interaction place in other class
    //setAttribute(Qt::WA_AcceptTouchEvents);
}

MapWidget::~MapWidget()
{
    delete imageBufer;
    imageBufer = nullptr;
    delete worldView;
    worldView = nullptr;
}

void MapWidget::paintEvent(QPaintEvent *event)
{
    qDebug() << worldView->scale();
    QPainter buferPainter(imageBufer);
    buferPainter.setRenderHint(QPainter::SmoothPixmapTransform);
    buferPainter.fillRect(0, 0, fringedArea.width(), fringedArea.height(), Qt::black);
    buferPainter.translate(fringe, fringe);

    qDebug() << "fringed:" << Area(fringedArea) << "bare:" << Area(0, 0, size());
    qDebug() << "bare inMap:" <<
             worldView->followingTransformator().transformToMapCordinates(Area(0, 0, size()));

    model.drawArea(buferPainter, Area(fringedArea), worldView->followingTransformator());

    QPainter mainPainter(this);
#ifdef CONTROL
    auto ims = imageBufer->size() + QSize(10, 10);
    qreal tmpScale = std::min((qreal)width() / ims.width(), (qreal)height() / ims.height());
    mainPainter.setTransform(QTransform(tmpScale, 0.0, 0.0, tmpScale, 5, 5));
    mainPainter.drawImage(0, 0, *imageBufer);
    mainPainter.translate(fringe, fringe);
    mainPainter.setPen(Qt::red);
    mainPainter.drawLine(0, 0, width(), 0);
    mainPainter.drawLine(width(), 0, width(), height());
    mainPainter.drawLine(width(), height(), 0, height());
    mainPainter.drawLine(0, height(), 0, 0);
#else
    mainPainter.drawImage(-fringe, -fringe, *imageBufer);
#endif

    Q_UNUSED(event);
}

void MapWidget::resizeEvent(QResizeEvent *event)
{
    fringedArea.setSize(size() + 2.0 * QSizeF(fringe, fringe));
    fringedArea.moveCenter(QPointF(width() / 2.0, height() / 2.0));
    auto newImageBufer  = new QImage(fringedArea.size().toSize(), QImage::Format_ARGB32_Premultiplied);
    delete imageBufer;
    imageBufer = newImageBufer;
    newImageBufer = nullptr;

    worldView->setScreenParameter(Area(0, 0, size()));

    Q_UNUSED(event);
}

void MapWidget::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_A:
        worldView->moveScreen(QPointF(-10.0, 0));
        repaint();
        break;

    case Qt::Key_D:
        worldView->moveScreen(QPointF(+10.0, 0));
        repaint();
        break;

    case Qt::Key_W:
        worldView->moveScreen(QPointF(0, -10.0));
        repaint();
        break;

    case Qt::Key_S:
        worldView->moveScreen(QPointF(0, +10.0));
        repaint();
        break;

    case Qt::Key_Down:
        worldView->modifyScale(1.01);
        repaint();
        break;

    case Qt::Key_Up:
        worldView->modifyScale(1.0 / 1.01);
        repaint();
        break;

    default:
        QWidget::keyPressEvent(event);
        break;
    }
}

/**
 * IOS & Android compability
*/
//bool MapWidget::event(QEvent *event)
//{
//    switch (event->type())
//    {
//    case QEvent::MouseButtonPress:
//    {
//        QMouseEvent * current_event = static_cast<QMouseEvent *>(event);
//        //d_origin = current_event->pos();
//        clickPos = current_event->pos();
//    }
//    case QEvent::MouseMove:
//    {
//        QMouseEvent * current_event = static_cast<QMouseEvent *>(event);

//        //origin.setX(origin.rx() + (current_event->pos().rx() - d_origin.rx()));
//        //origin.setY(origin.ry() + (current_event->pos().ry() - d_origin.ry()));
//        //d_origin = current_event->pos();
//        QPoint current = current_event->pos();
//        HG->addShift(current - clickPos);
//        clickPos = current;
//        update();
//        return true;
//    }

//    case QEvent::TouchBegin:
//    {
//        QTouchEvent * touchEvent = static_cast<QTouchEvent *>(event);
//        QList<QTouchEvent::TouchPoint> touchPoints = touchEvent->touchPoints();
//        if(touchPoints.count() == 1)
//        {
//            QTouchEvent::TouchPoint &point = touchPoints.first();
//            d_origin = QPoint((int)point.pos().rx(), (int)point.pos().ry());
//        }
//    }
//    case QEvent::TouchUpdate:
//    {
//        QTouchEvent *touchEvent = static_cast<QTouchEvent *>(event);
//        QList<QTouchEvent::TouchPoint> touchPoints = touchEvent->touchPoints();

//        if (touchPoints.count() == 1)
//        {
//            const QTouchEvent::TouchPoint &point = touchPoints.first();
//            origin.setX(origin.rx() + (point.pos().rx() - d_origin.rx()) );
//            origin.setY(origin.ry() + (point.pos().ry() - d_origin.ry()) );
//            d_origin = QPoint((int)point.pos().rx(), (int)point.pos().ry());
//            update();
//        }
//        else if (touchPoints.count() == 2)
//        {
//            const QTouchEvent::TouchPoint &touchPoint0 = touchPoints.first();
//            const QTouchEvent::TouchPoint &touchPoint1 = touchPoints.last();
//            qreal currentScaleFactor =
//                (QLineF(touchPoint0.pos(), touchPoint1.pos()).length()
//                 / QLineF(touchPoint0.startPos(), touchPoint1.startPos()).length());

//            if (currentScaleFactor > 1.0)
//                scale *= 1.02;
//            else
//                scale /= 1.02;

//            update();
//        }

//        return true;
//    }

//    case QEvent::TouchEnd:
//    default:
//        break;
//    }

//    return QWidget::event(event);
//}

