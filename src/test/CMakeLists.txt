
enable_testing()

# Add Qt
set(QT_MIN_VERSION "4.2.0")
set (QT_USE_QTTEST TRUE)
set (QT_DONT_USE_QTGUI TRUE)
find_package(Qt4 REQUIRED)
include(${QT_USE_FILE})
include_directories(${QT_QTTEST_INCLUDE_DIR})

# It's needed to solve linking problems under Windows when WIN32 option is specified in ADD_EXECUTABLE()
if (WIN32)
    link_libraries(${QT_QTMAIN_LIBRARY})
endif (WIN32)

# Engine
include_directories(${MAXCALC_SOURCE_DIR}/engine)
if (WIN32 AND MAXCALC_GETTEXT)
    link_directories(${MAXCALC_BINARY_DIR}/engine ${MAXCALC_SOURCE_DIR}/intl_win)
else (WIN32 AND MAXCALC_GETTEXT)
    link_directories(${MAXCALC_BINARY_DIR}/engine)
endif (WIN32 AND MAXCALC_GETTEXT)

# Files
set (SOURCES
    bigdecimaltest.cpp
    complextest.cpp
    main.cpp
    parsertest.cpp
    unitconversiontest.cpp
    variablestest.cpp)

set (MOC_HEADERS
    bigdecimaltest.h
    complextest.h
    parsertest.h
    unitconversiontest.h
    variablestest.h)
    
qt4_wrap_cpp(MOC_SOURCES ${MOC_HEADERS})

if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:wchar_t-")
endif (MSVC)

# Executable
add_executable(maxcalctest ${SOURCES} ${MOC_SOURCES})
if (WIN32 AND MAXCALC_GETTEXT)
    target_link_libraries(maxcalctest ${QT_LIBRARIES} engine intl)
else (WIN32 AND MAXCALC_GETTEXT)
    target_link_libraries(maxcalctest ${QT_LIBRARIES} engine)
endif (WIN32 AND MAXCALC_GETTEXT)
add_test(maxcalctest ${EXECUTABLE_OUTPUT_PATH}/maxcalctest MaxCalcTest)
