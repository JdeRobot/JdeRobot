IF (GAZEBO_INCLUDE_DIRS)

	add_subdirectory (${CMAKE_CURRENT_SOURCE_DIR}/plugins ${CMAKE_CURRENT_SOURCE_DIR}/plugins)


	## INSTALL ##

	# Install script
	INSTALL (FILES ${CMAKE_CURRENT_SOURCE_DIR}/gazebo-setup.sh DESTINATION /usr/local/share/jderobot/gazebo/)

	# Install Gazebo Models
	INSTALL (DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/models DESTINATION /usr/local/share/jderobot/gazebo/ PATTERN .svn EXCLUDE)

	# Install Gazebo Worlds
	INSTALL (DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/worlds DESTINATION /usr/local/share/jderobot/gazebo/ PATTERN .svn EXCLUDE)

	# Install Gazebo Plugins
	SET(PLUGINS "kinect" "nao" "pioneer")

	FOREACH (PLUGIN ${PLUGINS})
		FILE(GLOB_RECURSE SO_FILES ${CMAKE_CURRENT_SOURCE_DIR}/plugins/${PLUGIN}/*.so)
		INSTALL (FILES ${SO_FILES} DESTINATION /usr/local/share/jderobot/gazebo/plugins/${PLUGIN}/ )
		FILE(GLOB_RECURSE CFG_FILES ${CMAKE_CURRENT_SOURCE_DIR}/plugins/${PLUGIN}/*.cfg)
		INSTALL (FILES ${CFG_FILES} DESTINATION /usr/local/share/jderobot/gazebo/plugins/${PLUGIN}/ )	
	ENDFOREACH(PLUGIN)

ENDIF()
