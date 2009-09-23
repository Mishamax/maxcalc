
find_package(Qt4)
include(${QT_USE_FILE})


# It's needed to solve linking problems under Windows when WIN32 option is specified in ADD_EXECUTABLE()
if (WIN32)
	link_libraries(${QT_QTMAIN_LIBRARY})
endif (WIN32)

include_directories(${MAXCALC_SOURCE_DIR}/engine)
link_directories(${MAXCALC_BINARY_DIR}/engine)

add_definitions(-DMAXCALC_UNICODE)

add_executable(maxcalctest
    bigdecimaltest.cpp
    complextest.cpp
    main.cpp
    parsertest.cpp
    unitconversiontest.cpp
    variablestest.cpp)

target_link_libraries(maxcalctest engine)
