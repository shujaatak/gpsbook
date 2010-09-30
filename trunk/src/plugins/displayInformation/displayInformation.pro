# -------------------------------------------------
# Project created by QtCreator 2009-02-24T09:58:50
# -------------------------------------------------
include ( ../../GPSBook/GPSBookConfig.pro)

#DEFINES  += QT_NO_WARNING_OUTPUT
#DEFINES  += QT_NO_DEBUG_OUTPUT

TARGET = displayInformation
DESTDIR = ../../../delivery/plugins
TEMPLATE = lib
DEFINES += DISPLAYINFORMATION_LIBRARY
INCLUDEPATH += ../../GPSBook
SOURCES += displayinformation.cpp \
    displayinformationframe.cpp \
    displayinformationoptionsframe.cpp \
    displayinformationhelp.cpp \
    ../../GPSBook/gpsdata.cpp
HEADERS += displayinformation.h \
    ../../GPSBook/interface.h \
    pixmap.h \
    displayinformationframe.h \
    displayinformationoptionsframe.h \
    displayinformationhelp.h \
    ../../GPSBook/gpsdata.h
FORMS += displayinformationframe.ui \
    displayinformationoptionsframe.ui \
    displayinformationhelp.ui
TRANSLATIONS = displayInformation_fr_fr.ts \
               displayInformation_en_us.ts
