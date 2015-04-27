#-------------------------------------------------
#
# Project created by QtCreator 2015-01-19T19:31:08
#
#-------------------------------------------------

CONFIG += c++11

QT     += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets svg xml

CONFIG += console

qtHaveModule(opengl): QT += opengl

TARGET = AgeOfSteam
TEMPLATE = app


SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/graficitem.cpp \
    src/levelofdetalisation.cpp \
    src/mapwidget.cpp \
    src/hexagon.cpp \
    src/hexagonalgrid.cpp \
    src/mapgenerator.cpp \
    src/arraygrid.cpp \
    src/worldview.cpp

HEADERS += \
    include/mainwindow.h \
    include/graficitem.h \
    include/levelofdetalisation.h \
    include/mapwidget.h \
    include/hexagon.h \
    include/hexagonalgrid.h \
    include/mapgenerator.h \
    include/arraygrid.h \
    include/worldview.h

RESOURCES += \
    res/res.qrc
