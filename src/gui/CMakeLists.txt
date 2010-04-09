
# Add Qt
set(QT_MIN_VERSION "4.2.0")
find_package(Qt4 REQUIRED)

# Enable single instance mode only on Qt 4.4 and higher
if (${QT_VERSION_MINOR} AND ${QT_VERSION_MINOR} GREATER 3)
set(QT_USE_QTNETWORK true)
endif (${QT_VERSION_MINOR} AND ${QT_VERSION_MINOR} GREATER 3)

include(${QT_USE_FILE})

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
set(SOURCES
    aboutbox.cpp
    inputbox.cpp
    main.cpp
    mainwindow.cpp
    myaction.cpp
    outputsettings.cpp)

set(MOC_HEADERS
    aboutbox.h
    inputbox.h
    mainwindow.h
    myaction.h
    outputsettings.h)

# Enable single instance mode only on Qt 4.4 and higher
if (${QT_VERSION_MINOR} AND ${QT_VERSION_MINOR} GREATER 3)

    set (SOURCES ${SOURCES}
        qtsingleapplication/qtsingleapplication.cpp
        qtsingleapplication/qtlocalpeer.cpp
        qtsingleapplication/qtlockedfile.cpp)
    set(MOC_HEADERS ${MOC_HEADERS}
        qtsingleapplication/qtsingleapplication.h
        qtsingleapplication/qtlocalpeer.h)

    if (WIN32)
        set (SOURCES ${SOURCES} qtsingleapplication/qtlockedfile_win.cpp)
    else (WIN32)
        set (SOURCES ${SOURCES} qtsingleapplication/qtlockedfile_unix.cpp)
    endif (WIN32)
    
    add_definitions(-DMAXCALC_SINGLE_INSTANCE_MODE)

endif (${QT_VERSION_MINOR} AND ${QT_VERSION_MINOR} GREATER 3)

if (WIN32)
    if (MINGW)
        # Compile resources on MinGW
        ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/resources.o
            COMMAND windres.exe -I${CMAKE_CURRENT_SOURCE_DIR} -i${CMAKE_CURRENT_SOURCE_DIR}/resources.rc -o ${CMAKE_CURRENT_BINARY_DIR}/resources.o)
        SET(WIN32_RESOURCES ${CMAKE_CURRENT_BINARY_DIR}/resources.o)
    else (MINGW)
        set (WIN32_RESOURCES resources.rc)
    endif (MINGW)
endif (WIN32)

if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:wchar_t-")
endif (MSVC)

qt4_add_resources(QT_RESOURCES resources.qrc)
qt4_wrap_cpp(MOC_SOURCES ${MOC_HEADERS})

# Executable
if (WIN32)
    # WIN32 is specified on Windows to remove console
    add_executable(maxcalcwin WIN32 ${SOURCES} ${MOC_SOURCES} ${WIN32_RESOURCES} ${QT_RESOURCES})
    if (MAXCALC_GETTEXT)
        target_link_libraries(maxcalcwin ${QT_LIBRARIES} engine intl)
    else (MAXCALC_GETTEXT)
        target_link_libraries(maxcalcwin ${QT_LIBRARIES} engine)
    endif (MAXCALC_GETTEXT)
    install(TARGETS maxcalcwin RUNTIME DESTINATION .)
else (WIN32)
    add_executable(maxcalcgui ${SOURCES} ${MOC_SOURCES} ${QT_RESOURCES})
    target_link_libraries(maxcalcgui ${QT_LIBRARIES} engine)
    install(TARGETS maxcalcgui RUNTIME DESTINATION bin)
endif (WIN32)

set(EXECUTABLE_OUTPUT_PATH ../bin)

