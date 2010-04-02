
# Add gtkmm
find_package(PkgConfig)
pkg_check_modules(GTKMM gtkmm-2.4)
include_directories (${GTKMM_INCLUDE_DIRS})

# Engine
include_directories(${MAXCALC_SOURCE_DIR}/engine)
if (WIN32 AND MAXCALC_GETTEXT)
    link_directories(${MAXCALC_BINARY_DIR}/engine ${MAXCALC_SOURCE_DIR}/intl_win)
else (WIN32 AND MAXCALC_GETTEXT)
    link_directories(${MAXCALC_BINARY_DIR}/engine)
endif (WIN32 AND MAXCALC_GETTEXT)

# Files
set(SOURCES
    main.cpp
    mainwindow.cpp)

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

# Executable
if (WIN32)
    # WIN32 is specified on Windows to remove console
    add_executable(maxcalcwin-gtk WIN32 ${SOURCES} ${WIN32_RESOURCES})
    if (MAXCALC_GETTEXT)
        target_link_libraries(maxcalcwin-gtk ${GTKMM_LIBRARIES} engine intl)
    else (MAXCALC_GETTEXT)
        target_link_libraries(maxcalcwin-gtk ${GTKMM_LIBRARIES} engine)
    endif (MAXCALC_GETTEXT)
else (WIN32)
    add_executable(maxcalcgui-gtk ${SOURCES})
    target_link_libraries(maxcalcgui-gtk ${GTKMM_LIBRARIES} engine)
endif (WIN32)

set(EXECUTABLE_OUTPUT_PATH ../bin)
