# -------------------------------------------------
# Project created by QtCreator 2009-01-18T18:29:26
# -------------------------------------------------
include ( ../../GPSBook/GPSBookConfig.pro)

#DEFINES  += QT_NO_WARNING_OUTPUT
#DEFINES  += QT_NO_DEBUG_OUTPUT

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = displayStatistic
DESTDIR = ../../../delivery/plugins
TEMPLATE = lib
DEFINES += DISPLAYSTAT_LIBRARY
SOURCES += displaystatistic.cpp \
    ../../GPSBook/gpsdata.cpp \
    displaystatisticframe.cpp \
    displaystatisticoptionsframe.cpp \
    displaystatistichelp.cpp \
    serviceprocessminmaxaltitude.cpp \
    serviceprocessminmaxspeed.cpp \
    dialogprocessdistance.cpp \
    dialogprocessspeed.cpp \
    dialogprocessaltitude.cpp \
    dialogprocessacceleration.cpp \
    serviceprocesswaypointspeed.cpp \
    serviceprocesswaypointdistance.cpp \
    serviceprocesswaypointacceleration.cpp
HEADERS += displaystatistic.h \
    displaystatisticframe.h \
    displaystatisticoptionsframe.h \
    displaystatistichelp.h \
    ../../GPSBook/interface.h \
    ../../GPSBook/gpsdata.h \
    serviceprocessminmaxaltitude.h \
    serviceprocessminmaxspeed.h \
    pixmap.h \
    dialogprocessdistance.h \
    dialogprocessspeed.h \
    dialogprocessaltitude.h \
    dialogprocessacceleration.h \
    serviceprocesswaypointspeed.h \
    serviceprocesswaypointdistance.h \
    serviceprocesswaypointacceleration.h \
    ../displaySRTM/srtm.h
INCLUDEPATH += ../../GPSBook
FORMS += displaystatisticframe.ui \
    displaystatisticoptionsframe.ui \
    displaystatistichelp.ui \
    dialogprocessdistance.ui \
    dialogprocessspeed.ui \
    dialogprocessaltitude.ui \
    dialogprocessacceleration.ui
TRANSLATIONS = displayStatistic_fr_fr.ts \
               displayStatistic_en_us.ts

win32 {
    # Download libcurl from http://curl.haxx.se/download.html
    #   Win32 - Generic
    #   Win32 2000/XP	7.32.0	libcurl	SSL	 	Günter Knauf	3.32 MB
    # and unzip it int the path listed bellow
    INCLUDEPATH += C:\Qt\curl-7.32.0-devel-mingw32\include\
}

LIBS += -L../../../delivery/plugins -ldisplaySRTM

OTHER_FILES += \
    statistics.json \
    displaystatistic.xpm

RESOURCES += \
    displaystatistics.qrc
