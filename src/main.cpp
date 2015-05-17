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
#include "../include/logicalmap.h"
#include <QDebug>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    LogicalMap model(201, 201);
    GraphicalMap view(model);
    MapWidget window(view);
    window.show();

    return app.exec();
}
