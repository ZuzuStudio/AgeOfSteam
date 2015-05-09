/**********************
TODO: license here
***********************/

#include <QApplication>
#include <QString>
#ifndef QT_NO_OPENGL
#include <QGLFormat>
#endif

#include "../include/mapwidget.h"
#include "../include/mainwindow.h"
#include "../include/arraygrid.h"
#include "../include/mapgenerator.h"
#include <QDebug>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    ArrayGrid * model = MapGenerator::generate(21, 21);
    MapWidget window(*model);
    window.show();

    return app.exec();
}
