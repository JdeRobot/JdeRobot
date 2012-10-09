project (JDEROBOT)

cmake_minimum_required(VERSION 2.8)

# ENV VARS
SET(gladedir ./)

SET( INTERFACES_CPP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/interfaces/cpp) # Directorio con las interfaces ICE en C++
SET( LIBS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/libs) # Directorio donde se encuentran las librerias propias de jderobot
SET( SLICE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/interfaces/slice) # Directorio donde se encuentran las interfaces ICE
SET( LIBS_NEEDED   bgfgsegmentation jderobotice colorspaces jderobotutil progeo pioneer fuzzylib visionlib ) # Librerias de las que depende el componente
SET( DEPS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Deps) # Directorio donde se encuentran las dependencias
SET( COMPONENTS naooperator         basic_component		         
		bgfglab              giraffeclient       opencvdemo
		calibrator                               openniServer
		cameraserver         introrob            playerserver
		cameraview           kinectServer        recorder
		cameraview_icestorm  kinectViewer        replayer
		wiimoteClient
		wiimoteServer       colortuner
		
) #Componentes que forman JDErobot


###################
#                 #
#  CHECK SYSTEM   #
#                 #
###################

  # FIND & CHECK PRINCIPAL LIBRARIES

include(FindPkgConfig)
PKG_CHECK_MODULES(opencv REQUIRED opencv)
include_directories(${opencv_INCLUDE_DIRS})
link_directories(${opencv_LIBRARY_DIRS})
MESSAGE("${opencv_LIBRARY_DIRS}")
MESSAGE("${opencv_INCLUDE_DIRS}")
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

PKG_CHECK_MODULES(gsl REQUIRED gsl)
include_directories(${gsl_INCLUDE_DIRS})
link_directories(${gsl_LIBRARIES_DIRS})

include_directories(${LIBS_DIR}/progeo)

  # FIND AND CHECK OTHER DEPENDENCES
include(${DEPS_DIR}/gearbox/CMakeLists.txt)
include(${DEPS_DIR}/fireware/CMakeLists.txt)
include(${DEPS_DIR}/pcl/CMakeLists.txt)
include(${DEPS_DIR}/player/CMakeLists.txt)
include(${DEPS_DIR}/xerces/CMakeLists.txt)
include(${DEPS_DIR}/ice/CMakeLists.txt)
include(${DEPS_DIR}/cwiid/CMakeLists.txt)
include(${DEPS_DIR}/opencv/CMakeLists.txt)
#include(${CMAKE_CURRENT_SOURCE_DIR}/../../../libs/CMakeLists.txt)


#file(READ ${CMAKE_CURRENT_SOURCE_DIR}/jderobot.cfg contents)

# START RECURSIVE
add_subdirectory (${CMAKE_CURRENT_SOURCE_DIR}/src)


###################
#                 #
#    INSTALL      #
#                 #
###################

# Install libraries
FILE(GLOB_RECURSE LIBS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/libs/*.so)
INSTALL (FILES ${LIBS_FILES} DESTINATION /usr/local/lib/jderobot)

# Install libraries headers
FILE(GLOB_RECURSE HEADERS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/libs/*.h)
FOREACH(currentSourceFile ${HEADERS_FILES})
	string(REGEX REPLACE ".*/(.*/).*.h" "\\1" new_source1 ${currentSourceFile})
	INSTALL (FILES ${currentSourceFile} DESTINATION /usr/local/include/jderobot/${new_source1})
ENDFOREACH(currentSourceFile)

# Install Executables
FOREACH (currentBin ${COMPONENTS})
    SET(EXIST EXIST-NOTFOUND)
    find_file(EXIST NAMES ${currentBin} PATHS ${CMAKE_CURRENT_SOURCE_DIR}/src/components/${currentBin})
    IF (EXIST)
	INSTALL (FILES ${EXIST} DESTINATION /usr/local/bin PERMISSIONS OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE)
    ENDIF(EXIST)
    #MESSAGE("${EXIST}")
ENDFOREACH(currentBin)

# Install interfaces headers
FILE(GLOB HEADER_INTERFACE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/interfaces/cpp/jderobot/*.h)
INSTALL (FILES ${HEADER_INTERFACE_FILES} DESTINATION /usr/local/include/jderobot/jderobot)

# Install slice
FILE(GLOB SLICE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/interfaces/slice/jderobot/*.ice)
INSTALL (FILES ${SLICE_FILES} DESTINATION /usr/local/include/jderobot/slice)

# Install CONF
FILE(GLOB_RECURSE CONF_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/components/*.cfg)
INSTALL (FILES ${CONF_FILES} DESTINATION /usr/local/share/jderobot/conf)

# Install Glade
FILE(GLOB_RECURSE GLADE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/components/*.glade)
INSTALL (FILES ${GLADE_FILES} DESTINATION /usr/local/share/jderobot/glade)



###################
#                 #
#    UNINSTALL    #
#                 #
###################

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
