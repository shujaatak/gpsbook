# -------------------------------------------------
# Project created by QtCreator 2009-02-24T08:56:48
# -------------------------------------------------


include ( ../../GPSBook/GPSBookConfig.pro)

#DEFINES  += QT_NO_WARNING_OUTPUT
#DEFINES  += QT_NO_DEBUG_OUTPUT

QT += webkit
QT += network
TARGET = displayGpxViewDotCom
DESTDIR = ../../../delivery/plugins
TEMPLATE = lib
DEFINES += DISPLAYGPXVIEWDOTCOM_LIBRARY
INCLUDEPATH += ../../GPSBook
SOURCES += displaygpxviewdotcom.cpp \
    displaygpxviewdotcomframe.cpp \
    displaygpxviewdotcomhelp.cpp \
    displaygpxviewdotcomoptionsframe.cpp \
    ../../GPSBook/gpsdata.cpp
HEADERS += displaygpxviewdotcom.h \
    ../../GPSBook/gpsdata.h \
    pixmap.h \
    ../../GPSBook/interface.h \
    displaygpxviewdotcomhelp.h \
    displaygpxviewdotcomoptionsframe.h \
    displaygpxviewdotcomframe.h
TRANSLATIONS = displayGpxViewDotCom_fr_fr.ts \
               displayGpxViewDotCom_en_us.ts
OTHER_FILES +=
FORMS += displaygpxviewdotcomhelp.ui \
    displaygpxviewdotcomoptionsframe.ui \
    displaygpxviewdotcomframe.ui

# -------------------------------------------------
# http://www.gpx-view.com/ owner
#   Administrative Contact:
#   EMT-CONCEPT	 eric.massot@free.fr
#   Ens Les Tourterelles
#   AUSSONNE 31840
#   FR
#   +33534520969
# -------------------------------------------------
