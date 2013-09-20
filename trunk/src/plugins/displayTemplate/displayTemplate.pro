# -------------------------------------------------
# Project created by QtCreator 2009-02-24T08:55:52
# -------------------------------------------------
include ( ../../GPSBook/GPSBookConfig.pro)

DEFINES  += QT_NO_WARNING_OUTPUT
DEFINES  += QT_NO_DEBUG_OUTPUT

TARGET = displayTemplate
DESTDIR = ../../../delivery/plugins
TEMPLATE = lib
INCLUDEPATH += ../../GPSBook
DEFINES += DISPLAYTEMPLATE_LIBRARY
SOURCES += displaytemplate.cpp \
    displaytemplateoptionsframe.cpp \
    displaytemplateframe.cpp \
    displaytemplatehelp.cpp \
    ../../GPSBook/gpsdata.cpp
HEADERS += displaytemplate.h \
    displaytemplateoptionsframe.h \
    displaytemplateframe.h \
    displaytemplatehelp.h \
    ../../GPSBook/gpsdata.h \
    ../../GPSBook/interface.h \
    pixmap.h
FORMS += displaytemplatehelp.ui \
    displaytemplateframe.ui \
    displaytemplateoptionsframe.ui
TRANSLATIONS = displayTemplate_fr_fr.ts
