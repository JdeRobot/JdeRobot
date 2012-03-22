



#include_directories(/usr/include/gearbox)
add_library (jderobotutil STATIC jderobotutil.h observer.cpp observer.h paramdict.cpp paramdict.h time.cpp time.h uncopyable.h)

add_library (jderobotutilshare SHARED jderobotutil.h observer.cpp observer.h paramdict.cpp paramdict.h time.cpp time.h uncopyable.h)

set_target_properties(jderobotutilshare PROPERTIES OUTPUT_NAME jderobotutil)


#TARGET_LINK_LIBRARIES(jderobotice ${ice_LIBRARY_DIRS})
#TARGET_LINK_LIBRARIES(/usr/lib/gearbox/libGbxUtilAcfr.so)
#TARGET_LINK_LIBRARIES(/usr/lib/gearbox/libGbxIceUtilAcfr.so)

