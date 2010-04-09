TEMPLATE = app
win32:TARGET = maxcalcwin
unix:TARGET = maxcalcgui

include(../maxcalc_config.pri)

CONFIG += qt debug_and_release warn_on precompile_header
win32:CONFIG += windows

# Enable single instance mode only on Qt 4.4 and higher
contains(QT_MINOR_VERSION, 4) | contains(QT_MINOR_VERSION, 5) |
contains(QT_MINOR_VERSION, 6) | contains(QT_MINOR_VERSION, 7) {
    QT += network
    include(qtsingleapplication/qtsingleapplication.pri)
    DEFINES += MAXCALC_SINGLE_INSTANCE_MODE
}

HEADERS += \
        pch.h \
        mainwindow.h \
        inputbox.h \
        aboutbox.h \
        myaction.h \
        outputsettings.h

SOURCES += \
        main.cpp \
        mainwindow.cpp \
        inputbox.cpp \
        aboutbox.cpp \
        myaction.cpp \
        outputsettings.cpp

RESOURCES += resources.qrc

PRECOMPILED_HEADER = pch.h

INCLUDEPATH += ../engine

CONFIG(debug, debug|release) {
    DEFINES += _DEBUG
    DESTDIR = ../debug
    LIBS += -L../debug -lmaxcalcengine
    win32-msvc*:PRE_TARGETDEPS += ../debug/maxcalcengine.lib
    !win32-msvc*:PRE_TARGETDEPS += ../debug/libmaxcalcengine.a
}
CONFIG(release, debug|release) {
    DEFINES += NDEBUG
    DESTDIR = ../release
    LIBS += -L../release -lmaxcalcengine
    win32-msvc*:PRE_TARGETDEPS += ../release/maxcalcengine.lib
    !win32-msvc*:PRE_TARGETDEPS += ../release/libmaxcalcengine.a
}

win32:maxcalc_gettext:LIBS += -L../intl_win -lintl

maxcalc_unicode:DEFINES += MAXCALC_UNICODE
maxcalc_gettext:DEFINES += MAXCALC_GETTEXT
maxcalc_portable:DEFINES += MAXCALC_PORTABLE
win32:RC_FILE = resources.rc
