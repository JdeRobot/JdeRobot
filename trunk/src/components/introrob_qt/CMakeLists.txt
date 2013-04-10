cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
include (FindPkgConfig)
if (PKG_CONFIG_FOUND)
	pkg_check_modules(OpenCV opencv)
endif()

FIND_PACKAGE(Qt4 REQUIRED)

SET(qt_SOURCES main.cpp depuratewindow.cpp MyAlgorithm.cpp
				robot/actuators.cpp robot/robot.cpp robot/sensors.cpp robot/threadupdaterobot.cpp 
			 gui/gui.cpp gui/threadupdategui.cpp
			 gui/widget/cameraswidget.cpp gui/widget/controlvw.cpp gui/widget/glwidget.cpp gui/widget/laserwidget.cpp
)

SET(qt_HEADERS
			depuratewindow.h
			robot/actuators.h robot/robot.h robot/sensors.h robot/threadupdaterobot.h 
     		pioneer/pioneer.c pioneer/pioneeropengl.c
			gui/gui.h gui/stategui.h gui/threadupdategui.h
			gui/widget/cameraswidget.h gui/widget/controlvw.h gui/widget/glwidget.h gui/widget/laserwidget.h

)

SET(qt_RESOURCES resources.qrc)
QT4_ADD_RESOURCES(qt_RESOURCES_RCC ${qt_RESOURCES})

SET(QT_USE_QTOPENGL TRUE)

SET( LIBS_DIR /usr/local/lib/jderobot/)

set( CMAKE_CXX_FLAGS " -lIce -lIceUtil -lGL -lGLU -lglut " )


QT4_WRAP_CPP(qt_HEADERS_MOC ${qt_HEADERS})
INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})



include_directories(${opencv_INCLUDE_DIRS})
link_directories( ${opencv_library_dirs})


include_directories(
    /usr/local/include/jderobot/
)

add_executable( main main.cpp depuratewindow.cpp MyAlgorithm.cpp
				robot/actuators.cpp robot/robot.cpp robot/sensors.cpp robot/threadupdaterobot.cpp 
			 pioneer/pioneer.c pioneer/pioneeropengl.c
			 gui/gui.cpp gui/stategui.cpp gui/threadupdategui.cpp
			 gui/widget/cameraswidget.cpp gui/widget/controlvw.cpp gui/widget/glwidget.cpp gui/widget/laserwidget.cpp
			   ${qt_HEADERS_MOC}
			   ${qt_RESOURCES_RCC}
)
			   
target_link_libraries(main opencv_imgproc opencv_highgui opencv_core
	${QT_LIBRARIES}
	${LIBS_DIR}/libJderobotInterfaces.so
	${LIBS_DIR}/libjderobotice.so
	${LIBS_DIR}/libjderobotutil.so

)

