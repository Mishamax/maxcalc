TEMPLATE = lib
TARGET = maxcalcengine

CONFIG -= qt
CONFIG += staticlib debug_and_release warn_on precompile_header

include(decNumber/decNumber.pri)

HEADERS += \
        pch.h \
        bigdecimal.h \
        complex.h \
        precision.h \
        bigdecimalformat.h \
        complexformat.h \
        unicode.h \
        parsercontext.h \
        parser.h \
        version.h \
        variables.h \
        unitconversion.h \
        exceptions.h

SOURCES += \
        bigdecimal.cpp \
        complex.cpp \
        bigdecimalformat.cpp \
        complexformat.cpp \
        unicode.cpp \
        parsercontext.cpp \
        parser.cpp \
        variables.cpp \
        unitconversion.cpp

maxcalc_mathml {
    HEADERS += Markup.h mathmlparser.h
    SOURCES += Markup.cpp mathmlparser.cpp
    DEFINES += MAXCALC_MATHML
}

PRECOMPILED_HEADER = pch.h

CONFIG(debug, debug|release) {
    DEFINES += _DEBUG
    DESTDIR = ../debug
}
CONFIG(release, debug|release) {
    DEFINES += NDEBUG
    DESTDIR = ../release
}

DEFINES += MAXCALC_UNICODE _CRT_SECURE_NO_WARNINGS
win32:DEFINES += WIN32
