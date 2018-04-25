 cmake_minimum_required(VERSION 2.8)


SET( SOURCE_FILES replayController.cpp replayControllerGui.cpp)


add_definitions(-DGLADE_DIR="${gladedir}")

include_directories(
    ${INTERFACES_CPP_DIR}
    ${LIBS_DIR}/
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${gtkmm_INCLUDE_DIRS}
    ${libglademm_INCLUDE_DIRS}
    ${gtkglextmm_INCLUDE_DIRS}
    ${resourcelocator_INCLUDE_DIRS}
    ${easyiceconfig_INCLUDE_DIRS}
)

link_directories(
	${resourcelocator_LIBRARY_DIRS}
	${easyiceconfig_LIBRARY_DIRS}
)
add_executable (replayController ${SOURCE_FILES})


TARGET_LINK_LIBRARIES(replayController
    ${GLUT_LIBRARIES}
    ${OpenCV_LIBRARIES}
    ${OpenCVGUI_LIBRARIES}
    ${gtkmm_LIBRARIES}
    ${libglademm_LIBRARIES}
    ${gtkglextmm_LIBRARIES}
    colorspacesmm
    JderobotInterfaces
    jderobotutil
    progeo
    jderobotHandlers
    ${easyiceconfig_LIBRARIES} 
    ${ZeroCIce_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
	${resourcelocator_LIBRARIES}
		${GLOG_LIBRARIES}

		)

install(TARGETS replayController
    DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/
    COMPONENT replayController
    )

INSTALL (FILES ${CMAKE_CURRENT_SOURCE_DIR}/replayControllerGui.glade DESTINATION ${CMAKE_INSTALL_PREFIX}/share/jderobot/glade COMPONENT replayController)
INSTALL (FILES ${CMAKE_CURRENT_SOURCE_DIR}/replayController.cfg DESTINATION ${CMAKE_INSTALL_PREFIX}/share/jderobot/conf COMPONENT replayController)
