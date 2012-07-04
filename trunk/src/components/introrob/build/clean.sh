#!/bin/bash

make clean
rm ../cmake_install.cmake
rm -r CMakeFiles
rm -r ../CMakeFiles
mkdir ../temp
mv CMakeLists.txt ../temp
mv clean.sh ../temp
rm -r *
mv ../temp/CMakeLists.txt .
mv ../temp/clean.sh .
rm -r ../temp
