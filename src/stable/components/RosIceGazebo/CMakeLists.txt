cmake_minimum_required(VERSION 2.8.3)
project(RosIceGazebo)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
#  RosIceMessage
  cv_bridge
  image_transport
  roscpp
  rospy
  sensor_msgs
  std_msgs
)

## System dependencies are found with CMake's conventions
# find_package(Boost REQUIRED COMPONENTS system)

find_package(OpenCV REQUIRED)
find_package( ZLIB REQUIRED )

#file(GLOB jderobot_libraries "/usr/local/lib/jderobot/*.so")

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES RosIceGazebo
#  CATKIN_DEPENDS RosIceMessage
#  DEPENDS system_lib
)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
# include_directories(include)
set(slice_output_dir ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/cpp)

find_library(ice_LIBRARY1 Ice)
find_library(ice_LIBRARY2 IceUtil)
find_library(ice_LIBRARY3 IceStorm)
list(APPEND ice_LIBRARIES
    ${ice_LIBRARY1}
    ${ice_LIBRARY2}
    ${ice_LIBRARY3}
)


include_directories(
  ${catkin_INCLUDE_DIRS}
  ${slice_output_dir}
  ${slice_output_dir}/jderobot
  ${ROS_HEADER_DIR}
  ${OpenCV_INCLUDE_DIRS}
  ${INTERFACES_CPP_DIR}
  ${LIBS_DIR}
  #${CMAKE_CURRENT_SOURCE_DIR}/include
)

####add_subdirectory(interfaces)
## Declare a cpp library
# add_library(RosIceGazebo
#   src/${PROJECT_NAME}/RosIceGazebo.cpp
# )

## Declare a cpp executable

file(GLOB CPP_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_executable(RosGazeboInterface ${CPP_SRC})

## Add cmake target dependencies of the executable/library
## as an example, message headers may need to be generated before nodes
#add_dependencies(RosGazeboInterface RosIceMessage_generate_messages_cpp)

## Specify libraries to link a library or executable target against
# target_link_libraries(RosIceGazebo_node
#   ${catkin_LIBRARIES}
# )
 target_link_libraries(RosGazeboInterface
   ${catkin_LIBRARIES}
   ${ice_LIBRARIES}
     #jderobot_interfaces
   ${ZLIB_LIBRARIES}
     #${ZeroCIce_LIBRARIES}
   ${OpenCV_LIBRARIES}
   ${OpenCV_LIBS}
   JderobotInterfaces
   parallelIce
   colorspaces
   colorspacesmm
   ros_ice

 )

