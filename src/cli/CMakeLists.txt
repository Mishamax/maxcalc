
# Engine
include_directories(${MAXCALC_SOURCE_DIR}/engine)
link_directories(${MAXCALC_BINARY_DIR}/engine)

# Files

set(SOURCES
    main.cpp)

if (MSVC)
    set_source_files_properties(${SOURCES} PROPERTIES COMPILE_FLAGS /Zc:wchar_t-)
endif (MSVC)

# Executable
add_executable(maxcalc ${SOURCES})
target_link_libraries(maxcalc engine)
