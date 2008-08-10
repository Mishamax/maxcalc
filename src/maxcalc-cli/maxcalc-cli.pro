TEMPLATE = app
TARGET = maxcalc
CONFIG += qt debug_and_release warn_on
QT -= gui
win32:CONFIG += console

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
