#include "../include/worldview.h"


void WorldView::moveScreen(QPoint screenShift)
{
    QPoint mapShift = screenShift / scale;
    // TODO constraint
}
