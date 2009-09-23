project(MAXCALC)

cmake_minimum_required(VERSION 2.6)

# To eliminate warning when linking to Qt4
if (COMMAND cmake_policy)
	cmake_policy(SET CMP0003 NEW)
endif (COMMAND cmake_policy)

add_subdirectory(engine)
add_subdirectory(cli)
add_subdirectory(gui)
add_subdirectory(test)
