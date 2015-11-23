SET(SOURCE_FILES ./vehicleCounter.cpp)

include_directories(
	${LIBS_DIR}
	${OpenCV_INCLUDE_DIR}
	${Ice_INCLUDE_DIR}
	${INTERFACES_CPP_DIR})

link_directories(${JDE_LIBS})

add_executable(vehicleCounter 
	${SOURCE_FILES}
	${INTERFACES_CPP_DIR})

target_link_libraries(vehicleCounter
	${CMAKE_THREAD_LIBS_INIT}
	${Ice_LIBRARIES}
	${ZeroCIce_LIBRARIES}
	${OpenCV_LIBRARIES}
	${OpenCVGUI_LIBRARIES}
	cvblob
	JderobotInterfaces
	jderobotutil)
