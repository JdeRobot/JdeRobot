# CHECK OPENNI
FIND_LIBRARY(with_openni
    NAMES OpenNI
    PATHS /usr/lib /usr/local/lib)


IF(NOT with_openni)
    MESSAGE ("*** OpenNi libraries not found. Are needed to openniServer component")
endif()
IF(with_openni)
	FIND_LIBRARY( OpenNi_LIBRARY NAMES OpenNI PATHS ENV C++LIB ENV PATH PATH_SUFFIXES lib lib64)
    MESSAGE ("*** OpenNi OK")
	MESSAGE ( ${OpenNi_LIBRARY})
endif()

include_directories(/usr/include/openni)



