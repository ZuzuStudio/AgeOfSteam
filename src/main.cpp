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

    HexagonalGrid hex;
    QPainter painter;
    hex.drawRastr(&painter);
    for(int i = 0; i < 20; ++i)
    {
        for(int j = 0; j < 20 ; ++j)
        {
            qDebug() << hex.matrix[i][j];
        }
    }
    return app.exec();
}
