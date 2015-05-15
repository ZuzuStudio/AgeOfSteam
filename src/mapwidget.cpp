#include "../include/mapwidget.h"

#include <QDebug>

#define CONTROL
#include <algorithm>

// need draw transformed old bufer
// need draw strip

MapWidget::MapWidget(LogicalMap &model, QWidget *parent) :
    QWidget(parent),
    model(model),
    grid(nullptr),
    worldView(nullptr),
    imageBufer(nullptr),
    fringe(200),
    firstTime(0)
{
    fringedArea.setSize(size() + 2.0 * QSizeF(fringe, fringe));
    fringedArea.moveCenter(QPointF(width() / 2.0, height() / 2.0));

    grid = new HexagonalGrid(this->model.columns(), this->model.rows(), 128);//256 px full height of svg
    worldView = new WorldView(grid->mapBorder(), Area(0, 0, this->size()), 1.0);

    imageBufer  = new QImage(fringedArea.size().toSize(), QImage::Format_ARGB32_Premultiplied);
    savedImage  = new QImage(fringedArea.size().toSize(), QImage::Format_ARGB32_Premultiplied);

    auto lodSea = new LevelOfDetalisation(worldView->getScale());
    lodSea->addRenderer(QString(":/res/seeFlatLod1_res.svg"), 0.0);
    terrainTypes.push_back(lodSea);

    auto lodLand = new LevelOfDetalisation(worldView->getScale());
    lodLand->addRenderer(QString(":/res/landFlatLod1_res.svg"), 0.0);
    terrainTypes.push_back(lodLand);

    auto lodHill = new LevelOfDetalisation(worldView->getScale());
    lodHill->addRenderer(QString(":/res/hillFlatLod3_res.svg"), 0.0);
    lodHill->addRenderer(QString(":/res/hillFlatLod2_res.svg"), 0.1);
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
    qDebug() << worldView->getScale();
    QPainter buferPainter(imageBufer);
    buferPainter.setRenderHint(QPainter::SmoothPixmapTransform);
    //buferPainter.fillRect(0, 0, size().width(), size().height(), Qt::black);

    if(firstTime == 0)
    {
        QPainter savedPainter(savedImage);
        savedPainter.translate(fringe, 0);
        drawMapSubarea(&savedPainter, fringedArea.topLeft(), fringedArea.bottomRight());
        buferPainter.drawImage(0, 0, *savedImage);
        savedNW = worldView->transformToMapCordinates(fringedArea.topLeft());
        savedSE = worldView->transformToMapCordinates(fringedArea.bottomRight());
    }
    else
    {
        buferPainter.translate(0, fringe);
        auto screenSavedNW = worldView->transformToScreenCordinates(savedNW);
        auto screenSavedSE = worldView->transformToScreenCordinates(savedSE);

        drawOldBufer(&buferPainter, *savedImage, screenSavedNW, screenSavedSE);
        /*qDebug() << "overhead";
        auto ovNW = QPointF(fringedArea.left(), screenSavedNW.y());
        auto ovSE = fringedArea.topRight();
        qDebug() << ovNW << ovSE;
        qDebug() << worldView->transformToMapCordinates(ovNW) << worldView->transformToMapCordinates(ovSE);
        drawMapSubarea(&buferPainter, ovNW, ovSE);
        ovNW = fringedArea.bottomLeft();
        ovSE = QPointF(fringedArea.right(), screenSavedSE.y());
        drawMapSubarea(&buferPainter, ovNW, ovSE);
        ovNW = QPointF(screenSavedNW.x(), fringedArea.top());
        ovSE = fringedArea.bottomLeft();
        drawMapSubarea(&buferPainter, ovNW, ovSE);
        ovNW = fringedArea.topRight();
        ovSE = QPointF(screenSavedSE.x(), fringedArea.bottom());
        drawMapSubarea(&buferPainter, ovNW, ovSE);
        ovNW = QPointF(fringedArea.center().x()-20,fringedArea.top());
        ovSE = QPointF(fringedArea.center().x()+20,fringedArea.bottom());
        drawMapSubarea(&buferPainter, ovNW, ovSE);
        ovNW = QPointF(fringedArea.left(),fringedArea.center().y()-20);
        ovSE = QPointF(fringedArea.right(),fringedArea.center().y()+20);
        drawMapSubarea(&buferPainter, ovNW, ovSE);*/
    }

    firstTime = (firstTime + 1) % 10;



    qDebug() << "in paintEvent";
    qDebug() << *worldView;
    qDebug() << savedNW << savedSE;
    {
        auto screenSavedNW = worldView->transformToScreenCordinates(savedNW);
        auto screenSavedSE = worldView->transformToScreenCordinates(savedSE);
        auto screenSavedDiag = screenSavedNW - screenSavedSE;
        qDebug() << screenSavedNW << screenSavedSE << screenSavedDiag;
    }

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
    mainPainter.drawImage(0, 0, *imageBufer);
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
    firstTime = 0;

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

void MapWidget::drawMapSubarea(QPainter *painter, QPointF screenNW, QPointF screenSE)
{
    auto nwIndex = grid->indices(worldView->transformToMapCordinates(screenNW), QPointF(-1.0, -1.0)) - QPoint(1, 1);
    auto seIndex = grid->indices(worldView->transformToMapCordinates(screenSE), QPointF(+1.0, +1.0)) + QPoint(1, 1);
    auto diff = seIndex - nwIndex;
    qDebug() << diff << diff.x() * diff.y();

    // TODO in this part switch beetwen cluster and svg
    for(auto column = nwIndex.x(); column <= seIndex.x(); ++column)
        for(auto row = nwIndex.y(); row <= seIndex.y(); ++row)
        {
            terrainTypes[model.cell(column, row)]
            ->renderer()
            ->render(painter,
                     worldView->transformToScreenCordinates(grid->tilingBox(column, row)));
        }
}

void MapWidget::drawOldBufer(QPainter *painter, QImage image,
                             QPointF screenSavedNW, QPointF screenSavedSE)
{
    qDebug() << "in drawOldBufer";
    qDebug() << *worldView;
    qDebug() << savedNW << savedSE;
    painter->save();
    auto screenSavedDiag = screenSavedNW - screenSavedSE;
    qDebug() << screenSavedNW << screenSavedSE << screenSavedDiag;
    auto newNW = fringedArea.topLeft(), newSE = fringedArea.bottomRight();
    auto newDiag = newNW - newSE;
    auto sx = screenSavedDiag.x() / newDiag.x();
    auto sy = screenSavedDiag.y() / newDiag.y();
    auto dx = (screenSavedSE.x() * newNW.x() - screenSavedNW.x() * newSE.x()) / newDiag.x();
    auto dy = (screenSavedSE.y() * newNW.y() - screenSavedNW.y() * newSE.y()) / newDiag.y();
    qDebug() << sx << sy << dx << dy;
    painter->setTransform(QTransform(sx, 0.0, 0.0, sy, dx, dy));
    painter->drawImage(0, 0, image);
    painter->restore();
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

