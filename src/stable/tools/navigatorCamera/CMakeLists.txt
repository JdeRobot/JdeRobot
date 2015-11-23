SET( SOURCE_FILES
	navigatorCamera.cpp
	gui.cpp
	sharer.cpp
)

add_definitions(-DGLADE_DIR="${gladedir}")

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall" )

include_directories(
	${INTERFACES_CPP_DIR}
	${LIBS_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	${gtkmm_INCLUDE_DIRS}
	${libglademm_INCLUDE_DIRS}
	${gtkglextmm_INCLUDE_DIRS}
	${ZLIB_INCLUDE_DIRS}
)

add_executable (navigatorCamera  ${SOURCE_FILES})

TARGET_LINK_LIBRARIES(navigatorCamera
	${CMAKE_THREAD_LIBS_INIT}
	${gtkmm_LIBRARIES}
	${libglademm_LIBRARIES}
	${gtkglextmm_LIBRARIES}
	${OpenCV_LIBRARIES}
	colorspacesmm
	JderobotInterfaces
	parallelIce
	${ZeroCIce_LIBRARIES}
	${ZLIB_LIBRARIES}
)
