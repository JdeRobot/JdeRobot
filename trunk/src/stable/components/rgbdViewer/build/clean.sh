#!/bin/bash

make clean
rm ../cmake_install.cmake
rm -r CMakeFiles
rm -r ../CMakeFiles
mkdir ../temp
mv CMakeLists.txt ../temp
mv clean.sh ../temp
mv cmake_uninstall.cmake.in ../temp
rm -r *
mv ../temp/CMakeLists.txt .
mv ../temp/clean.sh .
mv ../temp/cmake_uninstall.cmake.in .
rm -r ../temp
cd ../../../../
./clean_repository 
