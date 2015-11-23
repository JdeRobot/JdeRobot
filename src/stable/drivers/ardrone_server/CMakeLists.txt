
if(ARDRONE_ALL_DEPS)

	set(CMAKE_CXX_FLAGS "-w -lIce -lIceUtil")

	include_directories(
		${INTERFACES_CPP_DIR}
		${OpenCV_INCLUDE_DIR}
		${Ice_INCLUDE_DIR}
		${LIBS_DIR}
		${Boost_INCLUDE_DIR}
		${EIGEN_INCLUDE_DIR}
		${libxmlpp_INCLUDE_DIRS}
		${ardronelib_INCLUDE_DIRS})

	link_directories(
		${ardronelib_LIBRARY_DIRS})

	add_executable(ardrone_server src/ardrone_driver.cpp 
					src/video.cpp 
					src/ardrone_sdk.cpp 
					src/teleop_twist.cpp
					src/components/cameraserver.cpp 
					src/components/cmdveli.cpp				
					src/components/pose3di.cpp
					src/components/remoteconfigi.cpp
					src/components/navdatai.cpp			
					src/components/ardroneextrai.cpp									
					src/ardrone_parser.cpp
					src/ardrone_config.cpp)

	target_link_libraries(ardrone_server 
				${ardronelib_LIBRARIES}
				SDL 
				${CMAKE_THREAD_LIBS_INIT})

	target_link_libraries(ardrone_server
				${OpenCV_LIBRARIES}
				${ZeroCIce_LIBRARIES}
				colorspacesmm
				JderobotInterfaces
				${libxmlpp_LIBRARIES})


ELSE()
	message(WARNING "ardrone_server marked to build, but required dependencies were not met.
		You should install third party library: ardonelib")
ENDIF()
