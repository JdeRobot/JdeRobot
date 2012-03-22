# Install script for directory: /home/mikel/Escritorio/CMAKE_jderobot

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/usr/local")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

# Install shared libraries without execute permission?
IF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  SET(CMAKE_INSTALL_SO_NO_EXE "1")
ENDIF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/lib/jderobot" TYPE FILE FILES
    "/home/mikel/Escritorio/CMAKE_jderobot/src/libs/colorspaces/libcolorspacesmm.so"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/libs/colorspaces/libcolorspaces.so"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/libs/fuzzylib/libfuzzylib.so"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/libs/jderobotice/libjderobotice.so"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/libs/visionlib/libvisionlib.so"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/libs/progeo/libprogeo.so"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/libs/bgfgsegmentation/libbgfgsegmentation.so"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/libs/pioneer/libpioneer.so"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/libs/jderobotutil/libjderobotutil.so"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/include/jderobot/colorspaces" TYPE FILE FILES "/home/mikel/Escritorio/CMAKE_jderobot/src/libs/colorspaces/imagecv.h")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/include/jderobot/colorspaces" TYPE FILE FILES "/home/mikel/Escritorio/CMAKE_jderobot/src/libs/colorspaces/colorspaces.h")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/include/jderobot/colorspaces" TYPE FILE FILES "/home/mikel/Escritorio/CMAKE_jderobot/src/libs/colorspaces/uncopyable.h")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/include/jderobot/colorspaces" TYPE FILE FILES "/home/mikel/Escritorio/CMAKE_jderobot/src/libs/colorspaces/colorspacesmm.h")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/include/jderobot/fuzzylib" TYPE FILE FILES "/home/mikel/Escritorio/CMAKE_jderobot/src/libs/fuzzylib/fuzzylib.h")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/include/jderobot/colorspacesice" TYPE FILE FILES "/home/mikel/Escritorio/CMAKE_jderobot/src/libs/colorspacesice/image.h")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/include/jderobot/jderobotice" TYPE FILE FILES "/home/mikel/Escritorio/CMAKE_jderobot/src/libs/jderobotice/exceptions.h")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/include/jderobot/jderobotice" TYPE FILE FILES "/home/mikel/Escritorio/CMAKE_jderobot/src/libs/jderobotice/catchutils.h")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/include/jderobot/jderobotice" TYPE FILE FILES "/home/mikel/Escritorio/CMAKE_jderobot/src/libs/jderobotice/tracerImpl.h")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/include/jderobot/jderobotice" TYPE FILE FILES "/home/mikel/Escritorio/CMAKE_jderobot/src/libs/jderobotice/jderobotice.h")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/include/jderobot/jderobotice" TYPE FILE FILES "/home/mikel/Escritorio/CMAKE_jderobot/src/libs/jderobotice/statusImpl.h")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/include/jderobot/jderobotice" TYPE FILE FILES "/home/mikel/Escritorio/CMAKE_jderobot/src/libs/jderobotice/componentthread.h")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/include/jderobot/jderobotice" TYPE FILE FILES "/home/mikel/Escritorio/CMAKE_jderobot/src/libs/jderobotice/component.h")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/include/jderobot/jderobotice" TYPE FILE FILES "/home/mikel/Escritorio/CMAKE_jderobot/src/libs/jderobotice/context.h")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/include/jderobot/jderobotice" TYPE FILE FILES "/home/mikel/Escritorio/CMAKE_jderobot/src/libs/jderobotice/subsystemthread.h")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/include/jderobot/jderobotice" TYPE FILE FILES "/home/mikel/Escritorio/CMAKE_jderobot/src/libs/jderobotice/interfaceconnect.h")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/include/jderobot/jderobotice" TYPE FILE FILES "/home/mikel/Escritorio/CMAKE_jderobot/src/libs/jderobotice/application.h")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/include/jderobot/visionlib" TYPE FILE FILES "/home/mikel/Escritorio/CMAKE_jderobot/src/libs/visionlib/image.h")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/include/jderobot/visionlib" TYPE FILE FILES "/home/mikel/Escritorio/CMAKE_jderobot/src/libs/visionlib/visionlib.h")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/include/jderobot/visionlib" TYPE FILE FILES "/home/mikel/Escritorio/CMAKE_jderobot/src/libs/visionlib/structs.h")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/include/jderobot/visionlib" TYPE FILE FILES "/home/mikel/Escritorio/CMAKE_jderobot/src/libs/visionlib/linesDetection.h")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/include/jderobot/visionlib" TYPE FILE FILES "/home/mikel/Escritorio/CMAKE_jderobot/src/libs/visionlib/geometry.h")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/include/jderobot/visionlib" TYPE FILE FILES "/home/mikel/Escritorio/CMAKE_jderobot/src/libs/visionlib/cvfast.h")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/include/jderobot/progeo" TYPE FILE FILES "/home/mikel/Escritorio/CMAKE_jderobot/src/libs/progeo/progeo.h")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/include/jderobot/bgfgsegmentation" TYPE FILE FILES "/home/mikel/Escritorio/CMAKE_jderobot/src/libs/bgfgsegmentation/bgfgsegmentation.h")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/include/jderobot/bgfgsegmentation" TYPE FILE FILES "/home/mikel/Escritorio/CMAKE_jderobot/src/libs/bgfgsegmentation/bgmodelfactory.h")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/include/jderobot/pioneer" TYPE FILE FILES "/home/mikel/Escritorio/CMAKE_jderobot/src/libs/pioneer/pioneer.h")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/include/jderobot/jderobotutil" TYPE FILE FILES "/home/mikel/Escritorio/CMAKE_jderobot/src/libs/jderobotutil/paramdict.h")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/include/jderobot/jderobotutil" TYPE FILE FILES "/home/mikel/Escritorio/CMAKE_jderobot/src/libs/jderobotutil/jderobotutil.h")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/include/jderobot/jderobotutil" TYPE FILE FILES "/home/mikel/Escritorio/CMAKE_jderobot/src/libs/jderobotutil/time.h")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/include/jderobot/jderobotutil" TYPE FILE FILES "/home/mikel/Escritorio/CMAKE_jderobot/src/libs/jderobotutil/observer.h")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/include/jderobot/jderobotutil" TYPE FILE FILES "/home/mikel/Escritorio/CMAKE_jderobot/src/libs/jderobotutil/uncopyable.h")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/bin" TYPE FILE PERMISSIONS OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE FILES "/home/mikel/Escritorio/CMAKE_jderobot/src/components/introrob/introrob")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/bin" TYPE FILE PERMISSIONS OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE FILES "/home/mikel/Escritorio/CMAKE_jderobot/src/components/basic_component/basic_component")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/bin" TYPE FILE PERMISSIONS OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE FILES "/home/mikel/Escritorio/CMAKE_jderobot/src/components/opencvdemo/opencvdemo")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/bin" TYPE FILE PERMISSIONS OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE FILES "/home/mikel/Escritorio/CMAKE_jderobot/src/components/colortuner/colortuner")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/bin" TYPE FILE PERMISSIONS OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE FILES "/home/mikel/Escritorio/CMAKE_jderobot/src/components/gazeboserver/gazeboserver")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/bin" TYPE FILE PERMISSIONS OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE FILES "/home/mikel/Escritorio/CMAKE_jderobot/src/components/replayer/replayer")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/bin" TYPE FILE PERMISSIONS OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE FILES "/home/mikel/Escritorio/CMAKE_jderobot/src/components/giraffeclient/giraffeclient")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/bin" TYPE FILE PERMISSIONS OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE FILES "/home/mikel/Escritorio/CMAKE_jderobot/src/components/varcolorserver/varcolorserver")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/bin" TYPE FILE PERMISSIONS OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE FILES "/home/mikel/Escritorio/CMAKE_jderobot/src/components/introrob-old/introrob-old")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/bin" TYPE FILE PERMISSIONS OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE FILES "/home/mikel/Escritorio/CMAKE_jderobot/src/components/varcolorviewgtkmm/varcolorviewgtkmm")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/bin" TYPE FILE PERMISSIONS OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE FILES "/home/mikel/Escritorio/CMAKE_jderobot/src/components/kinectServer/kinectServer")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/bin" TYPE FILE PERMISSIONS OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE FILES "/home/mikel/Escritorio/CMAKE_jderobot/src/components/cameraserver/cameraserver")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/bin" TYPE FILE PERMISSIONS OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE FILES "/home/mikel/Escritorio/CMAKE_jderobot/src/components/cameraview/cameraview")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/bin" TYPE FILE PERMISSIONS OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE FILES "/home/mikel/Escritorio/CMAKE_jderobot/src/components/calibrator/calibrator")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/bin" TYPE FILE PERMISSIONS OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE FILES "/home/mikel/Escritorio/CMAKE_jderobot/src/components/naooperator/naooperator")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/bin" TYPE FILE PERMISSIONS OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE FILES "/home/mikel/Escritorio/CMAKE_jderobot/src/components/bgfglab/bgfglab")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/bin" TYPE FILE PERMISSIONS OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE FILES "/home/mikel/Escritorio/CMAKE_jderobot/src/components/recorder/recorder")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/bin" TYPE FILE PERMISSIONS OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE FILES "/home/mikel/Escritorio/CMAKE_jderobot/src/components/cameraview_icestorm/cameraview_icestorm")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/include/jderobot/jderobot" TYPE FILE FILES
    "/home/mikel/Escritorio/CMAKE_jderobot/src/interfaces/cpp/jderobot/laser.h"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/interfaces/cpp/jderobot/pose3dmotors.h"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/interfaces/cpp/jderobot/image.h"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/interfaces/cpp/jderobot/jcm.h"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/interfaces/cpp/jderobot/camera.h"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/interfaces/cpp/jderobot/common.h"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/interfaces/cpp/jderobot/sonars.h"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/interfaces/cpp/jderobot/exceptions.h"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/interfaces/cpp/jderobot/bodyencoders.h"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/interfaces/cpp/jderobot/bodymotors.h"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/interfaces/cpp/jderobot/body.h"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/interfaces/cpp/jderobot/kinectleds.h"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/interfaces/cpp/jderobot/ptencoders.h"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/interfaces/cpp/jderobot/jointmotor.h"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/interfaces/cpp/jderobot/pointcloud.h"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/interfaces/cpp/jderobot/pose3dencoders.h"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/interfaces/cpp/jderobot/varcolor.h"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/interfaces/cpp/jderobot/bodymovements.h"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/interfaces/cpp/jderobot/containers.h"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/interfaces/cpp/jderobot/motors.h"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/interfaces/cpp/jderobot/datetime.h"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/interfaces/cpp/jderobot/ptmotors.h"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/interfaces/cpp/jderobot/cloudPoints.h"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/interfaces/cpp/jderobot/encoders.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/include/jderobot/slice" TYPE FILE FILES
    "/home/mikel/Escritorio/CMAKE_jderobot/src/interfaces/slice/jderobot/varcolor.ice"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/interfaces/slice/jderobot/cloudPoints.ice"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/interfaces/slice/jderobot/pointcloud.ice"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/interfaces/slice/jderobot/ptencoders.ice"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/interfaces/slice/jderobot/camera.ice"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/interfaces/slice/jderobot/laser.ice"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/interfaces/slice/jderobot/jointmotor.ice"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/interfaces/slice/jderobot/image.ice"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/interfaces/slice/jderobot/pose3dencoders.ice"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/interfaces/slice/jderobot/bodyencoders.ice"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/interfaces/slice/jderobot/encoders.ice"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/interfaces/slice/jderobot/bodymovements.ice"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/interfaces/slice/jderobot/common.ice"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/interfaces/slice/jderobot/ptmotors.ice"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/interfaces/slice/jderobot/motors.ice"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/interfaces/slice/jderobot/bodymotors.ice"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/interfaces/slice/jderobot/kinectleds.ice"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/interfaces/slice/jderobot/body.ice"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/interfaces/slice/jderobot/pose3dmotors.ice"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/interfaces/slice/jderobot/sonars.ice"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/interfaces/slice/jderobot/exceptions.ice"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/interfaces/slice/jderobot/datetime.ice"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/interfaces/slice/jderobot/containers.ice"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/interfaces/slice/jderobot/jcm.ice"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/share/jderobot/conf" TYPE FILE FILES
    "/home/mikel/Escritorio/CMAKE_jderobot/src/components/teleoperator/teleoperator.cfg"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/components/introrob/robot1introrob.cfg"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/components/introrob/robot2introrob.cfg"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/components/introrob/introrob.cfg"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/components/basic_component/basic_component.cfg"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/components/opencvdemo/opencvdemo.cfg"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/components/colortuner/colortuner2.3.1/colorTuner.cfg"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/components/colortuner/colorTuner.cfg"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/components/gazeboserver/robot2gazeboserver.cfg"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/components/gazeboserver/gazeboserver.cfg"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/components/gazeboserver/robot1gazeboserver.cfg"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/components/motiondetection/motiondetection.cfg"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/components/alarmgenerator/alarmgenerator.cfg"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/components/kinectViewer/kinectViewer.cfg"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/components/kinectViewer/config/lambecom.cfg"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/components/replayer/replayer.cfg"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/components/giraffeclient/giraffeclient.cfg"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/components/openniServer/openniServer.cfg"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/components/deprecated/varcolorserver/varcolorserver.cfg"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/components/deprecated/introrob-old/introrob.cfg"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/components/deprecated/varcolorviewgtkmm/varcolorviewgtkmm.cfg"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/components/kinectServer/kinectServer.cfg"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/components/cameraserver/cameraserver.cfg"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/components/giraffeServer/giraffeServer.cfg"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/components/cameraview/cameraview.cfg"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/components/naobody/naobody.cfg"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/components/calibrator/calibrator.cfg"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/components/naooperator/naooperator.cfg"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/components/playerserver/playerserver.cfg"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/components/bgfglab/bgfglab.cfg"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/components/recorder/recorder.cfg"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/components/cameraview_icestorm/cameraview_icestorm.cfg"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/share/jderobot/glade" TYPE FILE FILES
    "/home/mikel/Escritorio/CMAKE_jderobot/src/components/teleoperator/teleoperatorgui.glade"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/components/introrob/introrob.glade"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/components/basic_component/basic_component.glade"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/components/opencvdemo/opencvdemo.glade"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/components/motiondetection/motiondetection.glade"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/components/kinectViewer/kinectViewergui.glade"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/components/giraffeclient/giraffeclient.glade"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/components/deprecated/introrob-old/introrob.glade"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/components/deprecated/varcolorviewgtkmm/varcolorviewgtkmm.glade"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/components/cameraview/cameraview.glade"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/components/calibrator/calibrator.glade"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/components/naooperator/naooperatorgui.glade"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/components/bgfglab/bgfglab.glade"
    "/home/mikel/Escritorio/CMAKE_jderobot/src/components/cameraview_icestorm/cameraview.glade"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  INCLUDE("/home/mikel/Escritorio/CMAKE_jderobot/build/src/cmake_install.cmake")

ENDIF(NOT CMAKE_INSTALL_LOCAL_ONLY)

IF(CMAKE_INSTALL_COMPONENT)
  SET(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
ELSE(CMAKE_INSTALL_COMPONENT)
  SET(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
ENDIF(CMAKE_INSTALL_COMPONENT)

FILE(WRITE "/home/mikel/Escritorio/CMAKE_jderobot/build/${CMAKE_INSTALL_MANIFEST}" "")
FOREACH(file ${CMAKE_INSTALL_MANIFEST_FILES})
  FILE(APPEND "/home/mikel/Escritorio/CMAKE_jderobot/build/${CMAKE_INSTALL_MANIFEST}" "${file}\n")
ENDFOREACH(file)
