TEMPLATE = app
TARGET = maxcalc

CONFIG -= qt
CONFIG += debug_and_release warn_on console precompile_header link_prl
PRE_TARGETDEPS += ../engine

HEADERS += pch.h ../engine/i18n.h
SOURCES += main.cpp ../engine/i18n.cpp

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

DEFINES += MAXCALC_UNICODE MAXCALC_NO_I18N
win32:DEFINES += WIN32
