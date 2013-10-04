list_subdirectories( LIST_PLUGINS ${CMAKE_CURRENT_SOURCE_DIR} 1)

IF(NOT DEFINED build-default)
	SET(build-default ON)
ENDIF(NOT DEFINED build-default)

FOREACH (plugin ${LIST_PLUGINS})
	SET(build_${plugin} ${build-default} CACHE BOOL "Build plugin ${plugin}")
	IF(build_${plugin})
		MESSAGE(STATUS "Processing plugin ${plugin}")
	        ADD_SUBDIRECTORY (${plugin})
	ENDIF(build_${plugin})
ENDFOREACH()
