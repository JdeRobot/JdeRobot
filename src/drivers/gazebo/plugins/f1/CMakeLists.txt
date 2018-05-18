include_directories(
	${GAZEBO_INCLUDE_DIRS}
    ${INTERFACES_CPP_DIR}
    ${LIBS_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${easyiceconfig_INCLUDE_DIRS}
)

link_directories(${GAZEBO_LIBRARY_DIRS}
    ${easyiceconfig_LIBRARY_DIRS}
)

add_library(motorsF1 SHARED motors.cc)
target_link_libraries(motorsF1 
	${GAZEBO_libraries} 
	${ZeroCIce_LIBRARIES} 
	colorspacesmm
	JderobotInterfaces
    	${easyiceconfig_LIBRARIES}
)   

add_library(laserF1 SHARED laser.cc)
target_link_libraries(laserF1 
	RayPlugin 
	${GAZEBO_libraries} 
	${ZeroCIce_LIBRARIES} 
	colorspacesmm
    	JderobotInterfaces
	${easyiceconfig_LIBRARIES}
) 

add_library(pose3dF1 SHARED pose3d.cc)
target_link_libraries(pose3dF1
	${GAZEBO_libraries} 
	${ZeroCIce_LIBRARIES} 
	colorspacesmm
    	JderobotInterfaces
	${easyiceconfig_LIBRARIES}
) 

add_library(camera_dumpF1 SHARED camera_dump.cc)
target_link_libraries(camera_dumpF1 ${GAZEBO_libraries} 
	CameraPlugin 	
	${GAZEBO_libraries} 
	${ZeroCIce_LIBRARIES} 
	colorspacesmm
	JderobotInterfaces
	${easyiceconfig_LIBRARIES}
)

INSTALL (TARGETS motorsF1 laserF1 pose3dF1 camera_dumpF1 
	DESTINATION ${CMAKE_INSTALL_PREFIX}/share/jderobot/gazebo/plugins/f1/ 
	COMPONENT gazebo)

FILE(GLOB_RECURSE CFG_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cfg)
INSTALL (FILES ${CFG_FILES} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/jderobot/conf COMPONENT gazebo)

#MIO endif()
