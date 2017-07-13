cmake_minimum_required(VERSION 2.8)
project(joystick)

## FindIce
# since Ice is located at standard location (/usr),
# include_directories() and link_directories() can
# be ommited. Only Ice_LIBRARIES must be defined, and
# we can handle it just like pkg-config
#find_package(Ice)


set(HEADERS
    joystick.hh
)

set(SOURCES
    joystick.cc
)
set_property(SOURCE joystick.cc PROPERTY COMPILE_FLAGS "-std=c++0x")

## Adding shared library for common usage
add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS})

## Adding static library for single .so configurations
# since target is a shared library, -fPIC must be provided
add_library(${PROJECT_NAME}-embedded STATIC ${SOURCES} ${HEADERS})
set_property(TARGET ${PROJECT_NAME}-embedded PROPERTY POSITION_INDEPENDENT_CODE 1)

## Export library variables (like find_package)
set(${PROJECT_NAME}_FOUND 1 CACHE BOOL "Find(${PROJECT_NAME})")
set(${PROJECT_NAME}_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}" CACHE PATH "Find(${PROJECT_NAME})")
set(${PROJECT_NAME}_LIBRARY_DIRS "${CMAKE_CURRENT_BINARY_DIR}" CACHE PATH "Find(${PROJECT_NAME})")
set(${PROJECT_NAME}_LIBRARIES "${PROJECT_NAME}" CACHE STRINGS "Find(${PROJECT_NAME})")


### Install
install(TARGETS ${PROJECT_NAME} ${PROJECT_NAME}-embedded
    DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/jderobot
    COMPONENT core
)

