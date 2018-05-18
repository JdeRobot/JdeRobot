IF(NOT DEFINED ENABLE_ROS)
  SET(ENABLE_ROS ON)
ENDIF(NOT DEFINED ENABLE_ROS)

if(ENABLE_ROS)
  project (jderobot)
else()
  project (jderobot-only-ice)
endif()

# Change default install path
if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set (CMAKE_INSTALL_PREFIX /opt/jderobot CACHE PATH "default install path" FORCE)
endif()

cmake_minimum_required(VERSION 3.3)

# build presents
if (EXISTS buildpresets_userdefined.cmake)
	include(buildpresets_userdefined.cmake)
endif()
include(buildpresets.cmake)

#include jderobot macros
include(cmake/jderobot.cmake)

# include Modules 
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/modules")

# include pythonAux Macros
include(${PROJECT_SOURCE_DIR}/cmake/pythonAux.cmake)

#check compiller
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	include(CheckCXXCompilerFlag)
	CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
	CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
	message (version: ${CMAKE_CXX_COMPILER_VERSION})
	if(COMPILER_SUPPORTS_CXX11)
    	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
	elseif(COMPILER_SUPPORTS_CXX0X)
        #boost have problems with gcc < 4.7 and c++0x
        if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER "4.7.0")
    		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
        endif()
	else()
		    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support.")
	endif()
endif()

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall" ) # Opciones para el compilador

INCLUDE_DIRECTORIES(/opt/jderobot/include/openni2)
LINK_DIRECTORIES(/opt/jderobot/lib/openni2)
set (CMAKE_LIBRARY_PATH /opt/jderobot/lib/openni2)


# ENV VARS
SET(gladedir ./)
SET( DEPS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Deps) # Directory where the dependencies are there
# Folder with ICE cpp interfaces
SET( INTERFACES_CPP_DIR ${CMAKE_CURRENT_BINARY_DIR}/src/interfaces/cpp/jderobot ${CMAKE_CURRENT_BINARY_DIR}/src/interfaces/cpp/)
# Folder with ICE python interfaces
SET( INTERFACES_PYTHON_DIR ${CMAKE_CURRENT_BINARY_DIR}/src/interfaces/python/jderobot ${CMAKE_CURRENT_BINARY_DIR}/src/interfaces/python/)
# Folder with ICE java interfaces
SET( INTERFACES_JAVA_DIR ${CMAKE_CURRENT_BINARY_DIR}/src/interfaces/java/jderobot ${CMAKE_CURRENT_BINARY_DIR}/src/interfaces/java/)
# Folder JdeRobot libraries 
SET( LIBS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/libs) 
# Folder ICE interface definitions
SET( SLICE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/interfaces/slice)
# Main libraries
SET( LIBS_NEEDED  jderobotutil colorspaces colorspacesmm easyiceconfig parallelIce depthLib geometry logger ns progeo pioneer fuzzylib  )
SET( LIBS_EXTRA     jderobotHandlers jderobotViewer   resourcelocator xmlParser cvblob cvfast imgAnalyze JderobotInterfaces)

# Create directories if necessary
file(MAKE_DIRECTORY ${INTERFACES_PYTHON_DIR})
file(MAKE_DIRECTORY ${INTERFACES_JAVA_DIR})
file(MAKE_DIRECTORY ${INTERFACES_CPP_DIR})


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

# MACRO to add the list of subdirectories to another list
macro(list_subdirectories3 retval curdir return_relative)
  file(GLOB sub-dir RELATIVE ${curdir} *)
  set(list_of_dirs ${${retval}})
  foreach(dir ${sub-dir})
    if(IS_DIRECTORY ${curdir}/${dir})
      if (${return_relative})
        list (FIND list_of_dirs "${dir}" _index)
        if (NOT ${_index} GREATER -1)
          if (NOT ${dir} MATCHES "CMakeFiles" AND NOT ${dir} MATCHES ".svn")
                set(list_of_dirs ${list_of_dirs} ${dir})
          endif()
        endif()
      else()
        list (FIND list_of_dirs "${curdir}/${dir}" _index)
        if (NOT ${_index} GREATER -1)
          set(list_of_dirs ${list_of_dirs} ${curdir}/${dir})
        endif()
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


link_directories(${INTERFACES_CPP_DIR}/jderobot)
FOREACH(LibFile ${LIBS_NEEDED})
    link_directories(${LIBS_DIR}/${LibFile})
    MESSAGE("${LIBS_DIR}/${LibFile}")
ENDFOREACH(LibFile)

  # FIND AND CHECK OTHER DEPENDENCES

include(Deps/LoadJderobotDependencies.cmake)

list(APPEND DEPS_DEV git cmake make gcc-5 g++-5 gcc g++ pkg-config libgstreamer0.10-dev libgstreamer-plugins-base0.10-dev)
list(APPEND DEPS_DEV "${DEPS}")

# list(APPEND uses ; separator. We need ,
STRING(REGEX REPLACE ";" ", " DEPS "${DEPS}")
STRING(REGEX REPLACE ";" ", " DEPS_DEV "${DEPS_DEV}")



# START RECURSIVE
add_subdirectory (${CMAKE_CURRENT_SOURCE_DIR}/src)

configure_file(
      setup.bash.in
      setup.bash
      @ONLY
  )



###################
#                 #
#    INSTALL      #
#                 #
###################


# Install Executables
list_subdirectories2( LIST_DRIVERS ${CMAKE_CURRENT_SOURCE_DIR}/src/drivers/ 1)
list_subdirectories2( LIST_TOOLS ${CMAKE_CURRENT_SOURCE_DIR}/src/tools/ 1)

# Install Deps cmake's tree
INSTALL(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Deps DESTINATION ${CMAKE_INSTALL_PREFIX}/share/jderobot COMPONENT setup)

# Install ROS Launch files
INSTALL(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/scripts/launch DESTINATION ${CMAKE_INSTALL_PREFIX}/share/jderobot COMPONENT setup)

# Install setup.bash
INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/setup.bash DESTINATION ${CMAKE_INSTALL_PREFIX} COMPONENT setup)







#file(WRITE dummy_main1.c "int main(int argc, char** argv){return 0;}")
#add_executable(dummy_main1 dummy_main1.c)
#INSTALL (FILES ${CMAKE_CURRENT_SOURCE_DIR}/dummy_main1 DESTINATION /tmp COMPONENT deps OPTIONAL PERMISSIONS OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE)


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

# include cpack instructions
JDEROBOT_GENERATE_INSTALL_FILES_BY_TARGET()
include(${PROJECT_SOURCE_DIR}/cmake/cpackit.cmake)


