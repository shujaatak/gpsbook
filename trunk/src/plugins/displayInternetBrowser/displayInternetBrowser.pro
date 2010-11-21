# -------------------------------------------------
# Project created by QtCreator 2009-02-24T08:56:48
# -------------------------------------------------
include ( ../../GPSBook/GPSBookConfig.pro)

#DEFINES  += QT_NO_WARNING_OUTPUT
#DEFINES  += QT_NO_DEBUG_OUTPUT

QT += webkit
QT += network
TARGET = displayInternetBrowser
DESTDIR = ../../../delivery/plugins
TEMPLATE = lib
DEFINES += DISPLAYINTERNETBROWSER_LIBRARY
INCLUDEPATH += ../../GPSBook
SOURCES += displayinternetbrowser.cpp \
    displayinternetbrowserframe.cpp \
    displayinternetbrowserhelp.cpp \
    displayinternetbrowseroptionsframe.cpp \
    ../../GPSBook/gpsdata.cpp \
    ../../GPSBook/QProgressIndicator.cpp \
    cookiejar.cpp
HEADERS += displayinternetbrowser.h \
    ../../GPSBook/gpsdata.h \
    pixmap.h \
    ../../GPSBook/interface.h \
    displayinternetbrowserhelp.h \
    displayinternetbrowseroptionsframe.h \
    displayinternetbrowserframe.h \
    ../../GPSBook/QProgressIndicator.h \
    cookiejar.h
TRANSLATIONS = displayInternetBrowser_fr_fr.ts \
               displayInternetBrowser_en_us.ts
OTHER_FILES += \
    welcome.html
FORMS += displayinternetbrowserhelp.ui \
    displayinternetbrowseroptionsframe.ui \
    displayinternetbrowserframe.ui

# -------------------------------------------------
# http://www.gpx-view.com/ owner
#   Administrative Contact:
#   EMT-CONCEPT	 eric.massot@free.fr
#   Ens Les Tourterelles
#   AUSSONNE 31840
#   FR
#   +33534520969
# -------------------------------------------------

RESOURCES += \
    displayInternetBrowser.qrc
