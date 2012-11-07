# CHECK NITE
FIND_PATH( ni_INCLUDE_DIR NAMES XnOS.h  PATHS ENV C++LIB ENV PATH PATH_SUFFIXES include/ni/)
FIND_PATH( nite_INCLUDE_DIR NAMES XnVCircleDetector.h  PATHS ENV C++LIB ENV PATH PATH_SUFFIXES include/nite/)
MESSAGE(NI: ${ni_INCLUDE_DIR})
MESSAGE(NITE: ${nite_INCLUDE_DIR})


FIND_LIBRARY(with_nite
    NAMES XnVNite_1_5_2 XnVCNITE_1_5_2
    PATHS /usr/lib/ /usr/local/lib)

IF(NOT with_gearbox)
    MESSAGE ( "*** Nite libraries: XnVNite or XnVCNITE not found")
ENDIF()
IF(with_nite)
    MESSAGE ("*** Nite OK")
    include_directories( /usr/include)
include_directories(/usr/local/include)
link_directories(/usr/local/lib )
link_directories( /usr/lib )

ENDIF()




