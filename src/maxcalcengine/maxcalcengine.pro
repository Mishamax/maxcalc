TEMPLATE = lib
TARGET = maxcalcengine

CONFIG -= qt
CONFIG += staticlib debug_and_release warn_on precompile_header

HEADERS += decNumber/decContext.h decNumber/decNumber.h decNumber/decNumberLocal.h decNumber/stdint.h
SOURCES += decNumber/decContext.cpp decNumber/decNumber.cpp

HEADERS += bigdecimal.h complex.h precision.h bigdecimalformat.h complexformat.h unicode.h parsercontext.h parser.h version.h variables.h unitconversion.h
SOURCES += bigdecimal.cpp complex.cpp bigdecimalformat.cpp complexformat.cpp unicode.cpp parsercontext.cpp parser.cpp variables.cpp unitconversion.cpp

PRECOMPILED_HEADER = pch.h

CONFIG(debug, debug|release) {
	DEFINES += _DEBUG
	DESTDIR = ../debug
}
CONFIG(release, debug|release) {
	DEFINES += NDEBUG
	DESTDIR = ../release
}

DEFINES += MAXCALC_UNICODE
win32:DEFINES += WIN32
