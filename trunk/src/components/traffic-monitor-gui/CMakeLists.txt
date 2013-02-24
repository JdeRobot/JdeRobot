cmake_minimum_required(VERSION 2.8)

INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(cairomm cairomm-1.0)
include_directories(${cairomm_INCLUDE_DIRS})
link_directories(${cairomm_LIBRARY_DIRS})

PKG_CHECK_MODULES(gtkmm REQUIRED gtkmm-2.4)
include_directories(${gtkmm_INCLUDE_DIRS})
link_directories(${gtkmm_LIBRARY_DIRS})

PKG_CHECK_MODULES(libglademm REQUIRED libglademm-2.4)
include_directories(${libglademm_INCLUDE_DIRS})
link_directories(${libglademm_LIBRARY_DIRS})

INCLUDE_DIRECTORIES(/usr/include/mysql++/)
INCLUDE_DIRECTORIES(/usr/include/mysql/)

SET( LIBS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../libs)
SET( INTERFACES_CPP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../interfaces/cpp) # Directorio con las interfaces ICE en C++
SET( LIBS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../libs) # Directorio donde se encuentran las librerias propias de jderobot
SET( SLICE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../interfaces/slice) # Directorio donde se encuentran las interfaces ICE
SET( LIBS_NEEDED bgfgsegmentation jderobotice colorspaces jderobotutil progeo) # Librerias de las que depende el componente
SET( DEPS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../Deps) # Directorio donde se encuentran las dependencias
SET( GENERATED_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated)

include(${DEPS_DIR}/ice/CMakeLists.txt)

SET( GENERATED_FILES ${GENERATED_DIR}/traffic_monitor.cpp)
EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E make_directory generated)
ADD_CUSTOM_COMMAND(OUTPUT ${GENERATED_FILES} COMMAND slice2cpp  -I${CMAKE_CURRENT_SOURCE_DIR}/../../../src/interfaces/slice/ --output-dir ${GENERATED_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/traffic_monitor.ice)


SET( SOURCE_FILES main.cpp database.cpp viewgtk.cpp vehicle_model.cpp trafficmonitor_config.cpp ${GENERATED_FILES})

SET( CMAKE_CXX_FLAGS " -O2 -std=gnu++0x")

include_directories(
  ${INTERFACES_CPP_DIR}
  ${LIBS_DIR}
  ${LIBS_DIR}/colorspaces
  ${LIBS_DIR}/jderobotice/
  ${cairomm_INCLUDE_DIRS}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${GENERATED_DIR})


add_executable (trafficmonitor-gui ${SOURCE_FILES})

TARGET_LINK_LIBRARIES( trafficmonitor-gui
  -lmysqlpp
  ${gtkmm_LIBRARIES}
  ${cairomm_LIBRARIES}
  ${libglademm_LIBRARIES}
  ${LIBS_DIR}/jderobotice/libjderobotice.so
  ${LIBS_DIR}/jderobotutil/libjderobotutil.so
  ${LIBS_DIR}/colorspaces/libcolorspacesmm.so
  ${INTERFACES_CPP_DIR}/jderobot/libJderobotInterfaces.so
  )


