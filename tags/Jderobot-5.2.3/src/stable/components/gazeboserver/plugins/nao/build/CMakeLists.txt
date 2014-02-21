cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
include (FindPkgConfig)
if (PKG_CONFIG_FOUND)
  pkg_check_modules(GAZEBO gazebo)
  message(${GAZEBO_INCLUDE_DIRS})
endif()

SET( INTERFACES_CPP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../../interfaces/cpp) # Directorio con las interfaces ICE en C++
SET( LIBS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../../libs) # Directorio donde se encuentran las librerias propias de jderobot
SET( SLICE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../../interfaces/slice) # Directorio donde se encuentran las interfaces ICE
SET( LIBS_NEEDED   jderobotice colorspaces jderobotutil) # Librerias de las que depende el componente
SET( DEPS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../Deps) # Directorio donde se encuentran las dependencias

include(FindPkgConfig)

include(${DEPS_DIR}/gearbox/CMakeLists.txt)
include(${DEPS_DIR}/ice/CMakeLists.txt)
include(${DEPS_DIR}/opencv/CMakeLists.txt)

add_subdirectory (${CMAKE_CURRENT_SOURCE_DIR}/../../../.. ${CMAKE_CURRENT_SOURCE_DIR}/../../../..)
add_subdirectory (${CMAKE_CURRENT_SOURCE_DIR}/.. ${CMAKE_CURRENT_SOURCE_DIR}/..)
