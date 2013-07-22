FIND_PATH( Gearbox_INCLUDE_DIR NAMES gbxutilacfr/gbxutilacfr.h  PATHS ENV C++LIB ENV PATH PATH_SUFFIXES gearbox)
#FIND_PATH( Gearbox_INCLUDE_DIR NAMES noexiste.h  PATHS ENV C++LIB ENV PATH PATH_SUFFIXES gearbox)
 
IF( Gearbox_INCLUDE_DIR )
    FIND_LIBRARY( Gearbox_LIBRARY1 NAMES GbxUtilAcfr PATHS ENV C++LIB ENV PATH PATH_SUFFIXES lib/gearbox lib64/gearbox )
    FIND_LIBRARY( Gearbox_LIBRARY2 NAMES GbxIceUtilAcfr PATHS ENV C++LIB ENV PATH PATH_SUFFIXES lib/gearbox lib64/gearbox)
    SET (Gearbox_LIBRARIES ${Gearbox_LIBRARY1} ${Gearbox_LIBRARY2})
    IF( Gearbox_LIBRARIES )
		MESSAGE ("*** Gearbox ok")
		include_directories(${Gearbox_INCLUDE_DIR})
		link_directories(${Gearbox_LIBRARIES})
    ENDIF( Gearbox_LIBRARIES )
ELSE()
ENDIF(Gearbox_INCLUDE_DIR)

IF(NOT Gearbox_LIBRARIES)
		MESSAGE ("*** Gearbox not found")
ENDIF()

