# -------------------------------------------------
# Project created by QtCreator 2009-02-22T08:57:36
# -------------------------------------------------
include ( ../../GPSBook/GPSBookConfig.pro)

DEFINES  += QT_NO_WARNING_OUTPUT
DEFINES  += QT_NO_DEBUG_OUTPUT

TARGET = filterCleanup
DESTDIR = ../../../delivery/plugins
TEMPLATE = lib
DEFINES += FILTERSPORTTRACKER_LIBRARY
SOURCES += filtercleanup.cpp \
    filtercleanupdialog.cpp \
    filtercleanuphelp.cpp \
    filtercleanupoptionsframe.cpp \
    ../../GPSBook/gpsdata.cpp
HEADERS += filtercleanup.h \
    ../../GPSBook/interface.h \
    filtercleanupdialog.h \
    filtercleanuphelp.h \
    filtercleanupoptionsframe.h \
    pixmap.h \
    ../../GPSBook/gpsdata.h
FORMS += filtercleanupdialog.ui \
    filtercleanuphelp.ui \
    filtercleanupoptionsframe.ui \

INCLUDEPATH += ../../GPSBook
TRANSLATIONS = filterCleanup_fr_fr.ts \
               filterCleanup_en_us.ts
