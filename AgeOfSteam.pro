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
    src/levelofdetalisation.cpp \
    src/mapwidget.cpp \
    src/hexagonalgrid.cpp \
    src/mapgenerator.cpp \
    src/worldview.cpp \
    src/logicalmap.cpp \
    src/transformator.cpp \
    src/graphicalmap.cpp

HEADERS += \
    include/mainwindow.h \
    include/levelofdetalisation.h \
    include/mapwidget.h \
    include/hexagonalgrid.h \
    include/mapgenerator.h \
    include/worldview.h \
    include/logicalmap.h \
    include/area.h \
    include/transformator.h \
    include/graphicalmap.h

RESOURCES += \
    res/res.qrc
