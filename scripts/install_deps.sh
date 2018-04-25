#!/bin/bash
apt install sudo wget

sudo sh -c 'echo "deb http://packages.ros.org/ros/ubuntu $(lsb_release -sc) main" > /etc/apt/sources.list.d/ros-latest.list'

sudo apt-key adv --keyserver hkp://ha.pool.sks-keyservers.net:80 --recv-key 421C365BD9FF1F717815A3895523BAEEB01FA116

sudo apt-get update

sudo apt install -y ros-kinetic-roscpp ros-kinetic-std-msgs ros-kinetic-cv-bridge ros-kinetic-image-transport ros-kinetic-roscpp-core ros-kinetic-rospy ros-kinetic-nav-msgs ros-kinetic-geometry-msgs ros-kinetic-opencv3 ros-kinetic-kobuki-gazebo ros-kinetic-usb_cam

sudo apt-get install -y libpcl-dev

sudo apt-add-repository "deb http://zeroc.com/download/apt/ubuntu$(lsb_release -rs) stable main"
sudo apt-key adv --keyserver keyserver.ubuntu.com --recv 5E6DA83306132997


sudo sh -c 'echo "deb http://jderobot.org/apt xenial main" > /etc/apt/sources.list.d/jderobot.list'
sudo apt-key adv --keyserver ha.pool.sks-keyservers.net --recv-keys B0E7F58E82C8091DF945A0453DA08892EE69A25C

sudo apt update

sudo apt install -y build-essential libtool cmake g++ gcc git make

sudo apt install -y freeglut3 freeglut3-dev libgl1-mesa-dev libglu1-mesa

sudo apt-get install -y libgtk2.0-0 libgtk2.0-bin libgtk2.0-cil libgtk2.0-common libgtk2.0-dev libgtkgl2.0-1
sudo apt-get install -y libgtkgl2.0-dev libgtkglext1 libgtkglext1-dev libglademm-2.4-dev libgtkmm-2.4-dev 
sudo apt-get install -y libgnomecanvas2-0 libgnomecanvas2-dev  libgtkglext1-doc libgnomecanvasmm-2.6-dev
sudo apt-get install -y libgnomecanvasmm-2.6-1v5 libgtkglextmm-x11-1.2-0v5 libgtkglextmm-x11-1.2-dev

sudo apt-get install -y libgoocanvasmm-2.0-6 libgoocanvasmm-2.0-dev

sudo apt-get install -y libgsl2 gsl-bin libgsl-dev

sudo apt-get install -y libxml++2.6-2v5 libxml++2.6-dev libtinyxml-dev

sudo apt-get install -y libeigen3-dev

sudo apt-get install -y libdc1394-22 libdc1394-22-dev

sudo apt-get install -y libusb-1.0-0 libusb-1.0-0-dev

sudo apt-get install -y python-matplotlib python-pyqt5 python-pip python-numpy python-pyqt5.qtsvg python-pyqt5.qsci

sudo apt-get install -y qfi

sudo apt-get install -y qtbase5-dev libqt5declarative5 libqt5script5 libqt5svg5-dev

sudo apt-get install -y libboost-system-dev libboost-filesystem-dev

sudo apt-get install -y libgstreamer0.10-dev libgstreamer-plugins-base0.10-dev

sudo apt install -y libzeroc-ice3.6 zeroc-ice-utils libzeroc-icestorm3.6 zeroc-ice-slice libzeroc-ice-dev

sudo apt-get install -y libssl-dev libbz2-dev
sudo pip2 install --upgrade pip
sudo pip2 install zeroc-ice==3.6.4

sudo apt-get install -y libopenni2-dev libopenni-dev

sudo apt-get install -y gazebo7 libgazebo7-dev

sudo apt-get install -y nodejs

sudo apt-get install -y ardronelib libgoogle-glog-dev










