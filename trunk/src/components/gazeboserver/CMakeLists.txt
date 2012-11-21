include_directories(
	${GAZEBO_INCLUDE_DIRS}
    ${INTERFACES_CPP_DIR}
    ${LIBS_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR})

link_directories(${GAZEBO_LIBRARY_DIRS})
#boost_system 
add_library(motors SHARED motors.cc)
target_link_libraries(motors 
	${GAZEBO_libraries} 
	${ZeroCIce_LIBRARIES} 
    ${LIBS_DIR}/jderobotice/libjderobotice.so
    ${LIBS_DIR}/jderobotutil/libjderobotutil.so
    ${LIBS_DIR}/colorspaces/libcolorspacesmm.so
    ${INTERFACES_CPP_DIR}/jderobot/libJderobotInterfaces.so) 
    
add_library(encoders SHARED encoders.cc)
target_link_libraries(encoders 
	${GAZEBO_libraries} 

	${ZeroCIce_LIBRARIES} 
    ${LIBS_DIR}/jderobotice/libjderobotice.so
    ${LIBS_DIR}/jderobotutil/libjderobotutil.so
    ${LIBS_DIR}/colorspaces/libcolorspacesmm.so
    ${INTERFACES_CPP_DIR}/jderobot/libJderobotInterfaces.so)    

add_library(laser SHARED laser.cc)
target_link_libraries(laser 
	RayPlugin 
	${GAZEBO_libraries} 

	${ZeroCIce_LIBRARIES} 
    ${LIBS_DIR}/jderobotice/libjderobotice.so
    ${LIBS_DIR}/jderobotutil/libjderobotutil.so
    ${LIBS_DIR}/colorspaces/libcolorspacesmm.so
    ${INTERFACES_CPP_DIR}/jderobot/libJderobotInterfaces.so) 

add_library(camera_dump SHARED camera_dump.cc)
target_link_libraries(camera_dump ${GAZEBO_libraries} 
	CameraPlugin 	
	${GAZEBO_libraries} 

	${ZeroCIce_LIBRARIES} 
    ${LIBS_DIR}/jderobotice/libjderobotice.so
    ${LIBS_DIR}/jderobotutil/libjderobotutil.so
    ${LIBS_DIR}/colorspaces/libcolorspacesmm.so
    ${INTERFACES_CPP_DIR}/jderobot/libJderobotInterfaces.so)
    
add_library(pose3dencoders SHARED pose3dencoders.cc)
target_link_libraries(pose3dencoders ${GAZEBO_libraries} 	 	
	${GAZEBO_libraries} 

	${ZeroCIce_LIBRARIES} 
    ${LIBS_DIR}/jderobotice/libjderobotice.so
    ${LIBS_DIR}/jderobotutil/libjderobotutil.so
    ${LIBS_DIR}/colorspaces/libcolorspacesmm.so
    ${INTERFACES_CPP_DIR}/jderobot/libJderobotInterfaces.so)
