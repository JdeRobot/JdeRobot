find_package(roscpp QUIET)

if(roscpp_FOUND)

	MESSAGE("*** ROS LIBRARIES FOUND : ${roscpp_INCLUDE_DIRS}")

    SET (CMAKE_PREFIX_PATH /opt/ros/kinetic)

	list(APPEND DEPS ros-jade-roscpp ros-jade-std-msgs ros-jade-message-generation ros-jade-cv-bridge ros-jade-image-transport ros-jade-roscpp-core)

#	list(APPEND DEPS_DEV )
else()
	MESSAGE("*** ROS LIBRARIES NOT FOUND")
endif()

