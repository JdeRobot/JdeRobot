include_directories(
	${GAZEBO_INCLUDE_DIRS}
    ${INTERFACES_CPP_DIR}
    ${LIBS_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR})

link_directories(${GAZEBO_LIBRARY_DIRS})

add_library(camera_nao SHARED camera_dump.cc)
target_link_libraries(camera_nao ${GAZEBO_libraries} 
    ${OpenCV_LIBS}
	CameraPlugin 
	Ice
	IceUtil
	JderobotInterfaces
	colorspacesmm
	${GAZEBO_libraries} 
	${ZeroCIce_LIBRARIES} 
)

add_library(ballencoder SHARED ballred.cc)
target_link_libraries(ballencoder ${GAZEBO_libraries} 	 
    Ice
	IceUtil	
	JderobotInterfaces
	${GAZEBO_libraries} 
	${ZeroCIce_LIBRARIES} 
)

# NECK
add_library(poseneck SHARED poseneck.cc)
target_link_libraries(poseneck ${GAZEBO_libraries} 	 
    Ice
	IceUtil	
	JderobotInterfaces
	${GAZEBO_libraries} 
	${ZeroCIce_LIBRARIES} 
)

add_library(poseneckspeed SHARED poseneckspeed.cc)
target_link_libraries(poseneckspeed ${GAZEBO_libraries} 	 
    Ice
	IceUtil	
	JderobotInterfaces
	${GAZEBO_libraries} 
	${ZeroCIce_LIBRARIES} 
)

# SHOULDERS
add_library(poserightshoulder SHARED poserightshoulder.cc)
target_link_libraries(poserightshoulder ${GAZEBO_libraries} 
    Ice
	IceUtil
    JderobotInterfaces	
	${GAZEBO_libraries} 
	${ZeroCIce_LIBRARIES} 
)

add_library(poseleftshoulder SHARED poseleftshoulder.cc)
target_link_libraries(poseleftshoulder ${GAZEBO_libraries} 	 	
    Ice
	IceUtil
	JderobotInterfaces
	${GAZEBO_libraries} 
	${ZeroCIce_LIBRARIES}
)

# ELBOWS
add_library(poserightelbow SHARED poserightelbow.cc)
target_link_libraries(poserightelbow ${GAZEBO_libraries} 	 	
    Ice
	IceUtil
	JderobotInterfaces
	${GAZEBO_libraries} 
	${ZeroCIce_LIBRARIES}
)

add_library(poseleftelbow SHARED poseleftelbow.cc)
target_link_libraries(poseleftelbow ${GAZEBO_libraries} 	 	
    Ice
	IceUtil
	JderobotInterfaces
	${GAZEBO_libraries} 
	${ZeroCIce_LIBRARIES}
)

# HIPS
add_library(poserighthip SHARED poserighthip.cc)
target_link_libraries(poserighthip ${GAZEBO_libraries} 	 	
    Ice
	IceUtil
	JderobotInterfaces
	${GAZEBO_libraries} 
	${ZeroCIce_LIBRARIES}
)

add_library(poselefthip SHARED poselefthip.cc)
target_link_libraries(poselefthip ${GAZEBO_libraries} 	 	
    Ice
	IceUtil
	JderobotInterfaces
	${GAZEBO_libraries} 
	${ZeroCIce_LIBRARIES}
)

# KNEES
add_library(poserightknee SHARED poserightknee.cc)
target_link_libraries(poserightknee ${GAZEBO_libraries} 	 	
    Ice
	IceUtil
	JderobotInterfaces
	${GAZEBO_libraries} 
	${ZeroCIce_LIBRARIES}
)

add_library(poseleftknee SHARED poseleftknee.cc)
target_link_libraries(poseleftknee ${GAZEBO_libraries} 	 	
    Ice
	IceUtil
	JderobotInterfaces
	${GAZEBO_libraries} 
	${ZeroCIce_LIBRARIES}
)

# ANKLES
add_library(poserightankle SHARED poserightankle.cc)
target_link_libraries(poserightankle ${GAZEBO_libraries} 	 	
    Ice
	IceUtil
	JderobotInterfaces
	${GAZEBO_libraries} 
	${ZeroCIce_LIBRARIES}
)

add_library(poseleftankle SHARED poseleftankle.cc)
target_link_libraries(poseleftankle ${GAZEBO_libraries} 	 	
    Ice
	IceUtil
	JderobotInterfaces
	${GAZEBO_libraries} 
	${ZeroCIce_LIBRARIES}
)
