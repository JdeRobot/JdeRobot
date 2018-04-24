include_directories(
	${GAZEBO_INCLUDE_DIRS}
    ${INTERFACES_CPP_DIR}
    ${LIBS_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

link_directories(
    ${GAZEBO_LIBRARY_DIRS}
)
#boost_system 
add_library(carplugin SHARED carplugin.cc)
target_link_libraries(carplugin 
	${GAZEBO_libraries} 
)


INSTALL (TARGETS carplugin DESTINATION share/jderobot/gazebo/plugins/dummycar/ COMPONENT gazebo)
FILE(GLOB_RECURSE CFG_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cfg)
INSTALL (FILES ${CFG_FILES} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/jderobot/conf/ COMPONENT gazebo)





