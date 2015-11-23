PKG_CHECK_MODULES(libxmlpp REQUIRED libxml++-2.6)
include_directories(${libxmlpp_INCLUDE_DIRS})
link_directories(${libxmlpp_LIBRARY_DIRS})

include_directories(
	${GAZEBO_INCLUDE_DIRS}
    ${INTERFACES_CPP_DIR}
    ${LIBS_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/interfaces
)

link_directories(${GAZEBO_LIBRARY_DIRS}
    ${INTERFACES_CPP_DIR}
	${LIBS_DIR}/visionlib/colorspaces
    )

add_library(quadrotorplugin SHARED quadrotorplugin.cc
                quadrotor_remote_config.cc
                quadrotor_parser.cpp
                quadrotor_config.cpp
                interfaces/navdatai.cpp
)

target_link_libraries(quadrotorplugin 
	${OpenCV_LIBS}
	Ice
	IceUtil
	${libxmlpp_LIBRARIES}
    JderobotInterfaces
    colorspacesmm
	${GAZEBO_libraries} 
)

add_library(imuplugin SHARED imuplugin.cc)
target_link_libraries(imuplugin 
	Ice
	IceUtil
	JderobotInterfaces
	${GAZEBO_libraries} 
	${ZeroCIce_LIBRARIES}
    ${PROTOBUF_LIBRARIES}
)

add_library(poseplugin SHARED poseplugin.cc)
target_link_libraries(poseplugin 
	${GAZEBO_libraries} 
)

add_library(togglecamplugin SHARED togglecamplugin.cc)
target_link_libraries(togglecamplugin
    ${OpenCV_LIBS}
	CameraPlugin
    colorspacesmm
	${GAZEBO_libraries}
    ${PROTOBUF_LIBRARIES}
)


INSTALL (TARGETS quadrotorplugin imuplugin poseplugin togglecamplugin DESTINATION share/jderobot/gazebo/plugins/quadrotor/ )
FILE(GLOB_RECURSE CFG_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cfg)
INSTALL (FILES ${CFG_FILES} DESTINATION share/jderobot/gazebo/plugins/quadrotor/)


