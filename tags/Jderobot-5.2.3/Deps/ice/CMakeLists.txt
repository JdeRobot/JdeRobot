SET(CMAKE_MODULE_PATH "${DEPS_DIR}/ice")

SET(slice_path /usr/share/slice)
FIND_PACKAGE(ZeroCIceUtil)
FIND_PACKAGE(ZeroCIceStorm)
FIND_PACKAGE(ZeroCIce)
	IF( ZeroCIceStorm_LIBRARY )
		IF( ZeroCIceUtil_LIBRARY )
			MESSAGE("*** ICE ok")
			include_directories(${ZeroCIceUtil_INCLUDE_DIR})
			link_directories(${ZeroCIceUtil_LIBRARY})
			include_directories(${ZeroCIceCore_INCLUDE_DIR})
			link_directories(${ZeroCIceCore_LIBRARY})
			include_directories(${ZeroCIceStorm_INCLUDE_DIR})
			link_directories(${ZeroCIceStorm_LIBRARY})
			include_directories(${ZeroCIce_INCLUDE_DIR})
			link_directories(${ZeroCIce_LIBRARIES})

			SET(DEPS "${DEPS} libzeroc-ice34, libiceutil34, libicestorm34,")

		ENDIF( ZeroCIceUtil_LIBRARY )
	ENDIF( ZeroCIceStorm_LIBRARY )

IF(NOT  ZeroCIceStorm_LIBRARY )
	MESSAGE(FATAL_ERROR "*** IceStorm not found")
ENDIF()

IF(NOT  ZeroCIceUtil_LIBRARY )
	MESSAGE(FATAL_ERROR "*** IceUtil not found")
ENDIF()

