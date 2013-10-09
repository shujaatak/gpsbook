# -------------------------------------------------
# Project created by QtCreator 2009-02-24T09:02:35
# -------------------------------------------------
include ( ../../GPSBook/GPSBookConfig.pro)

#DEFINES  += QT_NO_WARNING_OUTPUT
#DEFINES  += QT_NO_DEBUG_OUTPUT

TARGET = displayGraphic3D

QT *= xml opengl
CONFIG *= qt opengl release

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DESTDIR = ../../../delivery/plugins
TEMPLATE = lib

INCLUDEPATH += ../../GPSBook
INCLUDEPATH += ../../../libQGLViewer-2.4.0/QGLViewer/

win32 {
    # Download libcurl from http://curl.haxx.se/download.html
    #   Win32 - Generic
    #   Win32 2000/XP	7.32.0	libcurl	SSL	 	Günter Knauf	3.32 MB
    # and unzip it int the path listed bellow
    INCLUDEPATH += C:\Qt\curl-7.32.0-devel-mingw32\include\
}

DEFINES += DISPLAYGRAPHIC3D_LIBRARY
SOURCES += displaygraphic3d.cpp \
    ../../GPSBook/gpsdata.cpp \
    viewer.cpp \
    displaygraphic3dframe.cpp \
    displaygraphic3doptionsframe.cpp \
    displaygraphic3dhelp.cpp
HEADERS += displaygraphic3d.h \
    ../../GPSBook/gpsdata.h \
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
    LIBS += -L../../../delivery/lib -lQGLViewer
    # libdisplaySRTM library
    LIBS += -L../../../delivery/plugins -ldisplaySRTM
}
mac {
    # libQGLViewer libraries
    LIBS *= -framework QGLViewer
}
win32 {
    # libQGLViewer libraries
    LIBS += -L..\..\..\delivery\lib -lQGLViewer
    # libdisplaySRTM library
    LIBS += -L..\..\..\delivery\plugins -ldisplaySRTM
}



FORMS += \
    displaygraphic3dframe.ui \
    displaygraphic3doptionsframe.ui \
    displaygraphic3dhelp.ui

RESOURCES += \
    icons/3D/3D.qrc \
    displayGraphic3D.qrc

OTHER_FILES += \
    graphic3D.json
