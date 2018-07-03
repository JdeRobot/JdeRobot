project (jderobot-gazebo-assets)


# Change default install path
if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set (CMAKE_INSTALL_PREFIX /opt/jderobot CACHE PATH "default install path" FORCE)
endif()

cmake_minimum_required(VERSION 3.3)

set (MODELS ${CMAKE_INSTALL_PREFIX}/share/jderobot/gazebo/models)
set (WORLDS ${CMAKE_INSTALL_PREFIX}/share/jderobot/gazebo/worlds)

configure_file(
    gazebo-assets-setup.sh.in
    gazebo-assets-setup.sh
    @ONLY
)

###################
#                 #
#    INSTALL      #
#                 #
###################

INSTALL(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/launch DESTINATION ${CMAKE_INSTALL_PREFIX}/share/jderobot/gazebo COMPONENT gazebo-assets)
INSTALL (DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/models DESTINATION share/jderobot/gazebo/ COMPONENT gazebo-assets PATTERN .svn EXCLUDE)
INSTALL (DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/worlds DESTINATION share/jderobot/gazebo/ COMPONENT gazebo-assets PATTERN .svn EXCLUDE)
INSTALL (FILES ${CMAKE_CURRENT_BINARY_DIR}/gazebo-assets-setup.sh DESTINATION share/jderobot/gazebo/ COMPONENT gazebo-assets )

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
SET (CPACK_DEBIAN_PACKAGE_SHLIBDEPS OFF)
SET (CPACK_PACKAGING_INSTALL_PREFIX "/usr/local")


# CPack version numbers for release tarball name.
SET (CPACK_PACKAGE_VERSION_MAJOR 5)
SET (CPACK_PACKAGE_VERSION_MINOR 6)
SET (CPACK_PACKAGE_VERSION_PATCH 5)
SET (CPACK_DEBIAN_PACKAGE_VERSION ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH})


SET (CPACK_DEBIAN_PACKAGE_PRIORITY "extra")
SET (CPACK_DEBIAN_PACKAGE_SECTION "net")
dpkg_arch(CPACK_DEBIAN_PACKAGE_ARCHITECTURE)
SET (CPACK_DEBIAN_PACKAGE_REPLACES "jderobot-gazeboserver")

SET (CPACK_MONOLITHIC_INSTALL OFF)

SET (CPACK_PACKAGE_DESCRIPTION_SUMMARY
"Assets")
SET (CPACK_PACKAGE_DESCRIPTION
"Assets")

## Patch: CPACK_PACKAGE_DESCRIPTION behavior is broken. Always use SUMMARY
SET (CPACK_PACKAGE_DESCRIPTION_SUMMARY ${CPACK_PACKAGE_DESCRIPTION})
SET (CPACK_PACKAGE_CONTACT "Francisco Perez <f.perez475@gmail.com>")
SET (CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}_${CPACK_DEBIAN_PACKAGE_VERSION}_${CPACK_DEBIAN_PACKAGE_ARCHITECTURE}")


include (CPack)



