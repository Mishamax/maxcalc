TEMPLATE = app
win32:TARGET = maxcalc-win
unix:TARGET = maxcalc-gui

CONFIG += qt debug_and_release warn_on precompile_header
win32:CONFIG += windows

HEADERS += mainwindow.h inputbox.h aboutbox.h
SOURCES += main.cpp mainwindow.cpp inputbox.cpp aboutbox.cpp

PRECOMPILED_HEADER = pch.h

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

DEFINES += UNICODE
win32:DEFINES += WIN32
