In this package there are two types of classes, the "Interfaces" and the "Plugins".

The Interfaces have the same structure as the Ice Classes in the JdeRobot::gazeboserver component, but they also have a rosCallback method that will convert the data received from the
Gazebo simulator into JdeRobot format.

The Plugins have the same structure as the Gazebo Classes in the JdeRobot::gazeboserver component, but they also have a ros::Publisher or ros::Subscriber in order to interact with the
"Interface" classes

Before compling this package, you need to first compile the RosIceMessage package and source it's setup.bash file and also export the path of the gazebo libraries which will be created by
this package.

Example: export GAZEBO_PLUGIN_PATH=~/catkin_ws/src/IceRosGazebo/devel/lib:${GAZEBO_PLUGIN_PATH}
export LD_LIBRARY_PATH=~/catkin_ws/src/IceRosGazebo/devel/lib:${LD_LIBRARY_PATH}
