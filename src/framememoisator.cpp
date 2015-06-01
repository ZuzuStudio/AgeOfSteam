#include "include/framememoisator.h"

#include <QImage>
#include <QPainter>
#include <QtMath>
#include "include/graphicalmap.h"
#include "include/worldview.h"

#include <QDebug>

const qreal epsilon = 1.0 / 1024.0;

#define CONTROL

using namespace std;

Area calculateFringedArea(const Area &screenGeometry, int fringe);
vector<Area> calculateHoles(const Area &newArea, const Area &oldArea);
QTransform scaleAndPlaceInScreenCenter(qreal quot, const Area &area);

FrameMemoisator::FrameMemoisator(GraphicalMap &model, const WorldView &worldView):
    model(model),
    worldView(worldView),
    fringe(200),
    fringedArea(calculateFringedArea(worldView.screenParameter(), fringe))
{
    frames.push_back(Frame(this->worldView.fixedScaleTransformator(0.8),
                           new QImage(fringedArea.toRectF().toRect().size(),
                                      QImage::Format_ARGB32_Premultiplied),
                           belowThis(fringedArea)));
}

void FrameMemoisator::drawFrame(QPainter &painter)
{
    qDebug() << worldView.scale();
    QPainter buferPainter(frames.back().image());
    //buferPainter.setRenderHint(QPainter::SmoothPixmapTransform);
    //buferPainter.fillRect(fringedArea.toRectF().toRect(), Qt::black);
    buferPainter.translate(fringe, fringe);

    auto oldArea = frames.back().previousInScreenArea();

    //buferPainter.save();
   // buferPainter.translate(-fringedArea.nw() + oldArea.nw());
    buferPainter.drawImage(oldArea.nw().x(), oldArea.nw().y(), QImage(*frames.back().image()));
   // buferPainter.restore();
    auto holes = calculateHoles(fringedArea, oldArea);

    for(auto hole : holes)
        model.drawArea(buferPainter, hole, frames.back().trator());

    frames.back().setPreviousArea(fringedArea);

    drawSelectedFrame(painter, 0);
}

void FrameMemoisator::resizeScreen()
{
    fringedArea = calculateFringedArea(worldView.screenParameter(), fringe);

    for(auto &frame : frames)
    {
        frame.setImage(new QImage(fringedArea.toRectF().toRect().size(),
                                  QImage::Format_ARGB32_Premultiplied));
        frame.setPreviousArea(belowThis(fringedArea));
    }
}

void FrameMemoisator::drawSelectedFrame(QPainter &painter, int selected)
{
    auto quot = worldView.scale() / frames[selected].trator()->scale();
    qDebug() << "quot:" << quot;

#ifdef CONTROL
    auto fringedSize = fringedArea.toRectF().size();
    auto screenSize = worldView.screenParameter().toRectF().size();
    qreal tmpScale = std::min(screenSize.width() / (fringedSize.width() + 20.0),
                              screenSize.height() / (fringedSize.height() + 20.0));
    painter.setTransform(QTransform(tmpScale, 0.0, 0.0, tmpScale, 10, 10));
    painter.save();
    painter.setTransform(scaleAndPlaceInScreenCenter(quot, fringedArea), true);
    painter.drawImage(0, 0, *frames[selected].image());
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

vector<Area> calculateHoles(const Area &newArea, const Area &oldArea)
{
    vector<Area> result;
    auto diff = newArea.nw() - oldArea.nw();

    if(qFabs(diff.y()) > epsilon)
    {
        if(diff.y() > 0.0)
            result.push_back(Area(newArea.nw().x(), oldArea.se().y(),
                                  newArea.se().x(), newArea.se().y()));
        else
            result.push_back(Area(newArea.nw().x(), newArea.nw().y(),
                                  newArea.se().x(), oldArea.nw().y()));
    }

    if(qFabs(diff.x()) > epsilon)
    {
        if(diff.x() > 0.0)
            result.push_back(Area(oldArea.se().x(), newArea.nw().y(),
                                  newArea.se().x(), newArea.se().y()));
        else
            result.push_back(Area(newArea.nw().x(), newArea.nw().y(),
                                  oldArea.nw().x(), newArea.se().y()));
    }

    return result;
}

QTransform scaleAndPlaceInScreenCenter(qreal quot, const Area &area)
{
    auto halfArea = area.toRectF().size() / 2.0;
    auto shift = halfArea * quot - halfArea;
    return QTransform(quot, 0, 0, quot, -shift.width(), -shift.height());
}
