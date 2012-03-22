include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library (fuzzylib STATIC fuzzylib.c)

add_library (fuzzyshare SHARED fuzzylib.c)

set_target_properties(fuzzyshare PROPERTIES OUTPUT_NAME fuzzylib)


#TARGET_LINK_LIBRARIES(jderobotice ${ice_LIBRARY_DIRS})
#TARGET_LINK_LIBRARIES(/usr/lib/gearbox/libGbxUtilAcfr.so)
#TARGET_LINK_LIBRARIES(/usr/lib/gearbox/libGbxIceUtilAcfr.so)