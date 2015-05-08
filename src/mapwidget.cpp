#include "../include/mapwidget.h"

#include <QDebug>

MapWidget::MapWidget(LogicalMap &model, QWidget *parent) :
    QWidget(parent),
    model(model),
    grid(nullptr),
    worldView(nullptr),
    imageBufer(nullptr)
{
    grid = new HexagonalGrid(this->model.columns(), this->model.rows(), 128);//256 px full height of svg
    worldView = new WorldView(grid->leftMapBorder(), grid->rightMapBorder(),
                              grid->topMapBorder(), grid->bottomMapBorder(),
                              -this->size().width() / 2, this->size().width() / 2,
                              this->size().height() / 2, -this->size().height() / 2,
                              1.0);
    imageBufer  = new QImage(size(), QImage::Format_ARGB32_Premultiplied);

    auto lodSea = new LevelOfDetalisation(worldView->getScale());
    lodSea->addRenderer(QString(":/res/seeFlatLod1_res.svg"), 0.0);
    terrainTypes.push_back(lodSea);

    auto lodHill = new LevelOfDetalisation(worldView->getScale());
    lodHill->addRenderer(QString(":/res/hillFlatLod3_res.svg"), 0.0);
    lodHill->addRenderer(QString(":/res/hillFlatLod2_res.svg"), 0.25);
    lodHill->addRenderer(QString(":/res/hillFlatLod1_res.svg"), 0.5);
    terrainTypes.push_back(lodHill);

    // NOTE maybe all controll and interaction place in other class
    //setAttribute(Qt::WA_AcceptTouchEvents);
}

MapWidget::~MapWidget()
{
    delete imageBufer;
    imageBufer = nullptr;

    for(auto it : terrainTypes)
        delete it;

    delete worldView;
    worldView = nullptr;
    delete grid;
    grid = nullptr;
}

void MapWidget::paintEvent(QPaintEvent *event)
{
    QPainter buferPainter(imageBufer);
    buferPainter.fillRect(0, 0, size().width(), size().height(), Qt::white);
    buferPainter.translate(width() / 2.0, height() / 2.0);

    qDebug() << "coordinate";
    qDebug() << "nw: " << worldView->getNW() << ", se: " << worldView->getSE();
    qDebug() << "scale: " << worldView->getScale();

    auto nwIndex = grid->indices(worldView->getNW(), QPointF(-1.0, -1.0)) - QPoint(1, 1);
    auto seIndex = grid->indices(worldView->getSE(), QPointF(+1.0, +1.0)) + QPoint(1, 1);

    qDebug() << "index";
    qDebug() << "nw: " << nwIndex << ", se: " << seIndex;

    // TODO in this part switch beetwen cluster and svg
    for(auto column = nwIndex.x(); column <= seIndex.x(); ++column)
        for(auto row = nwIndex.y(); row <= seIndex.y(); ++row)
        {
            //qDebug() << "cl: " << column << "rw: " << row;
            //qDebug() << "cell " << model.cell(column, row);
            //qDebug() << "LOD: " << terrainTypes[model.cell(column, row)];
            //qDebug() << "renderer" << terrainTypes[model.cell(column, row)]->renderer();
            terrainTypes[model.cell(column, row)]
            ->renderer()
            ->render(&buferPainter,
                     worldView->transformToScreenCordinates(grid->tilingBox(column, row)));
        }

    QPainter mainPainter(this);
    mainPainter.drawImage(0, 0, *imageBufer);

//    if (scale < 0.35) // full map rastr
//    {
//        p.scale(scale, scale);
//        HG->setScale(scale);
//        //HG->draw(&p);
//        //HG->gluingTogetherClasters(&p);
//    }
//    else // we need to show svg for low detalised texture
//    {
//        if (image->size() != this->size())
//        {
//            delete image;
//            image = new QImage(size(), QImage::Format_ARGB32_Premultiplied);
//        }

//        QPainter imagePainter(image);
//        imagePainter.fillRect(0, 0, size().width(), size().height(), Qt::white);
//        HG->setScale(scale);
//        HG->drawSVG(renderer, &imagePainter);
//        //-------------//
//        p.drawImage(0, 0, *image);
//    }
    Q_UNUSED(event);
}

void MapWidget::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_A:
        worldView->moveScreen(QPointF(+10.0, 0));
        repaint();
        break;

    case Qt::Key_D:
        worldView->moveScreen(QPointF(-10.0, 0));
        repaint();
        break;

    case Qt::Key_W:
        worldView->moveScreen(QPointF(0, +10.0));
        repaint();
        break;

    case Qt::Key_S:
        worldView->moveScreen(QPointF(0, -10.0));
        repaint();
        break;

    case Qt::Key_Down:
        worldView->increaseScale();
        repaint();
        break;

    case Qt::Key_Up:
        worldView->decreaseScale();
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

