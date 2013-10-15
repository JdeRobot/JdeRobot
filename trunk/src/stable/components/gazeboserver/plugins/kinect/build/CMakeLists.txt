cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

SET( INTERFACES_CPP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../../interfaces/cpp) # Directorio con las interfaces ICE en C++
SET( LIBS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../../libs) # Directorio donde se encuentran las librerias propias de jderobot
SET( LIBS_NEEDED   jderobotice jderobotutil) # Librerias de las que depende el componente
SET( DEPS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../Deps) # Directorio donde se encuentran las dependencias

include (FindPkgConfig)
if (PKG_CONFIG_FOUND)
  pkg_check_modules(GAZEBO gazebo)
endif()

find_package(PCL 1.2 REQUIRED)
include_directories(${PCL_INCLUDE_DIRS})
link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})


# FIND AND CHECK OTHER DEPENDENCES
include(${DEPS_DIR}/ice/CMakeLists.txt)
#include(${DEPS_DIR}/pcl/CMakeLists.txt)
include(${DEPS_DIR}/opencv/CMakeLists.txt)

add_subdirectory (${CMAKE_CURRENT_SOURCE_DIR}/../../../.. ${CMAKE_CURRENT_SOURCE_DIR}/../../../..)
add_subdirectory (${CMAKE_CURRENT_SOURCE_DIR}/.. ${CMAKE_CURRENT_SOURCE_DIR}/..)


