if(ARDRONE_ALL_DEPS)
	project(ardrone_server)

	# Ice msg
	set(SLICE_OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/include-slice)
	file(MAKE_DIRECTORY ${SLICE_OUTPUT})
	execute_process(
		COMMAND slice2cpp ${CMAKE_CURRENT_SOURCE_DIR}/slice/navdatagps.ice --output-dir=${SLICE_OUTPUT}
	)
	include_directories(${SLICE_OUTPUT})
	add_library(ardrone_server_msgs STATIC ${SLICE_OUTPUT}/navdatagps.cpp)


	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w -lIce -lIceUtil")

	include_directories(
		${INTERFACES_CPP_DIR}
		${OpenCV_INCLUDE_DIR}
		${Ice_INCLUDE_DIR}
		${LIBS_DIR}
		${Boost_INCLUDE_DIR}
		${EIGEN_INCLUDE_DIR}
		${libxmlpp_INCLUDE_DIRS}
		${ardronelib_INCLUDE_DIRS}
    		${easyiceconfig_INCLUDE_DIRS})

	link_directories(
		${ardronelib_LIBRARY_DIRS}
		${easyiceconfig_LIBRARY_DIRS})

	include_directories(include)


	add_executable(ardrone_server src/ardrone_driver.cpp 
					src/video.cpp 
					src/ardrone_sdk.cpp 
					src/teleop_twist.cpp

					include/ardrone_server/interfaces/cameraserver.cpp
					src/interfaces/cmdveli.cpp
					src/interfaces/pose3di.cpp
					src/interfaces/remoteconfigi.cpp
					src/interfaces/navdatai.cpp
					src/interfaces/ardroneextrai.cpp
					src/interfaces/navdatagpsi.cpp

					src/ardrone_parser.cpp
					src/ardrone_config.cpp
	)

	target_link_libraries(ardrone_server 
				${ardronelib_LIBRARIES}
				SDL 
				${CMAKE_THREAD_LIBS_INIT})

	target_link_libraries(ardrone_server
				${OpenCV_LIBRARIES}
				${ZeroCIce_LIBRARIES}
   				${easyiceconfig_LIBRARIES} 
				colorspacesmm
				JderobotInterfaces
				${libxmlpp_LIBRARIES})

	target_link_libraries(ardrone_server
				ardrone_server_msgs)


	add_custom_command(TARGET ardrone_server POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
			${CMAKE_CURRENT_SOURCE_DIR}/config $<TARGET_FILE_DIR:ardrone_server>/config)

	add_custom_command(TARGET ardrone_server POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			${CMAKE_CURRENT_SOURCE_DIR}/ardrone_interfaces.cfg $<TARGET_FILE_DIR:ardrone_server>)


	if(DEFINED ARDRONE_TEST_PARSER)
		message(STATUS "Enable testing for ardrone_server")
		add_executable(test_parser test/test_parser.cpp src/ardrone_parser.cpp src/ardrone_config.cpp)
		set_target_properties(ardrone_server test_parser PROPERTIES COMPILE_DEFINITIONS "ARDRONE_TEST_PARSER")
	endif()


	add_executable(ardrone_print_gps_data test/print_gps_data.cpp)
	target_link_libraries(ardrone_print_gps_data
		${ZeroCIce_LIBRARIES}
		ardrone_server_msgs
		${CMAKE_THREAD_LIBS_INIT}
    		${easyiceconfig_LIBRARIES})


    INSTALL(DIRECTORY config DESTINATION ${CMAKE_INSTALL_PREFIX}/share/jderobot/conf)

ELSE()
	message(WARNING "ardrone_server marked to build, but required dependencies were not met.
		You should install third party library: ardonelib")
ENDIF()
