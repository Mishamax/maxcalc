@echo off

rem Variables
set QT_DIR=D:\Programming\Qt4.4.3-msvc2008-static\bin
set MAXCALC_VERSION=1.9.0-m1

rem Build static binaries

set PATH=%QT_DIR%;%PATH%
pushd src
pushd maxcalcengine
nmake clean && qmake -config release && nmake release
popd
pushd maxcalc-cli
nmake clean && qmake -config release && nmake release
popd
pushd maxcalc-gui
nmake clean && qmake -config release && nmake release
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

copy src\release\maxcalc-win.exe release\tmp
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

pause
