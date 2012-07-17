FIND_PATH(ice_path Ice)

IF(NOT ice_path)
    MESSAGE (FATAL_ERROR "*** Ice library not found")
ENDIF()
IF(ice_path)
    MESSAGE ("*** ICE OK")
    SET(slice_path /usr/share/slice)
    include_directories( ${ice_path})
ENDIF()

