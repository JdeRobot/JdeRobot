
JdeRobot
========

# Introduction

*JdeRobot* is an open source software framework suite for robotics and
 vision distributed applications written in C++ language. It provides a
 programming environment where the robot control program is made up of
 a collection of several concurrent asynchronous threads named
 schemas. It is based on JDE cognitive architecture for autonomous
 robots. The underlying theoretical foundations can be found in the
 related publications.

JdeRobot simplifies the access to robotic hardware from the control
program. Getting sensor measurements is as simple as reading a local
variable, and ordering motor commands as easy as writing an actuator
variable. The platform updates those sensor variables with fresh
readings and implements such actuator variables. All of them together
set a shared variable API for the robot programming. The robotic
application reads and writes such variables to unfold its behavior.
They can be connected to real sensors and actuators or simulated ones,
both locally or remotely using the network. Several drivers have been
developed to support different physical sensors, actuators and
simulators. The drivers are used as plugins installed at will
depending on your configuration. They are included in the official
release.

On top of such variable API there may be perceptive and actuation
schemas as building blocks of the robotic application. Perceptive
schemas make some data processing to provide information about the
world or the robot. Actuation schemas make decisions in order to reacho
r maintain some goal. They order motor commands or activate new
schemas, because the schemas can be combined forming hierarchies.


* Official Web Page: http://jderobot.org

# Dependencies

```sh
$ sudo aptitude install build-essential libtinyxml-dev libtbb-dev libxml2-dev libqt4-dev pkg-config libprotoc-dev libfreeimage-dev libprotobuf-dev protobuf-compiler libboost-all-dev freeglut3-dev cmake libogre-dev libtar-dev libcurl4-openssl-dev libcegui-mk2-dev libswscale-dev libavformat-dev libavcodec-dev 
```
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
