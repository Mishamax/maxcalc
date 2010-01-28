TEMPLATE = app
TARGET = maxcalctest

CONFIG += qt qtestlib debug_and_release warn_on console precompile_header link_prl
PRE_TARGETDEPS += ../engine

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
}
CONFIG(release, debug|release) { 
    DEFINES += NDEBUG
    DESTDIR = ../release
    LIBS += -L../release -lmaxcalcengine
}

DEFINES += MAXCALC_UNICODE

win32:DEFINES += WIN32 _CRT_SECURE_NO_WARNINGS
