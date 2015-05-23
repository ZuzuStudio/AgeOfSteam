#include "include/framememoisator.h"

#include <QImage>
#include <QPainter>
#include "include/graphicalmap.h"
#include "include/worldview.h"

#include <QDebug>

#define CONTROL

Area calculateFringedArea(const Area &screenGeometry, int fringe);
QTransform scaleAndPlaceInScreenCenter(qreal quot, const Area &area);

FrameMemoisator::FrameMemoisator(GraphicalMap &model, const WorldView &worldView):
    model(model),
    worldView(worldView),
    fringe(200),
    fringedArea(calculateFringedArea(worldView.screenParameter(), fringe))
{
    frames.push_back(Frame(this->worldView.fixedScaleTransformator(0.8),
                           new QImage(fringedArea.toRectF().toRect().size(),
                                      QImage::Format_ARGB32_Premultiplied)));
}

void FrameMemoisator::drawFrame(QPainter &painter)
{
    qDebug() << worldView.scale();
    QPainter buferPainter(frames.back().image.get());
    //buferPainter.setRenderHint(QPainter::SmoothPixmapTransform);
    //buferPainter.fillRect(fringedArea.toRectF().toRect(), Qt::black);
    buferPainter.translate(fringe, fringe);

    model.drawArea(buferPainter, fringedArea, frames.back().trator.get());

    drawSelectedFrame(painter, 0);
}

void FrameMemoisator::resizeScreen()
{
    fringedArea = calculateFringedArea(worldView.screenParameter(), fringe);

    for(auto &frame : frames)
        frame.image = std::unique_ptr<QImage>(new QImage(fringedArea.toRectF().toRect().size(),
                                              QImage::Format_ARGB32_Premultiplied));
}

void FrameMemoisator::drawSelectedFrame(QPainter &painter, int selected)
{
    auto quot = worldView.scale() / frames[selected].trator->scale();
    qDebug() << "quot:" << quot;

#ifdef CONTROL
    auto fringedSize = fringedArea.toRectF().size();
    auto screenSize = worldView.screenParameter().toRectF().size();
    qreal tmpScale = std::min(screenSize.width() / (fringedSize.width() + 20.0),
                              screenSize.height() / (fringedSize.height() + 20.0));
    painter.setTransform(QTransform(tmpScale, 0.0, 0.0, tmpScale, 10, 10));
    painter.save();
    painter.setTransform(scaleAndPlaceInScreenCenter(quot, fringedArea), true);
    painter.drawImage(0, 0, *frames[selected].image);
    painter.restore();
    painter.translate(fringe, fringe);
    painter.setPen(Qt::red);
    painter.drawLine(0, 0, screenSize.width(), 0);
    painter.drawLine(screenSize.width(), 0, screenSize.width(), screenSize.height());
    painter.drawLine(screenSize.width(), screenSize.height(), 0, screenSize.height());
    painter.drawLine(0, screenSize.height(), 0, 0);
#else
    painter.setTransform(scaleAndPlaceInScreenCenter(quot, worldView.screenParameter()), true);
    painter.drawImage(-fringe, -fringe, *frames[selected].image);
#endif
}

Area calculateFringedArea(const Area &screenGeometry, int fringe)
{
    auto shift = QPoint(fringe, fringe);
    return Area(screenGeometry.nw() - shift, screenGeometry.se() + shift);
}

QTransform scaleAndPlaceInScreenCenter(qreal quot, const Area &area)
{
    auto halfArea = area.toRectF().size() / 2.0;
    auto shift = halfArea * quot - halfArea;
    return QTransform(quot, 0, 0, quot, -shift.width(), -shift.height());
}
