
# Add Qt
set(QT_MIN_VERSION "4.2.0")
find_package(Qt4 REQUIRED)
set(QT_USE_QTNETWORK true)
include(${QT_USE_FILE})

# It's needed to solve linking problems under Windows when WIN32 option is specified in ADD_EXECUTABLE()
if (WIN32)
    link_libraries(${QT_QTMAIN_LIBRARY})
endif (WIN32)

# Macros
add_definitions(-DMAXCALC_QT_I18N)


# Engine
include_directories(${MAXCALC_SOURCE_DIR}/engine)
link_directories(${MAXCALC_BINARY_DIR}/engine)


# Files
set(SOURCES
    aboutbox.cpp
    inputbox.cpp
    main.cpp
    mainwindow.cpp
    myaction.cpp
    ../engine/i18n.cpp
    qtsingleapplication/qtsingleapplication.cpp
    qtsingleapplication/qtlocalpeer.cpp
    qtsingleapplication/qtlockedfile.cpp)

set(MOC_HEADERS
    aboutbox.h
    inputbox.h
    mainwindow.h
    myaction.h
    ../engine/i18n.h
    qtsingleapplication/qtsingleapplication.h
    qtsingleapplication/qtlocalpeer.h
    qtsingleapplication/qtlockedfile.h)

set(LANG translations/maxcalcgui_ru.ts)
	
if (WIN32)
    set (SOURCES ${SOURCES} qtsingleapplication/qtlockedfile_win.cpp)
    set (WIN32_RESOURCES resources.rc)
else (WIN32)
    set (SOURCES ${SOURCES} qtsingleapplication/qtlockedfile_unix.cpp)
endif (WIN32)

if (MSVC)
    set_source_files_properties(${SOURCES} ${MOC_SOURCES} PROPERTIES COMPILE_FLAGS /Zc:wchar_t-)
endif (MSVC)

qt4_add_resources(QT_RESOURCES resources.qrc)
qt4_wrap_cpp(MOC_SOURCES ${MOC_HEADERS})
qt4_add_translation(TRANSLATIONS ${LANG})

# Executable
if (WIN32)
    # WIN32 is specified on Windows to remove console
    add_executable(maxcalcwin WIN32 ${SOURCES} ${MOC_SOURCES} ${WIN32_RESOURCES} ${QT_RESOURCES} ${TRANSLATIONS})
    target_link_libraries(maxcalcwin ${QT_LIBRARIES} engine)
else (WIN32)
    add_executable(maxcalcgui ${SOURCES} ${MOC_SOURCES} ${QT_RESOURCES} ${TRANSLATIONS})
    target_link_libraries(maxcalcgui ${QT_LIBRARIES} engine)
endif (WIN32)
