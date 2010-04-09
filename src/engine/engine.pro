TEMPLATE = lib
TARGET = maxcalcengine

include(../maxcalc_config.pri)

CONFIG -= qt
CONFIG += staticlib debug_and_release warn_on precompile_header

include(decNumber/decNumber.pri)

HEADERS += \
        pch.h \
        bigdecimal.h \
        complex.h \
        constants.h \
        bigdecimalformat.h \
        complexformat.h \
        unicode.h \
        parsercontext.h \
        parser.h \
        variables.h \
        unitconversion.h \
        exceptions.h \
        commandparser.h

SOURCES += \
        bigdecimal.cpp \
        complex.cpp \
        constants.cpp \
        unicode.cpp \
        parsercontext.cpp \
        parser.cpp \
        variables.cpp \
        unitconversion.cpp \
        commandparser.cpp

PRECOMPILED_HEADER = pch.h

CONFIG(debug, debug|release) {
    DEFINES += _DEBUG
    DESTDIR = ../debug
}
CONFIG(release, debug|release) {
    DEFINES += NDEBUG
    DESTDIR = ../release
}

maxcalc_unicode:DEFINES += MAXCALC_UNICODE
maxcalc_gettext:DEFINES += MAXCALC_GETTEXT
win32:DEFINES += _CRT_SECURE_NO_WARNINGS
