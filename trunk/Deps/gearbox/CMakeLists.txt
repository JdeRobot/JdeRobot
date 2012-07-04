# CHECK GEARBOX
FIND_LIBRARY(with_gearbox
    NAMES GbxGarminAcfr GbxIceUtilAcfr
    PATHS /usr/lib/gearbox /usr/local/lib/gearbox)
IF(NOT with_gearbox)
    MESSAGE (FATAL_ERROR "*** Gearbox libraries: GbxUtilAcfr or GbxIceUtilAcfr not found") 
ENDIF()



include_directories( /usr/include/gearbox /usr/local/include/gearbox)
link_directories( /usr/lib/gearbox /usr/local/lib/gearbox )





