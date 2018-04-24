include_directories(
	${GAZEBO_INCLUDE_DIRS}
        ${INTERFACES_CPP_DIR}
        ${LIBS_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}
	${easyiceconfig_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Headers trick for IDEs
file(GLOB_RECURSE turtlebot_headers "include/**")
add_library(turtlebot_headers ${turtlebot_headers})
set_target_properties(turtlebot_headers PROPERTIES LINKER_LANGUAGE CXX)

link_directories(
	${GAZEBO_LIBRARY_DIRS}
 	${easyiceconfig_LIBRARY_DIRS}
)


set(SOURCES src/turtlebotplugin.cc
        src/turtlebotice.cc

        src/turtlebotcontrol.cc
        src/turtlebotsensors.cc


        src/interfaces/motorsi.cpp
        src/interfaces/laseri.cpp
        src/interfaces/pose3di.cpp
        #src/interfaces/kinectPlugini.cpp
        src/interfaces/camerai.cpp
        src/interfaces/bumperi.cpp
        src/interfaces/cameraibase.cpp
)

add_library(turtlebotplugin SHARED ${SOURCES})
# Gazebo
target_link_libraries(turtlebotplugin ${GAZEBO_libraries})
set_target_properties(turtlebotplugin PROPERTIES COMPILE_FLAGS "${GAZEBO_CXX_FLAGS}")

# Ice
target_link_libraries(turtlebotplugin
        Ice
        IceUtil
        JderobotInterfaces
        ${ZeroCIce_LIBRARIES}
        ${easyiceconfig_LIBRARIES}
)

# Application
target_link_libraries(turtlebotplugin
        ${OpenCV_LIBS}
        ${PCL_LIBRARIES}
        ${OpenCV_LIBRARIES}
        CameraPlugin
        colorspacesmm
)

set_target_properties(turtlebotplugin PROPERTIES COMPILE_FLAGS "-Wall -Wextra")
    
INSTALL (TARGETS turtlebotplugin DESTINATION ${CMAKE_INSTALL_PREFIX}/share/jderobot/gazebo/plugins/turtlebot/ COMPONENT gazebo)
FILE(GLOB_RECURSE CFG_FILES ${CMAKE_CURRENT_SOURCE_DIR}/cfg/*.cfg)
INSTALL (FILES ${CFG_FILES} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/jderobot/conf COMPONENT gazebo)

#MIO endif()
