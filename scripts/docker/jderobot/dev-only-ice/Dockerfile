# Jderobot for developers without ROS
# only use ROS for opencv
FROM jderobot/ubuntu:base

LABEL manteiner Aitor Martínez Fernández+aitor.martinez.fernandez@gmail.com


## ROS ##
RUN apt-key adv --keyserver keyserver.ubuntu.com --recv-key 421C365BD9FF1F717815A3895523BAEEB01FA116

RUN echo "deb http://packages.ros.org/ros/ubuntu xenial main" > /etc/apt/sources.list.d/ros-latest.list

## ZeroC ##
RUN echo deb http://zeroc.com/download/apt/ubuntu16.04 stable main > /etc/apt/sources.list.d/zeroc.list

RUN apt-key adv --keyserver keyserver.ubuntu.com --recv 5E6DA83306132997

## Gazebo ##

RUN echo "deb http://packages.osrfoundation.org/gazebo/ubuntu-stable xenial main" > /etc/apt/sources.list.d/gazebo-stable.list

RUN apt-key adv --keyserver keyserver.ubuntu.com --recv-key 67170598AF249743


## JdeRobot ##
RUN sh -c 'echo "deb http://jderobot.org/apt xenial main" > /etc/apt/sources.list.d/jderobot.list'

RUN apt-key adv --keyserver keyserver.ubuntu.com --recv B0E7F58E82C8091DF945A0453DA08892EE69A25C



## install deps ##

# Basic libraries
RUN apt update && apt install -q -y \
    build-essential libtool cmake g++ gcc git make \
    && rm -rf /var/lib/apt/lists/*

# OpenGL
RUN apt update && apt install -q -y \
    freeglut3 freeglut3-dev libgl1-mesa-dev libglu1-mesa \
    && rm -rf /var/lib/apt/lists/*

# GTK2
RUN apt update && apt install -q -y \
    libgtk2.0-0 libgtk2.0-bin libgtk2.0-cil libgtk2.0-common libgtk2.0-dev libgtkgl2.0-1 \
    libgtkgl2.0-dev libgtkglext1 libgtkglext1-dev libglademm-2.4-dev libgtkmm-2.4-dev \
    libgnomecanvas2-0 libgnomecanvas2-dev  libgtkglext1-doc libgnomecanvasmm-2.6-dev \
    libgnomecanvasmm-2.6-1v5 libgtkglextmm-x11-1.2-0v5 libgtkglextmm-x11-1.2-dev \
    && rm -rf /var/lib/apt/lists/*

# GTK3
RUN apt update && apt install -q -y \
    libgoocanvasmm-2.0-6 libgoocanvasmm-2.0-dev \
    && rm -rf /var/lib/apt/lists/*

# GSL
RUN apt update && apt install -q -y \
    libgsl2 gsl-bin libgsl-dev \
    && rm -rf /var/lib/apt/lists/*

# LibXML
RUN apt update && apt install -q -y \
    libxml++2.6-2v5 libxml++2.6-dev libtinyxml-dev \
    && rm -rf /var/lib/apt/lists/*

# EIGEN
RUN apt update && apt install -q -y \
    libeigen3-dev \
    && rm -rf /var/lib/apt/lists/*

# FIREWARE
RUN apt update && apt install -q -y \
    libdc1394-22 libdc1394-22-dev \
    && rm -rf /var/lib/apt/lists/*

# USB
RUN apt update && apt install -q -y \
    libusb-1.0-0 libusb-1.0-0-dev \
    && rm -rf /var/lib/apt/lists/*

# CWIID
RUN apt update && apt install -q -y \
    libcwiid-dev \
    && rm -rf /var/lib/apt/lists/*

# PYTHON
RUN apt update && apt install -q -y \
    python-matplotlib python-pyqt5 python-pip python-numpy python-pyqt5.qtsvg \
    && rm -rf /var/lib/apt/lists/*

# QFI
RUN apt update && apt install -q -y \
    qfi \
    && rm -rf /var/lib/apt/lists/*

# QT5
RUN apt update && apt install -q -y \
    qtbase5-dev libqt5declarative5 libqt5script5 libqt5svg5-dev \
    && rm -rf /var/lib/apt/lists/*

# BOOST
RUN apt update && apt install -q -y \
    libboost-system-dev libboost-filesystem-dev \
    && rm -rf /var/lib/apt/lists/*

# GSTREAMER
RUN apt update && apt install -q -y \
    libgstreamer0.10-dev libgstreamer-plugins-base0.10-dev \
    && rm -rf /var/lib/apt/lists/*

# ICE
RUN apt update && apt install -q -y \
    libzeroc-ice3.6 zeroc-ice-utils libzeroc-icestorm3.6 zeroc-ice-slice libzeroc-ice-dev \
    && rm -rf /var/lib/apt/lists/*

# ICE PYTHON
RUN apt update && apt install -q -y \
    libssl-dev libbz2-dev \
    && pip2 install zeroc-ice==3.6.3 \
    && rm -rf /var/lib/apt/lists/*

# OPENNI 2
RUN apt update && apt install -q -y \
    libopenni2-dev libopenni-dev \
    && rm -rf /var/lib/apt/lists/*

# GAZEBO
RUN apt update && apt install -q -y \
    gazebo7 libgazebo7-dev \
    && rm -rf /var/lib/apt/lists/*

# PCL
RUN apt update && apt install -q -y \
    libpcl-dev \
    && rm -rf /var/lib/apt/lists/*

# OPENCV
RUN apt update && apt install -q -y \
    ros-kinetic-opencv3 \
    && rm -rf /var/lib/apt/lists/*

# NODEJS
RUN apt update && apt install -q -y \
    nodejs \
    && rm -rf /var/lib/apt/lists/*


# ARDRONELIB
RUN apt update && apt install -q -y \
    ardronelib \
    && rm -rf /var/lib/apt/lists/*

# GLOGS
RUN apt update && apt install -q -y \
    libgoogle-glog-dev \
    && rm -rf /var/lib/apt/lists/*

# KURT
#RUN apt update && apt install -q -y \
#    kurt-jderobot \
#    && rm -rf /var/lib/apt/lists/*

# SCRTACH2JDEROBOT DEPS
RUN  pip2 install termcolor==1.1.0 parse==1.8.2 imutils==0.4.3


COPY ./testPR /bin/

RUN git config --global user.name "JdeRobot" && git config --global user.email johndoe@example.com


WORKDIR /root



CMD ["bash"]