@echo off

rem Variables
call "%VS90COMNTOOLS%\vsvars32.bat"
set QT_DIR=D:\Programming\qt-4.5.1-msvc2008-static\bin
set MAXCALC_VERSION=1.9.2-m3
set PATH=D:\Misc\7-Zip;%PATH%

rem Build static binaries

set PATH=%QT_DIR%;%PATH%
pushd src
pushd maxcalcengine
qmake -config release && nmake clean && nmake release
popd
pushd maxcalc-cli
qmake -config release && nmake clean && nmake release
popd
pushd maxcalc-gui
qmake -config release && nmake clean && nmake release
popd
popd

rem CLI version

mkdir release\tmp

copy src\release\maxcalc.exe release\tmp
copy doc\Changelog.txt release\tmp
copy LICENSE.GPL2 release\tmp\LICENSE-GPL2.txt
copy LICENSE.ICU release\tmp\LICENSE-ICU.txt

pushd release
del \Q maxcalc-win-console-%MAXCALC_VERSION%.zip
pushd tmp
7z a -tzip ..\maxcalc-win-console-%MAXCALC_VERSION%.zip * -mx7
popd
del /Q tmp\*
rmdir /Q tmp
popd

@echo.
@echo CLI VERSION CREATED
@echo.

rem GUI version

mkdir release\tmp

copy src\release\maxcalcwin.exe release\tmp
copy doc\Changelog.txt release\tmp
copy LICENSE.GPL2 release\tmp\LICENSE-GPL2.txt
copy LICENSE.ICU release\tmp\LICENSE-ICU.txt

pushd release
del \Q maxcalc-win-gui-%MAXCALC_VERSION%.zip
pushd tmp
7z a -tzip ..\maxcalc-win-gui-%MAXCALC_VERSION%.zip * -mx7
popd
del /Q tmp\*
rmdir /Q tmp
popd

@echo.
@echo GUI VERSION CREATED
@echo.

rem Create folder for new version

pushd release
mkdir v%MAXCALC_VERSION%
move *.zip v%MAXCALC_VERSION%\
popd

@echo.
@echo RELEASE CREATED
@echo.

pause
