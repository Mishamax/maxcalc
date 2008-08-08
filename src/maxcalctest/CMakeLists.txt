# MaxCalc Test CMakeLists.txt

#
# MaxCalc Engine
#

INCLUDE_DIRECTORIES (${MAXCALC_SOURCE_DIR}/maxcalcengine)
LINK_DIRECTORIES (${MAXCALC_BINARY_DIR}/maxcalcengine)


#
# Add executable, link MaxCalc Engine and add test
#

ADD_DEFINITIONS (-DUNICODE -D_UNICODE)

ADD_EXECUTABLE (stringconv stringconv.cpp test.cpp)
TARGET_LINK_LIBRARIES (stringconv maxcalcengine)
ADD_TEST (stringconv ${MAXCALC_BINARY_DIR}/maxcalctest/stringconv.exe)
