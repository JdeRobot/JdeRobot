#!/bin/bash

function check_result {

	if [ "$?" != "0" ]; then
		echo "Error"
		clear
		exit -1
	else
		echo "OK"
	fi
}

source /opt/ros/kinetic/setup.bash

mkdir build && cd build
cmake .. $*
check_result

make -j6
check_result

sudo make install
check_result

# Package Debian
#cmake .
#cpack .
#check_result

#sudo chown jenkins -R /jenkins/ws/JdeRobot

