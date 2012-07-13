# CHECK GEARBOX
FIND_LIBRARY(with_gearbox
    NAMES GbxGarminAcfr GbxIceUtilAcfr
    PATHS /usr/lib/gearbox /usr/local/lib/gearbox)
IF(NOT with_gearbox)
    MESSAGE (FATAL_ERROR "*** Gearbox libraries: GbxUtilAcfr or GbxIceUtilAcfr not found")
ENDIF()
IF(with_gearbox)
    MESSAGE ("*** Gearbox OK")
    include_directories( /usr/include/gearbox)
include_directories(/usr/local/include/gearbox)
link_directories(/usr/local/lib/gearbox )
link_directories( /usr/lib/gearbox )

ENDIF()








