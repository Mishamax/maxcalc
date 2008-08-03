# MaxCalc main CMakeLists.txt

PROJECT (MAXCALC)

CMAKE_MINIMUM_REQUIRED (VERSION 2.4)

# To eliminate warning when linking to Qt4
IF (COMMAND cmake_policy)
	CMAKE_POLICY (SET CMP0003 NEW)
ENDIF (COMMAND cmake_policy)

ADD_SUBDIRECTORY (maxcalcengine)
ADD_SUBDIRECTORY (maxcalc)
ADD_SUBDIRECTORY (qmaxcalc)
