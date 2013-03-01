
cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
include (FindPkgConfig)
if (PKG_CONFIG_FOUND)
  pkg_check_modules(GAZEBO gazebo)
endif()

find_package(PCL 1.2 REQUIRED)
find_package(OpenCV REQUIRED)

include_directories( 
	${GAZEBO_INCLUDE_DIRS} 
	${PCL_INCLUDE_DIRS} 
	/usr/local/include/gearbox/ 
	/usr/local/include/jderobot
	
	) 
link_directories( 
	${GAZEBO_LIBRARY_DIRS} 
	${PCL_LIBRARY_DIRS} 
	)

set( CMAKE_CXX_FLAGS "-lIce -lIceUtil" ) # Opciones para el compilador

add_library(kinectPlugin SHARED kinectPlugin.cc)

target_link_libraries(kinectPlugin 
	${GAZEBO_libraries} 
	${PCL_LIBRARIES} 
	${OpenCV_LIBS} 
		/usr/local/lib/jderobot/libJderobotInterfaces.so
		/usr/local/lib/jderobot/libjderobotice.so
		/usr/local/lib/jderobot/libjderobotutil.so

)
