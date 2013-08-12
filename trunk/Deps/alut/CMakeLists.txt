include(FindPkgConfig)
FIND_PATH( alut_INCLUDE_DIR NAMES alut.h  PATHS ENV C++LIB ENV PATH PATH_SUFFIXES AL)

IF( alut_INCLUDE_DIR )
    FIND_LIBRARY( alut_LIBRARIES NAMES alut PATHS ENV C++LIB ENV PATH PATH_SUFFIXES lib lib64 )
    IF( alut_LIBRARIES )
                MESSAGE ("-- ALUT found at ${alut_LIBRARIES}")
                include_directories(${alut_INCLUDE_DIR})
                link_directories(${alut_LIBRARIES})
    ENDIF( alut_LIBRARIES )
ENDIF(alut_INCLUDE_DIR)

IF(NOT alut_LIBRARIES)
                MESSAGE ("*** ALUT not found")
ENDIF()

