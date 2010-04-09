pushd ..\src
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
rmdir /Q /S debug
rmdir /Q /S release
rmdir /Q /S cli\debug
rmdir /Q /S cli\release
rmdir /Q /S engine\debug
rmdir /Q /S engine\release
rmdir /Q /S gui\debug
rmdir /Q /S gui\release
rmdir /Q /S test\debug
rmdir /Q /S test\release
popd
