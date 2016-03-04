project (jderobot)

cmake_minimum_required(VERSION 2.8)

# build presents
if (EXISTS buildpresets_userdefined.cmake)
	include(buildpresets_userdefined.cmake)
endif()
include(buildpresets.cmake)


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

# ENV VARS
SET(gladedir ./)
SET( DEPS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Deps) # Directory where the dependencies are there
# Folder with ICE cpp interfaces
SET( INTERFACES_CPP_DIR ${CMAKE_CURRENT_BINARY_DIR}/src/stable/interfaces/cpp/jderobot ${CMAKE_CURRENT_BINARY_DIR}/src/stable/interfaces/cpp/)
# Folder with ICE python interfaces
SET( INTERFACES_PYTHON_DIR ${CMAKE_CURRENT_BINARY_DIR}/src/stable/interfaces/python/jderobot ${CMAKE_CURRENT_BINARY_DIR}/src/stable/interfaces/python/)
# Folder with ICE java interfaces
SET( INTERFACES_JAVA_DIR ${CMAKE_CURRENT_BINARY_DIR}/src/stable/interfaces/java/jderobot ${CMAKE_CURRENT_BINARY_DIR}/src/stable/interfaces/java/)
# Folder JdeRobot libraries 
SET( LIBS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/stable/libs) 
# Folder ICE interface definitions
SET( SLICE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/stable/interfaces/slice)
# Main libraries
SET( LIBS_NEEDED  colorspaces jderobotutil progeo pioneer fuzzylib visionlib parallelIce )

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

include(Deps/LoadJderobotDependencies.cmake)

list(APPEND DEPS libvtk5.8)
list(APPEND DEPS_DEV git cmake make gcc-4.8 g++-4.8 gcc g++ pkg-config libeigen3-dev libgsl0ldbl libgsl0-dev libgstreamer0.10-dev libgstreamer-plugins-base0.10-dev libvtk5-dev)
list(APPEND DEPS_DEV "${DEPS}")

# list(APPEND uses ; separator. We need ,
STRING(REGEX REPLACE ";" ", " DEPS "${DEPS}")
STRING(REGEX REPLACE ";" ", " DEPS_DEV "${DEPS_DEV}")



# START RECURSIVE
add_subdirectory (${CMAKE_CURRENT_SOURCE_DIR}/src)



###################
#                 #
#    INSTALL      #
#                 #
###################

INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/src/stable/interfaces/libJderobotInterfaces.so DESTINATION /usr/local/lib/jderobot COMPONENT core)

# Install python files
FILE(GLOB_RECURSE HEADERS_FILES ${CMAKE_CURRENT_BINARY_DIR}/src/stable/interfaces/python/*py)
FOREACH(header ${HEADERS_FILES})
	INSTALL(FILES ${header} DESTINATION /usr/lib/python2.7/jderobot/ COMPONENT core)
ENDFOREACH(header)

INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/src/stable/interfaces/python/jderobot/__init__.py DESTINATION /usr/lib/python2.7/jderobot/ COMPONENT core)

# install all libraries 
FILE(GLOB LIB_FILES ${CMAKE_CURRENT_BINARY_DIR}/src/stable/libs/*/*.so*)
FOREACH (lib ${LIB_FILES})
	INSTALL (FILES ${lib} DESTINATION /usr/local/lib/jderobot COMPONENT core)
ENDFOREACH(lib)

FILE(GLOB LIB_FILES ${CMAKE_CURRENT_BINARY_DIR}/src/stable/libs/visionlib/*/*.so*)
FOREACH (lib ${LIB_FILES})
	INSTALL (FILES ${lib} DESTINATION /usr/local/lib/jderobot COMPONENT core)
ENDFOREACH(lib)


FILE(GLOB_RECURSE HEADERS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/stable/libs/*.h)
FOREACH(currentSourceFile ${HEADERS_FILES})
    string(REGEX REPLACE "(.*/).*.h" "\\1" new_source1 ${currentSourceFile})
		string(REGEX REPLACE  "${CMAKE_CURRENT_SOURCE_DIR}/src/stable/libs" "" new_source2 ${new_source1})
    INSTALL (FILES ${currentSourceFile} DESTINATION /usr/local/include/jderobot/${new_source2} COMPONENT core)
ENDFOREACH(currentSourceFile)

# Install libraries headers
FILE(GLOB_RECURSE HEADERS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/stable/libs/*.h)
FOREACH(currentSourceFile ${HEADERS_FILES})
	string(REGEX REPLACE ".*/(.*/).*.h" "\\1" new_source1 ${currentSourceFile})
	INSTALL (FILES ${currentSourceFile} DESTINATION /usr/local/include/jderobot/${new_source1} COMPONENT core)
ENDFOREACH(currentSourceFile)

# Install Executables
list_subdirectories2( LIST_COMPONENTS ${CMAKE_CURRENT_SOURCE_DIR}/src/stable/components/ 1)

FOREACH (currentBin ${LIST_COMPONENTS})
    if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/src/stable/components/${currentBin}/${currentBin})
        INSTALL (PROGRAMS ${CMAKE_CURRENT_SOURCE_DIR}/src/stable/components/${currentBin}/${currentBin} DESTINATION /usr/local/bin OPTIONAL COMPONENT core)
    endif()
    if (EXISTS ${CMAKE_CURRENT_BINARY_DIR}/src/stable/components/${currentBin}/${currentBin})
        INSTALL (PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/src/stable/components/${currentBin}/${currentBin} DESTINATION /usr/local/bin OPTIONAL COMPONENT core)
    endif()
ENDFOREACH(currentBin)

# Install interfaces headers
INSTALL(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/src/stable/interfaces/cpp/jderobot/
    DESTINATION /usr/local/include/jderobot
	COMPONENT core
    FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
)

# Install slice
INSTALL(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/stable/interfaces/slice/
	DESTINATION /usr/local/share/jderobot/slice/
	COMPONENT core
)

# Install CONF
FILE(GLOB_RECURSE CONF_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/stable/components/*.cfg)
INSTALL (FILES ${CONF_FILES} DESTINATION /usr/local/share/jderobot/conf COMPONENT core)

# Install Glade
FILE(GLOB_RECURSE GLADE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/stable/components/*.glade)
INSTALL (FILES ${GLADE_FILES} DESTINATION /usr/local/share/jderobot/glade COMPONENT core)

# Install Deps cmake's tree
INSTALL(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Deps DESTINATION ${CMAKE_INSTALL_PREFIX}/share/jderobot)


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
SET (CPACK_SET_DESTDIR ON)
SET (CPACK_DEB_COMPONENT_INSTALL OFF)
SET (CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
SET (CPACK_PACKAGING_INSTALL_PREFIX "/usr/local")


# CPack version numbers for release tarball name.
SET (CPACK_PACKAGE_VERSION_MAJOR 5)
SET (CPACK_PACKAGE_VERSION_MINOR 3)
SET (CPACK_PACKAGE_VERSION_PATCH 1)
SET (CPACK_DEBIAN_PACKAGE_VERSION ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH})

## Inject release roll up (incremental releases)
set(PROJECT_VERSION ${CPACK_DEBIAN_PACKAGE_VERSION})
include(scripts/incremental_releases/incremental-releases.cmake)
set(CPACK_DEBIAN_PACKAGE_VERSION ${PROJECT_VERSION})


SET (CPACK_DEBIAN_PACKAGE_PRIORITY "extra")
SET (CPACK_DEBIAN_PACKAGE_SECTION "net")
dpkg_arch(CPACK_DEBIAN_PACKAGE_ARCHITECTURE)

SET (CPACK_MONOLITHIC_INSTALL ON)
#set(CPACK_COMPONENTS_ALL core deps)
#set(CPACK_COMPONENT_NONAME_FOR core)
#set(CPACK_COMPONENT_CORE_GROUP "Core")
#set(CPACK_COMPONENT_DEPS_GROUP "Development")

#SET (CPACK_DEBIAN_CORE_PACKAGE_SHLIBDEPS ON)
#SET (CPACK_DEBIAN_DEPS_PACKAGE_SHLIBDEPS ON)

SET(CPACK_DEBIAN_PACKAGE_DEPENDS "${DEPS}")
#set(CPACK_DEBIAN_CORE_PACKAGE_DEPENDS "${DEPS}")
#set(CPACK_DEBIAN_DEPS_PACKAGE_DEPENDS "${DEPS_DEV}")

if(DEFINED debug_deps)
MESSAGE("Dependencias: ${DEPS}")
MESSAGE("Dependencias_dev: ${DEPS_DEV}")
endif()

set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA
    "${CMAKE_CURRENT_SOURCE_DIR}/scripts/cmake/postinst"
    "${CMAKE_CURRENT_SOURCE_DIR}/scripts/cmake/postrm")


## Include Git HEAD into description (feature: traceback builds)
execute_process(COMMAND git rev-parse HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_HEAD
)
string(STRIP "${GIT_HEAD}" GIT_HEAD)

SET (CPACK_PACKAGE_DESCRIPTION_SUMMARY
"JdeRobot is a software development suite for robotics applications.")
SET (CPACK_PACKAGE_DESCRIPTION
"JdeRobot is a software development suite for robotics applications,
 written in C++ language.
 It provides a programming environment where the robot control program is made
 up of a collection of several concurrent asynchronous threads named schemas.
 It is based on JDE cognitive architecture for autonomous robots.
 .
 Get source from https://github.com/RoboticsURJC/JdeRobot
 Package created with revision ${GIT_HEAD}")

## Patch: CPACK_PACKAGE_DESCRIPTION behavior is broken. Always use SUMMARY
SET (CPACK_PACKAGE_DESCRIPTION_SUMMARY ${CPACK_PACKAGE_DESCRIPTION})

SET (CPACK_PACKAGE_CONTACT "Roberto Calvo <rocapal@gsyc.urjc.es>")
SET (CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}_${CPACK_DEBIAN_PACKAGE_VERSION}_${CPACK_DEBIAN_PACKAGE_ARCHITECTURE}")


## Metapackages (at cmake time)
include(scripts/metapackages/FindMetapackages.cmake)

SET (PACKAGE_VERSION ${CPACK_DEBIAN_PACKAGE_VERSION})
#set(PACKAGE_DEPENDS "${DEPS}")
#configure_file(${MAKE_PACKAGE_CONFIG_DIR}/jderobot-deps.info.in     ${CMAKE_BINARY_DIR}/jderobot-deps.info)

set(PACKAGE_DEPENDS "${DEPS_DEV}")
configure_file(${MAKE_PACKAGE_CONFIG_DIR}/jderobot-deps-dev.info.in ${CMAKE_BINARY_DIR}/jderobot-deps-dev.info)

execute_process(
    COMMAND ${MAKE_PACKAGE_EXECUTABLE}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

#SET (CPACK_COMPONENTS_ALL Libraries ApplicationData)
include (CPack Documentation)
