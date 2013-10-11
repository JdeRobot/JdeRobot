IF (FALSE)

include(qibuild.cmake)

# Generar los ficheros de JdeRobot. 
#Los ficheros se crean los respectivos directorios
# en src/interfaces/cpp/ .
include(GenerateJderobot.cmake)

# Todos los directorios con headers
include_directories(
	interfaces/cpp/jderobot 
	interfaces/slice/jderobot 
	interfaces/cpp 
	interfaces/slice
	hinges
	functions
	functions/vision
	include
)

# Todas las fuentes del proyecto
set(_srcs
interfaces/cpp/jderobot/exceptions.cpp interfaces/cpp/jderobot/exceptions.h
interfaces/cpp/jderobot/containers.cpp interfaces/cpp/jderobot/containers.h
interfaces/cpp/jderobot/datetime.cpp interfaces/cpp/jderobot/datetime.h
interfaces/cpp/jderobot/common.cpp interfaces/cpp/jderobot/common.h
interfaces/cpp/jderobot/image.cpp interfaces/cpp/jderobot/image.h
interfaces/cpp/jderobot/camera.cpp interfaces/cpp/jderobot/camera.h
interfaces/cpp/jderobot/motors.cpp interfaces/cpp/jderobot/motors.h
interfaces/cpp/jderobot/encoders.cpp interfaces/cpp/jderobot/encoders.h
interfaces/cpp/jderobot/pose3dmotors.cpp interfaces/cpp/jderobot/pose3dmotors.h
interfaces/cpp/jderobot/pose3dencoders.cpp interfaces/cpp/jderobot/pose3dencoders.h
interfaces/cpp/jderobot/naomotions.cpp interfaces/cpp/jderobot/naomotions.h
interfaces/cpp/jderobot/naofollowball.cpp interfaces/cpp/jderobot/naofollowball.h

functions/vision/VisionDefinitions.h
functions/vision/ImageInput.cpp functions/vision/ImageInput.h
functions/vision/ImageHandler.cpp functions/vision/ImageHandler.h
functions/vision/colorFilter/SimpleColorFilter.cpp functions/vision/colorFilter/SimpleColorFilter.h
functions/Dictionary.cpp functions/Dictionary.h
#functions/NaoServerCamera.cpp functions/NaoServerCamera.h
functions/NaoServerMotors.cpp functions/NaoServerMotors.h
functions/NaoServerMotions.cpp functions/NaoServerMotions.h

hinges/Singleton.h
hinges/Common.h
hinges/JointControl.cpp hinges/JointControl.h
hinges/PIDController.cpp hinges/PIDController.h
hinges/HingeMotors.cpp hinges/HingeMotors.h
hinges/NeckMotors.cpp hinges/NeckMotors.h
#hinges/NeckSpeed.cpp hinges/NeckSpeed.h
hinges/LeftShoulderMotors.cpp hinges/LeftShoulderMotors.h
hinges/RightShoulderMotors.cpp hinges/RightShoulderMotors.h
hinges/LeftElbowMotors.cpp hinges/LeftElbowMotors.h
hinges/RightElbowMotors.cpp hinges/RightElbowMotors.h
hinges/LeftHipMotors.cpp hinges/LeftHipMotors.h
hinges/RightHipMotors.cpp hinges/RightHipMotors.h
hinges/LeftKneeMotors.cpp hinges/LeftKneeMotors.h
hinges/RightKneeMotors.cpp hinges/RightKneeMotors.h
hinges/LeftAnkleMotors.cpp hinges/LeftAnkleMotors.h
hinges/RightAnkleMotors.cpp hinges/RightAnkleMotors.h
#hinges/NaoFollowBall.cpp hinges/NaoFollowBall.h
naoserver.cpp naoserver.h
mainServer.cpp
)

qi_create_lib(naohead ${_srcs} SHARED SUBFOLDER naoqi)
#qi_create_bin(naohead ${_srcs})

qi_use_lib(naohead ALCOMMON LIBICEE OPENCV)

ENDIF()
