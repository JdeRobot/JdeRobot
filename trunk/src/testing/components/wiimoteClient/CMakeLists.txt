SET( SOURCE_FILES wiimoteClient.cpp gui.cpp API.cpp control.cpp API.h gui.h control.h)


include_directories(
    ${INTERFACES_CPP_DIR}
    ${LIBS_DIR}/
    ${CMAKE_CURRENT_SOURCE_DIR}
)


add_executable (wiimoteClient ${SOURCE_FILES})

TARGET_LINK_LIBRARIES(wiimoteClient
    ${CMAKE_THREAD_LIBS_INIT}
	${GLUT_LIBRARIES}
	${ZeroCIce_LIBRARIES}
    ${opencv_LIBRARIES}
    ${gtkmm_LIBRARIES}
    ${libglademm_LIBRARIES}
    ${gthread_LIBRARIES}
    ${libgnomecanvas_LIBRARIES}
    ${libgnomecanvasmm_LIBRARIES}
    ${OpenCV_LIBRARIES}
    ${gtkglextmm_LIBRARIES}
    ${INTERFACES_CPP_DIR}/jderobot/libJderobotInterfaces.so
    ${LIBS_DIR}/jderobotutil/libjderobotutil.so
)

