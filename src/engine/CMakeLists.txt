
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
    variables.cpp
    commandparser.cpp
    constants.cpp)

if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:wchar_t-")
endif (MSVC)

# Library
add_library(engine STATIC ${SOURCES})
