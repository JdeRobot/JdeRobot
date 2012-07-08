# CHECK OPENNI
FIND_LIBRARY(with_openni
    NAMES OpenNI
    PATHS /usr/lib /usr/local/lib)
IF(NOT with_openni)
    MESSAGE ("*** OpenNi libraries not found. Are needed to openniServer component")
endif()

include_directories(/usr/include/openni)



