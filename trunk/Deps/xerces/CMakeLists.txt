# CHECK GEARBOX
FIND_LIBRARY(with_xerces
    NAMES xerces-c
    PATHS /usr/lib )
IF(NOT with_xerces)
    MESSAGE (FATAL_ERROR "*** Xerces libraries: libxerces-c not found, it needed in calibrator component") 
    MESSAGE (FATAL_ERROR "*** Xerces libraries: Please, insert the command: 'sudo apt-get install libxerces-c-dev' to install it") 
ENDIF()



include_directories( /usr/include/xercesc )
link_directories( /usr/lib/ )





