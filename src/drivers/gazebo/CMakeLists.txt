IF (GAZEBO_INCLUDE_DIRS)


	set (MODELS ${CMAKE_INSTALL_PREFIX}/share/jderobot/gazebo/models)
	set (WORLDS ${CMAKE_INSTALL_PREFIX}/share/jderobot/gazebo/worlds)
	set (PLUGINS ${CMAKE_INSTALL_PREFIX}/share/jderobot/gazebo/plugins)
	set (PLUGINLST "")

	file(GLOB sub-dir RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/plugins ${CMAKE_CURRENT_SOURCE_DIR}/plugins/*)
  	foreach(dir ${sub-dir})
		if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/plugins/${dir})
		  SET (PLUGINLST ${PLUGINLST}:${PLUGINS}/${dir})
		endif()
	endforeach()

	add_subdirectory (${CMAKE_CURRENT_SOURCE_DIR}/plugins)

	configure_file(
    	gazebo-setup.sh.in
    	gazebo-setup.sh
    	@ONLY
	)

	configure_file(
    	gazebo-assets-setup.sh.in
    	gazebo-assets-setup.sh
    	@ONLY
	)

	## INSTALL ##

	# Install script
	INSTALL (FILES ${CMAKE_CURRENT_BINARY_DIR}/gazebo-setup.sh DESTINATION share/jderobot/gazebo/ COMPONENT gazebo )

	# Install Gazebo Models
	INSTALL (DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/models DESTINATION share/jderobot/gazebo/ COMPONENT gazebo-assets PATTERN .svn EXCLUDE)
	# Install Gazebo Worlds
	INSTALL (DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/worlds DESTINATION share/jderobot/gazebo/ COMPONENT gazebo-assets PATTERN .svn EXCLUDE)
	INSTALL (FILES ${CMAKE_CURRENT_BINARY_DIR}/gazebo-assets-setup.sh DESTINATION share/jderobot/gazebo/ COMPONENT gazebo-assets )


ENDIF()
