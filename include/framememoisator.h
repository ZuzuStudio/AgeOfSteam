#ifndef FRAMEMEMOISATOR
#define FRAMEMEMOISATOR

#include "include/area.h"
#include "include/frame.h"

class QPainter;
class GraphicalMap;
class WorldView;


class FrameMemoisator final
{
public:
    FrameMemoisator(GraphicalMap &model, const WorldView &worldView);
    FrameMemoisator(const FrameMemoisator &original) = delete;
    FrameMemoisator &operator=(const FrameMemoisator &rhs) = delete;
    FrameMemoisator(FrameMemoisator &&victim) = default;
    FrameMemoisator &operator=(FrameMemoisator &&rhs) = default;
    ~FrameMemoisator() = default;

    void drawFrame(QPainter &painter);
    void resizeScreen();

private:
    GraphicalMap &model;
    const WorldView &worldView;
    std::vector<Frame> frames;
    int fringe;
    Area fringedArea;

    void drawSelectedFrame(QPainter &painter, int selected);
};

#endif // FRAMEMEMOISATOR

