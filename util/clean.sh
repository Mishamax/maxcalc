pushd ../src
find . -name "*.sln" -exec rm -rf {} \;
find . -name "*.vcproj" -exec rm -rf {} \;
find . -name "Makefile*" -exec rm -rf {} \;
find . -name "*.ncb" -exec rm -rf {} \;
find . -name "*.suo" -exec rm -rf {} \;
find . -name "*.idb" -exec rm -rf {} \;
find . -name "*.pdb" -exec rm -rf {} \;
find . -name "pch.h.cpp" -exec rm -rf {} \;
find . -name "object_script.*.Debug" -exec rm -rf {} \;
find . -name "object_script.*.Release" -exec rm -rf {} \;
find . -name "maxcalc.suo" -exec rm -rf {} \;
find . -name "*.qm" -exec rm -rf {} \;
rm -rf debug
rm -rf release
rm -rf cli/debug
rm -rf cli/release
rm -rf engine/debug
rm -rf engine/release
rm -rf gui/debug
rm -rf gui/release
rm -rf test/debug
rm -rf test/release
popd
