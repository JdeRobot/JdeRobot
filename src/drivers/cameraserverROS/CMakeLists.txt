SET( SOURCE_FILES CameraDriver.cpp)

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated" )

add_executable (cameraserverROS ${SOURCE_FILES})
include_directories(
${OpenCV_INCLUDE_DIRS}
    ${INTERFACES_CPP_DIR}
    ${LIBS_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${roscompat_INCLUDE_DIRS}
)
link_directories(
	${roscompat_LIBRARY_DIRS}
)

target_link_libraries(cameraserverROS

    ${catkin_LIBRARIES} 
    ${OpenCV_LIBS} 
    ${OpenCV_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT} 
    ${CMAKE_THREAD_LIBS_INIT}
    ${roscompat_LIBRARIES}
)
add_dependencies(cameraserverROS roscompat_generate_messages_cpp)
