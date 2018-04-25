SET(SOURCE_FILES rgbdViewer.cpp drawarea.cpp rgbdViewergui.cpp myprogeo.cpp)

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

add_executable (rgbdViewer ${SOURCE_FILES})


TARGET_LINK_LIBRARIES(rgbdViewer
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

INSTALL (TARGETS rgbdViewer DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/ COMPONENT rgbdViewer)
INSTALL (FILES ${CMAKE_CURRENT_SOURCE_DIR}/rgbdViewergui.glade DESTINATION ${CMAKE_INSTALL_PREFIX}/share/jderobot/glade COMPONENT rgbdViewer)
INSTALL (FILES ${CMAKE_CURRENT_SOURCE_DIR}/rgbdViewer.yml DESTINATION ${CMAKE_INSTALL_PREFIX}/share/jderobot/conf COMPONENT rgbdViewer)
INSTALL (FILES ${CMAKE_CURRENT_SOURCE_DIR}/rgbdViewerKinect2.yml DESTINATION ${CMAKE_INSTALL_PREFIX}/share/jderobot/conf COMPONENT rgbdViewer)
INSTALL (FILES ${CMAKE_CURRENT_SOURCE_DIR}/camera-0.cfg DESTINATION ${CMAKE_INSTALL_PREFIX}/share/jderobot/conf COMPONENT rgbdViewer)
