FROM ubuntu:xenial


LABEL manteiner Aitor Martínez Fernández+aitor.martinez.fernandez@gmail.com

########## USAGE ##############

LABEL Usage.run="docker run -itP --rm -p 7681:7681 jderobot/jderobot"
LABEL Usage.camserver="docker run -itP --rm jderobot/jderobot video [video_name]"
LABEL Usage.listVideos="docker run --rm jderobot/jderobot lsvideo"



########## setup Repositories ##############

## ROS ##
RUN apt-key adv --keyserver keyserver.ubuntu.com --recv-key 421C365BD9FF1F717815A3895523BAEEB01FA116
RUN echo "deb http://packages.ros.org/ros/ubuntu xenial main" > /etc/apt/sources.list.d/ros-latest.list

## ZeroC ##
RUN sh -c 'echo deb http://zeroc.com/download/apt/ubuntu16.04 stable main > /etc/apt/sources.list.d/zeroc.list'
RUN apt-key adv --keyserver keyserver.ubuntu.com --recv 5E6DA83306132997

## Gazebo ##
RUN echo "deb http://packages.osrfoundation.org/gazebo/ubuntu-stable xenial main" > /etc/apt/sources.list.d/gazebo-stable.list
RUN apt-key adv --keyserver keyserver.ubuntu.com --recv-key 67170598AF249743


## JdeRobot ##
RUN sh -c 'echo "deb http://jderobot.org/apt xenial main" > /etc/apt/sources.list.d/jderobot.list'
RUN apt-key adv --keyserver keyserver.ubuntu.com --recv B0E7F58E82C8091DF945A0453DA08892EE69A25C


########## Install JdeRobot ##############
RUN apt-get update && apt-get -y  install \
    python-pip \
    libssl-dev libbz2-dev \
    && rm -rf /var/lib/apt/lists/*

RUN apt-get update && apt-get -y  install \
	jderobot \
	xvfb \
	nano \
	wget \
	&& rm -rf /var/lib/apt/lists/*
	


####### Install gzweb dependences ########
RUN apt-get update && apt-get install -q -y \
    build-essential \
    cmake \
    libgazebo7-dev \
    imagemagick \
    libboost-dev \
    libgts-dev \
    libjansson-dev \
    libtinyxml-dev \
    mercurial \
    nodejs \
    nodejs-legacy \
    npm \
    pkg-config \
    psmisc \
    && rm -rf /var/lib/apt/lists/*



####### clone gzweb #######
RUN hg clone https://bitbucket.org/aitormf/gzweb ~/gzweb

####### build gzweb #######
COPY ./installGzweb.sh /
RUN /installGzweb.sh




####### setup environment JdeRobot #######
EXPOSE 8990
EXPOSE 8991
EXPOSE 8992
EXPOSE 8993
EXPOSE 8994
EXPOSE 8995
EXPOSE 8996
EXPOSE 8997
EXPOSE 8998
EXPOSE 8999
EXPOSE 9000
EXPOSE 9001
EXPOSE 9800
EXPOSE 9900
EXPOSE 9989
EXPOSE 9990
EXPOSE 9991
EXPOSE 9992
EXPOSE 9993
EXPOSE 9994
EXPOSE 9995
EXPOSE 9996
EXPOSE 9997
EXPOSE 9998
EXPOSE 9999

####### setup environment GzWeb #######
EXPOSE 8080
EXPOSE 7681

####### WebSockets for kobukiviewerjs #######

EXPOSE 7777
EXPOSE 11000
EXPOSE 11001



########## Configurations for Teaching Robotics ##############
COPY cfg /cfg

COPY bin /usr/bin

RUN mkdir /videos

RUN cd /videos && \
    wget http://jderobot.org/store/amartinflorido/uploads/curso/pelota_roja.avi
    
RUN cd /videos && \
    wget http://jderobot.org/store/amartinflorido/uploads/curso/pelotas_roja_azul.avi

RUN cd /videos && \
    wget http://jderobot.org/store/amartinflorido/uploads/curso/drone1.mp4

RUN cd /videos && \
    wget http://jderobot.org/store/amartinflorido/uploads/curso/drone2.mp4

COPY ./turtlebotplugin.cfg /opt/jderobot/share/jderobot/conf




########## ENTRYPOINT ##############
COPY ./jderobot_entrypoint.sh /

ENTRYPOINT ["/jderobot_entrypoint.sh"]

CMD ["bash"]




