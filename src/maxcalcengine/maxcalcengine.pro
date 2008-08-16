TEMPLATE = lib
TARGET = maxcalcengine

CONFIG += staticlib qt debug_and_release warn_on
QT -= gui

HEADERS += decNumber/decContext.h decNumber/decNumber.h decNumber/decNumberLocal.h decNumber/stdint.h
SOURCES += decNumber/decContext.cpp decNumber/decNumber.cpp

HEADERS += exception.h bigdecimal.h complex.h settings.h
SOURCES += bigdecimal.cpp complex.cpp

CONFIG(debug, debug|release) {
	DEFINES += _DEBUG
	DESTDIR = ../debug
}
CONFIG(release, debug|release) {
	DEFINES += NDEBUG
	DESTDIR = ../release
}
