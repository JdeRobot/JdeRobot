cmake_minimum_required(VERSION 2.8)
if(roscpp_FOUND)
    project(jderobotcomm)

    list(APPEND Ice_LIBRARIES Ice IceUtil)

    SET (CATKIN_DEVEL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR})

    find_package(catkin REQUIRED COMPONENTS
      roscpp
      std_msgs
    )

    catkin_package(
      CATKIN_DEPENDS roscpp std_msgs message_runtime
    )

    set(binname ${PROJECT_NAME})

    ### Project config
    include_directories(
        include
        ${jderobottypes_INCLUDE_DIRS}
        ${Ice_INCLUDE_DIR}
        ${LIBS_DIR}
        ${catkin_INCLUDE_DIRS}
        ${roscpp_INCLUDE_DIRS}
        ${ice_interfaces_INCLUDE_DIRS}
    )


    set(HEADERS
        include/jderobot/comm/laserClient.hpp
        include/jderobot/comm/interfaces/laserClient.hpp
        include/jderobot/comm/ice/laserIceClient.hpp
        include/jderobot/comm/ros/translators.hpp
        include/jderobot/comm/ros/listenerLaser.hpp
    )

    set(SOURCES
        src/laserClient.cpp
        src/ice/laserIceClient.cpp
        src/ros/translators.cpp
        src/ros/listenerLaser.cpp
    )

    ## Adding shared library for common usage
    add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS})
    target_link_libraries(${PROJECT_NAME} ${Ice_LIBRARIES} logger ${Boost_LIBRARIES} JderobotInterfaces ${catkin_LIBRARIES})

    ## Adding static library for single .so configurations
    # since target is a shared library, -fPIC must be provided
    add_library(${PROJECT_NAME}-embedded STATIC ${SOURCES} ${HEADERS})
    target_link_libraries(${PROJECT_NAME}-embedded ${Ice_LIBRARIES} logger ${Boost_LIBRARIES} JderobotInterfaces)
    set_property(TARGET ${PROJECT_NAME}-embedded PROPERTY POSITION_INDEPENDENT_CODE 1)



    ## Export library variables (like find_package)
    set(${PROJECT_NAME}_FOUND 1 CACHE BOOL "Find(${PROJECT_NAME})")
    set(${PROJECT_NAME}_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include" CACHE PATH "Find(${PROJECT_NAME})")
    set(${PROJECT_NAME}_LIBRARY_DIRS "${CMAKE_CURRENT_BINARY_DIR}" CACHE PATH "Find(${PROJECT_NAME})")
    set(${PROJECT_NAME}_LIBRARIES "${PROJECT_NAME}" CACHE STRINGS "Find(${PROJECT_NAME})")



    ### Install
    install(TARGETS ${PROJECT_NAME}
        DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/jderobot
        COMPONENT core
    )
    install(DIRECTORY include/
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include
        COMPONENT core
    )

endif() ## roscpp_FOUND
