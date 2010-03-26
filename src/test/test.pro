TEMPLATE = app
TARGET = maxcalctest

include(../maxcalc_config.pri)

CONFIG += qt qtestlib debug_and_release warn_on console precompile_header

QT = core

HEADERS += pch.h \
    utility.h \
    bigdecimaltest.h \
    complextest.h \
    parsertest.h \
    variablestest.h \
    unitconversiontest.h

SOURCES += main.cpp \
    bigdecimaltest.cpp \
    complextest.cpp \
    parsertest.cpp \
    variablestest.cpp \
    unitconversiontest.cpp

PRECOMPILED_HEADER = pch.h

INCLUDEPATH += ../engine

CONFIG(debug, debug|release) { 
    DEFINES += _DEBUG
    DESTDIR = ../debug
    LIBS += -L../debug -lmaxcalcengine
    win32:PRE_TARGETDEPS += ../debug/maxcalcengine.lib
    !win32:PRE_TARGETDEPS += ../debug/maxcalcengine.a
}
CONFIG(release, debug|release) { 
    DEFINES += NDEBUG
    DESTDIR = ../release
    LIBS += -L../release -lmaxcalcengine
    win32:PRE_TARGETDEPS += ../release/maxcalcengine.lib
    !win32:PRE_TARGETDEPS += ../release/maxcalcengine.a
}

win32:maxcalc_gettext:LIBS += -L../intl_win -lintl

maxcalc_unicode:DEFINES += MAXCALC_UNICODE
maxcalc_gettext:DEFINES += MAXCALC_GETTEXT
win32:DEFINES += _CRT_SECURE_NO_WARNINGS
