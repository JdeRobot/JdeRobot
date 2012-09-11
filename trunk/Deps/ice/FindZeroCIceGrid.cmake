# Find the ZeroC ICEGrid includes and libraries

#
# ZeroCIceGrid_INCLUDE_DIR
# ZeroCIceGrid_LIBRARIES
# ZerocCIceCore_FOUND


#
# Copyright (c) 2007, Pau Garcia i Quiles, <pgquiles@elpauer.org>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

FIND_PATH( ZeroCIceGrid_INCLUDE_DIR NAMES IceGrid/UserAccountMapper.h PATHS ENV C++LIB ENV PATH PATH_SUFFIXES include Ice Ice/include )

IF( ZeroCIceGrid_INCLUDE_DIR )
	FIND_LIBRARY( ZeroCIceGrid_LIBRARY NAMES IceGrid PATHS ENV C++LIB ENV PATH PATH_SUFFIXES Ice lib-release lib_release )

	IF( ZeroCIceGrid_LIBRARY )
		SET( ZeroCIceGrid_FOUND TRUE )
	ENDIF( ZeroCIceGrid_LIBRARY )

	IF(ZeroCIceGrid_FOUND)
		IF (NOT ZeroCIceGrid_FIND_QUIETLY)
			MESSAGE(STATUS "Found the ZeroC IceGrid library at ${ZeroCIceGrid_LIBRARY}")
			MESSAGE(STATUS "Found the ZeroC IceGrid headers at ${ZeroCIceGrid_INCLUDE_DIR}")
		ENDIF (NOT ZeroCIceGrid_FIND_QUIETLY)
	ELSE(ZeroCIceGrid_FOUND)
		IF(ZeroCIceGrid_FIND_REQUIRED)
			MESSAGE(FATAL_ERROR "Could NOT find ZeroC IceGrid")
		ENDIF(ZeroCIceGrid_FIND_REQUIRED)
	ENDIF(ZeroCIceGrid_FOUND)

ENDIF( ZeroCIceGrid_INCLUDE_DIR )
