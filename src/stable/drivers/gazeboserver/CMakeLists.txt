IF (GAZEBO_INCLUDE_DIRS)

	add_subdirectory (${CMAKE_CURRENT_SOURCE_DIR}/plugins)


	## INSTALL ##

	# Install script
	INSTALL (FILES ${CMAKE_CURRENT_SOURCE_DIR}/gazebo-setup.sh DESTINATION share/jderobot/gazebo/)

	# Install Gazebo Models
	INSTALL (DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/models DESTINATION share/jderobot/gazebo/ PATTERN .svn EXCLUDE)

	# Install Gazebo Worlds
	INSTALL (DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/worlds DESTINATION share/jderobot/gazebo/ PATTERN .svn EXCLUDE)

ENDIF()
