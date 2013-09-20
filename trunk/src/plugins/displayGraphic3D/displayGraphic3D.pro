# -------------------------------------------------
# Project created by QtCreator 2009-02-24T09:02:35
# -------------------------------------------------
include ( ../../GPSBook/GPSBookConfig.pro)

#DEFINES  += QT_NO_WARNING_OUTPUT
#DEFINES  += QT_NO_DEBUG_OUTPUT

TARGET = displayGraphic3D
QT *= xml opengl
CONFIG *= qt opengl release
DESTDIR = ../../../delivery/plugins
TEMPLATE = lib
INCLUDEPATH += ../../GPSBook
INCLUDEPATH += ./libQGLViewer/QGLViewer
DEFINES += DISPLAYGRAPHIC3D_LIBRARY
SOURCES += displaygraphic3d.cpp \
    ../../GPSBook/gpsdata.cpp \
    viewer.cpp \
    displaygraphic3dframe.cpp \
    displaygraphic3doptionsframe.cpp \
    displaygraphic3dhelp.cpp
HEADERS += displaygraphic3d.h \
    ../../GPSBook/gpsdata.h \
    pixmap.h \
    ../../GPSBook/interface.h \
    viewer.h \
    displaygraphic3dframe.h \
    displaygraphic3doptionsframe.h \
    displaygraphic3dhelp.h \
    ../displaySRTM/srtm.h
TRANSLATIONS = displayGraphic3D_fr_fr.ts \
               displayGraphic3D_en_us.ts

linux-g++ {
    # libQGLViewer libraries
    #LIBS += -L../../../delivery/lib -lQGLViewer
    # libdisplaySRTM library
    #LIBS += -L../../../delivery/plugins -ldisplaySRTM
}
mac {
    # libQGLViewer libraries
    LIBS *= -framework QGLViewer
}
win32 {
    # libQGLViewer libraries
    LIBS += -L..\..\..\delivery\bin -lQGLViewer2
}

FORMS += \
    displaygraphic3dframe.ui \
    displaygraphic3doptionsframe.ui \
    displaygraphic3dhelp.ui

RESOURCES += \
    icons/3D/3D.qrc
