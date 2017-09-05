configure_file(
    cameraview_yolo_py.in
    cameraview_yolo_py
    @ONLY
)

## INSTALL ##

# install Launcher
install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/cameraview_yolo_py
    PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ WORLD_EXECUTE WORLD_READ
    DESTINATION bin
)

# Install .py
FILE(GLOB_RECURSE HEADERS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*py)
FOREACH(header ${HEADERS_FILES})
	INSTALL(FILES ${header} DESTINATION share/jderobot/python/cameraview_yolo_py/ COMPONENT tools)
ENDFOREACH(header)

#not needed
# Install gui
#INSTALL (DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/gui DESTINATION share/jderobot/python/cameraview_yolo_py COMPONENT tools PATTERN .svn EXCLUDE)

# Install resources
#INSTALL (DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/resources DESTINATION share/jderobot/python/cameraview_yolo_py COMPONENT tools PATTERN .svn EXCLUDE)
INSTALL (FILES ${CMAKE_CURRENT_SOURCE_DIR}/cameraview_yolo_py.cfg DESTINATION ${CMAKE_INSTALL_PREFIX}/share/jderobot/conf)
