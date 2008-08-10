TEMPLATE = app
win32:TARGET = maxcalc-win
unix:TARGET = maxcalc-gui
debug {
	DESTDIR = ../debug
	LIBS += -L../debug -lmaxcalcengine
}
release {
	DESTDIR = ../release
	LIBS += -L../release -lmaxcalcengine
}
CONFIG += qt debug_and_release warn_on windows

SOURCES += main.cpp

INCLUDEPATH += ../maxcalcengine
