cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
include (FindPkgConfig)
if (PKG_CONFIG_FOUND)
  pkg_check_modules(GAZEBO gazebo)
endif()
include_directories(${GAZEBO_INCLUDE_DIRS})
link_directories(${GAZEBO_LIBRARY_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

#SET(DEPS_DIR /home/mikel/workSpace/jderobot/trunk/Deps)

set(CMAKE_CXX_FLAGS "-lIce -lIceUtil")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../../Deps/ice  ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../../Deps/ice)

add_library(hello_world SHARED hello_world.cc Printer.cpp)
target_link_libraries(hello_world
${ZeroCIce_LIBRARIES}
${GAZEBO_libraries})