cmake_minimum_required(VERSION 2.8.3)
project(RosLib)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
#  RosIceMessage
  cv_bridge
  geometry_msgs
  image_transport
  roscpp
  sensor_msgs
  std_msgs
  gazebo_ros
)

find_package(PCL 1.2 REQUIRED)
find_package(OpenCV REQUIRED)
find_package( ZLIB REQUIRED )
find_package(gazebo REQUIRED)



catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES RosIceGazebo
#  CATKIN_DEPENDS RosIceMessage
#  DEPENDS system_lib
)

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
  ${OpenCV_INCLUDE_DIRS}
  ${PCL_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIR}
  ${GAZEBO_INCLUDE_DIRS}
  ${ROS_HEADER_DIR}
  ${INTERFACES_CPP_DIR}
  ${LIBS_DIR}
)

link_directories(${GAZEBO_LIBRARY_DIRS})

add_library(ros_camera SHARED src/RosCameraPlugin.cpp)
#add_dependencies(ros_camera RosIceMessage_generate_messages_cpp)
target_link_libraries(ros_camera ${catkin_LIBRARIES} ${GAZEBO_LIBRARIES} CameraPlugin)

add_library(ros_laser SHARED src/RosLaserPlugin.cpp)
#add_dependencies(ros_laser RosIceMessage_generate_messages_cpp)
target_link_libraries(ros_laser ${catkin_LIBRARIES} ${GAZEBO_LIBRARIES} RayPlugin)

add_library(ros_pose3d SHARED src/RosPose3DPlugin.cpp)
#add_dependencies(ros_pose3d RosIceMessage_generate_messages_cpp)
target_link_libraries(ros_pose3d ${catkin_LIBRARIES} ${GAZEBO_LIBRARIES})

add_library(ros_encoders SHARED src/RosEncodersPlugin.cpp)
#add_dependencies(ros_encoders RosIceMessage_generate_messages_cpp)
target_link_libraries(ros_encoders ${catkin_LIBRARIES} ${GAZEBO_LIBRARIES})

add_library(ros_motors SHARED src/RosMotorsPlugin.cpp)
#add_dependencies(ros_motors RosIceMessage_generate_messages_cpp)
target_link_libraries(ros_motors ${catkin_LIBRARIES} ${GAZEBO_LIBRARIES} )

add_library(ros_pose3dencoders SHARED src/RosPose3DEncodersPlugin.cpp)
#add_dependencies(ros_pose3dencoders RosIceMessage_generate_messages_cpp)
target_link_libraries(ros_pose3dencoders ${catkin_LIBRARIES} ${GAZEBO_LIBRARIES})

file(GLOB CPP_INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/src/*Interface.cpp)
file(GLOB CPP_CLIENT ${CMAKE_CURRENT_SOURCE_DIR}/src/*Client.cpp)

add_library(ros_ice SHARED ${CPP_CLIENT} ${CPP_INTERFACE} src/TeleOperator.cpp)
#add_dependencies(ros_ice RosIceMessage_generate_messages_cpp)

 target_link_libraries(ros_ice
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
   #ros_ice

 )

#FILE(GLOB HEADER_ROS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/RosLib/*.h)
#FILE(GLOB HEADER_ROS_MESSAGE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/RosLib/RosIceMessage/*.h)
FILE(GLOB ROS_LIB ../../../../devel/lib/*.so)

#INSTALL (FILES ${HEADER_ROS_FILES} DESTINATION /usr/local/include/jderobot)
#INSTALL (FILES ${HEADER_ROS_MESSAGE_FILES} DESTINATION /usr/local/include/jderobot/RosIceMessage)
INSTALL (FILES ${ROS_LIB} DESTINATION /usr/local/lib/jderobot)


