# QMaxCalc CMakeLists.txt

#
# Qt4 support
#

FIND_PACKAGE (Qt4)
INCLUDE (${QT_USE_FILE})

# It's needed to solve linking problems under Windows when WIN32 option is specified in ADD_EXECUTABLE()
IF (WIN32)
	LINK_LIBRARIES (${QT_QTMAIN_LIBRARY})
ENDIF (WIN32)


#
# MaxCalc Engine
#

INCLUDE_DIRECTORIES (${MAXCALC_SOURCE_DIR}/maxcalcengine)
LINK_DIRECTORIES (${MAXCALC_BINARY_DIR}/maxcalcengine)


#
# Add executable and link MaxCalc Engine
#

ADD_DEFINITIONS (-DUNICODE -D_UNICODE)

# WIN32 is specified on Windows to remove console
IF (WIN32)
	ADD_EXECUTABLE (maxcalc-win WIN32 main.cpp)
	TARGET_LINK_LIBRARIES (maxcalc-win ${QT_LIBRARIES} maxcalcengine)
ELSE (WIN32)
	ADD_EXECUTABLE (maxcalc-gui main.cpp)
	TARGET_LINK_LIBRARIES (maxcalc-gui ${QT_LIBRARIES} maxcalcengine)
ENDIF (WIN32)

