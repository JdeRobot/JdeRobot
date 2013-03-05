# CHECK OPENNI
FIND_PATH( openni_INCLUDE_DIR NAMES XnOpenNI.h  PATHS ENV C++LIB ENV PATH PATH_SUFFIXES ni)
 
IF( openni_INCLUDE_DIR )
    FIND_LIBRARY( openni_LIBRARIES NAMES OpenNI PATHS ENV C++LIB ENV PATH PATH_SUFFIXES lib lib64)
    IF( openni_LIBRARIES)
		MESSAGE ("-- OpenNi found at ${openni_LIBRARIES}")
		include_directories(${openni_INCLUDE_DIR})
		link_directories(${openni_LIBRARIES})
    ENDIF( openni_LIBRARIES )
ENDIF(openni_INCLUDE_DIR)

IF(NOT openni_LIBRARIES)
		MESSAGE ("*** OpenNi not found")
ENDIF()


