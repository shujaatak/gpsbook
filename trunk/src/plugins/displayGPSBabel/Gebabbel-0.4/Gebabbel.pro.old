# About "universal binaries" see
# http://wiki.qtcentre.org/index.php?title=Installing_Qt4_on_Mac_OS_X
# http://www.macentwicklerwelt.net/index.php?title=Universal_Binaries
# http://blogs.qtdeveloper.net/archives/2005/12/
# http://blogs.qtdeveloper.net/ seems to be cool
# http://www.qtcentre.org/forum/f-qt-programming-2/t-application-build-mac-ppcintel-pro-file-2806.html

# For a release version, uncomment this to get a non-debug binary
CONFIG += qt release
# Be careful, as DEBUG overwrites "release":
# DEFINES += DEBUG


# Platform dependent stuff #

# Linux #
DEFINES += LINUX
TARGET = gebabbel

# Macintosh #
# The first two lines create a universal binary
# QMAKE_MAC_SDK=/Developer/SDKs/MacOSX10.4u.sdk
# CONFIG+=x86 ppc
# DEFINES += MACINTOSH
# TARGET = Gebabbel
# ICON = binincludes/icons/AppIcons.icns

# Windows #
# DEFINES += WINDOWS
# TARGET = Gebabbel
# RC_FILE = binincludes/WindowsResources.rc


TRANSLATIONS +=	binincludes/translations/de_DE.ts binincludes/translations/en_EN.ts binincludes/translations/fr_FR.ts
target path = /usr/local/bin
translations.path = :/binincludes/translations
translation.files = :/binincludes/translations/*.qm

# DEPENDPATH += . 
# INCLUDEPATH += . 
# LIBS += 

TEMPLATE = app
RESOURCES += binincludes/binincludes.qrc
QMAKE_CXXFLAGS = -O0 -g3

OBJECTS_DIR = obj
DESTDIR += bin
MOC_DIR += moc
UI_DIR += ui
FORMS += ui/MainWindow.ui ui/IOConfig.ui ui/FilterConfig.ui ui/PreferencesConfig.ui ui/SavePresetWindow.ui
HEADERS += src/MyMainWindow.h src/MyIOConfigWindow.h src/MyFilterConfigWindow.h src/MyPreferencesConfig.h src/ListItem.h src/MySavePresetWindow.h src/SettingsManager.h
SOURCES += src/main.cpp src/MyMainWindow.cpp src/MyIOConfigWindow.cpp src/MyFilterConfigWindow.cpp src/MyPreferencesConfig.cpp src/ListItem.cpp src/SettingsManager.cpp src/MySavePresetWindow.cpp





