cmake_minimum_required(VERSION 2.8)
project(jderobottypes)


### Project config
include_directories(
    include
)

set(HEADERS
    include/jderobot/types/laserData.h
)

#set(SOURCES
#    src/loader.cpp
#    src/initializer.cpp
#)
#set_property(SOURCE src/loader.cpp PROPERTY COMPILE_FLAGS "-std=c++0x")

## Adding shared library for common usage
#add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS})
#target_link_libraries(${PROJECT_NAME} ${Ice_LIBRARIES})

## Adding static library for single .so configurations
# since target is a shared library, -fPIC must be provided
#add_library(${PROJECT_NAME}-embedded STATIC ${SOURCES} ${HEADERS})
#target_link_libraries(${PROJECT_NAME}-embedded ${Ice_LIBRARIES})
#set_property(TARGET ${PROJECT_NAME}-embedded PROPERTY POSITION_INDEPENDENT_CODE 1)

## Export library variables (like find_package)
set(${PROJECT_NAME}_FOUND 1 CACHE BOOL "Find(${PROJECT_NAME})")
set(${PROJECT_NAME}_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include" CACHE PATH "Find(${PROJECT_NAME})")
set(${PROJECT_NAME}_LIBRARY_DIRS "${CMAKE_CURRENT_BINARY_DIR}" CACHE PATH "Find(${PROJECT_NAME})")
set(${PROJECT_NAME}_LIBRARIES "${PROJECT_NAME}" CACHE STRINGS "Find(${PROJECT_NAME})")



### Install
#install(TARGETS ${PROJECT_NAME}
#    DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/jderobot
#    COMPONENT core
#)
install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_PREFIX}/include
    COMPONENT core
)
