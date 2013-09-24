# -------------------------------------------------
# Project created by QtCreator 2009-01-03T23:50:32
# -------------------------------------------------
include ( ../../GPSBook/GPSBookConfig.pro)

#DEFINES  += QT_NO_WARNING_OUTPUT
#DEFINES  += QT_NO_DEBUG_OUTPUT

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DESTDIR = ../../../delivery/plugins
TEMPLATE = lib
DEFINES += ioLOC_LIBRARY
SOURCES += ioloc.cpp \
    ../../GPSBook/gpsdata.cpp \
    locreader.cpp \
    iolochelp.cpp \
    iolocoptionsframe.cpp
HEADERS += ioloc.h \
    ../../GPSBook/gpsdata.h \
    ../../GPSBook/interface.h \
    pixmap.h \
    locreader.h \
    iolochelp.h \
    iolocoptionsframe.h
INCLUDEPATH += ../../GPSBook/
TRANSLATIONS = ioLOC_fr_fr.ts \
               ioLOC_en_us.ts

FORMS += \
    iolochelp.ui \
    iolocoptionsframe.ui

RESOURCES += \
    ioloc.qrc

OTHER_FILES += \
    ioloc.json
