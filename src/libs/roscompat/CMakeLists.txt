#  Copyright (C) 2016 JDE Developers Team
#    Cmake template authors:
#      2016 Jan, Victor Arribas <v.arribas.urjc@gmail.com>
#  Authors:
#    Victor Arribas <v.arribas.urjc@gmail.com>
#    Francisco Perez <f.perez475@gmail.com>

cmake_minimum_required(VERSION 2.8.4)

if(roscpp_FOUND)

	project(roscompat)

	SET (CATKIN_DEVEL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR})

	find_package(catkin REQUIRED COMPONENTS
	  roscpp
	  std_msgs
	  message_generation
	  cv_bridge
	  image_transport
	)

	add_message_files(
	   FILES
	   Num.msg
	   Pose3d.msg
	)
	generate_messages(
	   DEPENDENCIES
	   std_msgs
	)

	catkin_package(
	  CATKIN_DEPENDS roscpp std_msgs message_runtime
	)

	set(binname ${PROJECT_NAME})

	include_directories(
		${CMAKE_CURRENT_SOURCE_DIR}/include
	)

	## Sources
	set(SOURCES
		include/roscompat/Num.h
		include/roscompat/Pose3d.h
		include/roscompat/roscompat.h
		include/roscompat/listener.h

		src/roscompat.cpp
		src/listener.cpp
	)

	add_library(${binname} SHARED ${SOURCES})
	target_link_libraries(${binname} ${catkin_LIBRARIES})

	## Options
	set_target_properties(${binname} PROPERTIES COMPILE_FLAGS "-Wall -Wextra")

	set(${binname}_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include" CACHE PATH "Find(${binname})")
	set(${binname}_LIBRARY_DIRS "${CMAKE_CURRENT_BINARY_DIR}" CACHE PATH "Find(${binname})")
	set(${binname}_LIBRARIES "${binname}" CACHE STRINGS "Find(${binname})")

	#add_executable(camdriver src/CameraDriver.cpp)
	#include_directories(${OpenCV_INCLUDE_DIRS})
	#target_link_libraries(camdriver ${catkin_LIBRARIES} ${OpenCV_LIBS} ${OpenCV_LIBRARIES})
	#add_dependencies(camdriver roscompat_generate_messages_cpp)
endif()
