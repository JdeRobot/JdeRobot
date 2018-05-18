
SET( SOURCE_FILES camserver.cpp)

add_definitions(-DGLADE_DIR="${gladedir}")

include_directories(
	${INTERFACES_CPP_DIR}
	${LIBS_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
    	${easyiceconfig_INCLUDE_DIRS}
)

link_directories(
	${easyiceconfig_LIBRARY_DIRS}
)

add_executable (camserver ${SOURCE_FILES})

TARGET_LINK_LIBRARIES(camserver
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

INSTALL(TARGETS camserver
    DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/
    COMPONENT camserver
    )
    
INSTALL (FILES ${CMAKE_CURRENT_SOURCE_DIR}/camserver.cfg 
	DESTINATION ${CMAKE_INSTALL_PREFIX}/share/jderobot/conf
	COMPONENT camserver
)
