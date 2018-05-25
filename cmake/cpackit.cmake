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
SET (CPACK_DEB_COMPONENT_INSTALL ON)
SET (CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
SET (CPACK_PACKAGING_INSTALL_PREFIX "/usr/local")


# CPack version numbers for release tarball name.
SET (CPACK_PACKAGE_VERSION_MAJOR 5)
SET (CPACK_PACKAGE_VERSION_MINOR 6)
SET (CPACK_PACKAGE_VERSION_PATCH 4)
SET (CPACK_DEBIAN_PACKAGE_VERSION ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH})


SET (CPACK_DEBIAN_PACKAGE_PRIORITY "extra")
SET (CPACK_DEBIAN_PACKAGE_SECTION "net")
dpkg_arch(CPACK_DEBIAN_PACKAGE_ARCHITECTURE)

SET (CPACK_MONOLITHIC_INSTALL OFF)
#set(CPACK_COMPONENTS_ALL depthlib)
#set(CPACK_COMPONENT_NONAME_FOR core)
#set(CPACK_COMPONENT_CORE_GROUP "Core")
#set(CPACK_COMPONENT_DEPS_GROUP "Development")

#SET(CPACK_DEBIAN_DEPTHLIB_PACKAGE_SHLIBDEPS ON)
#SET (CPACK_DEBIAN_CORE_PACKAGE_SHLIBDEPS ON)
#SET (CPACK_DEBIAN_DEPS_PACKAGE_SHLIBDEPS ON)

#SET(CPACK_DEBIAN_PACKAGE_DEPENDS "${DEPS}")
#set(CPACK_DEBIAN_CORE_PACKAGE_DEPENDS "${DEPS}")
#set(CPACK_DEBIAN_DEPS_PACKAGE_DEPENDS "${DEPS_DEV}")

##################################################################################################
##
##	TESTING AREA
##
###################################################################################################


#SET(CPACK_COMPONENTS_ALL easyice comm kobukiviewer)
#include(cmake/cpack_metainfo/3dviewer.cmake)
#include(cmake/cpack_metainfo/ardrone-server.cmake)
#include(cmake/cpack_metainfo/cameracalibrator.cmake)
#include(cmake/cpack_metainfo/cameraserver.cmake)
#include(cmake/cpack_metainfo/cameraview.cmake)
#include(cmake/cpack_metainfo/gazeboserver.cmake)
#include(cmake/cpack_metainfo/giraffeclient.cmake)
#include(cmake/cpack_metainfo/kobukiviewer.cmake)
#include(cmake/cpack_metainfo/namingservice.cmake)
#include(cmake/cpack_metainfo/navigatorcamera.cmake)
#include(cmake/cpack_metainfo/openniserver.cmake)
#include(cmake/cpack_metainfo/pclrgbdserver.cmake)
#include(cmake/cpack_metainfo/recorder2.cmake)
#include(cmake/cpack_metainfo/replaycontroller.cmake)
#include(cmake/cpack_metainfo/replayer2.cmake)
#nclude(cmake/cpack_metainfo/rgbdmanualcalibrator.cmake)
#include(cmake/cpack_metainfo/rgbdcalibrator.cmake)
#include(cmake/cpack_metainfo/rgbdviewer.cmake)
#include(cmake/cpack_metainfo/uav-viewer.cmake)
include(cmake/cpack_metainfo/tools.cmake)
include(cmake/cpack_metainfo/drivers.cmake)
include(cmake/cpack_metainfo/libs.cmake)


####################################################################################################


if(DEFINED debug_deps)
MESSAGE("Dependencias: ${DEPS}")
MESSAGE("Dependencias_dev: ${DEPS_DEV}")
endif()

set(CPACK_DEBIAN_SETUP_PACKAGE_CONTROL_EXTRA
    "${CMAKE_CURRENT_SOURCE_DIR}/scripts/cmake/postinst"
    "${CMAKE_CURRENT_SOURCE_DIR}/scripts/cmake/postrm")

SET(CPACK_DEBIAN_ZEROC-ICE-PYTHON_PACKAGE_CONTROL_EXTRA
    "${CMAKE_CURRENT_SOURCE_DIR}/scripts/cmake/ice/postinst"
    "${CMAKE_CURRENT_SOURCE_DIR}/scripts/cmake/ice/postrm")

SET(CPACK_DEBIAN_SCRATCH2JDEROBOT_PACKAGE_CONTROL_EXTRA
    "${CMAKE_CURRENT_SOURCE_DIR}/scripts/cmake/ice/postinst"
    "${CMAKE_CURRENT_SOURCE_DIR}/scripts/cmake/ice/postrm")


## Include Git HEAD into description (feature: traceback builds)
execute_process(COMMAND git rev-parse HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_HEAD
)
string(STRIP "${GIT_HEAD}" GIT_HEAD)

SET (CPACK_PACKAGE_DESCRIPTION_SUMMARY
"Software development suite for robotics and computer vision applications.")
SET (CPACK_PACKAGE_DESCRIPTION
"JdeRobot is a software development suite for robotics and computer vision applications. 
 The applications are made up of a collection of concurrent processes (named components) that
 interoperate among them. They exchange messages using ICE communication middleware. The components may 
 run in a distributed network of computers and may be written in several languages (C++, Python, JavaScript...). 
 JdeRobot includes a collection of drivers and tools.
 Get source from https://github.com/JdeRobot/JdeRobot.git
 Package created with revision ${GIT_HEAD}")

## Patch: CPACK_PACKAGE_DESCRIPTION behavior is broken. Always use SUMMARY
SET (CPACK_PACKAGE_DESCRIPTION_SUMMARY ${CPACK_PACKAGE_DESCRIPTION})
SET (CPACK_PACKAGE_CONTACT "Francisco Perez <f.perez475@gmail.com>")
SET (CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}_${CPACK_DEBIAN_PACKAGE_VERSION}_${CPACK_DEBIAN_PACKAGE_ARCHITECTURE}")


## Metapackages (at cmake time)
include(scripts/metapackages/FindMetapackages.cmake)

SET (PACKAGE_VERSION ${CPACK_DEBIAN_PACKAGE_VERSION})
set(PACKAGE_DEPENDS "${DEPS}")
configure_file(${MAKE_PACKAGE_CONFIG_DIR}/jderobot.info.in     ${CMAKE_BINARY_DIR}/jderobot_${PACKAGE_VERSION}_all.info)

set(PACKAGE_DEPENDS "${DEPS_DEV}")
configure_file(${MAKE_PACKAGE_CONFIG_DIR}/jderobot-deps-dev.info.in ${CMAKE_BINARY_DIR}/jderobot-deps-dev_${PACKAGE_VERSION}_all.info)

configure_file(${MAKE_PACKAGE_CONFIG_DIR}/jderobot-libs.info.in ${CMAKE_BINARY_DIR}/jderobot-libs_${PACKAGE_VERSION}_all.info)
configure_file(${MAKE_PACKAGE_CONFIG_DIR}/jderobot-tools.info.in ${CMAKE_BINARY_DIR}/jderobot-tools_${PACKAGE_VERSION}_all.info)
configure_file(${MAKE_PACKAGE_CONFIG_DIR}/jderobot-drivers.info.in ${CMAKE_BINARY_DIR}/jderobot-drivers_${PACKAGE_VERSION}_all.info)
configure_file(${MAKE_PACKAGE_CONFIG_DIR}/jderobot-examples.info.in ${CMAKE_BINARY_DIR}/jderobot-examples_${PACKAGE_VERSION}_all.info)
configure_file(${MAKE_PACKAGE_CONFIG_DIR}/jderobot-zeroc-ice-python.info.in ${CMAKE_BINARY_DIR}/jderobot-zeroc-ice-python_${PACKAGE_VERSION}_all.info)

execute_process(
    COMMAND ${MAKE_PACKAGE_EXECUTABLE}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)


## Inject release roll up (incremental releases)
# this step should be only for jderobot package
#set(PROJECT_VERSION ${CPACK_DEBIAN_PACKAGE_VERSION})
#include(scripts/incremental_releases/incremental-releases.cmake)
#set(CPACK_DEBIAN_PACKAGE_VERSION ${PROJECT_VERSION})


#SET (CPACK_COMPONENTS_ALL Libraries ApplicationData)

SET(JdeRobot_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/lib/jderobot )
SET(ALL_LIBS  ${LIBS_NEEDED} ${LIBS_EXTRA})
foreach (l ${ALL_LIBS} )
	if (TARGET ${l})
		SET(JdeRobot_LIBS_NAMES ${JdeRobot_LIBS_NAMES} ${l})
	endif()
endforeach()


configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/JdeRobotInstallConfig.cmake.in "${PROJECT_BINARY_DIR}/JdeRobotInstallConfig.cmake" @ONLY)
install (SCRIPT "${PROJECT_BINARY_DIR}/JdeRobotInstallConfig.cmake")
SET(INSTALL_CMAKE_DIR ${CMAKE_INSTALL_PREFIX}/share/jderobot)
install(FILES
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/JdeRobotConfig.cmake"
  "${PROJECT_BINARY_DIR}/JdeRobotConfigVersion.cmake"
  DESTINATION "${INSTALL_CMAKE_DIR}")

include (CPack)
