#include "../include/mapwidget.h"

MapWidget::MapWidget(QWidget *parent) :
    QWidget(parent),
    origin(0, 0),
    renderer(nullptr),
    image(nullptr),
    scale(20)
{
    image  = new QImage(size(), QImage::Format_ARGB32_Premultiplied);
    renderer = new QSvgRenderer(QString(":/res/Land.svg"),
                                this);//QString("../../hillFlat_res.svg")//QString("../src/files/bubbles.svg")
    //---------------//
    connect(renderer, SIGNAL(repaintNeeded()), this, SLOT(repaint()));
    setAttribute(Qt::WA_AcceptTouchEvents);

    HG = new HexagonalGrid(scale);
    HG->drawRastr(renderer);

}

MapWidget::~MapWidget()
{
    delete image;
    image = nullptr;
    delete HG;
    HG = nullptr;
}

void MapWidget::paintEvent(QPaintEvent *event)
{
    QPainter p(this);

    if (scale < 0.35) // full map rastr
    {
        p.scale(scale, scale);
        HG->setScale(scale);
        HG->gluingTogetherClasters(&p);
    }
    else // we need to show svg for low detalised texture
    {
        if (image->size() != this->size())
        {
            delete image;
            image = new QImage(size(), QImage::Format_ARGB32_Premultiplied);
        }

        QPainter imagePainter(image);
        imagePainter.fillRect(0, 0, size().width(), size().height(), Qt::white);
        HG->setScale(scale);
        HG->drawSVG(renderer, &imagePainter);
        //-------------//
        p.drawImage(0, 0, *image);
    }
    Q_UNUSED(event);
}

void MapWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_A:
        scale *= 1.05;
        repaint();
        break;

    case Qt::Key_D:
        scale /= 1.05;
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
bool MapWidget::event(QEvent *event)
{
    switch (event->type())
    {
    case QEvent::MouseButtonPress:
    {
        QMouseEvent * current_event = static_cast<QMouseEvent *>(event);
        //d_origin = current_event->pos();
        clickPos = current_event->pos();
    }
    case QEvent::MouseMove:
    {
        QMouseEvent * current_event = static_cast<QMouseEvent *>(event);

        //origin.setX(origin.rx() + (current_event->pos().rx() - d_origin.rx()));
        //origin.setY(origin.ry() + (current_event->pos().ry() - d_origin.ry()));
        //d_origin = current_event->pos();
        QPoint current = current_event->pos();
        HG->addShift(current - clickPos);
        clickPos = current;
        update();
        return true;
    }

    case QEvent::TouchBegin:
    {
        QTouchEvent * touchEvent = static_cast<QTouchEvent *>(event);
        QList<QTouchEvent::TouchPoint> touchPoints = touchEvent->touchPoints();
        if(touchPoints.count() == 1)
        {
            QTouchEvent::TouchPoint &point = touchPoints.first();
            d_origin = QPoint((int)point.pos().rx(), (int)point.pos().ry());
        }
    }
    case QEvent::TouchUpdate:
    {
        QTouchEvent *touchEvent = static_cast<QTouchEvent *>(event);
        QList<QTouchEvent::TouchPoint> touchPoints = touchEvent->touchPoints();

        if (touchPoints.count() == 1)
        {
            const QTouchEvent::TouchPoint &point = touchPoints.first();
            origin.setX(origin.rx() + (point.pos().rx() - d_origin.rx()) );
            origin.setY(origin.ry() + (point.pos().ry() - d_origin.ry()) );
            d_origin = QPoint((int)point.pos().rx(), (int)point.pos().ry());
            update();
        }
        else if (touchPoints.count() == 2)
        {
            const QTouchEvent::TouchPoint &touchPoint0 = touchPoints.first();
            const QTouchEvent::TouchPoint &touchPoint1 = touchPoints.last();
            qreal currentScaleFactor =
                (QLineF(touchPoint0.pos(), touchPoint1.pos()).length()
                 / QLineF(touchPoint0.startPos(), touchPoint1.startPos()).length());

            if (currentScaleFactor > 1.0)
                scale *= 1.02;
            else
                scale /= 1.02;

            update();
        }

        return true;
    }

    case QEvent::TouchEnd:
    default:
        break;
    }

    return QWidget::event(event);
}

