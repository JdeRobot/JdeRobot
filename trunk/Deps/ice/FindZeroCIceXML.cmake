# Find the ZeroC ICEXML includes and libraries

#
# ZeroCIceXML_INCLUDE_DIR
# ZeroCIceXML_LIBRARIES
# ZerocCIceCore_FOUND


#
# Copyright (c) 2007, Pau Garcia i Quiles, <pgquiles@elpauer.org>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

FIND_PATH( ZeroCIceXML_INCLUDE_DIR NAMES IceXML/Parser.h PATHS ENV C++LIB ENV PATH PATH_SUFFIXES include Ice Ice/include )

IF( ZeroCIceXML_INCLUDE_DIR )
	FIND_LIBRARY( ZeroCIceXML_LIBRARY NAMES IceXML PATHS ENV C++LIB ENV PATH PATH_SUFFIXES Ice lib-release lib_release )

	IF( ZeroCIceXML_LIBRARY )
		SET( ZeroCIceXML_FOUND TRUE )
	ENDIF( ZeroCIceXML_LIBRARY )

	IF(ZeroCIceXML_FOUND)
		IF (NOT ZeroCIceXML_FIND_QUIETLY)
			MESSAGE(STATUS "Found the ZeroC IceXML library at ${ZeroCIceXML_LIBRARY}")
			MESSAGE(STATUS "Found the ZeroC IceXML headers at ${ZeroCIceXML_INCLUDE_DIR}")
		ENDIF (NOT ZeroCIceXML_FIND_QUIETLY)
	ELSE(ZeroCIceXML_FOUND)
		IF(ZeroCIceXML_FIND_REQUIRED)
			MESSAGE(FATAL_ERROR "Could NOT find ZeroC IceXML")
		ENDIF(ZeroCIceXML_FIND_REQUIRED)
	ENDIF(ZeroCIceXML_FOUND)

ENDIF( ZeroCIceXML_INCLUDE_DIR )
