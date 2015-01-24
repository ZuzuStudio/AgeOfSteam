#include "mapwidget.h"
#include <QDebug>
#include <qmath.h>
#include <qsvggenerator.h>
#include <qfiledialog.h>

MapWidget::MapWidget(QWidget *parent) :
    QWidget(parent),
    origin(0, 0),
    pivotPoint(sqrt(3.0) * scaleFactor / 2.0 + sqrt(3.0) * scaleFactor / 4.0, scaleFactor),
    renderer(nullptr),
    image(nullptr),
    scale(scaleFactor),
    painters(0)
{
    QSize currentSize(10 * sqrt(3.0) * scale / 2.0 + sqrt(3.0) * scale / 4.0, 10 * scale);
    image = new QImage(currentSize, QImage::Format_ARGB32_Premultiplied);
    renderer = new QSvgRenderer(QString(":/files/hillFlatLod1_res.svg"),
                                this);//QString("../../hillFlat_res.svg")//QString("../src/files/bubbles.svg")
    //---------------//
    connect(renderer, SIGNAL(repaintNeeded()), this, SLOT(repaint()));
    setAttribute(Qt::WA_AcceptTouchEvents);

    for (int k = 0; k < numberOfClasters; ++k)
    {
        QImage currentImage(currentSize, QImage::Format_ARGB32_Premultiplied);
        QPainter * imagePainter = new QPainter(&currentImage);

        for (int j = 0; j < 15; ++j)
        {
            for (int i = 0; i < 15; ++i)
            {
                renderer->render(imagePainter, QRectF(sqrt(3.0) * scale * i / 2.0 + (j & 1) * sqrt(3.0) * scale / 4.0 - 1,
                                                      scale * 0.75 * j - 1 , 2 + scale, 2 + scale));
            }
        }

        painters.push_back(currentImage);
        delete imagePainter;
    }
    origin = QPoint(size().width() / 2, size().height() / 2);
}

MapWidget::~MapWidget()
{
    delete image;
    image = nullptr;
}

void MapWidget::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    //p.translate(origin);
    int counter = 1;
    int width = 0;
    int height = 0;

    if (scale < 0.35) // full map rastr
    {
        p.scale(scale, scale);
        p.drawImage(-pivotPoint.width() * 0.5, - pivotPoint.height() * 0.5, painters.at(0));

        while (counter < numberOfClasters)
        {
            QImage previous = painters.at(counter - 1);

            if (counter % 10 != 0)
            {
                width += previous.size().width();
                if(height == 0)
                {
                    p.drawImage(width -pivotPoint.width() * 0.5 - pivotPoint.width() * (counter % 10),
                                - pivotPoint.height() * 0.5, painters.at(counter));
                }
                else
                {
                    p.drawImage(width -pivotPoint.width() * 0.5 - pivotPoint.width() * (counter % 10),
                                height - pivotPoint.height() * 3 * (height / previous.size().height()), painters.at(counter));
                }
            }
            else
            {
                height += previous.size().height();
                width = 0;
                p.drawImage(-pivotPoint.width() * 0.5, height - pivotPoint.height() * 3 * (height / previous.size().height()), painters.at(counter));
            }

            ++counter;
        }
    }
    else // we need to show svg for low detalised texture
    {
        qreal width = 100 * (sqrt(3.0) * scale / 2.0 + sqrt(3.0) * scale / 4.0);
        qreal height = scale * 75;
        if (image->size() != this->size())
        {
            delete image;
            image = new QImage(size(), QImage::Format_ARGB32_Premultiplied);
        }

        QPainter imagePainter(image);
        imagePainter.fillRect(0, 0, size().width(), size().height(), Qt::white);
        for (int j = 0; j < static_cast<int>(size().width() / width) + 2; ++j)
            for (int i = 0; i < static_cast<int>(size().height() / height) + 1; ++i)
                renderer->render(&imagePainter, QRectF(sqrt(3.0) * scale * scaleFactor * i / 2.0 + (j & 1) *
                                                       sqrt(3.0) * scale * scaleFactor / 4.0 - 1,
                                                       scale * scaleFactor * 0.75 * j - 1 ,
                                                       2 + scaleFactor * scale ,
                                                       2 + scaleFactor * scale));

        imagePainter.end();
        //-------------//
        //p.translate(size().width() / 2, size().height() / 2);
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
        d_origin = current_event->pos();
    }
    case QEvent::MouseMove:
    {
        QMouseEvent * current_event = static_cast<QMouseEvent *>(event);

        origin.setX(origin.rx() + (current_event->pos().rx() - d_origin.rx()));
        origin.setY(origin.ry() + (current_event->pos().ry() - d_origin.ry()));
        d_origin = current_event->pos();
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
