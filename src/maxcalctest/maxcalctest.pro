TEMPLATE = app
TARGET = maxcalctest
debug {
	DESTDIR = ../debug
	LIBS += -L../debug -lmaxcalcengine
}
release {
	DESTDIR = ../release
	LIBS += -L../release -lmaxcalcengine
}
CONFIG += qt debug_and_release warn_on console
QT -= gui

HEADERS += test.h
SOURCES += test.cpp

INCLUDEPATH += ../maxcalcengine
