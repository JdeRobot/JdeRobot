IF (NODEJS_COMPILE)

	configure_file(
    	kobukiviewerjs.in
    	kobukiviewerjs
    	@ONLY
	)

	## INSTALL ##

	# install Launcher
	install(
	    FILES ${CMAKE_CURRENT_BINARY_DIR}/kobukiviewerjs
	    PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ WORLD_EXECUTE WORLD_READ
	    DESTINATION bin
	    COMPONENT webtools
	)

	# Install script
	INSTALL (FILES ${CMAKE_CURRENT_SOURCE_DIR}/run.js DESTINATION share/jderobot/webtools/kobukiviewerjs COMPONENT webtools )

	# Install public
	INSTALL (DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/public DESTINATION share/jderobot/webtools/kobukiviewerjs COMPONENT webtools PATTERN .svn EXCLUDE)


ENDIF()
