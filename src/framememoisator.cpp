#include "include/framememoisator.h"

#include <QDebug>

#define CONTROL

Area calculateFringedArea(const Area &screenGeometry, int fringe);

FrameMemoisator::FrameMemoisator(GraphicalMap &model, const Area &screenGeometry):
    model(model),
    imageBufer(nullptr),
    fringe(200),
    fringedArea(calculateFringedArea(screenGeometry, fringe))
{
    imageBufer  = new QImage(fringedArea.toRectF().toRect().size(),
                             QImage::Format_ARGB32_Premultiplied);
}

FrameMemoisator::~FrameMemoisator()
{
    delete imageBufer;
    imageBufer = nullptr;
}

void FrameMemoisator::drawFrame(QPainter &painter, const WorldView &worldView)
{
    qDebug() << worldView.scale();
    QPainter buferPainter(imageBufer);
    buferPainter.setRenderHint(QPainter::SmoothPixmapTransform);
    buferPainter.fillRect(fringedArea.toRectF().toRect(), Qt::black);
    buferPainter.translate(fringe, fringe);

    model.drawArea(buferPainter, fringedArea, worldView.followingTransformator());

#ifdef CONTROL
    auto width = fringedArea.se().x() - fringedArea.nw().x(),
         height = fringedArea.se().y() - fringedArea.nw().y();
    auto screenWidth = width - 2.0 * fringe, screenHeight = height - 2.0 * fringe;
    qreal tmpScale = std::min(screenWidth / (width + 20.0),
                              screenHeight / (height + 20.0));
    painter.setTransform(QTransform(tmpScale, 0.0, 0.0, tmpScale, 5, 5));
    painter.drawImage(0, 0, *imageBufer);
    painter.translate(fringe, fringe);
    painter.setPen(Qt::red);
    painter.drawLine(0, 0, screenWidth, 0);
    painter.drawLine(screenWidth, 0, screenWidth, screenHeight);
    painter.drawLine(screenWidth, screenHeight, 0, screenHeight);
    painter.drawLine(0, screenHeight, 0, 0);
#else
    painter.drawImage(-fringe, -fringe, *imageBufer);
#endif
}

void FrameMemoisator::resizeScreen(const Area &screenGeometry)
{
    fringedArea = calculateFringedArea(screenGeometry, fringe);
    auto newImageBufer  = new QImage(fringedArea.toRectF().toRect().size(), QImage::Format_ARGB32_Premultiplied);
    delete imageBufer;
    imageBufer = newImageBufer;
    newImageBufer = nullptr;
}

Area calculateFringedArea(const Area &screenGeometry, int fringe)
{
    auto shift = QPoint(fringe, fringe);
    return Area(screenGeometry.nw() - shift, screenGeometry.se() + shift);
}
