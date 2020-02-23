
JdeRobot
========

<!--[![Build Status](http://jenkins.jderobot.org/buildStatus/icon?job=JdeRobot-GitHub)](http://jenkins.jderobot.org/job/JdeRobot-GitHub/)-->
[![Current Release Version](https://img.shields.io/github/release/jderobot/base?style=flat-square&logo=github)](https://github.com/explosion/spaCy/releases)

# Introduction


JdeRobot is a software development suite for robotics, home-automation and computer vision applications. These domains include sensors (for instance, cameras), actuators, and intelligent software in between. It has been designed to help in programming with such intelligent softwares. It is mainly written in C++ language and provides a distributed component-based programming environment where the application program is made up of a collection of several concurrent asynchronous components. Each component may run in different computers and they are connected using Ice communication middleware. Components may be written in C++, python, Java... and all of them interoperate through explicit Ice interfaces.

JdeRobot simplifies the access to hardware devices from the control program. Getting sensor measurements is as simple as calling a local function, and ordering motor commands as easy as calling another local function. The platform attaches those calls to the remote invocation on the components connected to the sensor or the actuator devices. They can be connected to real sensors and actuators or simulated ones, both locally or remotely using the network. Those functions build the API for the Hardware Abstraction Layer. The robotic application get the sensor readings and order the actuator commands using it to unfold its behavior. Several driver components have been developed to support different physical sensors, actuators and simulators. The drivers are used as components installed at will depending on your configuration. They are included in the official release. Currently supported robots and devices:

* RGBD sensors: Kinect from Microsoft, Asus Xtion
* Pioneer robot from MobileRobotics Inc.
* Kobuki robot (TurtleBot) from Yujin Robot
* Nao humanoid from Aldebaran
* ArDrone quadrotor from Parrot
* Firewire cameras, USB cameras, video files (mpeg, avi...), IP cameras (like Axis)
* Pantilt unit PTU-D46 from Directed Perception Inc.
* Laser Scanners: LMS from SICK and URG from Hokuyo
* EVI PTZ camera from Sony
* Gazebo and Stage simulators
* Wiimote
* X10 home automation devices

JdeRobot includes several robot programming tools and libraries. First, viewers and teleoperators for several robots, its sensors and motors. Second, a camara calibration component and a tunning tool for color filters. Third, VisualStates tool for programming robot behavior using hierarchical finite state machines. It includes many sample components using OpenCV, PCL, OpenGL, etc.. In addition, it also provides a library to develop fuzzy controllers, a library for projective geometry and some computer vision processing.

Each component may have its own independent Graphical User Interface or none at all. Currently, GTK and Qt libraries are supported, and several examples of OpenGL for 3D graphics with both libraries are included.

JdeRobot is open-source software, licensed as GPL and LGPL. It also uses third-party softwares like Gazebo simulator, OpenGL, GTK, Qt, Player, Stage, Gazebo, GSL, OpenCV, PCL, Eigen, Ogre.

JdeRobot is a project developed by Robotics Group of Universidad Rey Juan Carlos (Madrid, Spain).

* Official Web Page: http://jderobot.org

# Installation

## Table of Contents

* [Getting environment ready](#env )
* [Installing from debian packages](#deb)
  * [Updating pakcages](#upd)
* [Installing from docker image](#dim)
* [Manual installation](#manual)
  * [Dependencies](#deps)
  * [Install from source](#source)



<a name="env"/>

### Getting environment ready

* Add the lastest ROS sources:

```
sudo sh -c 'echo "deb http://packages.ros.org/ros/ubuntu $(lsb_release -sc) main" > /etc/apt/sources.list.d/ros-latest.list'
sudo apt-key adv --keyserver 'hkp://keyserver.ubuntu.com:80' --recv-key C1CF6E31E6BADE8868B172B4F42ED6FBAB17C654
```

* Add the lastest Gazebo sources:

```
sudo sh -c 'echo "deb http://packages.osrfoundation.org/gazebo/ubuntu-stable `lsb_release -cs` main" > /etc/apt/sources.list.d/gazebo-stable.list'
sudo apt-key adv --keyserver keyserver.ubuntu.com --recv-key 67170598AF249743
```

* Add the lastest zeroc-ice sources:

```
sudo apt-key adv --keyserver keyserver.ubuntu.com --recv B6391CB2CFBA643D
sudo apt-add-repository "deb http://zeroc.com/download/Ice/3.7/ubuntu18.04 stable main"
```
* Add JdeRobot repository (using dedicated file /etc/apt/sources.list.d/jderobot.list):

```
sudo sh -c 'cat<<EOF>/etc/apt/sources.list.d/jderobot.list

deb [arch=amd64] http://wiki.jderobot.org/apt bionic main
EOF'
```

* Get and add the public key from the JdeRobot repository

```
sudo apt-key adv --keyserver keyserver.ubuntu.com --recv 24E521A4
```

* Update the repositories
```
sudo apt update
```



<a name="deb"/>
### Installing from debian packages (RECOMMENDED)

* Install JdeRobot:

```
sudo apt install jderobot
sudo apt install jderobot-gazebo-assets
```

* After installing the package, you can close the terminal and reopen it to source the environment variables, OR just type: 

```
source ~/.bashrc
```
<a name="upd"/>

#### Updating packages

* If you already have a previous version of the packages installed, you only have to do:
```
sudo apt update && sudo apt upgrade
```



<a name="dim"/>

### Installing from docker image

If you want to run JdeRobot in MS-Windows, MacOS or other Linux distributions you can use Docker containers. We have created a [Docker image](https://www.docker.com/) with current JdeRobot Release and all the necessary components to be used with [JdeRobot-Academy](http://wiki.jderobot.org/JdeRobot-Academy). To download it, use: 

```
docker pull jderobot/jderobot
```

For more information follow [this link](https://hub.docker.com/r/jderobot/jderobot/)  


<a name="manual"/>

### Manual installation

Downloading the source code from the GitHub is strongly NOT RECOMMENDED for new users unless you know what you are doing. 

<a name="deps"/>

#### Dependencies

You have two options here:
1. Install all the dependencies from a binary package
2. Install all the dependencies manually  (**NOT RECOMMENDED**)

For first one, you only have to type the following:

```
sudo apt install jderobot-deps-dev
```
and skip to the next section of this README.

For the second one... keep reading

JdeRobot has different external dependencies to build its structure. There are two types of dependencies: necessary dependencies and other dependencies. The firsts are needed to compile and install the basics of JdeRobot, that is, all its libraries and interfaces that are needed for the components or to develop components that use JdeRobot. The seconds are needed just for some components, but are not really necessary unless you want to use that components. For instance, the component gazeboserver uses Gazebo, a 3D robot simulator, so to use this component you may install Gazebo first.

First of all repeat the step [Getting environment ready](#env )

Some libraries are required to compile, link or run JdeRobot. Just type the following commands:

* **Basic libraries:**

```sudo apt install build-essential libtool cmake g++ gcc git make```

* **OpenGL libraries:**

```sudo apt install freeglut3 freeglut3-dev libgl1-mesa-dev libglu1-mesa```

* **GTK2 libraries:**

```
sudo apt install libgtk2.0-0 libgtk2.0-bin libgtk2.0-cil libgtk2.0-common libgtk2.0-dev libgtkgl2.0-1
sudo apt install libgtkgl2.0-dev libgtkglext1 libgtkglext1-dev libglademm-2.4-dev libgtkmm-2.4-dev 
sudo apt install libgnomecanvas2-0 libgnomecanvas2-dev  libgtkglext1-doc libgnomecanvasmm-2.6-dev
sudo apt install libgnomecanvasmm-2.6-1v5 libgtkglextmm-x11-1.2-0v5 libgtkglextmm-x11-1.2-dev
```

* **Gtk3 libraries:**

```sudo apt install libgoocanvasmm-2.0-6 libgoocanvasmm-2.0-dev```

* **GSL libraries:**

```sudo apt install libgsl23 gsl-bin libgsl-dev```

* **LibXML:**

```sudo apt install libxml++2.6-2v5 libxml++2.6-dev libtinyxml-dev```

* **Eigen:**

```sudo apt install libeigen3-dev```

* **Fireware:**

```sudo apt install libdc1394-22 libdc1394-22-dev```

* **USB:**

```sudo apt install libusb-1.0-0 libusb-1.0-0-dev```

* **CWIID:**

```sudo apt install libcwiid-dev```

* **Python components:**

```sudo apt install python-matplotlib python-pyqt5 python-pip python-numpy python-pyqt5.qtsvg```

* **Qfi**

It can be compiled and installed from source: https://github.com/JdeRobot/ThirdParty/tree/master/qflightinstruments

* **Qt 5**

```sudo apt install qtbase5-dev libqt5script5 libqt5svg5-dev```

* **Boost**

```sudo apt install libboost-system-dev libboost-filesystem-dev```

* **ROS**

```
sudo apt install ros-melodic-roscpp ros-melodic-std-msgs ros-melodic-cv-bridge ros-melodic-image-transport ros-melodic-roscpp-core ros-melodic-rospy ros-melodic-nav-msgs ros-melodic-geometry-msgs ros-melodic-mavros ros-melodic-gazebo-plugins
```

Once all ros packages are installed, install the script that tunes the environment variables ROS in your .bashrc configuration file, and run it for the current shell:

```
echo "source /opt/ros/melodic/setup.bash" >> ~/.bashrc 
source ~/.bashrc 
```

* **Google glog (logging)**

```sudo apt install libgoogle-glog-dev```

* **GStreamer**

```sudo apt-get install libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev```

* **ICE**

```sudo apt install libdb5.3-dev libdb5.3++-dev libssl-dev libbz2-dev libmcpp-dev```

compile ice:

```
git clone -b 3.7 https://github.com/zeroc-ice/ice.git 
cd ice/cpp
make CPP11=yes OPTIMIZE=yes
make install
```
Configure ICE for Python with pip
```
sudo pip2 install --upgrade pip
sudo pip2 install zeroc-ice==3.7.2
```

* **OpenNI 2**

```sudo apt-get install libopenni2-dev libopenni-dev```

* **Point Cloud Library**

```sudo apt-get install libpcl-dev```

* **OpenCV**

```sudo apt-get install libopencv-dev```

* **NodeJS**

```sudo apt-get install nodejs```

* **Kobuki robot libraries**

You can find the source code in our git repository (http://github.com/jderobot/thirdparty.git)

* **SDK Parrot for ArDrone**

If you want to install it manually from our third party repository, you only have to:

1. Create a folder to compile the code
```mkdir ardronelib-build && cd ardronelib-build```

2. Download the installer, a CMakeLists.txt file
```
wget https://raw.githubusercontent.com/RoboticsURJC/JdeRobot-ThirdParty/master/ardronelib/CMakeLists.txt
wget https://raw.githubusercontent.com/RoboticsURJC/JdeRobot-ThirdParty/master/ardronelib/ffmpeg-0.8.pc.in
wget https://raw.githubusercontent.com/RoboticsURJC/JdeRobot-ThirdParty/master/ardronelib/ardronelib.pc.in
```
3. Compile and install as usual:
```
cmake .
make
sudo make install
```

<a href="#source"/>

#### Install from source

After installing all the dependencies you can compile the project, you can clone this repo and build it doing the following:

-  Download the source code from git:

```
git clone http://github.com/RoboticsURJC/JdeRobot.git
cd JdeRobot/
```

- Check system and dependencies

```
mkdir build && cd build
cmake ..
```

- Compile

```
make
```

- Install

```
sudo make install
```

# How To Contribute
To see the collaborate workflow and coding style of `JdeRobot` community, please refer to the [wiki page](https://github.com/RoboticsURJC/JdeRobot/wiki/How-To-Contribute).

# Copyright and license


    Copyright 2015 - JderRobot Developers
    
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
=======
