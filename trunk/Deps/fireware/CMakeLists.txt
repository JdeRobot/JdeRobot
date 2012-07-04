# CHECK FIREWARE 
FIND_LIBRARY(with_fireware
    NAMES  dc1394 raw1394
    PATHS /usr/lib)
IF(NOT with_fireware)
    MESSAGE ("*** Fireware libraries: dc1394 or raw1394 not found") 
ENDIF()

include_directories(/usr/include)
link_directories(/usr/lib)