list_subdirectories( LIST_DRIVERS ${CMAKE_CURRENT_SOURCE_DIR} 1)

IF(NOT DEFINED build-default)
	SET(build-default ON)
ENDIF(NOT DEFINED build-default)

FOREACH (driver ${LIST_DRIVERS})
	if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${driver}/CMakeLists.txt)
	SET(build_${driver} ${build-default} CACHE BOOL "Build driver ${driver}")
	IF(build_${driver})
		MESSAGE(STATUS "Processing driver ${driver}")
	        ADD_SUBDIRECTORY (${driver})
	ENDIF(build_${driver})
	endif(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${driver}/CMakeLists.txt)
ENDFOREACH()

