
# Files
set(SOURCES
    decNumber/decContext.cpp
    decNumber/decNumber.cpp
    bigdecimal.cpp
    complex.cpp
    parser.cpp
    parsercontext.cpp
    unicode.cpp
    unitconversion.cpp
    variables.cpp)

if (MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:wchar_t-")
endif (MSVC)

# Library
add_library(engine STATIC ${SOURCES})
