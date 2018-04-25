cmake_minimum_required(VERSION 2.8)
if(roscpp_FOUND)
    add_definitions(-DJDERROS)
    SET(JDEROBOTCOM_ROS ON)
ELSE()
    SET(JDEROBOTCOM_ROS OFF)
endif()

project(comm)

usePython(2)


IF (JDEROBOTCOM_ROS)
    catkin_package(
    CATKIN_DEPENDS roscpp std_msgs message_runtime
    )
ENDIF()

set(binname ${PROJECT_NAME})

### Project config
include_directories(
    include
    ${jderobottypes_INCLUDE_DIRS}
    ${Ice_INCLUDE_DIR}
    ${LIBS_DIR}
    ${catkin_INCLUDE_DIRS}
    ${roscpp_INCLUDE_DIRS}
    ${INTERFACES_CPP_DIR}
    ${config_INCLUDE_DIRS}
)


set(HEADERS
    include/jderobot/comm/communicator.hpp
    include/jderobot/comm/tools.hpp
    include/jderobot/comm/laserClient.hpp
    include/jderobot/comm/interfaces/laserClient.hpp
    include/jderobot/comm/ice/laserIceClient.hpp
    include/jderobot/comm/cameraClient.hpp
    include/jderobot/comm/interfaces/cameraClient.hpp
    include/jderobot/comm/ice/cameraIceClient.hpp
    include/jderobot/comm/pose3dClient.hpp
    include/jderobot/comm/interfaces/pose3dClient.hpp
    include/jderobot/comm/ice/pose3dIceClient.hpp
    include/jderobot/comm/motorsClient.hpp
    include/jderobot/comm/interfaces/motorsClient.hpp
    include/jderobot/comm/ice/motorsIceClient.hpp
    include/jderobot/comm/rgbdClient.hpp
    include/jderobot/comm/interfaces/rgbdClient.hpp
    include/jderobot/comm/ice/rgbdIceClient.hpp
    include/jderobot/comm/cmdvelClient.hpp
    include/jderobot/comm/interfaces/cmdvelClient.hpp
    include/jderobot/comm/ice/cmdvelIceClient.hpp
    include/jderobot/comm/navdataClient.hpp
    include/jderobot/comm/interfaces/navdataClient.hpp
    include/jderobot/comm/ice/navdataIceClient.hpp
    include/jderobot/comm/ardroneextraClient.hpp
    include/jderobot/comm/interfaces/ardroneextraClient.hpp
    include/jderobot/comm/ice/ardroneextraIceClient.hpp
    include/jderobot/comm/bumperClient.hpp
    include/jderobot/comm/interfaces/bumperClient.hpp
    include/jderobot/comm/ice/bumperIceClient.hpp
)

set(SOURCES
    src/communicator.cpp
    src/tools.cpp
    src/laserClient.cpp
    src/ice/laserIceClient.cpp
    src/cameraClient.cpp
    src/ice/cameraIceClient.cpp
    src/pose3dClient.cpp
    src/ice/pose3dIceClient.cpp
    src/motorsClient.cpp
    src/ice/motorsIceClient.cpp
    src/rgbdClient.cpp
    src/ice/rgbdIceClient.cpp
    src/cmdvelClient.cpp
    src/ice/cmdvelIceClient.cpp
    src/ardroneextraClient.cpp
    src/ice/ardroneextraIceClient.cpp
    src/navdataClient.cpp
    src/ice/navdataIceClient.cpp
    src/bumperClient.cpp
    src/ice/bumperIceClient.cpp
)



IF(JDEROBOTCOM_ROS)
    set(HEADERS
        ${HEADERS}
        include/jderobot/comm/ros/translators.hpp
        include/jderobot/comm/ros/listenerLaser.hpp
        include/jderobot/comm/ros/listenerCamera.hpp
        include/jderobot/comm/ros/listenerPose.hpp
        include/jderobot/comm/ros/publisherMotors.hpp
        include/jderobot/comm/ros/listenerBumper.hpp
        include/jderobot/comm/ros/listenerRgbd.hpp
    )

    set(SOURCES
        ${SOURCES}
        src/ros/translators.cpp
        src/ros/listenerLaser.cpp
        src/ros/listenerCamera.cpp
        src/ros/listenerPose.cpp
        src/ros/publisherMotors.cpp
        src/ros/listenerBumper.cpp
        src/ros/listenerRgbd.cpp
    )

ENDIF()

## Adding shared library for common usage
JDEROBOT_ADD_LIBRARY(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS})

add_dependencies(${PROJECT_NAME} ${config_LIBRARIES})

target_link_libraries(${PROJECT_NAME} 
    ${ZeroCIce_LIBRARIES} 
    logger 
    ${Boost_LIBRARIES} 
    colorspacesmm 
    ${catkin_LIBRARIES}
    ${config_LIBRARIES})

## Adding static library for single .so configurations
# since target is a shared library, -fPIC must be provided
JDEROBOT_ADD_LIBRARY(${PROJECT_NAME}-embedded STATIC ${SOURCES} ${HEADERS})

target_link_libraries(${PROJECT_NAME}-embedded 
    ${ZeroCIce_LIBRARIES} 
    logger 
    ${Boost_LIBRARIES} 
    colorspacesmm 
    JderobotInterfaces
    ${catkin_LIBRARIES}
    ${config_LIBRARIES})
set_property(TARGET ${PROJECT_NAME}-embedded PROPERTY POSITION_INDEPENDENT_CODE 1)



## Export library variables (like find_package)
set(${PROJECT_NAME}_FOUND 1 CACHE BOOL "Find(${PROJECT_NAME})")
set(${PROJECT_NAME}_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include" CACHE PATH "Find(${PROJECT_NAME})")
set(${PROJECT_NAME}_LIBRARY_DIRS "${CMAKE_CURRENT_BINARY_DIR}" CACHE PATH "Find(${PROJECT_NAME})")
set(${PROJECT_NAME}_LIBRARIES "${PROJECT_NAME}" CACHE STRINGS "Find(${PROJECT_NAME})")



### Install
install(TARGETS ${PROJECT_NAME} ${PROJECT_NAME}-embedded
    DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
    COMPONENT comm
)
install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_PREFIX}/include
    COMPONENT comm
)

