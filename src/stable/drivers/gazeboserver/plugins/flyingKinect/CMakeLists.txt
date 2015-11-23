if (PCL_LIBRARIES)

include_directories( 
	${GAZEBO_INCLUDE_DIRS}
    ${INTERFACES_CPP_DIR}
    ${LIBS_DIR}
	${GEARBOX_INCLUDE_DIRS}
	/usr/include/vtk-5.8
    ${CMAKE_CURRENT_SOURCE_DIR}
	) 

link_directories( 
	${GAZEBO_LIBRARY_DIRS} 
	)

set( CMAKE_CXX_FLAGS "-Wall -lpcl_filters" ) # Opciones para el compilador

add_library(kinect SHARED kinect.cc sharer.cc)

target_link_libraries(kinect 
    JderobotInterfaces
    jderobotutil
    colorspacesmm
    ${GAZEBO_libraries} 
    ${pcl_LIBRARIES}  
    ${OpenCV_LIBRARIES}
    ${ZeroCIce_LIBRARIES}
)

add_library(pose3d SHARED pose3d.cc sharer.cc)

target_link_libraries(pose3d
	${GAZEBO_libraries}
	Ice
	IceUtil
	JderobotInterfaces
)


INSTALL (TARGETS pose3d kinect DESTINATION share/jderobot/gazebo/plugins/flyingkinect/ )
FILE(GLOB_RECURSE CFG_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cfg)
INSTALL (FILES ${CFG_FILES} DESTINATION share/jderobot/gazebo/plugins/flyingkinect/ )	

endif()
