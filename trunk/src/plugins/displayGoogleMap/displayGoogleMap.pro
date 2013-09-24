# -------------------------------------------------
# Project created by QtCreator 2009-02-24T08:56:48
# -------------------------------------------------
include ( ../../GPSBook/GPSBookConfig.pro)

#DEFINES  += QT_NO_WARNING_OUTPUT
#DEFINES  += QT_NO_DEBUG_OUTPUT

QT += network
greaterThan(QT_MAJOR_VERSION, 4): {
    QT += widgets webkitwidgets
} else {
    QT += webkit
}

TARGET = displayGoogleMap
DESTDIR = ../../../delivery/plugins
TEMPLATE = lib
DEFINES += DISPLAYGOOGLEMAP_LIBRARY
INCLUDEPATH += ../../GPSBook
SOURCES += displaygooglemap.cpp \
           displaygooglemaphelp.cpp \
           displaygooglemapframe.cpp \
           displaygooglemapoptionsframe.cpp \
           ../../GPSBook/gpsdata.cpp \
    ../../GPSBook/QProgressIndicator.cpp
HEADERS += displaygooglemap.h \
           displaygooglemaphelp.h \
           displaygooglemapoptionsframe.h \
           displaygooglemapframe.h \
           ../../GPSBook/gpsdata.h \
           ../../GPSBook/interface.h \
    ../../GPSBook/QProgressIndicator.h
TRANSLATIONS = displayGoogleMap_fr_fr.ts  \
               displayGoogleMap_en_us.ts
OTHER_FILES += index.html \
    googlemap.json \
    google.xpm
FORMS += displaygooglemapframe.ui \
         displaygooglemapoptionsframe.ui \
         displaygooglemaphelp.ui
RESOURCES += displayGoogleMap.qrc
