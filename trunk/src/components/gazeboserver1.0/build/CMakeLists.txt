cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
include (FindPkgConfig)
if (PKG_CONFIG_FOUND)
  pkg_check_modules(GAZEBO gazebo)
  message(${GAZEBO_INCLUDE_DIRS})
endif()

SET( INTERFACES_CPP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../interfaces/cpp) # Directorio con las interfaces ICE en C++
SET( LIBS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../libs) # Directorio donde se encuentran las librerias propias de jderobot
SET( SLICE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../interfaces/slice) # Directorio donde se encuentran las interfaces ICE
SET( LIBS_NEEDED   jderobotice colorspaces jderobotutil) # Librerias de las que depende el componente
SET( DEPS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../../Deps) # Directorio donde se encuentran las dependencias

include(FindPkgConfig)
#PKG_CHECK_MODULES(opencv REQUIRED opencv)
#include_directories(${opencv_INCLUDE_DIRS})
#link_directories(${opencv_LIBRARY_DIRS})

PKG_CHECK_MODULES(gtk20 REQUIRED gtk+-2.0)
include_directories(${gtk20_INCLUDE_DIRS})
link_directories(${gtk20_LIBRARY_DIRS})

PKG_CHECK_MODULES(gtkgl20 REQUIRED gtkgl-2.0)
include_directories(${gtkgl20_INCLUDE_DIRS})
link_directories(${gtkgl20_LIBRARY_DIRS})

PKG_CHECK_MODULES(gtkmm REQUIRED gtkmm-2.4)
include_directories(${gtkmm_INCLUDE_DIRS})
link_directories(${gtkmm_LIBRARY_DIRS})

PKG_CHECK_MODULES(libglademm REQUIRED libglademm-2.4)
include_directories(${libglademm_INCLUDE_DIRS})
link_directories(${libglademm_LIBRARY_DIRS})

PKG_CHECK_MODULES(gthread REQUIRED gthread-2.0)
include_directories(${gthread_INCLUDE_DIRS})
link_directories(${gthread_LIBRARY_DIRS})

PKG_CHECK_MODULES(libgnomecanvas REQUIRED libgnomecanvas-2.0)
include_directories(${libgnomecanvas_INCLUDE_DIRS})
link_directories(${libgnomecanvas_LIBRARY_DIRS})

PKG_CHECK_MODULES(libgnomecanvasmm REQUIRED libgnomecanvasmm-2.6)
include_directories(${libgnomecanvasmm_INCLUDE_DIRS})
link_directories(${libgnomecanvasmm_LIBRARY_DIRS})

PKG_CHECK_MODULES(gtkglextmm REQUIRED gtkglextmm-1.2)
include_directories(${gtkglextmm_INCLUDE_DIRS})
link_directories(${gtkglextmm_LIBRARY_DIRS})

include(${DEPS_DIR}/gearbox/CMakeLists.txt)
include(${DEPS_DIR}/ice/CMakeLists.txt)
include(${DEPS_DIR}/opencv/CMakeLists.txt)

add_subdirectory (${CMAKE_CURRENT_SOURCE_DIR}/../../.. ${CMAKE_CURRENT_SOURCE_DIR}/../../..)
add_subdirectory (${CMAKE_CURRENT_SOURCE_DIR}/.. ${CMAKE_CURRENT_SOURCE_DIR}/..)
