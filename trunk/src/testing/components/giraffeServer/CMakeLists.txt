cmake_minimum_required(VERSION 2.6)
PROJECT( jointmotorComp )

# Sources set
SET ( SOURCES
  jointmotorComp.cpp
  jointmotorI.cpp
  worker.cpp
  dynamixel.cpp
  #fakehandler.cpp
  #megaroboticshandler.cpp
  monitor.cpp
  servo.cpp
  $ENV{ROBOCOMP}/Classes/rapplication/rapplication.cpp
  $ENV{ROBOCOMP}/Classes/q4serialport/q4serialport.cpp

)

# Headers set
SET ( HEADERS
  worker.h
  monitor.h
  $ENV{ROBOCOMP}/Classes/q4serialport/q4serialport.h
)

# RoboComp
INCLUDE( $ENV{ROBOCOMP}/CMake/robocomp.cmake )
ROBOCOMP_INITIALIZE( $ENV{ROBOCOMP}/ )
ROBOCOMP_WRAP_ICE( JointMotor)

SET (EXECUTABLE_OUTPUT_PATH $ENV{ROBOCOMP}/Components/HAL/jointmotorComp/bin)

# IPP
# INCLUDE( $ENV{ROBOCOMP}/CMake/ipp.cmake )

# Qt4
ADD_DEFINITIONS( -Wall -DQT_DLL -DQT_GUI_LIB -DQT_CORE_LIB )
FIND_PACKAGE( Qt4 REQUIRED )
SET( QT_USE_QTGUI TRUE )
SET( QT_USE_QTOPENGL TRUE )
INCLUDE( ${QT_USE_FILE} )
QT4_WRAP_CPP( MOC_SOURCES ${HEADERS} )

# Gazebo
INCLUDE( $ENV{ROBOCOMP}/CMake/gazebo.cmake )
IF (GAZEBO_FOUND EQUAL 0)
  MESSAGE(STATUS "\n\nGazebo motor handle will NOT be built!\n\n")
ELSE (GAZEBO_FOUND EQUAL 0)
  MESSAGE(STATUS "\n\nGazebo motor handle WILL be built!\n\n")
ENDIF (GAZEBO_FOUND EQUAL 0)

# Specify construction and link process
ADD_EXECUTABLE( jointmotorComp ${SOURCES} ${MOC_SOURCES} ${RC_SOURCES} ${UI_HEADERS} )
TARGET_LINK_LIBRARIES( jointmotorComp ${QT_LIBRARIES} ${LIBS})

INSTALL(FILES ${EXECUTABLE_OUTPUT_PATH}/jointmotorComp DESTINATION /opt/robocomp/bin/ )
