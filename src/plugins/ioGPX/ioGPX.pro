# -------------------------------------------------
# Project created by QtCreator 2009-01-03T23:50:32
# -------------------------------------------------
include ( ../../GPSBook/GPSBookConfig.pro)

#DEFINES  += QT_NO_WARNING_OUTPUT
#DEFINES  += QT_NO_DEBUG_OUTPUT

greaterThan(QT_MAJOR_VERSION, 4): QT += core gui widgets

DESTDIR = ../../../delivery/plugins
TEMPLATE = lib
DEFINES += ioGPX_LIBRARY
SOURCES += iogpx.cpp \
    ../../GPSBook/gpsdata.cpp \
    gpxreader.cpp \
    gpxwriter.cpp \
    iogpxhelp.cpp \
    iogpxoptionsframe.cpp
HEADERS += iogpx.h \
    ../../GPSBook/gpsdata.h \
    ../../GPSBook/interface.h \
    pixmap.h \
    gpxreader.h \
    gpxwriter.h \
    iogpxhelp.h \
    iogpxoptionsframe.h
INCLUDEPATH += ../../GPSBook/
TRANSLATIONS = ioGPX_fr_fr.ts \
               ioGPX_en_us.ts

FORMS += \
    iogpxhelp.ui \
    iogpxoptionsframe.ui

OTHER_FILES += \
    iogpx.json

RESOURCES += \
    iogpx.qrc
