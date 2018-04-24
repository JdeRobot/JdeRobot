
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
add_library(lasertaxi SHARED laser.cc)
target_link_libraries(lasertaxi 
	RayPlugin 
	${GAZEBO_libraries} 
	${ZeroCIce_LIBRARIES} 
	colorspacesmm
    	JderobotInterfaces
	${easyiceconfig_LIBRARIES}
) 

INSTALL (TARGETS lasertaxi 
	DESTINATION ${CMAKE_INSTALL_PREFIX}/share/jderobot/gazebo/plugins/taxiholoLaser/ 
	COMPONENT gazebo)

FILE(GLOB_RECURSE CFG_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cfg)
INSTALL (FILES ${CFG_FILES} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/jderobot/conf/ COMPONENT gazebo)
