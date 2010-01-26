TEMPLATE = app
win32:TARGET = maxcalcwin
unix:TARGET = maxcalcgui

CONFIG += qt debug_and_release warn_on precompile_header link_prl
win32:CONFIG += windows
QT += network
PRE_TARGETDEPS += maxcalcengine

include(qtsingleapplication/qtsingleapplication.pri)

HEADERS += \
        pch.h \
        mainwindow.h \
        inputbox.h \
        aboutbox.h \
        myaction.h \
        ../engine/i18n.h

SOURCES += \
        main.cpp \
        mainwindow.cpp \
        inputbox.cpp \
        aboutbox.cpp \
        myaction.cpp \
        ../engine/i18n.cpp

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

DEFINES += MAXCALC_UNICODE MAXCALC_QT_I18N
win32:DEFINES += WIN32

RC_FILE = resources.rc
