# -------------------------------------------------
# Project created by QtCreator 2009-01-18T18:29:26
# -------------------------------------------------
include ( ../../GPSBook/GPSBookConfig.pro)

#DEFINES  += QT_NO_WARNING_OUTPUT
#DEFINES  += QT_NO_DEBUG_OUTPUT

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

LIBS += -L../../../delivery/plugins -ldisplaySRTM
