
## Dependencies
include_directories(
	${GAZEBO_INCLUDE_DIRS}
	${INTERFACES_CPP_DIR}
	${LIBS_DIR}
	${easyiceconfig_INCLUDE_DIRS}
)

link_directories(
	${GAZEBO_LIBRARY_DIRS}
	${INTERFACES_CPP_DIR}
	${LIBS_DIR}/visionlib/colorspaces
	${easyiceconfig_LIBRARY_DIRS}
)


## Project
include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Headers trick for IDEs
file(GLOB_RECURSE quadrotor2_headers "include/**")
add_library(quadrotor2_headers ${quadrotor2_headers})
set_target_properties(quadrotor2_headers PROPERTIES LINKER_LANGUAGE CXX)


#add_library(quadrotor2_ice_interfaces STATIC
#	src/interfaces/pose3di.cpp
#	src/interfaces/navdatai.cpp
#)
#target_link_libraries(quadrotor2_ice_interfaces
#	Ice
#	IceUtil
#	JderobotInterfaces
#	${ZeroCIce_LIBRARIES}
#)



add_library(quadrotorplugin2 SHARED
	src/quadrotorplugin.cc
	src/quadrotorsensors.cc
	src/quadrotorcontrol.cc

	src/quadrotorcontrol_hector.cc
	src/pidcontroller.cc

	src/cameraproxy.cc

	src/quadrotorice.cc
	src/interfaces/pose3di.cpp
	src/interfaces/navdatai.cpp
	src/interfaces/dronecontroli.cpp
	src/interfaces/cmdveli.cpp
	src/interfaces/camerai.cpp
	src/interfaces/pushcamerai.cpp
	src/interfaces/cameraibase.cpp
)

# Gazebo
target_link_libraries(quadrotorplugin2 ${GAZEBO_libraries})
set_target_properties(quadrotorplugin2 PROPERTIES COMPILE_FLAGS "${GAZEBO_CXX_FLAGS}")

# Ice
target_link_libraries(quadrotorplugin2
	Ice
	IceUtil
	JderobotInterfaces
	${ZeroCIce_LIBRARIES}
	${easyiceconfig_LIBRARIES}
)

# Application
target_link_libraries(quadrotorplugin2
	${OpenCV_LIBS}
	colorspacesmm
)

set_target_properties(quadrotorplugin2 PROPERTIES COMPILE_FLAGS "-Wall -Wextra")

INSTALL (TARGETS quadrotorplugin2 DESTINATION ${CMAKE_INSTALL_PREFIX}/share/jderobot/gazebo/plugins/quadrotor/ COMPONENT gazebo)
FILE(GLOB_RECURSE CFG_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cfg)
INSTALL (FILES ${CFG_FILES} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/jderobot/conf COMPONENT gazebo)
