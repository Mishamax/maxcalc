@echo off

call "%VS90COMNTOOLS%\vsvars32.bat"

pushd src
pushd maxcalcengine
qmake && nmake clean && nmake debug && nmake release
popd
pushd maxcalc-cli
qmake && nmake clean && nmake debug && nmake release
popd
pushd maxcalc-gui
qmake && nmake clean && nmake debug && nmake release
popd
popd
