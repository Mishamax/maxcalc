# MaxCalc CMakeLists.txt

#
# Qt4 support
#

FIND_PACKAGE (Qt4)
SET(QT_DONT_USE_QTGUI 1)
INCLUDE (${QT_USE_FILE})

#
# MaxCalc Engine
#

INCLUDE_DIRECTORIES (${MAXCALC_SOURCE_DIR}/maxcalcengine)
LINK_DIRECTORIES (${MAXCALC_BINARY_DIR}/maxcalcengine)


#
# Add executable and link MaxCalc Engine
#

ADD_DEFINITIONS (-DUNICODE -D_UNICODE)

ADD_EXECUTABLE (maxcalc main.cpp)
TARGET_LINK_LIBRARIES (maxcalc ${QT_LIBRARIES} maxcalcengine)
