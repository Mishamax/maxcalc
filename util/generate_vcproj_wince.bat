call "%VS90COMNTOOLS%\vsvars32.bat"
set PATH=D:\Programming\qt-embedded-wince-4.5.1-wincewm50smart-msvc2008\bin;%PATH%
call setcepaths wincewm50smart-msvc2008
cd ..\src
call qmake -tp vc -r
