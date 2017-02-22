if (${QT5_COMPILE} AND ${roscpp_FOUND})
	
    set(GUI_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/gui)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS} ${Qt5Core_EXECUTABLE_COMPILE_FLAGS} ${Qt5Gui_EXECUTABLE_COMPILE_FLAGS} ${Qt5OpenGL_EXECUTABLE_COMPILE_FLAGS}")


    add_definitions(${Qt5Widgets_DEFINITIONS}
                        ${Qt5Core_DEFINITIONS}
                        ${Qt5Gui_DEFINITIONS}
                        ${Qt5OpenGL_DEFINITIONS}
    )


    SET(HEADERS robot/robot.h gui/gui.h gui/widget/cameraswidget.h gui/widget/controlvw.h gui/widget/glwidget.h)
    SET(RESOURCES resources.qrc)

	SET(SOURCE main.cpp depuratewindow.cpp 
				    robot/actuators.cpp robot/robot.cpp robot/sensors.cpp robot/threadupdaterobot.cpp 
			        pioneer/pioneer.c pioneer/pioneeropengl.c
			        gui/gui.cpp gui/stategui.cpp gui/threadupdategui.cpp
			        gui/widget/cameraswidget.cpp gui/widget/controlvw.cpp gui/widget/glwidget.cpp gui/widget/laserwidget.cpp
			        ${HEADERS}
			        ${RESOURCES}
    )

    include_directories(
        ${Qt5Widgets_INCLUDE_DIRS}
        ${Qt5Core_INCLUDE_DIRS}
        ${Qt5Gui_INCLUDE_DIRS}
        ${Qt5OpenGL_INCLUDE_DIRS}
        ${OpenCV_INCLUDE_DIR}
        ${Ice_INCLUDE_DIR}
        ${INTERFACES_CPP_DIR}
        ${LIBS_DIR}/
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${easyiceconfig_INCLUDE_DIRS}
        ${jderobottypes_INCLUDE_DIRS}
        ${jderobotcomm_INCLUDE_DIRS}
        ${roscpp_INCLUDE_DIRS}
    )

    link_directories(${JDE_LIBS}
        ${easyiceconfig_LIBRARY_DIRS}
        ${jderobotcomm_LIBRARY_DIRS}
    )

    add_executable( kobukiViewer ${SOURCE} ${INTERFACES_CPP_DIR})
			       
    target_link_libraries(kobukiViewer
        ${Qt5Widgets_LIBRARIES}
        ${Qt5Core_LIBRARIES}
        ${Qt5Gui_LIBRARIES}
        ${Qt5OpenGL_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT}
        ${OpenCV_LIBRARIES}
        ${easyiceconfig_LIBRARIES}
        ${jderobotcomm_LIBRARIES}
        ${ZeroCIce_LIBRARIES}
        ${Ice_LIBRARIES}
		${OPENGL_LIBRARIES} ${GLUT_LIBRARY}
        JderobotInterfaces
        jderobotutil
        ${catkin_LIBRARIES}
    )
ENDIF()

