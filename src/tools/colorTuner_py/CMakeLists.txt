configure_file(
    colorTuner.in
    colorTuner
    @ONLY
)
## INSTALL ##

install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/colorTuner
    PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ WORLD_EXECUTE WORLD_READ
    DESTINATION bin
    COMPONENT colortuner-python
)


INSTALL(FILES colorTuner.py DESTINATION share/jderobot/python/colorTuner_py/ COMPONENT colortuner-python)
INSTALL(FILES resources_rc.py DESTINATION share/jderobot/python/colorTuner_py/ COMPONENT colortuner-python)

INSTALL(FILES resources/HLSColorSpace.png DESTINATION share/jderobot/python/colorTuner_py/resources COMPONENT colortuner-python)

# Install gui
INSTALL (DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/gui DESTINATION share/jderobot/python/colorTuner_py COMPONENT colortuner-python PATTERN .svn EXCLUDE)

# Install sensors
INSTALL (DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/sensors DESTINATION share/jderobot/python/colorTuner_py COMPONENT colortuner-python PATTERN .svn EXCLUDE)

# Install Filters
INSTALL (DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/filters DESTINATION share/jderobot/python/colorTuner_py COMPONENT colortuner-python PATTERN .svn EXCLUDE)

INSTALL (FILES ${CMAKE_CURRENT_SOURCE_DIR}/colorTuner_py.yml DESTINATION ${CMAKE_INSTALL_PREFIX}/share/jderobot/conf COMPONENT colortuner-python)

#install the dark theme style sheet
INSTALL(FILES QDarkStyleSheet/qdarkstyle/pyqt5_style_rc.py DESTINATION ${JDEROBOT_PYTHON2_MODULE_PATH}/qdarkstyle/ COMPONENT colortuner-python)
INSTALL(FILES QDarkStyleSheet/qdarkstyle/__init__.py DESTINATION ${JDEROBOT_PYTHON2_MODULE_PATH}/qdarkstyle/ COMPONENT colortuner-python)
