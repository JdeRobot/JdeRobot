SET(CMAKE_MODULE_PATH "${DEPS_DIR}/ice")
#MESSAGE("${DEPS_DIR}/")
SET(slice_path /usr/share/slice)
FIND_PACKAGE(ZeroCIceUtil)
FIND_PACKAGE(ZeroCIceStorm)
FIND_PACKAGE(ZeroCIce)
include_directories(${ZeroCIceUtil_INCLUDE_DIR})
link_directories(${ZeroCIceUtil_LIBRARY})
include_directories(${ZeroCIceCore_INCLUDE_DIR})
link_directories(${ZeroCIceCore_LIBRARY})
include_directories(${ZeroCIceStorm_INCLUDE_DIR})
link_directories(${ZeroCIceStorm_LIBRARY})
include_directories(${ZeroCIce_INCLUDE_DIR})
link_directories(${ZeroCIce_LIBRARIES})






