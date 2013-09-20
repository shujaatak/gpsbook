# -------------------------------------------------
# Project created by QtCreator 2009-02-24T08:55:52
# -------------------------------------------------
include ( ../../GPSBook/GPSBookConfig.pro)

#DEFINES  += QT_NO_WARNING_OUTPUT
#DEFINES  += QT_NO_DEBUG_OUTPUT

TARGET = displayGPSBabel
DESTDIR = ../../../delivery/plugins
TEMPLATE = lib
INCLUDEPATH += ../../GPSBook
INCLUDEPATH += Gebabbel-0.4/src/
INCLUDEPATH += Gebabbel-0.4/ui/
DEFINES += DISPLAYGPSBABEL_LIBRARY
SOURCES += displaygpsbabel.cpp \
    displaygpsbabeloptionsframe.cpp \
    displaygpsbabelhelp.cpp \
    ../../GPSBook/gpsdata.cpp \
    Gebabbel-0.4/src/SettingsManager.cpp \
    Gebabbel-0.4/src/MySavePresetWindow.cpp \
    Gebabbel-0.4/src/MyPreferencesConfig.cpp \
    Gebabbel-0.4/src/MyMainWindow.cpp \
    Gebabbel-0.4/src/MyIOConfigWindow.cpp \
    Gebabbel-0.4/src/MyFilterConfigWindow.cpp \
    Gebabbel-0.4/src/ListItem.cpp
HEADERS += displaygpsbabel.h \
    displaygpsbabeloptionsframe.h \
    displaygpsbabelhelp.h \
    ../../GPSBook/gpsdata.h \
    ../../GPSBook/interface.h \
    pixmap.h \
    Gebabbel-0.4/src/SettingsManager.h \
    Gebabbel-0.4/src/MySavePresetWindow.h \
    Gebabbel-0.4/src/MyPreferencesConfig.h \
    Gebabbel-0.4/src/MyMainWindow.h \
    Gebabbel-0.4/src/MyIOConfigWindow.h \
    Gebabbel-0.4/src/MyFilterConfigWindow.h \
    Gebabbel-0.4/src/ListItem.h
FORMS += displaygpsbabelhelp.ui \
    displaygpsbabeloptionsframe.ui \
    Gebabbel-0.4/ui/Gebabbel.ui \
    Gebabbel-0.4/ui/SavePresetWindow.ui \
    Gebabbel-0.4/ui/PresetConfig.ui \
    Gebabbel-0.4/ui/PreferencesConfig.ui \
    Gebabbel-0.4/ui/IOConfig.ui \
    Gebabbel-0.4/ui/FilterConfig.ui

#TRANSLATIONS = displayTemplate_fr_fr.ts

RESOURCES += \
    Gebabbel-0.4/binincludes/binincludes.qrc
