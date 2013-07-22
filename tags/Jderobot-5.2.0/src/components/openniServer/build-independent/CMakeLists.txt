 cmake_minimum_required(VERSION 2.8)
SET( SOURCE_FILES ../openniServer.cpp ../myprogeo.cpp)
SET( LIBS_DIR /usr/local/lib/jderobot/)



include_directories(
	  /usr/local/include/jderobot
)

add_executable (openniServer  ${SOURCE_FILES})

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


#PKG_CHECK_MODULES(gearbox gearbox)
#include_directories(${gearbox_INCLUDE_DIRS})
#link_directories(${gearbox_LIBRARY_DIRS})
#message(STATUS "${gearbox_LIBRARY_DIRS}")
#message(STATUS "${gearbox_INCLUDE_DIRS}")


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
	  ${opencv_LIBRARIES}
	  ${gsl_LIBRARIES}
	  ${Ice_LIBRARIES}
	  ${LIBS_DIR}/libcolorspacesmm.so
	  ${LIBS_DIR}/libJderobotInterfaces.so
	  ${LIBS_DIR}/libjderobotutil.so
	  ${LIBS_DIR}/libprogeo.so
	  ${OPENNI2_LIB}/libOpenNI2.so
	  ${NITE2_LIB}/libNiTE2.so
	)
else()
	TARGET_LINK_LIBRARIES(openniServer
	  ${opencv_LIBRARIES}
	  ${gsl_LIBRARIES}
	  ${Ice_LIBRARIES}
	  ${LIBS_DIR}/libcolorspacesmm.so
	  ${LIBS_DIR}/libJderobotInterfaces.so
	  ${LIBS_DIR}/libjderobotutil.so
	  ${LIBS_DIR}/libprogeo.so
	  ${OPENNI2_LIB}/libOpenNI2.so
	)
endif()
