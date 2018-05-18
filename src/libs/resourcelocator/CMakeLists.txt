#  Copyright (C) 2016 JDE Developers Team
#    Cmake template authors:
#      2016 Jan, Victor Arribas <v.arribas.urjc@gmail.com>
#  Authors:
#    Victor Arribas <v.arribas.urjc@gmail.com>
#    Francisco Perez <f.perez475@gmail.com>

cmake_minimum_required(VERSION 2.8.4)

project(resourcelocator)

set(binname ${PROJECT_NAME})

list(APPEND LIBRARIES)

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${CMAKE_CURRENT_BINARY_DIR}/include
)

## Sources
set(SOURCES
	include/resourcelocator/stdutils.hpp
	include/resourcelocator/resourcelocator.hpp
	include/resourcelocator/gladelocator.hpp

	src/resourcelocator.cpp
)

set_property(SOURCE src/resourcelocator.cpp PROPERTY COMPILE_FLAGS "-std=c++0x")

JDEROBOT_ADD_LIBRARY(${binname} SHARED ${SOURCES})
target_link_libraries(${binname} ${LIBRARIES})

## Options
set_target_properties(${binname} PROPERTIES COMPILE_FLAGS "-Wall -Wextra")

set(${binname}_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include;${CMAKE_CURRENT_BINARY_DIR}/include" CACHE PATH "Find(${binname})")
set(${binname}_LIBRARY_DIRS "${CMAKE_CURRENT_BINARY_DIR}" CACHE PATH "Find(${binname})")
set(${binname}_LIBRARIES "${binname}" CACHE STRINGS "Find(${binname})")

## Tests
add_executable(test_gladelocator src/test/test_gladelocator.cpp)
target_link_libraries(test_gladelocator ${binname})

### Install
install(TARGETS ${PROJECT_NAME}
    DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
    COMPONENT resourcelocator
)
install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_PREFIX}/include
    COMPONENT resourcelocator
)
