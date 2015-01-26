/**********************
TODO: license here
***********************/

#include <QApplication>
#include <QString>
#ifndef QT_NO_OPENGL
#include <QGLFormat>
#endif

#include "../include/mapwidget.h"
#include "include/hexagonalgrid.h"
#include <QDebug>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    MapWidget window;
    window.setScale(0.2);
    window.show();

    return app.exec();
}
