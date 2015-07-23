include_directories(${GAZEBO_INCLUDE_DIRS}
		    ${Boost_INCLUDE_DIR})

link_directories(${GAZEBO_LIBRARY_DIRS}
		 ${Boost_LIBRARIES})

add_library(carplugin SHARED carplugin.cc)
target_link_libraries(carplugin 
		${GAZEBO_LIBRARIES} 
		${Boost_LIBRARIES})
INSTALL(TARGETS carplugin DESTINATION share/jderobot/gazebo/plugins/car/)
