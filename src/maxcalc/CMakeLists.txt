# MaxCalc CMakeLists.txt

#
# MaxCalc Engine
#

INCLUDE_DIRECTORIES (${MAXCALC_SOURCE_DIR}/maxcalcengine)
LINK_DIRECTORIES (${MAXCALC_BINARY_DIR}/maxcalcengine)


#
# Add executable and link MaxCalc Engine
#

ADD_EXECUTABLE (maxcalc main.cpp)
TARGET_LINK_LIBRARIES (maxcalc maxcalcengine)
