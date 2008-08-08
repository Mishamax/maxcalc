# MaxCalc Engine CMakeLists.txt

ADD_DEFINITIONS (-DUNICODE -D_UNICODE)

ADD_LIBRARY (maxcalcengine STATIC
	decNumber/decNumber.cpp
	decNumber/decContext.cpp
	
	stringconv.cpp
	)
