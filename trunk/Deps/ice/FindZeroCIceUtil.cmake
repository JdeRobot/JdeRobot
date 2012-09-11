# Find the ZeroC ICEUtil includes and libraries

#
# ZeroCIceUtil_INCLUDE_DIR
# ZeroCIceUtil_LIBRARIES
# ZerocCIceCore_FOUND


#
# Copyright (c) 2007, Pau Garcia i Quiles, <pgquiles@elpauer.org>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

FIND_PATH( ZeroCIceUtil_INCLUDE_DIR NAMES IceUtil/IceUtil.h PATHS ENV C++LIB ENV PATH PATH_SUFFIXES include Ice Ice/include )

IF( ZeroCIceUtil_INCLUDE_DIR )
	FIND_LIBRARY( ZeroCIceUtil_LIBRARY NAMES IceUtil PATHS ENV C++LIB ENV PATH PATH_SUFFIXES Ice lib-release lib_release )

	IF( ZeroCIceUtil_LIBRARY )
		SET( ZeroCIceUtil_FOUND TRUE )
	ENDIF( ZeroCIceUtil_LIBRARY )

	IF(ZeroCIceUtil_FOUND)
		IF (NOT ZeroCIceUtil_FIND_QUIETLY)
			MESSAGE(STATUS "Found the ZeroC IceUtil library at ${ZeroCIceUtil_LIBRARY}")
			MESSAGE(STATUS "Found the ZeroC IceUtil headers at ${ZeroCIceUtil_INCLUDE_DIR}")
		ENDIF (NOT ZeroCIceUtil_FIND_QUIETLY)
	ELSE(ZeroCIceUtil_FOUND)
		IF(ZeroCIceUtil_FIND_REQUIRED)
			MESSAGE(FATAL_ERROR "Could NOT find ZeroC IceUtil")
		ENDIF(ZeroCIceUtil_FIND_REQUIRED)
	ENDIF(ZeroCIceUtil_FOUND)

ENDIF( ZeroCIceUtil_INCLUDE_DIR )
