@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars86.bat" > NUL 2> NUL
mkdir build
pushd build
cmake ..
cmake --build . --config RelWithDebInfo
popd
