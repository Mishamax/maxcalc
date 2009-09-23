add_definitions(-DMAXCALC_UNICODE -D_CRT_SECURE_NO_WARNINGS)

add_library(engine STATIC
    decNumber/decContext.cpp
    decNumber/decNumber.cpp
    bigdecimal.cpp
    bigdecimalformat.cpp
    complex.cpp
    complexformat.cpp
    parser.cpp
    parsercontext.cpp
    unicode.cpp
    unitconversion.cpp
    variables.cpp)
