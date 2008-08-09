# MaxCalc Test CMakeLists.txt

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
# Add executable, link MaxCalc Engine and add test
#

ADD_DEFINITIONS (-DUNICODE -D_UNICODE)

ADD_EXECUTABLE (stringconv test.cpp)
TARGET_LINK_LIBRARIES (stringconv ${QT_LIBRARIES} maxcalcengine)
ADD_TEST (stringconv ${MAXCALC_BINARY_DIR}/maxcalctest/stringconv.exe)
