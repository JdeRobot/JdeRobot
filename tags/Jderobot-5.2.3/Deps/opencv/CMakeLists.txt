FIND_PATH( OpenCVHG_INCLUDE_DIR NAMES highgui.h PATHS ENV C++LIB ENV PATH PATH_SUFFIXES include/opencv/ )

IF (OpenCVHG_INCLUDE_DIR)
 
	MESSAGE("OpenCVHighGui was found in: ${OpenCVHG_INCLUDE_DIR}")
  
	FIND_PATH( OpenCV_INCLUDE_DIR NAMES cv.h PATHS ENV C++LIB ENV PATH PATH_SUFFIXES include/opencv/)

	IF( OpenCV_INCLUDE_DIR )
    	MESSAGE("OpenCV was found in: ${OpenCV_INCLUDE_DIR}")
	
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
	    FIND_LIBRARY( OpenCV_LIBRARY14 NAMES opencv_video PATHS ENV C++LIB ENV PATH PATH_SUFFIXES lib lib64)
	    SET (OpenCV_LIBRARIES ${OpenCV_LIBRARY1} ${OpenCV_LIBRARY5}  ${OpenCV_LIBRARY7}  ${OpenCV_LIBRARY9}  ${OpenCV_LIBRARY14} )
	    SET (OpenCVGUI_LIBRARIES ${OpenCV_LIBRARY2} ${OpenCV_LIBRARY3} ${OpenCV_LIBRARY4} ${OpenCV_LIBRARY6} ${OpenCV_LIBRARY8} ${OpenCV_LIBRARY11})

	    IF( OpenCV_LIBRARIES )
			#MESSAGE("OPENCV LIBRARY FOUND IN ${OpenCV_LIBRARY2}")

			link_directories(${OpenCV_LIBRARIES})	
		    include_directories(${OpenCV_INCLUDE_DIR})

			SET(DEPS "${DEPS}libopencv-calib3d2.3, libopencv-contrib2.3, libopencv-core2.3, libopencv-features2d2.3, libopencv-flann2.3, libopencv-gpu2.3, libopencv-highgui2.3, libopencv-imgproc2.3, libopencv-legacy2.3, libopencv-ml2.3, libopencv-objdetect2.3, libopencv-video2.3, libcv2.3, libcvaux2.3, libhighgui2.3, libopencv-highgui2.3,")

	    ENDIF( OpenCV_LIBRARIES )


	ELSE(OpenCV_INCLUDE_DIR)
		MESSAGE( FATAL_ERROR "*** OpenCV not found")
	ENDIF(OpenCV_INCLUDE_DIR)

ELSE(OpenCVHG_INCLUDE_DIR)

	MESSAGE(FATAL_ERROR "*** OpenCV: libhighgui-dev not found!")

ENDIF(OpenCVHG_INCLUDE_DIR)
