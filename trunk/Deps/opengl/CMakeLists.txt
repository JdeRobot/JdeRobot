include(FindPkgConfig)
find_package(GLUT)
find_package(OpenGL REQUIRED)
include_directories(${GLUT_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIRS})

link_directories(${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )


IF (GLUT_LIBRARIES) 
	MESSAGE("OpenGL libraries found at ${GLUT_LIBRARY}")
ELSE()
	MESSAGE ("*** OpenGL not found")
ENDIF()

