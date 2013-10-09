# -------------------------------------------------
# Project created by QtCreator 2009-01-02T13:40:04
# -------------------------------------------------
include (GPSBookConfig.pro)

#DEFINES  += QT_NO_WARNING_OUTPUT
#DEFINES  += QT_NO_DEBUG_OUTPUT

QT += sql
QT += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


TARGET = gpsbook
DESTDIR = ../../delivery/bin
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    dialogfirststart.cpp \
    gpsdata.cpp \
    gpsbookapplication.cpp \
    database.cpp \
    dialogtrackproperty.cpp \
    QProgressIndicator.cpp \
    webproxy.cpp \
    qtrackslistwidget.cpp
HEADERS += mainwindow.h \
    interface.h \
    dialogfirststart.h \
    gpsdata.h \
    gpsbookapplication.h \
    database.h \
    dialogtrackproperty.h \
    QProgressIndicator.h \
    webproxy.h \
    qtrackslistwidget.h
FORMS += mainwindow.ui \
    dialogfirststart.ui \
    dialogtrackproperty.ui
TRANSLATIONS = GPSBook_fr_fr.ts \
               GPSBook_en_us.ts
RESOURCES += \
    icons/silk/silk_icons.qrc \
    icons/gtk/gtk_icons.qrc \
    icons/gpsbook/gpsbook_icons.qrc \
    icons/map-icons-collection/map-icons.qrc \
    icons/google-marker/google-marker.qrc \
    icons/geocaching/geocaching.qrc
#icon
win32:RC_FILE = gpsbook.rc
ICON = GPSBook.icns
