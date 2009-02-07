TEMPLATE = app
TARGET = maxcalc

CONFIG -= qt
CONFIG += debug_and_release warn_on precompile_header
win32:CONFIG += console

HEADERS += unicode.h
SOURCES += main.cpp

PRECOMPILED_HEADER = pch.h

INCLUDEPATH += ../maxcalcengine

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
win32:DEFINES += WIN32
