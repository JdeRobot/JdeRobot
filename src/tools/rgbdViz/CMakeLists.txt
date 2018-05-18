SET(SOURCE_FILES rgbdViz.cpp drawarea.cpp rgbdVizgui.cpp myprogeo.cpp)

include_directories(
    ${INTERFACES_CPP_DIR}
    ${LIBS_DIR}/
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/cameras
    ${gtkmm_INCLUDE_DIRS}
    ${libglademm_INCLUDE_DIRS}
    ${gtkglextmm_INCLUDE_DIRS}
    ${resourcelocator_INCLUDE_DIRS}
    ${jderobottypes_INCLUDE_DIRS}
    ${comm_INCLUDE_DIRS}
    ${config_INCLUDE_DIRS}
    ${roscpp_INCLUDE_DIRS}
)

link_directories(
	${resourcelocator_LIBRARY_DIRS}
    ${comm_LIBRARY_DIRS}
    ${config_LIBRARY_DIRS}
)

add_executable (rgbdViz ${SOURCE_FILES})


TARGET_LINK_LIBRARIES(rgbdViz
    ${CMAKE_THREAD_LIBS_INIT} 
    ${GLUT_LIBRARIES}
    ${OpenCV_LIBRARIES}
    ${gtkmm_LIBRARIES}
    ${OpenCVGUI_LIBRARIES}
    ${libglademm_LIBRARIES}
    ${gtkglextmm_LIBRARIES}
    colorspacesmm
    progeo
	geometry
    ${gsl_LIBRARIES}
    ${ZeroCIce_LIBRARIES}
    ${ZLIB_LIBRARIES}
	${resourcelocator_LIBRARIES}
	${GLOG_LIBRARIES}
    ${comm_LIBRARIES}
    ${config_LIBRARIES}
    JderobotInterfaces
    jderobotutil
	)

INSTALL (TARGETS rgbdViz DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/ COMPONENT rgbdViz)
INSTALL (FILES ${CMAKE_CURRENT_SOURCE_DIR}/rgbdVizgui.glade DESTINATION ${CMAKE_INSTALL_PREFIX}/share/jderobot/glade COMPONENT rgbdViz)
INSTALL (FILES ${CMAKE_CURRENT_SOURCE_DIR}/rgbdViz.yml DESTINATION ${CMAKE_INSTALL_PREFIX}/share/jderobot/conf COMPONENT rgbdViz)
INSTALL (FILES ${CMAKE_CURRENT_SOURCE_DIR}/rgbdVizKinect2.yml DESTINATION ${CMAKE_INSTALL_PREFIX}/share/jderobot/conf COMPONENT rgbdViz)
INSTALL (FILES ${CMAKE_CURRENT_SOURCE_DIR}/camera-0.cfg DESTINATION ${CMAKE_INSTALL_PREFIX}/share/jderobot/conf COMPONENT rgbdViz)
