export GAZEBO_MASTER_URI=http://localhost:11345
export GAZEBO_MODEL_DATABASE_URI=http://gazebosim.org/models
export GAZEBO_RESOURCE_PATH=/usr/share/gazebo-1.8:/usr/share/gazebo_models:${HOME}/.gazebo/models
export GAZEBO_PLUGIN_PATH=/usr/lib/gazebo-1.8/plugins:/usr/lib/gazebo-1.8/plugins-jderobot/pioneer:/usr/lib/gazebo-1.8/plugins-jderobot/nao:/usr/lib/gazebo-1.8/plugins-jderobot/kinect
export LD_LIBRARY_PATH=/usr/lib/gazebo-1.8/plugins:/usr/lib/gazebo-1.8/plugins-jderobot/pioneer:/usr/lib/gazebo-1.8/plugins-jderobot/nao:/usr/lib/gazebo-1.8/plugins-jderobot/kinect:${LD_LIBRARY_PATH}
export OGRE_RESOURCE_PATH=/usr/lib/$(uname -i)-linux-gnu/OGRE-1.7.4

