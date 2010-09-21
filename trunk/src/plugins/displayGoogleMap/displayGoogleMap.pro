# -------------------------------------------------
# Project created by QtCreator 2009-02-24T08:56:48
# -------------------------------------------------
include ( ../../GPSBook/GPSBookConfig.pro)

DEFINES  += QT_NO_WARNING_OUTPUT
DEFINES  += QT_NO_DEBUG_OUTPUT

QT += webkit
QT += network
TARGET = displayGoogleMap
DESTDIR = ../../../delivery/plugins
TEMPLATE = lib
DEFINES += DISPLAYGOOGLEMAP_LIBRARY
INCLUDEPATH += ../../GPSBook
SOURCES += displaygooglemap.cpp \
           displaygooglemaphelp.cpp \
           displaygooglemapframe.cpp \
           displaygooglemapoptionsframe.cpp \
           ../../GPSBook/gpsdata.cpp
HEADERS += displaygooglemap.h \
           displaygooglemaphelp.h \
           displaygooglemapoptionsframe.h \
           displaygooglemapframe.h \
           pixmap.h \
           ../../GPSBook/gpsdata.h \
           ../../GPSBook/interface.h
TRANSLATIONS = displayGoogleMap_fr_fr.ts  \
               displayGoogleMap_en_us.ts
OTHER_FILES += index.html
FORMS += displaygooglemapframe.ui \
         displaygooglemapoptionsframe.ui \
         displaygooglemaphelp.ui
RESOURCES += displayGoogleMap.qrc
