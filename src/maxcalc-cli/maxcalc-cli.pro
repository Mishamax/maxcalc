TEMPLATE = app
TARGET = maxcalc
CONFIG += qt debug_and_release warn_on precompile_header
QT -= gui
win32:CONFIG += console

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
