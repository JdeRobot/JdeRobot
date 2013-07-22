include(FindPkgConfig)
find_package(GLUT)
include_directories(${GLUT_INCLUDE_DIRS})
link_directories(${GLUT_LIBRARY_DIRS})

IF (GLUT_LIBRARIES) 
	MESSAGE("OpenGL libraries found at ${GLUT_LIBRARIES}")
ELSE()
	MESSAGE ("*** OpenGL not found")
ENDIF()

