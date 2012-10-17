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
    ${opencv_LIBRARIES}	
	${ZeroCIce_LIBRARIES} 
    ${LIBS_DIR}/jderobotice/libjderobotice.so
    ${LIBS_DIR}/jderobotutil/libjderobotutil.so
    ${LIBS_DIR}/colorspaces/libcolorspacesmm.so
    ${INTERFACES_CPP_DIR}/jderobot/libJderobotInterfaces.so) 
    
add_library(encoders SHARED encoders.cc)
target_link_libraries(encoders 
	${GAZEBO_libraries} 
    ${opencv_LIBRARIES}	
	${ZeroCIce_LIBRARIES} 
    ${LIBS_DIR}/jderobotice/libjderobotice.so
    ${LIBS_DIR}/jderobotutil/libjderobotutil.so
    ${LIBS_DIR}/colorspaces/libcolorspacesmm.so
    ${INTERFACES_CPP_DIR}/jderobot/libJderobotInterfaces.so)    

add_library(laser SHARED laser.cc)
target_link_libraries(laser 
	RayPlugin 
	${GAZEBO_libraries} 
    ${opencv_LIBRARIES}	
	${ZeroCIce_LIBRARIES} 
    ${LIBS_DIR}/jderobotice/libjderobotice.so
    ${LIBS_DIR}/jderobotutil/libjderobotutil.so
    ${LIBS_DIR}/colorspaces/libcolorspacesmm.so
    ${INTERFACES_CPP_DIR}/jderobot/libJderobotInterfaces.so) 

add_library(camera_dump SHARED camera_dump.cc)
target_link_libraries(camera_dump ${GAZEBO_libraries} 
	CameraPlugin 	
	${GAZEBO_libraries} 
    ${opencv_LIBRARIES}	
	${ZeroCIce_LIBRARIES} 
    ${LIBS_DIR}/jderobotice/libjderobotice.so
    ${LIBS_DIR}/jderobotutil/libjderobotutil.so
    ${LIBS_DIR}/colorspaces/libcolorspacesmm.so
    ${INTERFACES_CPP_DIR}/jderobot/libJderobotInterfaces.so)
    
add_library(ptencoders SHARED ptencoders.cc)
target_link_libraries(ptencoders ${GAZEBO_libraries} 	 	
	${GAZEBO_libraries} 
    ${opencv_LIBRARIES}	
	${ZeroCIce_LIBRARIES} 
    ${LIBS_DIR}/jderobotice/libjderobotice.so
    ${LIBS_DIR}/jderobotutil/libjderobotutil.so
    ${LIBS_DIR}/colorspaces/libcolorspacesmm.so
    ${INTERFACES_CPP_DIR}/jderobot/libJderobotInterfaces.so)
    
add_library(ptmotors SHARED ptmotors.cc)
target_link_libraries(ptmotors ${GAZEBO_libraries} 	 	
	${GAZEBO_libraries} 
    ${opencv_LIBRARIES}	
	${ZeroCIce_LIBRARIES} 
    ${LIBS_DIR}/jderobotice/libjderobotice.so
    ${LIBS_DIR}/jderobotutil/libjderobotutil.so
    ${LIBS_DIR}/colorspaces/libcolorspacesmm.so
    ${INTERFACES_CPP_DIR}/jderobot/libJderobotInterfaces.so)      

add_executable(clientLaser clientLaser.cpp)
target_link_libraries(clientLaser 	
	${GAZEBO_libraries} 
    ${opencv_LIBRARIES}	
	${ZeroCIce_LIBRARIES} 
    ${LIBS_DIR}/jderobotice/libjderobotice.so
    ${LIBS_DIR}/jderobotutil/libjderobotutil.so
    ${LIBS_DIR}/colorspaces/libcolorspacesmm.so
    ${INTERFACES_CPP_DIR}/jderobot/libJderobotInterfaces.so) 

add_executable(clientEncoders clientEncoders.cpp)
target_link_libraries(clientEncoders 
	${GAZEBO_libraries} 
    ${opencv_LIBRARIES}	
	${ZeroCIce_LIBRARIES} 
    ${LIBS_DIR}/jderobotice/libjderobotice.so
    ${LIBS_DIR}/jderobotutil/libjderobotutil.so
    ${LIBS_DIR}/colorspaces/libcolorspacesmm.so
    ${INTERFACES_CPP_DIR}/jderobot/libJderobotInterfaces.so) 


add_executable(clientMotor clientMotor.cpp)
target_link_libraries(clientMotor 
	${GAZEBO_libraries} 
    ${opencv_LIBRARIES}	
	${ZeroCIce_LIBRARIES} 
    ${LIBS_DIR}/jderobotice/libjderobotice.so
    ${LIBS_DIR}/jderobotutil/libjderobotutil.so
    ${LIBS_DIR}/colorspaces/libcolorspacesmm.so
    ${INTERFACES_CPP_DIR}/jderobot/libJderobotInterfaces.so) 
