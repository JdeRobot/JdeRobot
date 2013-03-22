cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
include(FindPkgConfig)
PKG_CHECK_MODULES(opencv REQUIRED opencv)
include_directories(${opencv_INCLUDE_DIRS})
link_directories(${opencv_LIBRARY_DIRS})
MESSAGE("${opencv_LIBRARY_DIRS}")
MESSAGE("${opencv_INCLUDE_DIRS}")
PKG_CHECK_MODULES(gtk20 REQUIRED gtk+-2.0)
include_directories(${gtk20_INCLUDE_DIRS})
link_directories(${gtk20_LIBRARY_DIRS})

PKG_CHECK_MODULES(gtkgl20 REQUIRED gtkgl-2.0)
include_directories(${gtkgl20_INCLUDE_DIRS})
link_directories(${gtkgl20_LIBRARY_DIRS})

PKG_CHECK_MODULES(gtkmm REQUIRED gtkmm-2.4)
include_directories(${gtkmm_INCLUDE_DIRS})
link_directories(${gtkmm_LIBRARY_DIRS})

PKG_CHECK_MODULES(libglademm REQUIRED libglademm-2.4)
include_directories(${libglademm_INCLUDE_DIRS})
link_directories(${libglademm_LIBRARY_DIRS})

PKG_CHECK_MODULES(gthread REQUIRED gthread-2.0)
include_directories(${gthread_INCLUDE_DIRS})
link_directories(${gthread_LIBRARY_DIRS})

PKG_CHECK_MODULES(libgnomecanvas REQUIRED libgnomecanvas-2.0)
include_directories(${libgnomecanvas_INCLUDE_DIRS})
link_directories(${libgnomecanvas_LIBRARY_DIRS})

PKG_CHECK_MODULES(libgnomecanvasmm REQUIRED libgnomecanvasmm-2.6)
include_directories(${libgnomecanvasmm_INCLUDE_DIRS})
link_directories(${libgnomecanvasmm_LIBRARY_DIRS})

PKG_CHECK_MODULES(gtkglextmm REQUIRED gtkglextmm-1.2)
include_directories(${gtkglextmm_INCLUDE_DIRS})
link_directories(${gtkglextmm_LIBRARY_DIRS})

PKG_CHECK_MODULES(gsl REQUIRED gsl)
include_directories(${gsl_INCLUDE_DIRS})
link_directories(${gsl_LIBRARIES_DIRS})


include (FindPkgConfig)
if (PKG_CONFIG_FOUND)
  pkg_check_modules(GAZEBO gazebo)
  message(${GAZEBO_INCLUDE_DIRS})
endif()
include_directories(${GAZEBO_INCLUDE_DIRS} /usr/local/include/jderobot /usr/local/include/gearbox )

link_directories(${GAZEBO_LIBRARY_DIRS}  /usr/local/lib/jderobot)
set( CMAKE_CXX_FLAGS "-lIce -lIceUtil" ) # Opciones para el compilador

add_library(motors SHARED ../motors.cc)
target_link_libraries(motors 
	${GAZEBO_libraries} 
    ${opencv_LIBRARIES}	
	${ZeroCIce_LIBRARIES} 
    libjderobotice.so
    libjderobotutil.so
    libcolorspacesmm.so
    libJderobotInterfaces.so) 
    
add_library(encoders SHARED ../encoders.cc)
target_link_libraries(encoders 
	${GAZEBO_libraries} 
    ${opencv_LIBRARIES}	
	${ZeroCIce_LIBRARIES} 
    libjderobotice.so
    libjderobotutil.so
    libcolorspacesmm.so
    libJderobotInterfaces.so)    

add_library(laser SHARED ../laser.cc)
target_link_libraries(laser 
	RayPlugin 
	${GAZEBO_libraries} 
    ${opencv_LIBRARIES}	
	${ZeroCIce_LIBRARIES} 
    libjderobotice.so
    libjderobotutil.so
    libcolorspacesmm.so
    libJderobotInterfaces.so) 

add_library(camera_dump SHARED ../camera_dump.cc)
target_link_libraries(camera_dump 
	CameraPlugin 	
	${GAZEBO_libraries} 
    ${opencv_LIBRARIES}	
	${ZeroCIce_LIBRARIES} 
    libjderobotice.so
    libjderobotutil.so
    libcolorspacesmm.so
    libJderobotInterfaces.so)
    

add_library(pose3dencoders SHARED ../pose3dencoders.cc)
target_link_libraries(pose3dencoders 	
	${GAZEBO_libraries} 
    ${opencv_LIBRARIES}	
	${ZeroCIce_LIBRARIES} 
    libjderobotice.so
    libjderobotutil.so
    libcolorspacesmm.so
    libJderobotInterfaces.so)
