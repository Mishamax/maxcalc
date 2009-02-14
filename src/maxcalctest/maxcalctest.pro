TEMPLATE = app
TARGET = maxcalctest

CONFIG += qt qtestlib debug_and_release warn_on console
QT = core

HEADERS += utility.h bigdecimaltest.h complextest.h parsertest.h bigdecimalbench.h variablestest.h
SOURCES += main.cpp bigdecimaltest.cpp complextest.cpp parsertest.cpp bigdecimalbench.cpp variablestest.cpp

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
win32:DEFINES += WIN32 _CRT_SECURE_NO_WARNINGS
