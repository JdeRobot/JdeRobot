
list_subdirectories( LIST_COMPONENTS ${CMAKE_CURRENT_SOURCE_DIR} 1)

FOREACH (component ${LIST_COMPONENTS})
	#MESSAGE(${component})
	ADD_SUBDIRECTORY (${component})
ENDFOREACH()
