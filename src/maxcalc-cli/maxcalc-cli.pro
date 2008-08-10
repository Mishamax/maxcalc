TEMPLATE = app
TARGET = maxcalc
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

SOURCES += main.cpp

INCLUDEPATH += ../maxcalcengine
