# Jderobot for developers
FROM jderobot/jderobot:dev-only-ice

LABEL manteiner Aitor Martínez Fernández+aitor.martinez.fernandez@gmail.com


# install ROS deps
RUN apt update && apt install -q -y \
    ros-kinetic-roscpp ros-kinetic-std-msgs ros-kinetic-cv-bridge ros-kinetic-image-transport \
    ros-kinetic-roscpp-core ros-kinetic-rospy ros-kinetic-nav-msgs ros-kinetic-geometry-msgs \
    ros-kinetic-kobuki-gazebo ros-kinetic-kobuki ros-kinetic-kobuki-core ros-kinetic-rplidar-ros ros-kinetic-urg-node\
    && rm -rf /var/lib/apt/lists/*


CMD ["bash"]
