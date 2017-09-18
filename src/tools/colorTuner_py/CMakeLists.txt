
## INSTALL ##

# Install .py
#FILE(GLOB_RECURSE HEADERS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*py)
#FOREACH(header ${HEADERS_FILES})
#	INSTALL(FILES ${header} DESTINATION share/jderobot/python/colorTuner_py/ COMPONENT tools)
#ENDFOREACH(header)

INSTALL(FILES colorTuner.py DESTINATION share/jderobot/python/colorTuner_py/ COMPONENT tools)

# Install gui
INSTALL (DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/gui DESTINATION share/jderobot/python/colorTuner_py COMPONENT tools PATTERN .svn EXCLUDE)

# Install sensors
INSTALL (DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/sensors DESTINATION share/jderobot/python/colorTuner_py COMPONENT tools PATTERN .svn EXCLUDE)

# Install Filters
INSTALL (DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/filters DESTINATION share/jderobot/python/colorTuner_py COMPONENT tools PATTERN .svn EXCLUDE)

INSTALL (FILES ${CMAKE_CURRENT_SOURCE_DIR}/colorTuner_py.cfg DESTINATION ${CMAKE_INSTALL_PREFIX}/share/jderobot/conf)

#install the dark theme style sheet
INSTALL(FILES QDarkStyleSheet/qdarkstyle/pyqt5_style_rc.py DESTINATION /usr/local/lib/python2.7/dist-packages/qdarkstyle/ COMPONENT tools)
INSTALL(FILES QDarkStyleSheet/qdarkstyle/__init__.py DESTINATION /usr/local/lib/python2.7/dist-packages/qdarkstyle/ COMPONENT tools)
