# Find the ZeroC ICESSL includes and libraries

#
# ZeroCIceSSL_INCLUDE_DIR
# ZeroCIceSSL_LIBRARIES
# ZerocCIceCore_FOUND


#
# Copyright (c) 2007, Pau Garcia i Quiles, <pgquiles@elpauer.org>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

FIND_PATH( ZeroCIceSSL_INCLUDE_DIR NAMES IceSSL/Plugin.h PATHS ENV C++LIB ENV PATH PATH_SUFFIXES include Ice Ice/include )

IF( ZeroCIceSSL_INCLUDE_DIR )
	FIND_LIBRARY( ZeroCIceSSL_LIBRARY NAMES IceSSL PATHS ENV C++LIB ENV PATH PATH_SUFFIXES Ice lib-release lib_release )

	IF( ZeroCIceSSL_LIBRARY )
		SET( ZeroCIceSSL_FOUND TRUE )
	ENDIF( ZeroCIceSSL_LIBRARY )

	IF(ZeroCIceSSL_FOUND)
		IF (NOT ZeroCIceSSL_FIND_QUIETLY)
			MESSAGE(STATUS "Found the ZeroC IceSSL library at ${ZeroCIceSSL_LIBRARY}")
			MESSAGE(STATUS "Found the ZeroC IceSSL headers at ${ZeroCIceSSL_INCLUDE_DIR}")
		ENDIF (NOT ZeroCIceSSL_FIND_QUIETLY)
	ELSE(ZeroCIceSSL_FOUND)
		IF(ZeroCIceSSL_FIND_REQUIRED)
			MESSAGE(FATAL_ERROR "Could NOT find the ZeroC Ice cores")
		ENDIF(ZeroCIceSSL_FIND_REQUIRED)
	ENDIF(ZeroCIceSSL_FOUND)

ENDIF( ZeroCIceSSL_INCLUDE_DIR )
