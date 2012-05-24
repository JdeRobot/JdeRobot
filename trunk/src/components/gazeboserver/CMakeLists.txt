project (JDEROBOT_GAZEBOSERVER)

cmake_minimum_required(VERSION 2.8)

# ENV VARS
SET(gladedir ./)

SET( INTERFACES_CPP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../interfaces/cpp)
SET( INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../libs)
include_directories(${INCLUDE_DIR}) 
SET( LIBS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../libs)
link_directories(${LIBS_DIR})
SET( SLICE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../interfaces/slice)
#SET( LIBS_NEEDED   bgfgsegmentation jderobotice colorspaces jderobotutil progeo pioneer fuzzylib visionlib ) 

##SET( COMPONENTS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/components)

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

# CHECK GEARBOX
FIND_LIBRARY(with_gearbox
    NAMES GbxGarminAcfr GbxIceUtilAcfr
    PATHS /usr/lib/gearbox /usr/local/lib/gearbox)
IF(NOT with_gearbox)
    MESSAGE (FATAL_ERROR "*** Gearbox libraries: GbxUtilAcfr or GbxIceUtilAcfr not found") 
ENDIF()
include_directories(/usr/include/gearbox)
include_directories(/usr/local/include/gearbox)
link_directories(/usr/lib/gearbox)
link_directories(/usr/local/lib/gearbox)

# CHECK FIREWARE 
FIND_LIBRARY(with_fireware
    NAMES  dc1394 raw1394
    PATHS /usr/lib)
IF(NOT with_fireware)
    MESSAGE ("*** Fireware libraries: dc1394 or raw1394 not found") 
ENDIF()


# CHECK PCL
FIND_LIBRARY(with_pcl
    NAMES pcl_surface pcl_registration pcl_segmentation pcl_filters pcl_sample_consensus pcl_io pcl_octree pcl_features pcl_kdtree pcl_common
    PATHS /usr/lib)
IF(NOT with_pcl)
    MESSAGE ("*** PCL libraries not found. Are needed to KinectServer component")
endif()

# CHECK Player
FIND_LIBRARY(with_player
    NAMES playerc
    PATHS /usr/local/lib)

IF(NOT with_player)
    MESSAGE ("*** Player libraries not found. Are needed to PlayerServer component")
endif()

#file(READ ${CMAKE_CURRENT_SOURCE_DIR}/jderobot.cfg contents)

# START RECURSIVE
#add_subdirectory (${CMAKE_CURRENT_SOURCE_DIR}/../.. ${CMAKE_CURRENT_SOURCE_DIR}/../..)
add_subdirectory (${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR}/src)


###################
#                 #
#    INSTALL      #
#                 #
###################

# Install libraries
##FILE(GLOB_RECURSE LIBS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/libs/*.so)
##INSTALL (FILES ${LIBS_FILES} DESTINATION /usr/local/lib/jderobot)

# Install libraries headers
##FILE(GLOB_RECURSE HEADERS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/libs/*.h)
##FOREACH(currentSourceFile ${HEADERS_FILES})
##	string(REGEX REPLACE ".*/(.*/).*.h" "\\1" new_source1 ${currentSourceFile})
##	INSTALL (FILES ${currentSourceFile} DESTINATION /usr/local/include/jderobot/${new_source1})
##ENDFOREACH(currentSourceFile)

# Install Executables
##FILE(GLOB_RECURSE BIN_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/components/*cfg)
##FOREACH(currentSourceFile ${BIN_FILES})
##	string(REGEX REPLACE ".*/(.*)/.*.cfg" "\\1" new_source1 ${currentSourceFile})
##	list(APPEND new_list ${new_source1})
##ENDFOREACH(currentSourceFile)

##list(REMOVE_DUPLICATES new_list)
##list(REMOVE_ITEM new_list opencvdemo2.3.1 motiondetection alarmgenerator kinectViewer config openniServer giraffeServer naobody playerserver teleoperator colortuner2.3.1)

##FOREACH(currentBinFile ${new_list})
##    INSTALL (FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/components/${currentBinFile}/${currentBinFile} DESTINATION /usr/local/bin PERMISSIONS OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE) 
##ENDFOREACH(currentBinFile)

# Install interfaces headers
##FILE(GLOB HEADER_INTERFACE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/interfaces/cpp/jderobot/*.h)
##INSTALL (FILES ${HEADER_INTERFACE_FILES} DESTINATION /usr/local/include/jderobot/jderobot)

# Install slice
##FILE(GLOB SLICE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/interfaces/slice/jderobot/*.ice)
##INSTALL (FILES ${SLICE_FILES} DESTINATION /usr/local/include/jderobot/slice)

# Install CONF 
##FILE(GLOB_RECURSE CONF_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/components/*.cfg)
##INSTALL (FILES ${CONF_FILES} DESTINATION /usr/local/share/jderobot/conf)

# Install Glade 
##FILE(GLOB_RECURSE GLADE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/components/*.glade)
##INSTALL (FILES ${GLADE_FILES} DESTINATION /usr/local/share/jderobot/glade)



###################
#                 #
#    UNINSTALL    #
#                 #
###################

##configure_file(
##    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
##    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
##    IMMEDIATE @ONLY)

##add_custom_target(uninstall
##    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)

