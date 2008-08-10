TEMPLATE = lib
TARGET = maxcalcengine
debug {
	DESTDIR = ../debug
}
release {
	DESTDIR = ../release
}
CONFIG += staticlib qt debug_and_release warn_on
QT -= gui

HEADERS += decNumber/decContext.h decNumber/decNumber.h decNumber/decNumberLocal.h decNumber/stdint.h
SOURCES += decNumber/decContext.cpp decNumber/decNumber.cpp

HEADERS += bigdecimal.h
SOURCES += bigdecimal.cpp

