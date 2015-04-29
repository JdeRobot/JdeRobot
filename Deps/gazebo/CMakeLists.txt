PKG_CHECK_MODULES(GAZEBO QUIET gazebo<=5.1.0 )
#include_directories(${GAZEBO_INCLUDE_DIRS})
link_directories(${GAZEBO_LIBRARY_DIRS})


if (GAZEBO_INCLUDE_DIRS)

	FOREACH(gazeboInc ${GAZEBO_INCLUDE_DIRS})
        	include_directories(${gazeboInc}/gazebo/)
	        include_directories(${gazeboInc})
	ENDFOREACH(gazeboInc)
	MESSAGE("*** GAZEBO LIBRARIES FOUND: ${GAZEBO_INCLUDE_DIRS} - ${OGRE_INCLUDE_DIRS}")
	SET(DEPS "${DEPS} gazebo,")
else()
	MESSAGE("*** GAZEBO LIBRARIES NOT FOUND")
endif()

