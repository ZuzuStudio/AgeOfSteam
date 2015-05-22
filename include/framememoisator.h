#ifndef FRAMEMEMOISATOR
#define FRAMEMEMOISATOR

#include <QRect>

#include "include/graphicalmap.h"
#include "include/area.h"
#include "include/worldview.h"

class FrameMemoisator final
{
public:
    FrameMemoisator(GraphicalMap &model, const Area &screenGeometry);
    FrameMemoisator(const FrameMemoisator &original) = delete;
    FrameMemoisator &operator=(const FrameMemoisator &rhs) = delete;
    FrameMemoisator(FrameMemoisator &&victim) = default;
    FrameMemoisator &operator=(FrameMemoisator &&rhs) = default;
    ~FrameMemoisator();

    void drawFrame(QPainter &painter, const WorldView &worldView);
    void resizeScreen(const Area &screenGeometry);

private:
    GraphicalMap &model;
    QImage *imageBufer;
    int fringe;
    Area fringedArea;
};

#endif // FRAMEMEMOISATOR

