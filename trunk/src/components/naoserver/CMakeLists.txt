cmake_minimum_required(VERSION 2.8)
project(player)
include(qibuild.cmake)


option(PLAYER_IS_REMOTE
  "is player remote?"
  ON
)

option(WEBOTS
  "Are you using webots?"
  OFF
)

# Generar los ficheros de JdeRobot. 
#Los ficheros se crean los respectivos directorios
# en src/interfaces/cpp/ .
include(GenerateJderobot.cmake)

# Todos los directorios con headers
include_directories(
	src/interfaces/cpp/jderobot 
	src/interfaces/slice/jderobot 
	src/interfaces/cpp 
	src/interfaces/slice 
	src/Components 
	src/Modules 
	src/lib 
	src/lib/Sched 
	src/lib/vision
	src/Components/NaoServer 
	include
)

# Todas las fuentes del proyecto
set(_srcs
src/interfaces/cpp/jderobot/exceptions.cpp src/interfaces/cpp/jderobot/exceptions.h
src/interfaces/cpp/jderobot/containers.cpp src/interfaces/cpp/jderobot/containers.h
src/interfaces/cpp/jderobot/datetime.cpp src/interfaces/cpp/jderobot/datetime.h
src/interfaces/cpp/jderobot/common.cpp src/interfaces/cpp/jderobot/common.h
src/interfaces/cpp/jderobot/image.cpp src/interfaces/cpp/jderobot/image.h
src/interfaces/cpp/jderobot/camera.cpp src/interfaces/cpp/jderobot/camera.h
src/interfaces/cpp/jderobot/motors.cpp src/interfaces/cpp/jderobot/motors.h
src/interfaces/cpp/jderobot/encoders.cpp src/interfaces/cpp/jderobot/encoders.h
src/interfaces/cpp/jderobot/pose3dmotors.cpp src/interfaces/cpp/jderobot/pose3dmotors.h
src/interfaces/cpp/jderobot/pose3dencoders.cpp src/interfaces/cpp/jderobot/pose3dencoders.h

src/lib/vision/VisionDefinitions.h
src/lib/vision/ImageInput.cpp src/lib/vision/ImageInput.h
src/lib/vision/ImageHandler.cpp src/lib/vision/ImageHandler.h
src/lib/vision/colorFilter/SimpleColorFilter.cpp src/lib/vision/colorFilter/SimpleColorFilter.h
src/lib/Sched/CascadeScheduler.cpp src/lib/Sched/CascadeScheduler.h
src/lib/Dictionary.cpp src/lib/Dictionary.h
src/lib/Matrix.cpp src/lib/Matrix.h
src/lib/progeo.cpp src/lib/progeo.h
src/lib/JointControl.cpp src/lib/JointControl.h
src/lib/FSR.cpp src/lib/FSR.h
src/lib/Common.h
src/lib/PIDController.cpp src/lib/PIDController.h

src/Components/NaoServer/NaoServerCamera.cpp src/Components/NaoServer/NaoServerCamera.h
src/Components/NaoServer/NaoServerMotors.cpp src/Components/NaoServer/NaoServerMotors.h
src/Components/NaoServer/NaoServerEncoders.cpp src/Components/NaoServer/NaoServerEncoders.h
src/Components/NaoServer/NaoServerPose3DMotors.cpp src/Components/NaoServer/NaoServerPose3DMotors.h
src/Components/NaoServer/NaoServerPose3DEncoders.cpp src/Components/NaoServer/NaoServerPose3DEncoders.h
src/Components/Component.cpp src/Components/Component.h
src/Components/Perception.cpp src/Components/Perception.h
src/Components/Body.cpp src/Components/Body.h
src/Components/Head.cpp src/Components/Head.h
src/Components/Kinematics.cpp src/Components/Kinematics.h
src/Components/Kick.cpp src/Components/Kick.h
src/Components/SensorRecorderNao.cpp src/Components/SensorRecorderNao.h

src/Modules/Coach.cpp
src/Player.cpp src/Player.h
)

if(WEBOTS)
	 add_definitions(" -DWEBOTS ")
endif()

if(PLAYER_IS_REMOTE)
  add_definitions(" -DPLAYER_IS_REMOTE ")
  qi_create_bin(player ${_srcs})
else()
  qi_create_lib(player ${_srcs} SHARED SUBFOLDER naoqi)
endif()

qi_use_lib(player ALCOMMON LIBICEE OPENCV)

