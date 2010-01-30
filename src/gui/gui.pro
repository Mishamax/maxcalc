TEMPLATE = app
win32:TARGET = maxcalcwin
unix:TARGET = maxcalcgui

CONFIG += qt debug_and_release warn_on precompile_header link_prl
win32:CONFIG += windows
QT += network
PRE_TARGETDEPS += ../engine

include(qtsingleapplication/qtsingleapplication.pri)
include(../i18n/i18n.pri)

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

INCLUDEPATH += ../engine ../i18n

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

DEFINES += MAXCALC_UNICODE MAXCALC_QT_I18N
win32:DEFINES += WIN32

RC_FILE = resources.rc
