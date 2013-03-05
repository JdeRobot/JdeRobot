# CHECK NITE
FIND_PATH( nite_INCLUDE_DIR NAMES XnVCircleDetector.h  PATHS ENV C++LIB ENV PATH PATH_SUFFIXES include/nite/)
 
IF( nite_INCLUDE_DIR )
	FIND_LIBRARY(nite_LIBRARIES NAMES XnVNite_1_5_2 XnVCNITE_1_5_2 PATHS /usr/lib/ /usr/local/lib)
    IF( nite_LIBRARIES)
		MESSAGE ("-- Nite found at ${nite_LIBRARIES}")
		include_directories(${nite_INCLUDE_DIR})
		link_directories(${nite_LIBRARIES})
    ENDIF( nite_LIBRARIES )
ENDIF(nite_INCLUDE_DIR)

IF(NOT nite_LIBRARIES)
		MESSAGE ("*** Nite not found")
ENDIF()



