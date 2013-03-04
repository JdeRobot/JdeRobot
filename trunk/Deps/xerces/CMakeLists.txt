FIND_PATH( xerces_INCLUDE_DIR NAMES libxml/parser.h  PATHS ENV C++LIB ENV PATH PATH_SUFFIXES libxml2)

IF( xerces_INCLUDE_DIR )
    FIND_LIBRARY( xerces_LIBRARIES NAMES xml2  PATHS ENV C++LIB ENV PATH PATH_SUFFIXES /usr/lib)
    IF( xerces_LIBRARIES )
		MESSAGE ("*** Xerces ok")
		MESSAGE ("${xerces_INCLUDE_DIR}")
		include_directories(${xerces_INCLUDE_DIR})
		link_directories(${xerces_LIBRARIES})
    ENDIF( xerces_LIBRARIES )
ENDIF(xerces_INCLUDE_DIR)

IF(NOT xerces_LIBRARIES)
		MESSAGE ("*** *** Xerces libraries: libxerces-c not found")
ENDIF()

