# CHECK GEARBOX
FIND_LIBRARY(with_xerces
    NAMES xerces-c
    PATHS /usr/lib )
IF(NOT with_xerces)
    MESSAGE (FATAL_ERROR "*** Xerces libraries: libxerces-c not found, it needed in calibrator component")     
ENDIF()

IF(NOT with_xerces)
    MESSAGE ("*** Xerces OK")     
ENDIF()




include_directories( /usr/include/xercesc )
link_directories( /usr/lib/ )





