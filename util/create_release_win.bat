@echo off

rem Variables - adjust them to environment
set VSCOMNTOOLS="%VS90COMNTOOLS%\vsvars32.bat"
set QT_DIR=D:\Programming\qt-4.5.2-msvc2008-static\bin
set SEVEN_ZIP_PATH=D:\Misc\7-Zip-x86
set RELEASE=..\..\release
set MAXCALC_VERSION=1.9.3-m4

rem Set up environment

call %VSCOMNTOOLS%
set PATH=%QT_DIR%;%SEVEN_ZIP_PATH%;%PATH%

rem Build static binaries

pushd ..\src
pushd engine
call qmake -config release && nmake clean && nmake release
popd
pushd cli
call qmake -config release && nmake clean && nmake release
popd
pushd gui
call qmake -config release && nmake clean && nmake release
popd
popd

rem CLI version

mkdir %RELEASE%\tmp

copy ..\src\release\maxcalc.exe %RELEASE%\tmp
copy ..\Changelog.txt %RELEASE%\tmp
copy ..\LICENSE.GPL2 %RELEASE%\tmp\LICENSE-GPL2.txt
copy ..\LICENSE.ICU %RELEASE%\tmp\LICENSE-ICU.txt

pushd %RELEASE%
del /Q maxcalc-win-console-%MAXCALC_VERSION%.zip
pushd tmp
call 7z a -tzip ..\maxcalc-win-console-%MAXCALC_VERSION%.zip * -mx7
popd
del /Q tmp\*
rmdir /Q tmp
popd

@echo.
@echo CLI VERSION CREATED
@echo.

rem GUI version

mkdir %RELEASE%\tmp

copy ..\src\release\maxcalcwin.exe %RELEASE%\tmp
copy ..\Changelog.txt %RELEASE%\tmp
copy ..\LICENSE.GPL2 %RELEASE%\tmp\LICENSE-GPL2.txt
copy ..\LICENSE.ICU %RELEASE%\tmp\LICENSE-ICU.txt

pushd %RELEASE%
del /Q maxcalc-win-gui-%MAXCALC_VERSION%.zip
pushd tmp
call 7z a -tzip ..\maxcalc-win-gui-%MAXCALC_VERSION%.zip * -mx7
popd
del /Q tmp\*
rmdir /Q tmp
popd

@echo.
@echo GUI VERSION CREATED
@echo.

rem Create folder for new version

pushd %RELEASE%
mkdir v%MAXCALC_VERSION%
move *.zip v%MAXCALC_VERSION%\
popd

@echo.
@echo RELEASE CREATED
@echo.

pause
