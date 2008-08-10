TEMPLATE = app
win32:TARGET = maxcalc-win
unix:TARGET = maxcalc-gui

CONFIG += qt debug_and_release warn_on
win32:CONFIG += windows

SOURCES += main.cpp

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
