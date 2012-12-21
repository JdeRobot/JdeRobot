IF(with_fireware)
    SET( SOURCE_FILES cameraserver.cpp gstpipeline.cpp)

    add_definitions(-DGLADE_DIR="${gladedir}")

    include_directories(
	${INTERFACES_CPP_DIR}
	${LIBS_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
    )

    add_executable (cameraserver ${SOURCE_FILES})

    TARGET_LINK_LIBRARIES(cameraserver 
	${gstreamerapp_LIBRARIES}
	${gstreamer_LIBRARIES}
    ${OpenCV_LIBRARIES}        
	${LIBS_DIR}/jderobotice/libjderobotice.so
	${LIBS_DIR}/colorspaces/libcolorspacesmm.so
	${INTERFACES_CPP_DIR}/jderobot/libJderobotInterfaces.so
    ${Gearbox_LIBRARIES}
	${ZeroCIce_LIBRARIES}
	${with_fireware_LIBRARIES}
	raw1394
	dc1394)
	
ENDIF(with_fireware)
