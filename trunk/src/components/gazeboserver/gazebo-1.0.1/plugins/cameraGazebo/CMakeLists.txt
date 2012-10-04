cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
include (FindPkgConfig)
if (PKG_CONFIG_FOUND)
  pkg_check_modules(GAZEBO gazebo)
  message(${GAZEBO_INCLUDE_DIRS})
endif()
include_directories(${GAZEBO_INCLUDE_DIRS} /usr/local/include/jderobot /usr/local/include/gearbox/)
link_directories(${GAZEBO_LIBRARY_DIRS} /usr/local/lib/jderobot)

set( CMAKE_CXX_FLAGS "-lIce -lIceUtil" ) # Opciones para el compilador

add_library(camera_dump SHARED camera_dump.cc)
target_link_libraries(camera_dump ${GAZEBO_libraries} CameraPlugin opencv_core opencv_highgui opencv_imgproc boost_system ${ZeroCIce_LIBRARIES} jderobotice jderobotutil colorspacesmm JderobotInterfaces) 

add_library(camera_move SHARED camera_move.cc)
target_link_libraries(camera_move ${GAZEBO_libraries})
