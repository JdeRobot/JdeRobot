project (JDEROBOT_WIIMOTECLIENT)

cmake_minimum_required(VERSION 2.8)

# ENV VARS
SET(gladedir ./)

SET( INTERFACES_CPP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../interfaces/cpp) # Directorio con las interfaces ICE en C++
SET( LIBS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../libs) # Directorio donde se encuentran las librerias propias de jderobot
SET( SLICE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../interfaces/slice) # Directorio donde se encuentran las interfaces ICE
SET( LIBS_NEEDED jderobotice jderobotutil) # Librerias de las que depende el componente
SET( DEPS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../../Deps) # Directorio donde se encuentran las dependencias



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

PKG_CHECK_MODULES(gsl REQUIRED gsl)
include_directories(${gsl_INCLUDE_DIRS})
link_directories(${gsl_LIBRARIES_DIRS})


PKG_CHECK_MODULES(cwiid REQUIRED cwiid)
include_directories(${cwiid_INCLUDE_DIRS})
link_directories(${cwiid_LIBRARY_DIRS})


  # FIND AND CHECK OTHER DEPENDENCES
include(${DEPS_DIR}/gearbox/CMakeLists.txt)
include(${DEPS_DIR}/ice/CMakeLists.txt)


# START RECURSIVE
add_subdirectory (${CMAKE_CURRENT_SOURCE_DIR}/../../.. ${CMAKE_CURRENT_SOURCE_DIR}/../../..)
add_subdirectory (${CMAKE_CURRENT_SOURCE_DIR}/.. ${CMAKE_CURRENT_SOURCE_DIR}/..)


###################
#                 #
#    INSTALL      #
#                 #
###################

# Install libraries
#FILE(GLOB_RECURSE LIBS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/../../../../src/libs/*.so)
#INSTALL (FILES ${LIBS_FILES} DESTINATION /usr/local/lib/jderobot)
#MESSAGE ( "COMARE ${LIBS_FILES} ")
# Install libraries headers
#FILE(GLOB_RECURSE HEADERS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/../../../../src/libs/*.h)
#FOREACH(currentSourceFile ${HEADERS_FILES})
#    string(REGEX REPLACE ".*/(.*/).*.h" "\\1" new_source1 ${currentSourceFile})
#    INSTALL (FILES ${currentSourceFile} DESTINATION /usr/local/include/jderobot/${new_source1})
# MESSAGE ( "COMARE ${currentSourceFile} ")

#ENDFOREACH(currentSourceFile)

# Install Executables
#FILE(GLOB_RECURSE BIN_FILES ${CMAKE_CURRENT_SOURCE_DIR}/../*cfg)
#    string(REGEX REPLACE ".*/(.*).cfg" "\\1" new_source1 ${BIN_FILES})
#INSTALL (FILES ../${new_source1}  DESTINATION /usr/local/bin PERMISSIONS OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE)



# Install interfaces headers
#FILE(GLOB HEADER_INTERFACE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/../../../../src/interfaces/cpp/jderobot/*.h)
#INSTALL (FILES ${HEADER_INTERFACE_FILES} DESTINATION /usr/local/include/jderobot/jderobot)

# Install slice
#FILE(GLOB SLICE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/../../../../src/interfaces/slice/jderobot/*.ice)
#INSTALL (FILES ${SLICE_FILES} DESTINATION /usr/local/include/jderobot/slice)

# Install CONF
#FILE(GLOB_RECURSE CONF_FILES  ${CMAKE_CURRENT_SOURCE_DIR}/../*cfg)
#INSTALL (FILES ${CONF_FILES} DESTINATION /usr/local/share/jderobot/conf)

# Install Glade
#FILE(GLOB_RECURSE GLADE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/../*.glade)
#INSTALL (FILES ${GLADE_FILES} DESTINATION /usr/local/share/jderobot/glade)



###################
#                 #
#    UNINSTALL    #
#                 #
###################

#configure_file(
#    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
#    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
 #   IMMEDIATE @ONLY)

#add_custom_target(uninstall
#    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)

