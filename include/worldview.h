#ifndef WORLDVIEW_H
#define WORLDVIEW_H

#include <QPoint>

class WorldView
{
public:
    //G VI default by time

    void moveScreen(QPoint screenShift);
    void moveScreen(int x, int y){moveScreen(QPoint(x,y));}
private:
    int mapLeft, mapRight, mapTop, mapBottom;
    int screenLeft, screenRight, screenTop, screenBottom;
    QPoint screenCenter;
    qreal scale;
};

#endif // WORLDVIEW_H

