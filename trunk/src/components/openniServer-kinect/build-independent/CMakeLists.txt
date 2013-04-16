 cmake_minimum_required(VERSION 2.8)
SET( SOURCE_FILES ../openniServer-kinect.cpp ../myprogeo.cpp)
SET( LIBS_DIR /usr/local/lib/jderobot/)



include_directories(
	  /usr/local/include/jderobot
)

add_executable (openniServer-kinect  ${SOURCE_FILES})

#automated opencv
include(FindPkgConfig)
PKG_CHECK_MODULES(opencv REQUIRED opencv)
include_directories(${opencv_INCLUDE_DIRS})
link_directories(${opencv_LIBRARY_DIRS})
#automated gsl
PKG_CHECK_MODULES(gsl REQUIRED gsl)
include_directories(${gsl_INCLUDE_DIRS})
link_directories(${gsl_LIBRARY_DIRS})




#manual openni libraries
FIND_PATH( openni_INCLUDE_DIR NAMES XnOpenNI.h  PATHS ENV C++LIB ENV PATH PATH_SUFFIXES ni)

 
IF( openni_INCLUDE_DIR )
    FIND_LIBRARY( openni_LIBRARIES NAMES OpenNI PATHS ENV C++LIB ENV PATH PATH_SUFFIXES lib lib64)
    IF( openni_LIBRARIES)
		MESSAGE ("-- OpenNi found at ${openni_LIBRARIES}")
		include_directories(${openni_INCLUDE_DIR})
		link_directories(${openni_LIBRARIES})
    ENDIF( openni_LIBRARIES )
ENDIF(openni_INCLUDE_DIR)

IF(NOT openni_LIBRARIES)
		MESSAGE ("*** OpenNi not found")
ENDIF()


#manual nite libraries
FIND_PATH( nite_INCLUDE_DIR NAMES XnVCircleDetector.h  PATHS ENV C++LIB ENV PATH PATH_SUFFIXES include/nite/)
 
IF( nite_INCLUDE_DIR )
	FIND_LIBRARY(nite_LIBRARIES NAMES XnVNite_1_5_2 XnVCNITE_1_5_2 PATHS /usr/lib/ /usr/local/lib)
    IF( nite_LIBRARIES)
		MESSAGE ("-- Nite found at ${nite_LIBRARIES}")
		include_directories(${nite_INCLUDE_DIR})
		link_directories(${nite_LIBRARIES})
    ENDIF( nite_LIBRARIES )
ENDIF(nite_INCLUDE_DIR)

IF(NOT nite_LIBRARIES)
		MESSAGE ("*** Nite not found")
ENDIF()





#manual gearbox libraries
FIND_PATH( Gearbox_INCLUDE_DIR NAMES gbxutilacfr/gbxutilacfr.h  PATHS ENV C++LIB ENV PATH PATH_SUFFIXES gearbox)
 
IF( Gearbox_INCLUDE_DIR )
    FIND_LIBRARY( Gearbox_LIBRARY1 NAMES GbxUtilAcfr PATHS ENV C++LIB ENV PATH PATH_SUFFIXES lib/gearbox lib64/gearbox )
    FIND_LIBRARY( Gearbox_LIBRARY2 NAMES GbxIceUtilAcfr PATHS ENV C++LIB ENV PATH PATH_SUFFIXES lib/gearbox lib64/gearbox)
    SET (Gearbox_LIBRARIES ${Gearbox_LIBRARY1} ${Gearbox_LIBRARY2})
    IF( Gearbox_LIBRARIES )
		MESSAGE ("-- Gearbox found at ${Gearbox_LIBRARIES}")
		include_directories(${Gearbox_INCLUDE_DIR})
		link_directories(${Gearbox_LIBRARIES})
    ENDIF( Gearbox_LIBRARIES )
ENDIF(Gearbox_INCLUDE_DIR)

IF(NOT Gearbox_LIBRARIES)
		MESSAGE ("*** Gearbox not found")
ENDIF()


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


#PKG_CHECK_MODULES(gearbox gearbox)
#include_directories(${gearbox_INCLUDE_DIRS})
#link_directories(${gearbox_LIBRARY_DIRS})
#message(STATUS "${gearbox_LIBRARY_DIRS}")
#message(STATUS "${gearbox_INCLUDE_DIRS}")


set( CMAKE_CXX_FLAGS "-Wno-deprecated -Wl,-rpath ./" ) # Opciones para el compilador


TARGET_LINK_LIBRARIES(openniServer-kinect
  ${opencv_LIBRARIES}
  ${gsl_LIBRARIES}
  ${Gearbox_LIBRARIES}
  ${Ice_LIBRARIES} 
  ${openni_LIBRARIES}
  ${nite_LIBRARIES}
  ${LIBS_DIR}/libcolorspacesmm.so
  ${LIBS_DIR}/libJderobotInterfaces.so
  ${LIBS_DIR}/libjderobotice.so
  ${LIBS_DIR}/libjderobotutil.so
  ${LIBS_DIR}/libprogeo.so
 
 
)
