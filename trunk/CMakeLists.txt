project (jderobot)

cmake_minimum_required(VERSION 2.8)

# ENV VARS
SET(gladedir ./)
SET( DEPS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Deps) # Directorio donde se encuentran las dependencias
SET( INTERFACES_CPP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/stable/interfaces/cpp) # Directorio con las interfaces ICE en C++
SET( LIBS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/stable/libs) # Directorio donde se encuentran las librerias propias de jderobot
SET( SLICE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/stable/interfaces/slice) # Directorio donde se encuentran las interfaces ICE
SET( LIBS_NEEDED  colorspaces jderobotutil progeo pioneer fuzzylib visionlib parallelIce ) # Librerias de las que depende el componente



# MACRO to get the list of subdirectories
macro(list_subdirectories retval curdir return_relative)
  file(GLOB sub-dir RELATIVE ${curdir} *)
  set(list_of_dirs "")
  foreach(dir ${sub-dir})
    if(IS_DIRECTORY ${curdir}/${dir})
      if (${return_relative})
        if (NOT ${dir} MATCHES "CMakeFiles" AND NOT ${dir} MATCHES ".svn")
                set(list_of_dirs ${list_of_dirs} ${dir})
        endif()
      else()
        set(list_of_dirs ${list_of_dirs} ${curdir}/${dir})
      endif()
    endif()
  endforeach()
  set(${retval} ${list_of_dirs})
endmacro()

# MACRO to get the list subdir of other path distinct to current path
macro(list_subdirectories2 retval curdir return_relative)
  file(GLOB sub-dir RELATIVE ${curdir} ${curdir}*)
  set(list_of_dirs "")
  foreach(dir ${sub-dir})
    if(IS_DIRECTORY ${curdir}/${dir})
      if (${return_relative})        
        if (NOT ${dir} MATCHES "CMakeFiles" AND NOT ${dir} MATCHES ".svn")
                set(list_of_dirs ${list_of_dirs} ${dir})
        endif()
      else()
        set(list_of_dirs ${list_of_dirs} ${curdir}/${dir})
      endif()
    endif()
  endforeach()
  set(${retval} ${list_of_dirs})
endmacro()




###################
#                 #
#  CHECK SYSTEM   #
#                 #
###################

  # FIND & CHECK PRINCIPAL LIBRARIES

include(FindPkgConfig)

PKG_CHECK_MODULES(gsl REQUIRED gsl)
include_directories(${gsl_INCLUDE_DIRS})
link_directories(${gsl_LIBRARIES_DIRS})

PKG_CHECK_MODULES(eigen REQUIRED eigen3)
include_directories(${eigen_INCLUDE_DIRS})


link_directories(${INTERFACES_CPP_DIR}/jderobot)
FOREACH(LibFile ${LIBS_NEEDED})
    link_directories(${LIBS_DIR}/${LibFile})
    MESSAGE("${LIBS_DIR}/${LibFile}")
ENDFOREACH(LibFile)

  # FIND AND CHECK OTHER DEPENDENCES

include(${DEPS_DIR}/gazebo/CMakeLists.txt)
include(${DEPS_DIR}/fireware/CMakeLists.txt)
include(${DEPS_DIR}/pcl/CMakeLists.txt)
include(${DEPS_DIR}/openni/CMakeLists.txt)
include(${DEPS_DIR}/nite/CMakeLists.txt)
include(${DEPS_DIR}/xerces/CMakeLists.txt)
include(${DEPS_DIR}/ice/CMakeLists.txt)
include(${DEPS_DIR}/cwiid/CMakeLists.txt)
include(${DEPS_DIR}/opencv/CMakeLists.txt)
include(${DEPS_DIR}/opengl/CMakeLists.txt)
include(${DEPS_DIR}/openni2/CMakeLists.txt)
include(${DEPS_DIR}/nite2/CMakeLists.txt)
include(${DEPS_DIR}/pthread/CMakeLists.txt)
include(${DEPS_DIR}/libxml++/CMakeLists.txt)
include(${DEPS_DIR}/libxml2/CMakeLists.txt)
include(${DEPS_DIR}/boost/CMakeLists.txt)
include(${DEPS_DIR}/alut/CMakeLists.txt)
include(${DEPS_DIR}/tinyxml/CMakeLists.txt)
include(${DEPS_DIR}/gtk2/CMakeLists.txt)
include(${DEPS_DIR}/qt/CMakeLists.txt)
include(${DEPS_DIR}/gtk3/CMakeLists.txt)
include(${DEPS_DIR}/kobuki/CMakeLists.txt)
include(${DEPS_DIR}/libusb/CMakeLists.txt)
#include(${CMAKE_CURRENT_SOURCE_DIR}/../../../libs/CMakeLists.txt)


#file(READ ${CMAKE_CURRENT_SOURCE_DIR}/jderobot.cfg contents)

# START RECURSIVE
add_subdirectory (${CMAKE_CURRENT_SOURCE_DIR}/src)


###################
#                 #
#    INSTALL      #
#                 #
###################

INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/stable/interfaces/cpp/jderobot/libJderobotInterfaces.so DESTINATION /usr/local/lib/jderobot)

# install all libraries 
FILE(GLOB LIB_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/stable/libs/*/*.so*)
FOREACH (lib ${LIB_FILES})
	INSTALL (FILES ${lib} DESTINATION /usr/local/lib/jderobot)
ENDFOREACH(lib)

FILE(GLOB LIB_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/stable/libs/visionlib/*/*.so*)
FOREACH (lib ${LIB_FILES})
	INSTALL (FILES ${lib} DESTINATION /usr/local/lib/jderobot)
ENDFOREACH(lib)


FILE(GLOB_RECURSE HEADERS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/stable/libs/*.h)
FOREACH(currentSourceFile ${HEADERS_FILES})
    string(REGEX REPLACE "(.*/).*.h" "\\1" new_source1 ${currentSourceFile})
		string(REGEX REPLACE  "${CMAKE_CURRENT_SOURCE_DIR}/src/stable/libs" "" new_source2 ${new_source1})
    INSTALL (FILES ${currentSourceFile} DESTINATION /usr/local/include/jderobot/${new_source2})
ENDFOREACH(currentSourceFile)

# Install libraries headers
FILE(GLOB_RECURSE HEADERS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/stable/libs/*.h)
FOREACH(currentSourceFile ${HEADERS_FILES})
	string(REGEX REPLACE ".*/(.*/).*.h" "\\1" new_source1 ${currentSourceFile})
	INSTALL (FILES ${currentSourceFile} DESTINATION /usr/local/include/jderobot/${new_source1})
ENDFOREACH(currentSourceFile)

# Install Executables
list_subdirectories2( LIST_COMPONENTS ${CMAKE_CURRENT_SOURCE_DIR}/src/stable/components/ 1)

FOREACH (currentBin ${LIST_COMPONENTS})
    #SET(EXIST EXIST-NOTFOUND)
    find_file(EXIST NAMES ${currentBin} PATHS ${CMAKE_CURRENT_SOURCE_DIR}/src/components/${currentBin})
    #IF (EXIST)
    INSTALL (FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/stable/components/${currentBin}/${currentBin} DESTINATION /usr/local/bin OPTIONAL PERMISSIONS OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE)
    #ENDIF(EXIST)
    #MESSAGE("${EXIST}")
ENDFOREACH(currentBin)

# Install interfaces headers
FILE(GLOB HEADER_INTERFACE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/stable/interfaces/cpp/jderobot/*.h)
INSTALL (FILES ${HEADER_INTERFACE_FILES} DESTINATION /usr/local/include/jderobot)

# Install slice
FILE(GLOB SLICE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/stable/interfaces/slice/jderobot/*.ice)
INSTALL (FILES ${SLICE_FILES} DESTINATION /usr/local/include/jderobot/slice)

# Install CONF
FILE(GLOB_RECURSE CONF_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/stable/components/*.cfg)
INSTALL (FILES ${CONF_FILES} DESTINATION /usr/local/share/jderobot/conf)

# Install Glade
FILE(GLOB_RECURSE GLADE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/stable/components/*.glade)
INSTALL (FILES ${GLADE_FILES} DESTINATION /usr/local/share/jderobot/glade)

# Install Gazebo Models
INSTALL (DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/stable/components/gazeboserver/models DESTINATION /usr/local/share/jderobot/gazebo/ PATTERN .svn EXCLUDE)

# Install Gazebo Plugins
SET(PLUGINS "kinect" "nao" "pioneer")

FOREACH (PLUGIN ${PLUGINS})
	FILE(GLOB_RECURSE SO_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/stable/components/gazeboserver/plugins/${PLUGIN}/*.so)
	INSTALL (FILES ${SO_FILES} DESTINATION /usr/lib/gazebo-1.8/plugins-jderobot/${PLUGIN}/ )
	FILE(GLOB_RECURSE CFG_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/stable/components/gazeboserver/plugins/${PLUGIN}/*.cfg)
	INSTALL (FILES ${CFG_FILES} DESTINATION /usr/lib/gazebo-1.8/plugins-jderobot/${PLUGIN}/ )
	INSTALL (DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/stable/components/gazeboserver/plugins/${PLUGIN}/worlds/ 
		 DESTINATION /usr/lib/gazebo-1.8/plugins-jderobot/${PLUGIN}/worlds/
		 PATTERN .svn EXCLUDE)
ENDFOREACH(PLUGIN)




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

#
# Deb packages
#

# Determine current architecture
macro(dpkg_arch VAR_NAME)
        find_program(DPKG_PROGRAM dpkg DOC "dpkg program of Debian-based systems")
        if (DPKG_PROGRAM) 
          execute_process(
            COMMAND ${DPKG_PROGRAM} --print-architecture
            OUTPUT_VARIABLE ${VAR_NAME}
            OUTPUT_STRIP_TRAILING_WHITESPACE
          )
        endif(DPKG_PROGRAM)
endmacro(dpkg_arch)

include (InstallRequiredSystemLibraries)
SET (CPACK_GENERATOR "DEB")
SET (CPACK_SOURCE_GENERATOR TGZ)
SET (CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
SET (CPACK_SET_DESTDIR "on")
SET (CPACK_PACKAGING_INSTALL_PREFIX "/usr/local")


SET (VERSION 5.2.3-1)
# CPack version numbers for release tarball name.
SET (CPACK_PACKAGE_VERSION_MAJOR 5)
SET (CPACK_PACKAGE_VERSION_MINOR 2)
SET (CPACK_PACKAGE_VERSION_PATCH 3)
SET (${VERSION} CPACK_DEBIAN_PACKAGE_VERSION)

SET (CPACK_DEBIAN_PACKAGE_PRIORITY "extra")
SET (CPACK_DEBIAN_PACKAGE_SECTION "net")
dpkg_arch(CPACK_DEBIAN_PACKAGE_ARCHITECTURE)

#MESSAGE("Dependencias: ${DEPS}")

SET(CPACK_DEBIAN_PACKAGE_DEPENDS "${DEPS}")

set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA
    "${CMAKE_CURRENT_SOURCE_DIR}/scripts/cmake/postinst"
    "${CMAKE_CURRENT_SOURCE_DIR}/scripts/cmake/postrm")

SET (CPACK_PACKAGE_DESCRIPTION_SUMMARY "jderobot is a software development suite for robotics applications.")
SET (CPACK_PACKAGE_DESCRIPTION "jderobot is a software development suite for robotics applications, written in C++ language. It provides a programming environment where the robot control program is made up of a collection of several concurrent asynchronous threads named schemas. It is based on JDE cognitive architecture for autonomous robots.")

SET (CPACK_PACKAGE_CONTACT "Roberto Calvo <rocapal@gsyc.urjc.es>")
SET (CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}_${VERSION}_${CPACK_DEBIAN_PACKAGE_ARCHITECTURE}")

SET (CPACK_COMPONENTS_ALL Libraries ApplicationData)
include (CPack Documentation)
