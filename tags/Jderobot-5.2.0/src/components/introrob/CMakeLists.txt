
SET( SOURCE_FILES API.cpp  control.cpp drawarea.cpp gui.cpp introrob.cpp MyAlgorithms.cpp pioneer.c pioneeropengl.c)


include_directories(
    ${INTERFACES_CPP_DIR}
    ${LIBS_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/cameras
    ${CMAKE_CURRENT_SOURCE_DIR}/../calibrator
)

set( CMAKE_CXX_FLAGS "") #"-lgsl -lgslcblas" ) # Opciones para el compilador

add_executable (introrob  ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(introrob
    ${CMAKE_THREAD_LIBS_INIT} 
    ${opencv_LIBRARIES}
    ${gtkmm_LIBRARIES}
    ${libglademm_LIBRARIES}
    ${gthread_LIBRARIES}
    ${libgnomecanvas_LIBRARIES}
    ${libgnomecanvasmm_LIBRARIES}
    ${gtkglextmm_LIBRARIES}
    ${OpenCV_LIBRARIES}
    ${LIBS_DIR}/colorspaces/libcolorspacesmm.so
    ${INTERFACES_CPP_DIR}/jderobot/libJderobotInterfaces.so
    ${LIBS_DIR}/jderobotutil/libjderobotutil.so
    ${LIBS_DIR}/progeo/libprogeo.so
    ${LIBS_DIR}/pioneer/libpioneer.so
    "glut"
    "gsl"
    "gslcblas"
#	${gsl_LIBRARIES}
	${ZeroCIce_LIBRARIES}
	${Gearbox_LIBRARIES}
)
