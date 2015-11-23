include_directories(
	${GAZEBO_INCLUDE_DIRS}
    ${INTERFACES_CPP_DIR}
    ${LIBS_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR})

link_directories(${GAZEBO_LIBRARY_DIRS})

#set( CMAKE_CXX_FLAGS "-Wall" )
    
add_library(motorsTurtlebotJde SHARED motors.cc)
target_link_libraries(motorsTurtlebotJde 
	${GAZEBO_libraries} 
	${ZeroCIce_LIBRARIES} 
	colorspacesmm
	JderobotInterfaces) 
    
add_library(encodersTurtlebotJde SHARED encoders.cc)
target_link_libraries(encodersTurtlebotJde 
	${GAZEBO_libraries} 
	${ZeroCIce_LIBRARIES} 
	colorspacesmm
	JderobotInterfaces)    

add_library(laserTurtlebotJde SHARED laser.cc)
target_link_libraries(laserTurtlebotJde 
	RayPlugin 
	${GAZEBO_libraries} 
	${ZeroCIce_LIBRARIES} 
	colorspacesmm
    	JderobotInterfaces) 

add_library(kinectPluginTurtlebotJde SHARED kinectPlugin.cc)
target_link_libraries(kinectPluginTurtlebotJde
    JderobotInterfaces
    jderobotutil
    ${GAZEBO_libraries}
    ${PCL_LIBRARIES}
    ${OpenCV_LIBRARIES}
    ${ZeroCIce_LIBRARIES}
    colorspacesmm
)


add_library(camera_dumpTurtlebotJde SHARED camera_dump.cc)
target_link_libraries(camera_dumpTurtlebotJde ${GAZEBO_libraries} 
	CameraPlugin 	
	${GAZEBO_libraries} 
	${ZeroCIce_LIBRARIES} 
	colorspacesmm
	JderobotInterfaces)

add_library(pose3dencodersTurtlebotJde SHARED pose3dencoders.cc)
target_link_libraries(pose3dencodersTurtlebotJde 
	${GAZEBO_libraries} 	 	
	${GAZEBO_libraries} 
	${ZeroCIce_LIBRARIES} 
	colorspacesmm
	JderobotInterfaces)


INSTALL (TARGETS motorsTurtlebotJde encodersTurtlebotJde laserTurtlebotJde camera_dumpTurtlebotJde pose3dencodersTurtlebotJde DESTINATION share/jderobot/gazebo/plugins/turtlebot/ )
FILE(GLOB_RECURSE CFG_FILES ${CMAKE_CURRENT_SOURCE_DIR}/cfg/*.cfg)
INSTALL (FILES ${CFG_FILES} DESTINATION share/jderobot/gazebo/plugins/turtlebot/)

#MIO endif()
