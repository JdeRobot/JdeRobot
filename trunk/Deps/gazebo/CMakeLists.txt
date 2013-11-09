PKG_CHECK_MODULES(GAZEBO QUIET gazebo<=2.0.0 )
include_directories(${GAZEBO_INCLUDE_DIRS})
link_directories(${GAZEBO_LIBRARY_DIRS})



#PKG_CHECK_MODULES(GAZEBO QUIET gazebo>=2.0.0 )
#include_directories(${GAZEBO_INCLUDE_DIRS})
#link_directories(${GAZEBO_LIBRARY_DIRS})

#foreach(dir ${GAZEBO_INCLUDE_DIRS})
#	if ( dir  MATCHES "gazebo" )
#		include_directories(${dir}/gazebo)
#	endif()
#endforeach()

if (GAZEBO_INCLUDE_DIRS)
	MESSAGE("*** GAZEBO LIBRARIES FOUND")
else()
	MESSAGE("*** GAZEBO LIBRARIES NOT FOUND")
endif()

