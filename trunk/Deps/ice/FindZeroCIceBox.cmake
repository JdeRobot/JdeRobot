# Find the ZeroC ICEBox includes and libraries

#
# ZeroCIceBox_INCLUDE_DIR
# ZeroCIceBox_LIBRARIES
# ZeroCIceBox_FOUND


#
# Copyright (c) 2007, Pau Garcia i Quiles, <pgquiles@elpauer.org>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

FIND_PATH( ZeroCIceBox_INCLUDE_DIR NAMES IceBox/IceBox.h PATHS ENV C++LIB ENV PATH PATH_SUFFIXES include Ice Ice/include )

IF( ZeroCIceBox_INCLUDE_DIR )
	FIND_LIBRARY( ZeroCIceBox_LIBRARY NAMES IceBox PATHS ENV C++LIB ENV PATH PATH_SUFFIXES Ice lib-release lib_release )

	IF( ZeroCIceBox_LIBRARY )
		SET( ZeroCIceBox_FOUND TRUE )
	ENDIF( ZeroCIceBox_LIBRARY )

	IF(ZeroCIceBox_FOUND)
		IF (NOT ZeroCIceBox_FIND_QUIETLY)
			MESSAGE(STATUS "Found the ZeroC IceBox library at ${ZeroCIceBox_LIBRARY}")
			MESSAGE(STATUS "Found the ZeroC IceBox headers at ${ZeroCIceBox_INCLUDE_DIR}")
		ENDIF (NOT ZeroCIceBox_FIND_QUIETLY)
	ELSE(ZeroCIceBox_FOUND)
		IF(ZeroCIceBox_FIND_REQUIRED)
			MESSAGE(FATAL_ERROR "Could NOT find ZeroC IceBox")
		ENDIF(ZeroCIceBox_FIND_REQUIRED)
	ENDIF(ZeroCIceBox_FOUND)

ENDIF( ZeroCIceBox_INCLUDE_DIR )
