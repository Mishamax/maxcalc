@echo off

rem Variables - adjust them to environment
set VSCOMNTOOLS="%VS90COMNTOOLS%\vsvars32.bat"
set QT_DIR=D:\Programming\qt-4.6.2-msvc2008-static\bin
set SEVEN_ZIP_PATH=D:\Misc\7-Zip-x86
set RELEASE=..\..\release
set MAXCALC_VERSION=2.0.1

rem Set up environment

call %VSCOMNTOOLS%
set PATH=%QT_DIR%;%SEVEN_ZIP_PATH%;%PATH%

rem Build static binaries

call clean.bat

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
copy ..\License.txt %RELEASE%\tmp
copy ..\Readme.txt %RELEASE%\tmp
rem copy ..\src\intl_win\intl.dll %RELEASE%\tmp
todos -p %RELEASE%\tmp\Changelog.txt
todos -p %RELEASE%\tmp\License.txt
todos -p %RELEASE%\tmp\Readme.txt

pushd %RELEASE%
pushd tmp
call 7z a -tzip ..\maxcalc-win-console-%MAXCALC_VERSION%.zip * -mx9
popd
rmdir /Q /S tmp
popd

@echo.
@echo CLI VERSION CREATED
@echo.

rem GUI version

mkdir %RELEASE%\tmp

copy ..\src\release\maxcalcwin.exe %RELEASE%\tmp
copy ..\Changelog.txt %RELEASE%\tmp
copy ..\License.txt %RELEASE%\tmp
copy ..\Readme.txt %RELEASE%\tmp
rem copy ..\src\intl_win\intl.dll %RELEASE%\tmp
todos -p %RELEASE%\tmp\Changelog.txt
todos -p %RELEASE%\tmp\License.txt
todos -p %RELEASE%\tmp\Readme.txt

pushd %RELEASE%
pushd tmp
call 7z a -tzip ..\maxcalc-win-gui-%MAXCALC_VERSION%.zip * -mx9
popd
rmdir /Q /S tmp
popd

@echo.
@echo GUI VERSION CREATED
@echo.

rem Create archive with source

call hg archive -t files %RELEASE%\tmp
pushd %RELEASE%
pushd tmp
call 7z a -tzip ..\maxcalc-src-%MAXCALC_VERSION%.zip * -mx9
popd
rmdir /Q /S tmp
popd

@echo.
@echo SOURCE ARCHIVE CREATED
@echo.

rem Create folder for new version

pushd %RELEASE%
mkdir v%MAXCALC_VERSION%
move /Y *.zip v%MAXCALC_VERSION%\
popd

@echo.
@echo RELEASE CREATED
@echo.

pause
