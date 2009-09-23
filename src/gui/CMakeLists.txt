
# Add Qt
set(QT_MIN_VERSION "4.2.0")
find_package(Qt4 REQUIRED)
include(${QT_USE_FILE})

# It's needed to solve linking problems under Windows when WIN32 option is specified in ADD_EXECUTABLE()
if (WIN32)
	link_libraries(${QT_QTMAIN_LIBRARY})
endif (WIN32)

# Engine
include_directories(${MAXCALC_SOURCE_DIR}/engine)
link_directories(${MAXCALC_BINARY_DIR}/engine)

# Macros
add_definitions(-DMAXCALC_UNICODE)

# Files
set(SOURCES
       aboutbox.cpp
        inputbox.cpp
        main.cpp
        mainwindow.cpp
        myaction.cpp)

set(HEADERS
    aboutbox.h
    inputbox.h
    mainwindow.h
    myaction.h)

qt4_wrap_cpp(MOC_SOURCES ${HEADERS})

# WIN32 is specified on Windows to remove console
if (WIN32)
    add_executable(maxcalcwin WIN32 ${SOURCES} ${MOC_SOURCES})
    target_link_libraries(maxcalcwin ${QT_LIBRARIES} engine)
else (WIN32)
    add_executable(maxcalcgui ${SOURCES} ${MOC_SOURCES})
    target_link_libraries(maxcalcgui ${QT_LIBRARIES} engine)
endif (WIN32)
