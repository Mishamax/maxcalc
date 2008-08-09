# MaxCalc Engine CMakeLists.txt

#
# Qt4 support
#

FIND_PACKAGE (Qt4)
SET(QT_DONT_USE_QTGUI 1)
INCLUDE (${QT_USE_FILE})

#
# Add library
#

ADD_DEFINITIONS (-DUNICODE -D_UNICODE)

ADD_LIBRARY (maxcalcengine STATIC
	decNumber/decNumber.cpp
	decNumber/decContext.cpp
	
	bigdecimal.cpp
	)

TARGET_LINK_LIBRARIES (maxcalcengine ${QT_LIBRARIES})
