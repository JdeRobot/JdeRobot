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

add_library(motorsMBOT SHARED motors.cc)
target_link_libraries(motorsMBOT 
	${GAZEBO_libraries} 
	${ZeroCIce_LIBRARIES} 
	colorspacesmm
	JderobotInterfaces
    	${easyiceconfig_LIBRARIES}
)   

add_library(sonarMBOT SHARED sonar.cc)
target_link_libraries(sonarMBOT 
	SonarPlugin 
	${GAZEBO_libraries} 
	${ZeroCIce_LIBRARIES} 
	colorspacesmm
   	JderobotInterfaces
	${easyiceconfig_LIBRARIES}
) 

add_library(pose3dMBOT SHARED pose3d.cc)
target_link_libraries(pose3dMBOT
	${GAZEBO_libraries} 
	${ZeroCIce_LIBRARIES} 
	colorspacesmm
    	JderobotInterfaces
	${easyiceconfig_LIBRARIES}
) 

add_library(camera_dumpMBOT SHARED camera_dump.cc)
target_link_libraries(camera_dumpMBOT ${GAZEBO_libraries} 
	CameraPlugin 	
	${GAZEBO_libraries} 
	${ZeroCIce_LIBRARIES} 
	colorspacesmm
	JderobotInterfaces
	${easyiceconfig_LIBRARIES}
)

add_library(irMBOT SHARED ir.cc)
target_link_libraries(irMBOT ${GAZEBO_libraries} 
	CameraPlugin 	
	${GAZEBO_libraries} 
	${ZeroCIce_LIBRARIES} 
	colorspacesmm
	JderobotInterfaces
	${easyiceconfig_LIBRARIES}
)
 
INSTALL (TARGETS motorsMBOT sonarMBOT pose3dMBOT camera_dumpMBOT irMBOT
	DESTINATION ${CMAKE_INSTALL_PREFIX}/share/jderobot/gazebo/plugins/pibot/ 
	COMPONENT gazeboserver)

FILE(GLOB_RECURSE CFG_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cfg)
INSTALL (FILES ${CFG_FILES} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/jderobot/conf COMPONENT gazeboserver)

#MIO endif()
