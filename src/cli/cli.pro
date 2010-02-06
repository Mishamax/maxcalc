TEMPLATE = app
TARGET = maxcalc

include(../maxcalc_config.pri)

CONFIG -= qt
CONFIG += debug_and_release warn_on console precompile_header

HEADERS += pch.h
SOURCES += main.cpp

PRECOMPILED_HEADER = pch.h

INCLUDEPATH += ../engine

CONFIG(debug, debug|release) {
    DEFINES += _DEBUG
    DESTDIR = ../debug
    LIBS += -L../debug -lmaxcalcengine
}
CONFIG(release, debug|release) {
    DEFINES += NDEBUG
    DESTDIR = ../release
    LIBS += -L../release -lmaxcalcengine
}

win32:maxcalc_gettext:LIBS += -L../intl_win -lintl

maxcalc_unicode:DEFINES += MAXCALC_UNICODE
maxcalc_gettext:DEFINES += MAXCALC_GETTEXT
win32:DEFINES += WIN32
win32:RC_FILE = resources.rc
