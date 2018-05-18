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
add_library(wall1plugin SHARED wall1.cc)
target_link_libraries(wall1plugin 
	${GAZEBO_libraries} 
)

add_library(wall2plugin SHARED wall2.cc)
target_link_libraries(wall2plugin 
	${GAZEBO_libraries} 
)

add_library(wall3plugin SHARED wall3.cc)
target_link_libraries(wall3plugin 
	${GAZEBO_libraries} 
)

add_library(wall4plugin SHARED wall4.cc)
target_link_libraries(wall4plugin 
	${GAZEBO_libraries} 
)

INSTALL (TARGETS wall1plugin wall2plugin wall3plugin wall4plugin DESTINATION share/jderobot/gazebo/plugins/gymkhana/ COMPONENT gazebo)
FILE(GLOB_RECURSE CFG_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cfg)
INSTALL (FILES ${CFG_FILES} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/jderobot/conf/ COMPONENT gazebo)





