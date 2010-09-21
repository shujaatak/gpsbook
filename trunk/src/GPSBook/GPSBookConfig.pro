QMAKE_CXXFLAGS += -DBUILD_VERSION=\\\"0.1\\\"

unix {
    # build date
    QMAKE_CXXFLAGS += -DBUILD_DATE=\\\"`date +%Y-%m-%d`\\\"
    # svn version
    QMAKE_CXXFLAGS += -DSVN_VERSION=\\\"`svnversion . | cut -f'2' -d':'`\\\"
}
mac {

    QMAKE_CXXFLAGS += -DBUILD_DATE=\\\"\\\"
    QMAKE_CXXFLAGS += -DSVN_VERSION=\\\"Alpha\\\"
}

win32 {
    QMAKE_CXXFLAGS += -DBUILD_DATE=\\\"\\\"
    QMAKE_CXXFLAGS += -DSVN_VERSION=\\\"Alpha\\\"
    CONFIG -= console
}
