if (GAZEBO_INCLUDE_DIRS)

include_directories(
	${GAZEBO_INCLUDE_DIRS}
    ${INTERFACES_CPP_DIR}
    ${LIBS_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR})

link_directories(${GAZEBO_LIBRARY_DIRS})

add_library(camera_nao SHARED camera_dump.cc)
target_link_libraries(camera_nao ${GAZEBO_libraries} 
	CameraPlugin 
	JderobotInterfaces
	colorspacesmm	
	${GAZEBO_libraries} 
	${ZeroCIce_LIBRARIES} 
)
    
add_library(poseneck SHARED poseneck.cc)
target_link_libraries(poseneck ${GAZEBO_libraries} 	 	
	JderobotInterfaces
	${GAZEBO_libraries} 
	${ZeroCIce_LIBRARIES} 
)

add_library(poserightshoulder SHARED poserightshoulder.cc)
target_link_libraries(poserightshoulder ${GAZEBO_libraries} 
    JderobotInterfaces	 	
	${GAZEBO_libraries} 
	${ZeroCIce_LIBRARIES} 
)

add_library(poseleftshoulder SHARED poseleftshoulder.cc)
target_link_libraries(poseleftshoulder ${GAZEBO_libraries} 	 	
	JderobotInterfaces
	${GAZEBO_libraries} 
	${ZeroCIce_LIBRARIES}
)

add_library(poserightelbow SHARED poserightelbow.cc)
target_link_libraries(poserightelbow ${GAZEBO_libraries} 	 	
	JderobotInterfaces
	${GAZEBO_libraries} 
	${ZeroCIce_LIBRARIES}
)

add_library(poseleftelbow SHARED poseleftelbow.cc)
target_link_libraries(poseleftelbow ${GAZEBO_libraries} 	 	
	JderobotInterfaces
	${GAZEBO_libraries} 
	${ZeroCIce_LIBRARIES}
)

ENDIF()
