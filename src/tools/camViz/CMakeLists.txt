SET( SOURCE_FILES camViz.cpp viewer.cpp)

add_definitions(-DGLADE_DIR="${gladedir}")

include_directories(
    ${INTERFACES_CPP_DIR}
    ${LIBS_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${gtkmm_INCLUDE_DIRS}
    ${libglademm_INCLUDE_DIRS}
    ${gtkglextmm_INCLUDE_DIRS}
    ${ZLIB_INCLUDE_DIRS}
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

add_executable (camViz ${SOURCE_FILES})

TARGET_LINK_LIBRARIES(camViz
    ${CMAKE_THREAD_LIBS_INIT} 
    ${OpenCV_LIBRARIES}
    ${OpenCVGUI_LIBRARIES}
    ${gtkmm_LIBRARIES}
    ${libglademm_LIBRARIES}
    colorspacesmm
    ${ZeroCIce_LIBRARIES}
    ${ZLIB_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
	${resourcelocator_LIBRARIES}
    ${catkin_LIBRARIES}
    ${comm_LIBRARIES}
    ${config_LIBRARIES}
		${GLOG_LIBRARIES}
    JderobotInterfaces
        jderobotutil
		)

install(TARGETS camViz
    DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/
    COMPONENT camviz
    )


INSTALL (FILES ${CMAKE_CURRENT_SOURCE_DIR}/camViz.glade DESTINATION ${CMAKE_INSTALL_PREFIX}/share/jderobot/glade COMPONENT camviz)
INSTALL (FILES ${CMAKE_CURRENT_SOURCE_DIR}/camViz.yml DESTINATION ${CMAKE_INSTALL_PREFIX}/share/jderobot/conf COMPONENT camviz)
