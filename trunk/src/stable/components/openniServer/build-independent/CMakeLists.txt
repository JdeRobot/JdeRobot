 cmake_minimum_required(VERSION 2.8)
SET( SOURCE_FILES ../openniServer.cpp ../myprogeo.cpp)
SET( LIBS_DIR /usr/local/lib/jderobot/)



include_directories(
	  /usr/local/include/jderobot
)

add_executable (openniServer  ${SOURCE_FILES})

include(FindPkgConfig)
#manual opencv
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

link_directories(${OpenCV_LIBRARIES})
include_directories(${OpenCV_INCLUDE_DIR})





#automated gsl
PKG_CHECK_MODULES(gsl REQUIRED gsl)
include_directories(${gsl_INCLUDE_DIRS})
link_directories(${gsl_LIBRARY_DIRS})

find_package(Threads)


#manual openni libraries
SET ( OPENNI2_INC $ENV{OPENNI2_INCLUDE})
SET ( OPENNI2_LIB $ENV{OPENNI2_REDIST})

if (OPENNI2_INC AND OPENNI2_LIB)
	message(STATUS "OpenNI found at ${OPENNI2_LIB}")
else()
	message(FATAL_ERROR "OpenNI2 library not found")
endif()
include_directories(${OPENNI2_INC})
link_directories(${OPENNI2_LIB}/libOpenNI2.so)


#manual nite libraries
SET ( NITE2_INC $ENV{NITE2_INCLUDE})
SET ( NITE2_LIB $ENV{NITE2_REDIST})

if (NITE2_INC AND NITE2_LIB)
	message(STATUS "NITE found at ${NITE2_LIB}")
	set_property(
  		SOURCE openniServer.cpp
   	PROPERTY COMPILE_DEFINITIONS WITH_NITE2=1
   	)
else()
	message("NITE2 library not found")
endif()
include_directories(${NITE2_INC})
link_directories(${NITE2_LIB}/libNiTE2.so)


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



set( CMAKE_CXX_FLAGS "-Wno-deprecated -Wl,-rpath ./" ) # Opciones para el compilador

if (NOT IS_DIRECTORY ./OpenNI2)
	message ("--CMake will install local OpenNI2 lib and Driver")
	file (COPY ${OPENNI2_LIB}/libOpenNI2.so DESTINATION .)
	file (COPY ${OPENNI2_LIB}/OpenNI2 DESTINATION .)
	
endif()




if  ( NITE2_LIB )
	if (NOT IS_DIRECTORY ./NiTE2)
	message ("--CMake will install local NiTE2 lib and Driver")
	file (COPY ${NITE2_LIB}/libNiTE2.so DESTINATION .)
	file (COPY ${NITE2_LIB}/NiTE2 DESTINATION .)

	endif()

	TARGET_LINK_LIBRARIES(openniServer
		${CMAKE_THREAD_LIBS_INIT}
	  ${OpenCV_LIBRARIES}
		${OpenCVGUI_LIBRARIES}
	  ${gsl_LIBRARIES}
	  ${Ice_LIBRARIES}
	  ${LIBS_DIR}/libcolorspacesmm.so
	  ${LIBS_DIR}/libJderobotInterfaces.so
	  ${LIBS_DIR}/libprogeo.so
	  ${OPENNI2_LIB}/libOpenNI2.so
	  ${NITE2_LIB}/libNiTE2.so
	)
else()
	TARGET_LINK_LIBRARIES(openniServer
		${CMAKE_THREAD_LIBS_INIT}
	  ${OpenCV_LIBRARIES}
		${OpenCVGUI_LIBRARIES}
	  ${gsl_LIBRARIES}
	  ${Ice_LIBRARIES}
	  ${LIBS_DIR}/libcolorspacesmm.so
	  ${LIBS_DIR}/libJderobotInterfaces.so
	  ${LIBS_DIR}/libprogeo.so
	  ${OPENNI2_LIB}/libOpenNI2.so
	)
endif()
