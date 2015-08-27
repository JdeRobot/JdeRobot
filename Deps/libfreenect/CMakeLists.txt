FIND_PACKAGE(Freenect)

IF(Freenect_FOUND)
MESSAGE(STATUS "  With Freenect = YES")
ELSE()
MESSAGE(STATUS "  With Freenect = NO (libfreenect not found)")
ENDIF()

