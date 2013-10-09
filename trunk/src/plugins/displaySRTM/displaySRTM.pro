# -------------------------------------------------
# Project created by QtCreator 2009-02-24T08:55:52
# -------------------------------------------------
include ( ../../GPSBook/GPSBookConfig.pro)

#DEFINES  += QT_NO_WARNING_OUTPUT
#DEFINES  += QT_NO_DEBUG_OUTPUT

QT += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = displaySRTM
DESTDIR = ../../../delivery/plugins
TEMPLATE = lib
INCLUDEPATH += ../../GPSBook
win32 {
    # Download libcurl from http://curl.haxx.se/download.html
    #   Win32 - Generic
    #   Win32 2000/XP	7.32.0	libcurl	SSL	 	Günter Knauf	3.32 MB
    # and unzip it int the path listed bellow
    INCLUDEPATH += C:\Qt\curl-7.32.0-devel-mingw32\include\

    # Download quazip from http://sourceforge.net/projects/quazip/
    # Unzip it in C:/Qt/quazip-0.5.1/
    # Add the following conf into win32 section of C:\Qt\quazip-0.5.1\quazip\quazip.pro
    # greaterThan(QT_MAJOR_VERSION, 4) {
    #    LIBS += C:/Qt/curl-7.32.0-devel-mingw32/zlib1.dll
    # }
    # Compile quazip
    INCLUDEPATH += C:/Qt/quazip-0.5.1
    LIBS += C:/Qt/quazip-0.5.1/quazip/release/quazip.dll
}
DEFINES += DISPLAYSRTM_LIBRARY
SOURCES += ../../GPSBook/gpsdata.cpp \
    displaysrtm.cpp \
    displaysrtmoptionsframe.cpp \
    displaysrtmframe.cpp \
    displaysrtmhelp.cpp \
    srtm.cpp \
    zip.cpp
HEADERS += ../../GPSBook/gpsdata.h \
    ../../GPSBook/interface.h \
    displaysrtm.h \
    displaysrtmoptionsframe.h \
    displaysrtmframe.h \
    displaysrtmhelp.h \
    pixmap.h \
    srtm.h \
    zip.h \
    curl/types.h \
    curl/typecheck-gcc.h \
    curl/stdcheaders.h \
    curl/multi.h \
    curl/mprintf.h \
    curl/easy.h \
    curl/curlver.h \
    curl/curlrules.h \
    curl/curlbuild.h \
    curl/curl.h
FORMS += displaysrtmhelp.ui \
    displaysrtmframe.ui \
    displaysrtmoptionsframe.ui
TRANSLATIONS = displaySRTM_fr_fr.ts

RESOURCES += \
    icons/icons_srtm.qrc \
    ../../GPSBook/icons/gtk/gtk_icons.qrc

win32 {
    DEFINES += CURL_STATICLIB
    LIBS += -LC:\Qt\curl-7.32.0-devel-mingw32\bin -lcurl
}

unix {
    LIBS += -lcurl
    LIBS += -lquazip
}

OTHER_FILES += \
    srtm.json \
    srtm.xpm
