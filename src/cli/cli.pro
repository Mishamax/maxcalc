TEMPLATE = app
TARGET = maxcalc

include(../maxcalc_config.pri)

CONFIG -= qt
CONFIG += debug_and_release warn_on console precompile_header

HEADERS += pch.h \
           simpleini.h \
           ConvertUTF.h

SOURCES += main.cpp \
           ConvertUTF.cpp

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
win32:DEFINES += _CRT_SECURE_NO_WARNINGS
win32:RC_FILE = resources.rc
