#include "include/framememoisator.h"

#include <QImage>
#include <QPainter>
#include "include/graphicalmap.h"
#include "include/worldview.h"

#include <QDebug>

#define CONTROL

Area calculateFringedArea(const Area &screenGeometry, int fringe);

FrameMemoisator::FrameMemoisator(GraphicalMap &model, const Area &screenGeometry):
    model(model),
    // imageBufer(nullptr),
    fringe(200),
    fringedArea(calculateFringedArea(screenGeometry, fringe)),
    firstTime(1)
{
    //imageBufer  = new QImage(fringedArea.toRectF().toRect().size(),
    //                         QImage::Format_ARGB32_Premultiplied);
}

FrameMemoisator::~FrameMemoisator()
{
    //delete imageBufer;
    //imageBufer = nullptr;
}

void FrameMemoisator::drawFrame(QPainter &painter, const WorldView &worldView)
{
    if(firstTime)
    {
        frames.push_back(Frame(worldView.fixedScaleTransformator(0.8),
                               new QImage(fringedArea.toRectF().toRect().size(),
                                          QImage::Format_ARGB32_Premultiplied)));
        firstTime = 0;
    }

    qDebug() << worldView.scale();
    QPainter buferPainter(frames.back().image.get());
    //buferPainter.setRenderHint(QPainter::SmoothPixmapTransform);
    //buferPainter.fillRect(fringedArea.toRectF().toRect(), Qt::black);
    buferPainter.translate(fringe, fringe);

    model.drawArea(buferPainter, fringedArea, frames.back().trator.get());

    auto quot = worldView.scale() / frames.back().trator->scale();
    qDebug() << "quot:" << quot;

    painter.setRenderHint(QPainter::SmoothPixmapTransform);
#ifdef CONTROL
    auto width = fringedArea.se().x() - fringedArea.nw().x(),
         height = fringedArea.se().y() - fringedArea.nw().y();
    auto screenWidth = width - 2.0 * fringe, screenHeight = height - 2.0 * fringe;
    qreal tmpScale = std::min(screenWidth / (width + 20.0),
                              screenHeight / (height + 20.0));
    painter.setTransform(QTransform(tmpScale, 0.0, 0.0, tmpScale, 5, 5));
    painter.save();
    auto screen = frames.back().image->size() / 2.0;
    auto shift = screen*quot-screen;
    painter.setTransform(QTransform(quot, 0, 0, quot, -shift.width(), -shift.height()), true);
    painter.drawImage(0, 0, *frames.back().image);
    painter.restore();
    painter.translate(fringe, fringe);
    painter.setPen(Qt::red);
    painter.drawLine(0, 0, screenWidth, 0);
    painter.drawLine(screenWidth, 0, screenWidth, screenHeight);
    painter.drawLine(screenWidth, screenHeight, 0, screenHeight);
    painter.drawLine(0, screenHeight, 0, 0);
#else
    auto diag = fringedArea.nw() - fringedArea.se();
    qDebug() << diag;
    auto screen = (diag + QPointF(2 * fringe, 2 * fringe))/2.0;
    auto shift = screen*quot-screen;
    qDebug() << screen<<screen*quot<<shift;
    painter.setTransform(QTransform(quot, 0, 0, quot, shift.x(), shift.y()), true);
    painter.drawImage(-fringe, -fringe, *frames.back().image);
#endif
}

void FrameMemoisator::resizeScreen(const Area &screenGeometry)
{
    fringedArea = calculateFringedArea(screenGeometry, fringe);

    for(auto &frame : frames)
        frame.image = std::unique_ptr<QImage>(new QImage(fringedArea.toRectF().toRect().size(),
                                              QImage::Format_ARGB32_Premultiplied));
}

Area calculateFringedArea(const Area &screenGeometry, int fringe)
{
    auto shift = QPoint(fringe, fringe);
    return Area(screenGeometry.nw() - shift, screenGeometry.se() + shift);
}
