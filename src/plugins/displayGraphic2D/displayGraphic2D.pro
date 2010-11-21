# -------------------------------------------------
# Project created by QtCreator 2009-01-24T20:10:58
# -------------------------------------------------
include ( ../../GPSBook/GPSBookConfig.pro)
TARGET = displayGraphic2D

DEFINES  += QT_NO_WARNING_OUTPUT
DEFINES  += QT_NO_DEBUG_OUTPUT

DESTDIR = ../../../delivery/plugins
TEMPLATE = lib
DEFINES += DISPLAYGRAPHIC2D_LIBRARY
SOURCES += displaygraphic2D.cpp \
    ../../GPSBook/gpsdata.cpp \
    scalepicker.cpp \
    plot.cpp \
    displaygraphic2Dframe.cpp \
    displaygraphic2dhelp.cpp \
    displaygraphic2doptionsframe.cpp \
    canvaspicker.cpp \
    curve.cpp
HEADERS += displaygraphic2D.h \
    scalepicker.h \
    plot.h \
    displaygraphic2Dframe.h \
    pixmap.h \
    ../../GPSBook/gpsdata.h \
    ../../GPSBook/interface.h \
    displaygraphic2dhelp.h \
    displaygraphic2doptionsframe.h \
    canvaspicker.h \
    curve.h
INCLUDEPATH += ../../GPSBook
INCLUDEPATH += ../../../../qwt/src/
FORMS += displaygraphic2Dframe.ui \
    displaygraphic2dhelp.ui \
    displaygraphic2doptionsframe.ui
TRANSLATIONS = displayGraphic2D_fr_fr.ts \
               displayGraphic2D_en_us.ts
unix {
    # qwt libraries
    LIBS += -L../../../delivery/lib -lqwt
}
mac {
}
win32 {
    # qwt libraries
    LIBS += -L..\..\..\delivery\bin -lqwt6
}
