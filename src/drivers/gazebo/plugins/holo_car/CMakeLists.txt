
include_directories(
	${GAZEBO_INCLUDE_DIRS}
    ${INTERFACES_CPP_DIR}
    ${LIBS_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${easyiceconfig_INCLUDE_DIRS}
)

link_directories(
    ${GAZEBO_LIBRARY_DIRS}
    ${easyiceconfig_LIBRARY_DIRS}
)
#boost_system 
add_library(holoCarMotors SHARED holoCarMotors.cc)
target_link_libraries(holoCarMotors 
	${GAZEBO_libraries} 
	${ZeroCIce_LIBRARIES} 

    ${easyiceconfig_LIBRARIES}
    colorspacesmm
    JderobotInterfaces
)

add_library(holoCarPose3D SHARED holoCarPose3d.cc)
target_link_libraries(holoCarPose3D
	${GAZEBO_libraries} 
	${ZeroCIce_LIBRARIES} 

    ${easyiceconfig_LIBRARIES}
    colorspacesmm
    JderobotInterfaces
)


INSTALL (TARGETS holoCarMotors DESTINATION ${CMAKE_INSTALL_PREFIX}/share/jderobot/gazebo/plugins/holo_car/ COMPONENT gazebo)
INSTALL (TARGETS holoCarPose3D DESTINATION ${CMAKE_INSTALL_PREFIX}/share/jderobot/gazebo/plugins/holo_car/ COMPONENT gazebo)
FILE(GLOB_RECURSE CFG_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cfg)
INSTALL (FILES ${CFG_FILES} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/jderobot/conf/ COMPONENT gazebo)


