cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
include (FindPkgConfig)
if (PKG_CONFIG_FOUND)
	pkg_check_modules(OpenCV opencv)
endif()

FIND_PACKAGE(Qt4 REQUIRED)

SET(qt_SOURCES main.cpp 
			   sensors/threadsensors.cpp sensors/sensors.cpp
			   gui/threadgui.cpp gui/gui.cpp
)

SET(qt_HEADERS
			   sensors/threadsensors.h sensors/sensors.h
			   gui/threadgui.h gui/gui.h

)

SET( LIBS_DIR /usr/local/lib/jderobot/)

set( CMAKE_CXX_FLAGS " -lIce -lIceUtil " )


QT4_WRAP_CPP(qt_HEADERS_MOC ${qt_HEADERS})
INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})



include_directories(${opencv_INCLUDE_DIRS})
link_directories( ${opencv_library_dirs})


include_directories(
    /usr/local/include/jderobot/
)

add_executable( main main.cpp 
			   sensors/threadsensors.cpp sensors/sensors.cpp
			   gui/threadgui.cpp gui/gui.cpp
			   ${qt_HEADERS_MOC}
)
			   
target_link_libraries(main opencv_imgproc opencv_highgui opencv_core
	${QT_LIBRARIES}
	${LIBS_DIR}/libJderobotInterfaces.so
	${LIBS_DIR}/libjderobotice.so
	${LIBS_DIR}/libjderobotutil.so
	
)

