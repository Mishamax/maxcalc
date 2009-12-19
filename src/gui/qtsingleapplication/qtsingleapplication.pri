
HEADERS += \
        qtsingleapplication/qtsingleapplication.h \
        qtsingleapplication/qtlocalpeer.h \
        qtsingleapplication/qtlockedfile.h

SOURCES += \
        qtsingleapplication/qtsingleapplication.cpp \
        qtsingleapplication/qtlocalpeer.cpp \
        qtsingleapplication/qtlockedfile.cpp

win32:SOURCES += qtsingleapplication/qtlockedfile_win.cpp
unix:SOURCES += qtsingleapplication/qtlockedfile_unix.cpp
