# -------------------------------------------------
# Project created by QtCreator 2009-02-24T09:58:50
# -------------------------------------------------
include ( ../../GPSBook/GPSBookConfig.pro)

#DEFINES  += QT_NO_WARNING_OUTPUT
#DEFINES  += QT_NO_DEBUG_OUTPUT

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = displayInformation
DESTDIR = ../../../delivery/plugins
TEMPLATE = lib
DEFINES += DISPLAYINFORMATION_LIBRARY
INCLUDEPATH += ../../GPSBook
SOURCES += displayinformation.cpp \
    displayinformationframe.cpp \
    displayinformationoptionsframe.cpp \
    displayinformationhelp.cpp \
    ../../GPSBook/gpsdata.cpp \
    ../../GPSBook/dialoglinkedition.cpp \
    ../../GPSBook/dialogselectsymbol.cpp
HEADERS += displayinformation.h \
    ../../GPSBook/interface.h \
    pixmap.h \
    displayinformationframe.h \
    displayinformationoptionsframe.h \
    displayinformationhelp.h \
    ../../GPSBook/gpsdata.h \
    ../../GPSBook/dialoglinkedition.h \
    ../../GPSBook/dialogselectsymbol.h
FORMS += displayinformationframe.ui \
    displayinformationoptionsframe.ui \
    displayinformationhelp.ui \
    ../../GPSBook/dialogselectsymbol.ui \
    ../../GPSBook/dialoglinkedition.ui
TRANSLATIONS = displayInformation_fr_fr.ts \
               displayInformation_en_us.ts

RESOURCES += \
    displayinformation.qrc

OTHER_FILES += \
    displayinformation.json
