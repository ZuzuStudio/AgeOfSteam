HEADERS       = \
    mapwidget.h \
    graficmap.h \
    graficitem.h \
    levelofdetalisation.h
RESOURCES     = \
    MapFragment.qrc
SOURCES       = \
    main.cpp \
    mapwidget.cpp \
    graficmap.cpp \
    graficitem.cpp \
    levelofdetalisation.cpp
QT           += widgets svg xml
CONFIG += c++11

qtHaveModule(opengl): QT += opengl

CONFIG += console

# install
# target.path = $$[QT_INSTALL_EXAMPLES]/svg/svgviewer
# INSTALLS += target

wince*: {
     addFiles.files = files\\*.svg
     addFiles.path = "\\My Documents"
     DEPLOYMENT += addFiles
}
