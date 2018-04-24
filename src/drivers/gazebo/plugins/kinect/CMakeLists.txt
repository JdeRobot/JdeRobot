if (PCL_LIBRARIES)

include_directories( 
	${GAZEBO_INCLUDE_DIRS}
    ${INTERFACES_CPP_DIR}
    ${LIBS_DIR}
	${GEARBOX_INCLUDE_DIRS}
	/usr/include/vtk-5.8
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${easyiceconfig_INCLUDE_DIRS}
	) 

link_directories( 
	${GAZEBO_LIBRARY_DIRS} 
	${easyiceconfig_LIBRARY_DIRS}
	)

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -lpcl_filters" ) # Opciones para el compilador

add_library(kinectPlugin SHARED kinectPlugin.cc)

target_link_libraries(kinectPlugin 
    JderobotInterfaces
    jderobotutil
    ${GAZEBO_libraries} 
    ${PCL_LIBRARIES}  
    ${OpenCV_LIBRARIES}
    ${ZeroCIce_LIBRARIES}
    colorspacesmm
)

INSTALL (TARGETS kinectPlugin DESTINATION ${CMAKE_INSTALL_PREFIX}/share/jderobot/gazebo/plugins/kinect/ COMPONENT gazebo)
FILE(GLOB_RECURSE CFG_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cfg)
INSTALL (FILES ${CFG_FILES} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/jderobot/conf COMPONENT gazebo)

endif()
