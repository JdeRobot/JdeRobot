
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
add_library(opelMotors SHARED opelMotors.cc)
target_link_libraries(opelMotors 
	${GAZEBO_libraries} 
	${ZeroCIce_LIBRARIES} 

    ${easyiceconfig_LIBRARIES}
    colorspacesmm
    JderobotInterfaces
)

add_library(opelPose3D SHARED pose3d.cc)
target_link_libraries(opelPose3D
	${GAZEBO_libraries} 
	${ZeroCIce_LIBRARIES} 

    ${easyiceconfig_LIBRARIES}
    colorspacesmm
    JderobotInterfaces
)

add_library(camera_dumpopel SHARED camera_dump.cc)
target_link_libraries(camera_dumpopel
	${GAZEBO_libraries} 
	${ZeroCIce_LIBRARIES} 
	CameraPlugin
    ${easyiceconfig_LIBRARIES}
    colorspacesmm
    JderobotInterfaces
)

INSTALL (TARGETS opelMotors camera_dumpopel opelPose3D DESTINATION share/jderobot/gazebo/plugins/opel/ COMPONENT gazebo)
FILE(GLOB_RECURSE CFG_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cfg)
INSTALL (FILES ${CFG_FILES} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/jderobot/conf/ COMPONENT gazebo)

