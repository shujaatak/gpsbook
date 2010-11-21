# -------------------------------------------------
# Project created by QtCreator 2009-02-24T08:55:52
# -------------------------------------------------
include ( ../../GPSBook/GPSBookConfig.pro)

DEFINES  += QT_NO_WARNING_OUTPUT
DEFINES  += QT_NO_DEBUG_OUTPUT

QT += network
TARGET = displaySRTM
DESTDIR = ../../../delivery/plugins
TEMPLATE = lib
INCLUDEPATH += ../../GPSBook
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
    curl/curl.h \
    zzip/zzip.h \
    zzip/write.h \
    zzip/wrap.h \
    zzip/types.h \
    zzip/stdint.h \
    zzip/plugin.h \
    zzip/mmapped.h \
    zzip/memdisk.h \
    zzip/lib.h \
    zzip/info.h \
    zzip/fseeko.h \
    zzip/format.h \
    zzip/file.h \
    zzip/fetch.h \
    zzip/conf.h \
    zzip/autoconf.h
FORMS += displaysrtmhelp.ui \
    displaysrtmframe.ui \
    displaysrtmoptionsframe.ui
TRANSLATIONS = displaySRTM_fr_fr.ts

RESOURCES += \
    icons/icons_srtm.qrc

LIBS += -lcurl
LIBS += -lzzip
