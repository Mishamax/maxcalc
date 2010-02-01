TEMPLATE = app
win32:TARGET = maxcalcwin
unix:TARGET = maxcalcgui

include(../maxcalc_config.pri)

CONFIG += qt debug_and_release warn_on precompile_header
win32:CONFIG += windows
QT += network

include(qtsingleapplication/qtsingleapplication.pri)

HEADERS += \
        pch.h \
        mainwindow.h \
        inputbox.h \
        aboutbox.h \
        myaction.h

SOURCES += \
        main.cpp \
        mainwindow.cpp \
        inputbox.cpp \
        aboutbox.cpp \
        myaction.cpp

RESOURCES += resources.qrc

TRANSLATIONS = translations/maxcalcgui_ru.ts

PRECOMPILED_HEADER = pch.h

INCLUDEPATH += ../engine

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

win32:maxcalc_gettext:LIBS += -L../intl_win -lintl

maxcalc_unicode:DEFINES += MAXCALC_UNICODE
maxcalc_gettext:DEFINES += MAXCALC_GETTEXT
win32:DEFINES += WIN32
win32:RC_FILE = resources.rc
