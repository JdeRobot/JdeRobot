 cmake_minimum_required(VERSION 2.8)


SET( SOURCE_FILES ../recorder2.cpp ../recordergui.cpp ../poolWriteImages.cpp ../poolWritePose3dEncoders.cpp ../poolWriteLasers.cpp ../poolWritePointCloud.cpp ../poolWriteEncoders.cpp)
SET( LIBS_DIR /usr/local/lib/jderobot/)



include_directories(
	  /usr/local/include/jderobot
)

PROJECT(recorderProject)
add_executable (recorder2  ${SOURCE_FILES})

#automated opencv
include(FindPkgConfig)
PKG_CHECK_MODULES(opencv REQUIRED opencv)
include_directories(${opencv_INCLUDE_DIRS})
link_directories(${opencv_LIBRARY_DIRS})



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


#automated gtk
PKG_CHECK_MODULES(gtkmm REQUIRED gtkmm-2.4)
include_directories(${gtkmm_INCLUDE_DIRS})
link_directories(${gtkmm_LIBRARY_DIRS})
#automated gtkmm
PKG_CHECK_MODULES(libglademm REQUIRED libglademm-2.4)
include_directories(${libglademm_INCLUDE_DIRS})
link_directories(${libglademm_LIBRARY_DIRS})
#automated gtkglextmm
PKG_CHECK_MODULES(gtkglextmm REQUIRED gtkglextmm-x11-1.2)
include_directories(${gtkglextmm_INCLUDE_DIRS})
link_directories(${gtkglextmm_LIBRARY_DIRS})



set( CMAKE_CXX_FLAGS "-Wno-deprecated" ) # Opciones para el compilador



TARGET_LINK_LIBRARIES(recorder2
  ${opencv_LIBRARIES}
  ${Ice_LIBRARIES}
  ${gtkglextmm_LIBRARIES}
  ${libglademm_LIBRARIES}
  ${gtkmm_LIBRARIES}
  ${LIBS_DIR}/libcolorspacesmm.so
  ${LIBS_DIR}/libJderobotInterfaces.so
  #${LIBS_DIR}/libjderobotice.so
  ${LIBS_DIR}/libjderobotutil.so
)
