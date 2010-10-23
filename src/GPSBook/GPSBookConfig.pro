QMAKE_CXXFLAGS += -DBUILD_VERSION=\\\"0.1\\\"

unix {
    # build date
    QMAKE_CXXFLAGS += -DBUILD_DATE=\\\"`date +%Y-%m-%d`\\\"
    # svn version
    QMAKE_CXXFLAGS += -DSVN_VERSION=\\\"`svnversion . | cut -f'2' -d':'`\\\"
}
mac {

    QMAKE_CXXFLAGS += -DBUILD_DATE=\\\"20101007\\\"
    QMAKE_CXXFLAGS += -DSVN_VERSION=\\\"Alpha6\\\"
}

win32 {
    QMAKE_CXXFLAGS += -DBUILD_DATE=\\\"20101007\\\"
    QMAKE_CXXFLAGS += -DSVN_VERSION=\\\"Alpha6\\\"
    CONFIG -= console
}
