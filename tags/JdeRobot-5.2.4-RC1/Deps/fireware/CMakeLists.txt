# CHECK FIREWARE 

FIND_PATH( raw_INCLUDE_DIR NAMES raw1394.h  PATHS ENV C++LIB ENV PATH PATH_SUFFIXES libraw1394)
FIND_PATH(dc_INCLUDE_DIR  NAMES dc1394.h  PATHS ENV C++LIB ENV PATH PATH_SUFFIXES dc1394)

FIND_LIBRARY( with_fireware NAMES dc1394 raw1394 PATHS ENV C++LIB ENV PATH PATH_SUFFIXES lib lib64 /usr/lib)

IF(NOT with_fireware)
    MESSAGE ("*** Fireware libraries: dc1394 or raw1394 not found") 
ENDIF()

IF(with_fireware)
    MESSAGE ("*** Fireware ok") 
    include_directories(/usr/include)
    link_directories(/usr/lib)
    SET(DEPS "${DEPS} libdc1394-22,")
ENDIF(with_fireware)



