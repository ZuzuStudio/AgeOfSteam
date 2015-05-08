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
    LogicalMap model(21, 21);
    MapWidget window(model);
    window.show();

    return app.exec();
}
