# CHECK GEARBOX
#FIND_LIBRARY(with_gearbox
#    NAMES GbxGarminAcfr GbxIceUtilAcfr
#    PATHS /usr/lib/gearbox /usr/local/lib/gearbox)
#IF(NOT with_gearbox)
#    MESSAGE (FATAL_ERROR "*** Gearbox libraries: GbxUtilAcfr or GbxIceUtilAcfr
#not found")
#ENDIF()
#IF(with_gearbox)
#    MESSAGE ("*** Gearbox OK")
#    include_directories( /usr/include/gearbox)
#include_directories(/usr/local/include/gearbox)
#link_directories(/usr/local/lib/gearbox )
#link_directories( /usr/lib/gearbox )

#ENDIF()


FIND_PATH( OpenCV_INCLUDE_DIR NAMES cv.h  PATHS ENV C++LIB ENV PATH PATH_SUFFIXES include/opencv/)
MESSAGE("OpenCV IN ${OpenCV_INCLUDE_DIR}")
IF( OpenCV_INCLUDE_DIR )
    FIND_LIBRARY( OpenCV_LIBRARY1 NAMES opencv_core PATHS ENV C++LIB ENV PATH PATH_SUFFIXES lib lib64)
    FIND_LIBRARY( OpenCV_LIBRARY2 NAMES opencv_highgui PATHS ENV C++LIB ENV PATH PATH_SUFFIXES lib lib64)
    FIND_LIBRARY( OpenCV_LIBRARY3 NAMES opencv_contrib PATHS ENV C++LIB ENV PATH PATH_SUFFIXES lib lib64)
    FIND_LIBRARY( OpenCV_LIBRARY4 NAMES opencv_calib3d PATHS ENV C++LIB ENV PATH PATH_SUFFIXES lib lib64)
    FIND_LIBRARY( OpenCV_LIBRARY5 NAMES opencv_flann PATHS ENV C++LIB ENV PATH PATH_SUFFIXES lib lib64)
    FIND_LIBRARY( OpenCV_LIBRARY6 NAMES opencv_gpu PATHS ENV C++LIB ENV PATH PATH_SUFFIXES lib lib64)
    FIND_LIBRARY( OpenCV_LIBRARY7 NAMES opencv_imgproc PATHS ENV C++LIB ENV PATH PATH_SUFFIXES lib lib64)
    FIND_LIBRARY( OpenCV_LIBRARY8 NAMES opencv_legacy PATHS ENV C++LIB ENV PATH PATH_SUFFIXES lib lib64)
    FIND_LIBRARY( OpenCV_LIBRARY9 NAMES opencv_ml PATHS ENV C++LIB ENV PATH PATH_SUFFIXES lib lib64)
    FIND_LIBRARY( OpenCV_LIBRARY11 NAMES opencv_objdetect PATHS ENV C++LIB ENV PATH PATH_SUFFIXES lib lib64)
    FIND_LIBRARY( OpenCV_LIBRARY13 NAMES opencv_ts PATHS ENV C++LIB ENV PATH PATH_SUFFIXES lib lib64)
    FIND_LIBRARY( OpenCV_LIBRARY14 NAMES opencv_video PATHS ENV C++LIB ENV PATH PATH_SUFFIXES lib lib64)
    SET (OpenCV_LIBRARIES ${OpenCV_LIBRARY1} ${OpenCV_LIBRARY2} ${OpenCV_LIBRARY3} ${OpenCV_LIBRARY4} ${OpenCV_LIBRARY5} ${OpenCV_LIBRARY6} ${OpenCV_LIBRARY7} ${OpenCV_LIBRARY8} ${OpenCV_LIBRARY9} ${OpenCV_LIBRARY11} ${OpenCV_LIBRARY13} ${OpenCV_LIBRARY14} )
    IF( OpenCV_LIBRARIES )
	MESSAGE("OPENCV LIBRARY FOUND IN ${OpenCV_LIBRARIES}")
    ENDIF( OpenCV_LIBRARIES )
ENDIF(OpenCV_INCLUDE_DIR)

include_directories(${OpenCV_INCLUDE_DIR})
