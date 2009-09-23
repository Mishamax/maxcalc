include_directories(${MAXCALC_SOURCE_DIR}/engine)
link_directories(${MAXCALC_BINARY_DIR}/engine)

add_definitions(-DMAXCALC_UNICODE)
add_executable(maxcalc main.cpp)
target_link_libraries(maxcalc engine)
