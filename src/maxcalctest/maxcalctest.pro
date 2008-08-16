TEMPLATE = app
TARGET = maxcalctest

CONFIG += qt debug_and_release warn_on console
QT = core testlib

HEADERS += bigdecimaltest.h
SOURCES += main.cpp bigdecimaltest.cpp

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