cmake_minimum_required(VERSION 2.8.3)
project(RosPCL)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  roscpp
  cv_bridge
  std_msgs
  image_transport
  sensor_msgs
  message_generation
)

find_package(PCL 1.2 REQUIRED)
find_package(OpenCV REQUIRED)


catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES RosPCL
  CATKIN_DEPENDS roscpp std_msgs image_transport cv_bridge
#  DEPENDS system_lib
)


include_directories(
  ${catkin_INCLUDE_DIRS}
  ${PCL_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIR}
  ${ROS_HEADER_DIR}
  ${INTERFACES_CPP_DIR}
  ${LIBS_DIR}
)

link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})

find_library(ice_LIBRARY1 Ice)
find_library(ice_LIBRARY2 IceUtil)
find_library(ice_LIBRARY3 IceStorm)
list(APPEND ice_LIBRARIES
    ${ice_LIBRARY1}
    ${ice_LIBRARY2}
    ${ice_LIBRARY3}
)



## Declare a cpp library
# add_library(RosPCL
#   src/${PROJECT_NAME}/RosPCL.cpp
# )

file(GLOB CPP_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_executable(openni_ros_grabber src/main_ros_grabber.cpp src/RosKinectGrabber.cpp)

#add_dependencies(openni_grabber RosPCL_generate_messages_cpp)
add_executable(openni_ice_interface src/main_ice_interface.cpp)

add_executable(IceRosInterface src/main_ice_grabber.cpp)
add_executable(RosViewer src/main_ros_viewer.cpp src/RosOpenNIViewer.cpp)

 target_link_libraries(openni_ros_grabber
   ${catkin_LIBRARIES}
   ${ice_LIBRARIES}
   #jderobot_interfaces
   ${ZLIB_LIBRARIES}
   #${ZeroCIce_LIBRARIES}
   ${OpenCV_LIBRARIES}
   ${OpenCV_LIBS}
   ${PCL_LIBRARIES}
   JderobotInterfaces
   parallelIce
   colorspaces
   colorspacesmm
   ros_ice

 )

 target_link_libraries(openni_ice_interface
   ${catkin_LIBRARIES}
   ${ice_LIBRARIES}
   #jderobot_interfaces
   ${ZLIB_LIBRARIES}
   #${ZeroCIce_LIBRARIES}
   ${OpenCV_LIBRARIES}
   ${OpenCV_LIBS}
   ${PCL_LIBRARIES}
   JderobotInterfaces
   parallelIce
   colorspaces
   colorspacesmm
   ros_ice
 )

 target_link_libraries(IceRosInterface
   ${catkin_LIBRARIES}
   ${ice_LIBRARIES}
   #jderobot_interfaces
   ${ZLIB_LIBRARIES}
   #${ZeroCIce_LIBRARIES}
   ${OpenCV_LIBRARIES}
   ${OpenCV_LIBS}
   ${PCL_LIBRARIES}
   JderobotInterfaces
   parallelIce
   colorspaces
   colorspacesmm
   ros_ice
 )

 target_link_libraries(RosViewer
   ${catkin_LIBRARIES}
   ${ice_LIBRARIES}
   #jderobot_interfaces
   ${ZLIB_LIBRARIES}
   #${ZeroCIce_LIBRARIES}
   ${OpenCV_LIBRARIES}
   ${OpenCV_LIBS}
   ${PCL_LIBRARIES}
   JderobotInterfaces
   parallelIce
   colorspaces
   colorspacesmm
   ros_ice
 )

