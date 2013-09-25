list_subdirectories( LIST_COMPONENTS ${CMAKE_CURRENT_SOURCE_DIR} 1)

IF(NOT DEFINED build-default)
	SET(build-default ON)
ENDIF(NOT DEFINED build-default)

FOREACH (component ${LIST_COMPONENTS})
	SET(build_${component} ${build-default} CACHE BOOL "Build component ${component}")
	IF(build_${component})
		MESSAGE(STATUS "Processing component ${component}")
	        ADD_SUBDIRECTORY (${component})
	ENDIF(build_${component})
ENDFOREACH()

