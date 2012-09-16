# CHECK GEARBOX
#FIND_LIBRARY(with_gearbox
#    NAMES GbxGarminAcfr GbxIceUtilAcfr
#    PATHS /usr/lib/gearbox /usr/local/lib/gearbox)
#IF(NOT with_gearbox)
#    MESSAGE (FATAL_ERROR "*** Gearbox libraries: GbxUtilAcfr or GbxIceUtilAcfr
#not found")
#ENDIF()
#IF(with_gearbox)
#    MESSAGE ("*** Gearbox OK")
#    include_directories( /usr/include/gearbox)
#include_directories(/usr/local/include/gearbox)
#link_directories(/usr/local/lib/gearbox )
#link_directories( /usr/lib/gearbox )

#ENDIF()


FIND_PATH( Gearbox_INCLUDE_DIR NAMES gbxutilacfr/gbxutilacfr.h  PATHS ENV C++LIB ENV PATH PATH_SUFFIXES include/gearbox/)
MESSAGE("GEARBOX IN ${Gearbox_INCLUDE_DIR}")
IF( Gearbox_INCLUDE_DIR )
    FIND_LIBRARY( Gearbox_LIBRARY1 NAMES GbxUtilAcfr PATHS ENV C++LIB ENV PATH PATH_SUFFIXES lib/gearbox lib64/gearbox )
    FIND_LIBRARY( Gearbox_LIBRARY2 NAMES GbxIceUtilAcfr PATHS ENV C++LIB ENV PATH PATH_SUFFIXES lib/gearbox lib64/gearbox)
    SET (Gearbox_LIBRARIES ${Gearbox_LIBRARY1} ${Gearbox_LIBRARY2})
    IF( Gearbox_LIBRARIES )
	MESSAGE("GEARBOX LIBRARY FOUND IN ${Gearbox_LIBRARIES}")
    ENDIF( Gearbox_LIBRARIES )
ENDIF(Gearbox_INCLUDE_DIR)

include_directories(${Gearbox_INCLUDE_DIR})
