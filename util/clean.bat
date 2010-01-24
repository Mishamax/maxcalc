pushd ..\src
del /Q debug\*
del /Q release\*
del /Q cli\debug\*
del /Q cli\release\*
del /Q engine\debug\*
del /Q engine\release\*
del /Q gui\debug\*
del /Q gui\release\*
del /Q test\debug\*
del /Q test\release\*
del /Q *.sln
del /Q /S *.vcproj
del /Q /S Makefile*
del /Q /S *.ncb
del /Q /S *.suo
del /Q /S *.idb
del /Q /S *.pdb
del /Q /S pch.h.cpp
del /Q /S object_script.*.Debug
del /Q /S object_script.*.Release
del /Q /A:H maxcalc.suo
del /Q /S *.qm
rmdir debug
rmdir release
rmdir cli\debug
rmdir cli\release
rmdir engine\debug
rmdir engine\release
rmdir gui\debug
rmdir gui\release
rmdir test\debug
rmdir test\release
popd
