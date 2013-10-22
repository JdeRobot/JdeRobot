cmake_minimum_required(VERSION 2.8)
include(FindPkgConfig)

SET( SOURCE_FILES ../control.cpp ../basic_component.cpp ../API.cpp ../gui.cpp)

SET(INTERFACES_CPP_DIR /usr/local/lib )

include_directories(
    ${INTERFACES_CPP_DIR}
    /usr/local/include/jderobot/
)


PKG_CHECK_MODULES(gtkmm REQUIRED gtkmm-2.4)
include_directories(${gtkmm_INCLUDE_DIRS})
link_directories(${gtkmm_LIBRARY_DIRS})

PKG_CHECK_MODULES(gthread REQUIRED gthread-2.0)
include_directories(${gthread_INCLUDE_DIRS})
link_directories(${gthread_LIBRARY_DIRS})

PKG_CHECK_MODULES(libglademm REQUIRED libglademm-2.4)
include_directories(${libglademm_INCLUDE_DIRS})
link_directories(${libglademm_LIBRARY_DIRS})


FIND_PATH( OpenCV_INCLUDE_DIR NAMES cv.h  PATHS ENV C++LIB ENV PATH PATH_SUFFIXES include/opencv/)
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
		#MESSAGE("OPENCV LIBRARY FOUND IN ${OpenCV_LIBRARIES}")
    ENDIF( OpenCV_LIBRARIES )
ENDIF(OpenCV_INCLUDE_DIR)

IF (NOT  OpenCV_INCLUDE_DIR)
	MESSAGE("*** OpenCV not found")
ENDIF()

find_package(Threads)


#manual ICE
FIND_PATH( Ice_INCLUDE_DIR NAMES Ice/Ice.h  PATHS ENV C++LIB ENV)
 
IF( Ice_INCLUDE_DIR )
    FIND_LIBRARY( Ice_LIBRARY1 NAMES Ice PATHS ENV C++LIB ENV PATH PATH_SUFFIXES lib lib64 )
    FIND_LIBRARY( Ice_LIBRARY2 NAMES IceUtil PATHS ENV C++LIB ENV PATH PATH_SUFFIXES lib lib64)
    SET (Ice_LIBRARIES ${Ice_LIBRARY1} ${Ice_LIBRARY2})
    IF( Ice_LIBRARIES )
		MESSAGE ("-- Ice found at ${Ice_LIBRARIES}")
		include_directories(${Ice_INCLUDE_DIR})
		link_directories(${Ice_LIBRARIES})
    ENDIF( Ice_LIBRARIES )
ENDIF(Ice_INCLUDE_DIR)

IF(NOT Ice_LIBRARIES)
		MESSAGE ("*** Ice not found")
ENDIF()


add_executable (basic_component  ${SOURCE_FILES})

TARGET_LINK_LIBRARIES(basic_component 
	  ${CMAKE_THREAD_LIBS_INIT}
    ${INTERFACES_CPP_DIR}/jderobot/libJderobotInterfaces.so
    ${INTERFACES_CPP_DIR}/jderobot/libjderobotutil.so
    ${INTERFACES_CPP_DIR}/jderobot/libcolorspacesmm.so
    ${libglademm_LIBRARIES}
    ${OpenCV_LIBRARIES}
		${Ice_LIBRARIES}
)
