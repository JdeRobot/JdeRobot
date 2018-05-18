configure_file(
    camviz_py.in
    camviz_py
    @ONLY
)

## INSTALL ##

# install Launcher
install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/camviz_py
    PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ WORLD_EXECUTE WORLD_READ
    DESTINATION bin
    COMPONENT camviz-python
)

# Install .py
FILE(GLOB_RECURSE HEADERS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*py)
FOREACH(header ${HEADERS_FILES})
	INSTALL(FILES ${header} DESTINATION share/jderobot/python/camViz_py/ COMPONENT camviz-python)
ENDFOREACH(header)

# Install resources
INSTALL (FILES ${CMAKE_CURRENT_SOURCE_DIR}/camviz_py.cfg DESTINATION ${CMAKE_INSTALL_PREFIX}/share/jderobot/conf COMPONENT camviz-python)
