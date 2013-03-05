 cmake_minimum_required(VERSION 2.8)
SET( SOURCE_FILES ../replayer.cpp)
SET( LIBS_DIR /usr/local/lib/jderobot/)



include_directories(
	  /usr/local/include/jderobot
)

add_executable (replayer  ${SOURCE_FILES})

#automated opencv
include(FindPkgConfig)
PKG_CHECK_MODULES(opencv REQUIRED opencv)
include_directories(${opencv_INCLUDE_DIRS})
link_directories(${opencv_LIBRARY_DIRS})
#automated gsl
PKG_CHECK_MODULES(gsl REQUIRED gsl)
include_directories(${gsl_INCLUDE_DIRS})
link_directories(${gsl_LIBRARY_DIRS})
#automated eigen
PKG_CHECK_MODULES(eigen REQUIRED eigen3)
include_directories(${eigen_INCLUDE_DIRS})
link_directories(${eigen_LIBRARY_DIRS})


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


# CHECK PCL
FIND_PATH( pcl_INCLUDE_DIR1 NAMES point_cloud.h PATHS ENV C++LIB ENV PATH PATH_SUFFIXES include/pcl-1.5/ )
FIND_PATH( pcl_INCLUDE_DIR2 NAMES pcl PATHS ENV C++LIB ENV PATH PATH_SUFFIXES include/pcl-1.6/ )
IF ((pcl_INCLUDE_DIR1) AND (pcl_INCLUDE_DIR2))
	SET (pcl_INCLUDE_DIR ${pcl_INCLUDE_DIR1} ${pcl_INCLUDE_DIR2})
ELSE ((pcl_INCLUDE_DIR1) AND (pcl_INCLUDE_DIR2))
	IF (pcl_INCLUDE_DIR1)
		SET (pcl_INCLUDE_DIR ${pcl_INCLUDE_DIR1})
	ENDIF(pcl_INCLUDE_DIR1)

	IF (pcl_INCLUDE_DIR2)
		SET (pcl_INCLUDE_DIR ${pcl_INCLUDE_DIR2})
	ENDIF(pcl_INCLUDE_DIR2)
ENDIF ((pcl_INCLUDE_DIR1) AND (pcl_INCLUDE_DIR2))

#MESSAGE("PCL LIBRARY FOUND IN ${pcl_INCLUDE_DIR}")
FIND_LIBRARY(with_pcl
    NAMES pcl_surface pcl_registration pcl_segmentation pcl_filters pcl_sample_consensus pcl_io pcl_octree pcl_features pcl_kdtree pcl_common
    PATHS /usr/lib)


IF( pcl_INCLUDE_DIR )
	include_directories (${pcl_INCLUDE_DIR})
    FIND_LIBRARY( pcl_LIBRARY1 NAMES pcl_surface PATHS ENV C++LIB ENV PATH PATH_SUFFIXES lib lib64)
    FIND_LIBRARY( pcl_LIBRARY2 NAMES pcl_registration PATHS ENV C++LIB ENV PATH PATH_SUFFIXES lib lib64)
    FIND_LIBRARY( pcl_LIBRARY3 NAMES pcl_segmentation PATHS ENV C++LIB ENV PATH PATH_SUFFIXES lib lib64)
    FIND_LIBRARY( pcl_LIBRARY4 NAMES pcl_filters PATHS ENV C++LIB ENV PATH PATH_SUFFIXES lib lib64)
    FIND_LIBRARY( pcl_LIBRARY5 NAMES pcl_sample_consensus PATHS ENV C++LIB ENV PATH PATH_SUFFIXES lib lib64)
    FIND_LIBRARY( pcl_LIBRARY6 NAMES pcl_io PATHS ENV C++LIB ENV PATH PATH_SUFFIXES lib lib64)
    FIND_LIBRARY( pcl_LIBRARY7 NAMES pcl_octree PATHS ENV C++LIB ENV PATH PATH_SUFFIXES lib lib64)
    FIND_LIBRARY( pcl_LIBRARY8 NAMES pcl_features PATHS ENV C++LIB ENV PATH PATH_SUFFIXES lib lib64)
    FIND_LIBRARY( pcl_LIBRARY9 NAMES pcl_kdtree PATHS ENV C++LIB ENV PATH PATH_SUFFIXES lib lib64)
    FIND_LIBRARY( pcl_LIBRARY11 NAMES pcl_common PATHS ENV C++LIB ENV PATH PATH_SUFFIXES lib lib64)
    SET (pcl_LIBRARIES ${pcl_LIBRARY1} ${pcl_LIBRARY2} ${pcl_LIBRARY3} ${pcl_LIBRARY4} ${pcl_LIBRARY5} ${pcl_LIBRARY6} ${pcl_LIBRARY7} ${pcl_LIBRARY8} ${pcl_LIBRARY9} ${pcl_LIBRARY11})
    IF( pcl_LIBRARIES )
		#MESSAGE("PCL LIBRARY FOUND IN ${pcl_LIBRARIES}")
    ENDIF( pcl_LIBRARIES )
ENDIF(pcl_INCLUDE_DIR)



IF(NOT with_pcl)
    MESSAGE ("*** PCL libraries not found. Are needed to KinectServer component")
endif()
IF(with_pcl)
    MESSAGE ("*** PCL OK")
endif()



set( CMAKE_CXX_FLAGS "-Wno-deprecated" ) # Opciones para el compilador



TARGET_LINK_LIBRARIES(replayer
  ${opencv_LIBRARIES}
  ${gsl_LIBRARIES}
  ${Gearbox_LIBRARIES}
  ${Ice_LIBRARIES}
  ${pcl_LIBRARIES}
  ${eigen_LIBRARIES}
  ${LIBS_DIR}/libcolorspacesmm.so
  ${LIBS_DIR}/libJderobotInterfaces.so
  ${LIBS_DIR}/libjderobotice.so
  ${LIBS_DIR}/libjderobotutil.so
)
