HEADERS += handler.h worker.h monitor.h config.h dynamixel.h JointMotor.h jointmotorI.h servo.h q4serialport/q4serialport.h rapplication/rapplication.h
SOURCES += jointmotorComp.cpp worker.cpp monitor.cpp dynamixel.cpp JointMotor.cpp jointmotorI.cpp servo.cpp q4serialport/q4serialport.cpp

CONFIG += qt

INCLUDEPATH += . /usr/include
LIBS = -lIce -lIceUtil -lIceGrid -lIceBox -lIceStorm -lIceStormService

#qmake -o Makefile giraffeServer.pro
#make
