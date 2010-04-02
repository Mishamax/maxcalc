
project(MAXCALC)

cmake_minimum_required(VERSION 2.4.4)

### Options ###

# Unicode build.
# Note that only engine and cli can be built without unicode support
# (so that basic console version can be created).
# To build gui end test projects, unicode must be enabled.
option(MAXCALC_UNICODE "Enable Unicode support (note that without unicode only console version (engine and cli) can be built)" ON)

# GetText support.
# This enables localization.
option(MAXCALC_GETTEXT "Enable GetText localization support" OFF)


# To eliminate warning when linking to Qt4
if (COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
endif (COMMAND cmake_policy)

# Macros
if (MAXCALC_UNICODE)
	add_definitions(-DMAXCALC_UNICODE)
endif (MAXCALC_UNICODE)
if (MAXCALC_GETTEXT)
	add_definitions(-DMAXCALC_GETTEXT)
endif (MAXCALC_GETTEXT)
if (MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif(MSVC)

add_subdirectory(engine)
add_subdirectory(cli)
if (MAXCALC_UNICODE)
	add_subdirectory(gui)
	add_subdirectory(gui_gtk)
	add_subdirectory(test)
endif (MAXCALC_UNICODE)
