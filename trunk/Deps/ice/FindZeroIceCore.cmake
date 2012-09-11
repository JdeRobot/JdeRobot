# Find the ZeroC ICE essential includes and libraries

#
# ZeroCIceCore_INCLUDE_DIR
# ZeroCIceCore_LIBRARIES
# ZeroCIceCore_FOUND


#
# Copyright (c) 2007, Pau Garcia i Quiles, <pgquiles@elpauer.org>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

FIND_PATH( ZeroCIceCore_INCLUDE_DIR NAMES Ice/Ice.h PATHS ENV C++LIB ENV PATH PATH_SUFFIXES include Ice Ice/include )

IF( ZeroCIceCore_INCLUDE_DIR )
	FIND_LIBRARY( ZeroCIceCore_LIBRARY NAMES Ice PATHS ENV C++LIB ENV PATH PATH_SUFFIXES Ice lib-release lib_release )

	IF( ZeroCIceCore_LIBRARY )
		SET( ZeroCIceCore_FOUND TRUE )
	ENDIF( ZeroCIceCore_LIBRARY )

	IF(ZeroCIceCore_FOUND)
		IF (NOT ZeroCIceCore_FIND_QUIETLY)
			MESSAGE(STATUS "Found the core ZeroC Ice library at ${ZeroCIceCore_LIBRARY}")
			MESSAGE(STATUS "Found the core ZeroC Ice headers at ${ZeroCIceCore_INCLUDE_DIR}")
		ENDIF (NOT ZeroCIceCore_FIND_QUIETLY)
	ELSE(ZeroCIceCore_FOUND)
		IF(ZeroCIceCore_FIND_REQUIRED)
			MESSAGE(FATAL_ERROR "Could NOT find the ZeroC Ice cores")
		ENDIF(ZeroCIceCore_FIND_REQUIRED)
	ENDIF(ZeroCIceCore_FOUND)

ENDIF( ZeroCIceCore_INCLUDE_DIR )
