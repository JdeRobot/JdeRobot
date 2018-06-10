file(GLOB SOURCE_FILES "src/*.cpp")

add_definitions(-DGLADE_DIR="${gladedir}")

include_directories( "/opt/vc/include"
	${INTERFACES_CPP_DIR}
	${LIBS_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
    	${easyiceconfig_INCLUDE_DIRS}
)

link_directories(
	${easyiceconfig_LIBRARY_DIRS}
	"/opt/vc/lib"
)

add_executable (picamserver ${SOURCE_FILES})


TARGET_LINK_LIBRARIES(picamserver mmal mmal_core mmal_util
    jderobotutil
	${OpenCV_LIBRARIES}
    ${OpenCVGUI_LIBRARIES}
	colorspacesmm
	JderobotInterfaces
	${easyiceconfig_LIBRARIES}
	${ZeroCIce_LIBRARIES}
	${CMAKE_THREAD_LIBS_INIT}
	${ZLIB_LIBRARIES}
	logger
    crypto
    ns
	${GLOG_LIBRARIES}
)


INSTALL(TARGETS picamserver
    DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/
    COMPONENT picameraser
    )

INSTALL (FILES ${CMAKE_CURRENT_SOURCE_DIR}/picamserver.cfg
	DESTINATION ${CMAKE_INSTALL_PREFIX}/share/jderobot/conf
	COMPONENT picamserver
)
