@echo off

rem Variables
set QT_DIR=D:\Programming\Qt4.4.3-msvc2008\bin
set MAXCALC_VERSION=1.9.0-m1

rem CLI version

mkdir release\tmp

copy src\release\maxcalc.exe release\tmp
copy doc\Changelog.txt release\tmp
copy LICENSE.GPL2 release\tmp
copy LICENSE.ICU release\tmp

pushd release
pushd tmp
7z a -tzip ..\maxcalc-%MAXCALC_VERSION%.zip * -mx7
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
copy %QT_DIR%\QtGui4.dll release\tmp
copy %QT_DIR%\QtCore4.dll release\tmp
copy doc\Changelog.txt release\tmp
copy LICENSE.GPL2 release\tmp
copy LICENSE.ICU release\tmp

pushd release
pushd tmp
7z a -tzip ..\maxcalc-win-%MAXCALC_VERSION%.zip * -mx7
popd
del /Q tmp\*
rmdir /Q tmp
popd

@echo.
@echo GUI VERSION CREATED
@echo.
