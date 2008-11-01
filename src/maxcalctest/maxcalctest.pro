TEMPLATE = app
TARGET = maxcalctest

CONFIG += qt qtestlib debug_and_release warn_on console
QT = core

HEADERS += utility.h bigdecimaltest.h complextest.h
SOURCES += main.cpp bigdecimaltest.cpp complextest.cpp

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

DEFINES += UNICODE
win32:DEFINES += WIN32
