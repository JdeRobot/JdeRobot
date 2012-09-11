# Find the ZeroC ICEPatch2 includes and libraries

#
# ZeroCIcePatch2_INCLUDE_DIR
# ZeroCIcePatch2_LIBRARIES
# ZerocCIceCore_FOUND


#
# Copyright (c) 2007, Pau Garcia i Quiles, <pgquiles@elpauer.org>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

FIND_PATH( ZeroCIcePatch2_INCLUDE_DIR NAMES IcePatch2/ClientUtil.h PATHS ENV C++LIB ENV PATH PATH_SUFFIXES include Ice Ice/include )

IF( ZeroCIcePatch2_INCLUDE_DIR )
	FIND_LIBRARY( ZeroCIcePatch2_LIBRARY NAMES IcePatch2 PATHS ENV C++LIB ENV PATH PATH_SUFFIXES Ice lib-release lib_release )

	IF( ZeroCIcePatch2_LIBRARY )
		SET( ZeroCIcePatch2_FOUND TRUE )
	ENDIF( ZeroCIcePatch2_LIBRARY )

	IF(ZeroCIcePatch2_FOUND)
		IF (NOT ZeroCIcePatch2_FIND_QUIETLY)
			MESSAGE(STATUS "Found the ZeroC IcePatch2 library at ${ZeroCIcePatch2_LIBRARY}")
			MESSAGE(STATUS "Found the ZeroC IcePatch2 headers at ${ZeroCIcePatch2_INCLUDE_DIR}")
		ENDIF (NOT ZeroCIcePatch2_FIND_QUIETLY)
	ELSE(ZeroCIcePatch2_FOUND)
		IF(ZeroCIcePatch2_FIND_REQUIRED)
			MESSAGE(FATAL_ERROR "Could NOT find ZeroC IcePatch2")
		ENDIF(ZeroCIcePatch2_FIND_REQUIRED)
	ENDIF(ZeroCIcePatch2_FOUND)

ENDIF( ZeroCIcePatch2_INCLUDE_DIR )
