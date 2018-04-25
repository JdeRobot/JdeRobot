
INSTALL(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/jderobot_comm
        DESTINATION ${CMAKE_INSTALL_PREFIX}/share/visualstates_py/examples COMPONENT visualStates-python)

# install cpp sources for ros node generation
INSTALL(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ros
        DESTINATION ${CMAKE_INSTALL_PREFIX}/share/visualstates_py/examples COMPONENT visualStates-python)