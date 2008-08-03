# MaxCalc Test CMakeLists.txt

#
# MaxCalc Engine
#

INCLUDE_DIRECTORIES (${MAXCALC_SOURCE_DIR}/maxcalcengine)
LINK_DIRECTORIES (${MAXCALC_BINARY_DIR}/maxcalcengine)


#
# Add executable, link MaxCalc Engine and add test
#

ADD_EXECUTABLE (maxcalctest main.cpp)
TARGET_LINK_LIBRARIES (maxcalctest maxcalcengine)
ADD_TEST (maxcalctest ${MAXCALC_BINARY_DIR}/maxcalctest/maxcalctest.exe)
