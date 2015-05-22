#include "include/mapwidget.h"

#include <QKeyEvent>

#include "include/graphicalmap.h"
#include "include/framememoisator.h"
#include "include/worldview.h"

MapWidget::MapWidget(GraphicalMap &model, QWidget *parent) :
    QWidget(parent),
    memoisator(nullptr),
    worldView(nullptr)
{
    memoisator = new FrameMemoisator(model, Area(0, 0, size()));
    worldView = new WorldView(model.mapBorder(), Area(0, 0, size()), 1.0);
    // NOTE maybe all controll and interaction place in other class
    //setAttribute(Qt::WA_AcceptTouchEvents);
}

MapWidget::~MapWidget()
{
    delete memoisator;
    memoisator = nullptr;
    delete worldView;
    worldView = nullptr;
}

void MapWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    memoisator->drawFrame(painter, *worldView);
    Q_UNUSED(event);
}

void MapWidget::resizeEvent(QResizeEvent *event)
{
    memoisator->resizeScreen(Area(0, 0, size()));
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

