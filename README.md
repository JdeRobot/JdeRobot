
JdeRobot
========

# Introduction


JdeRobot is a software development suite for robotics, home-automation and computer vision applications. These domains include sensors (for instance, cameras), actuators, and intelligent software in between. It has been designed to help in programming such intelligent software. It is mainly written in C++ language and provides a distributed component-based programming environment where the application program is made up of a collection of several concurrent asynchronous components. Each component may run in different computers and they are connected using ICE communication middleware. Components may be written in C++, python, Java... and all of them interoperate through explicit ICE interfaces.

JdeRobot simplifies the access to hardware devices from the control program. Getting sensor measurements is as simple as calling a local function, and ordering motor commands as easy as calling another local function. The platform attaches those calls to the remote invocation on the components connected to the sensor or the actuator devices. They can be connected to real sensors and actuators or simulated ones, both locally or remotely using the network. Those functions build the API for the Hardware Abstraction Layer. The robotic application get the sensor readings and order the actuator commands using it to unfold its behavior. Several driver components have been developed to support different physical sensors, actuators and simulators. The drivers are used as components installed at will depending on your configuration. They are included in the official release. Currently supported robots and devices:

RGBD sensors: Kinect from Microsoft, Asus Xtion
Pioneer robot from MobileRobotics Inc.
Kobuki robot (TurtleBot) from Yujin Robot
Nao humanoid from Aldebaran
ArDrone quadrotor from Parrot
Firewire cameras, USB cameras, video files (mpeg, avi...), IP cameras (like Axis)
Pantilt unit PTU-D46 from Directed Perception Inc.
Laser Scanners: LMS from SICK and URG from Hokuyo
EVI PTZ camera from Sony
Gazebo and Stage simulators
Wiimote
X10 home automation devices
JdeRobot includes several robot programming tools and libraries. First, viewers and teleoperators for several robots, its sensors and motors. Second, a camara calibration component and a tunning tool for color filters. Third, VisualHFSM tool for programming robot behavior using hierarchical finite state machines. It includes many sample components using OpenCV, PCL, OpenGL, etc.. In addition, it also provides a library to develop fuzzy controllers, a library for projective geometry and some computer vision processing.

Each component may have its own independent Graphical User Interface or none at all. Currently, GTK and Qt libraries are supported, and several examples of OpenGL for 3D graphics with both libraries are included.

JdeRobot is open-source software, licensed as GPL and LGPL. It also uses third-party software like Gazebo simulator, OpenGL, GTK, Qt, Player, Stage, Gazebo, GSL, OpenCV, PCL, Eigen, Ogre.

JdeRobot is a project developed by Robotics Group of Universidad Rey Juan Carlos (Madrid, Spain).

* Official Web Page: http://jderobot.org

# Dependencies

```sh
$ sudo aptitude install build-essential libtinyxml-dev libtbb-dev libxml2-dev libqt4-dev pkg-config libprotoc-dev libfreeimage-dev libprotobuf-dev protobuf-compiler libboost-all-dev freeglut3-dev cmake libogre-dev libtar-dev libcurl4-openssl-dev libcegui-mk2-dev libswscale-dev libavformat-dev libavcodec-dev 
```

For other dependencies, like the `GTK` stuff, refer to the [Installation Manual](http://jderobot.org/Manual-5#Installing_JdeRobot_5). You may want to install the missing dependencies according to your `cmake` output.

# Download source code

```sh
$ git clone https://github.com/RoboticsURJC/JdeRobot
```
# Compilation
```sh
$ cmake .
$ make 
```

### Compilation by components

```sh
$ cmake -Dbuild-default=OFF -Dbuild_introrob=ON .
$ make
```

# How To Contricute
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
