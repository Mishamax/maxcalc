
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
    ConvertUTF.cpp)

# Platform-specific files
if (WIN32)
    set(WIN32_RESOURCES resources.rc)
endif (WIN32)

if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:wchar_t-")
endif (MSVC)

# Executable
if (WIN32)
    add_executable(maxcalc ${SOURCES} ${WIN32_RESOURCES})
    install(TARGETS maxcalc RUNTIME DESTINATION .)
else (WIN32)
    add_executable(maxcalc ${SOURCES})
    install(TARGETS maxcalc RUNTIME DESTINATION bin)
endif (WIN32)
if (WIN32 AND MAXCALC_GETTEXT)
    target_link_libraries(maxcalc engine intl)
else (WIN32 AND MAXCALC_GETTEXT)
    target_link_libraries(maxcalc engine)
endif (WIN32 AND MAXCALC_GETTEXT)

set(EXECUTABLE_OUTPUT_PATH ../bin)

